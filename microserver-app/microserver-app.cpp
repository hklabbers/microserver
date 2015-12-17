//
// microserver-app.cpp
//
// Copyright (c) November 2015 Hans Klabbers
//
#include "requesthandler/RequestHandlerDefinition.h"
#include "requesthandler/MicroServerRequestHandlerFactory.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>

using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Util::AbstractConfiguration;

class MicroServer : public Poco::Util::ServerApplication {
public:
  MicroServer() : _helpRequested(false) {}

  ~MicroServer() {}

protected:
  void initialize(Application &self) {
    loadConfiguration(); // load default configuration files, if present
    ServerApplication::initialize(self);
  }

  void uninitialize() { ServerApplication::uninitialize(); }

  void defineOptions(OptionSet &options) {
    ServerApplication::defineOptions(options);

    options.addOption(
        Option("help", "h",
               "display help information on command line arguments")
            .required(false)
            .repeatable(false));
  }

  void handleOption(const std::string &name, const std::string &value) {
    ServerApplication::handleOption(name, value);

    if (name == "help")
      _helpRequested = true;
  }

  void displayHelp() {
    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("A web server that serves microservices");
    helpFormatter.format(std::cout);
  }

  int main(const std::vector<std::string> &args) {
    if (_helpRequested) {
      displayHelp();
    } else {
      if (loadLibraryConfiguration(requestHandlers)) {
        unsigned short port = (unsigned short)config().getInt(
            "microrSserver.settings.port", 9980);
        int maxQueued = config().getInt("microServer.settings.maxQueued", 100);
        int maxThreads = config().getInt("microServer.settings.maxThreads", 16);
        ThreadPool::defaultPool().addCapacity(maxThreads);

        HTTPServerParams *pParams = new HTTPServerParams;
        pParams->setMaxQueued(maxQueued);
        pParams->setMaxThreads(maxThreads);

        ServerSocket svs(port);
        this->logger().information("before srv");
        HTTPServer srv(
            new MicroServerRequestHandlerFactory(requestHandlers, lazyLoading),
            svs, pParams);
        this->logger().information("before srv.start()");
        srv.start();
        waitForTerminationRequest();
        srv.stop();
      } else {
        return Application::EXIT_CONFIG;
      }
    }
    return Application::EXIT_OK;
  }

private:
  bool _helpRequested;
  std::vector<RequestHandlerDefinition> requestHandlers;
  bool lazyLoading = false;

  ///
  /// This method will add all library definitions to the vector that is
  /// provided as a parameter. The bool that is returned indicates whether the
  /// library configuration is complete.
  bool loadLibraryConfiguration(
      std::vector<RequestHandlerDefinition> &requestHandlers) {
    // Application &app = Application::instance();
    MicroServer &app = *this;
    bool noErrors = true;

    const std::string keyPrefix = {"microserver.library"};
    const std::string keyClassLabel = {"class"};
    const std::string keyPathLabel = {"path"};

    Poco::Util::AbstractConfiguration::Keys libraryConfigKeys;
    app.config().keys(keyPrefix, libraryConfigKeys);
    for (auto key : libraryConfigKeys) {
      std::string libraryKey = {keyPrefix};
      libraryKey.append(".").append(key);
      std::string uriKey = {libraryKey};
      std::string classNameKey = {libraryKey};
      classNameKey.append(".").append(keyClassLabel);
      std::string pathKey = {libraryKey};
      pathKey.append(".").append(keyPathLabel);
      try {
        const std::string uriValue = app.config().getString(libraryKey);
        const std::string classNameValue = app.config().getString(classNameKey);
        const std::string pathValue = app.config().getString(pathKey);
        const RequestHandlerDefinition requestHandler = {
            uriValue, key, pathValue, Poco::SharedLibrary::suffix(),
            classNameValue};
        requestHandlers.push_back(requestHandler);
        app.logger().information(requestHandler.toString());
      } catch (Poco::NotFoundException ex) {
        noErrors = false;
        app.logger().information(
            "Configuration for library " + key +
            " is not complete. Microserver is not started.");
      }
    }
    lazyLoading =
        app.config().getBool("microserver.settings.lazyLoading", false);
    return noErrors;
  }
};

int main(int argc, char **argv) {
  MicroServer app;
  return app.run(argc, argv);
}
