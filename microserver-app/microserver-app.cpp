//
// microserver-app.cpp
//
// Copyright (c) November 2015 Hans Klabbers
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Redistributions in any form must be accompanied by information on
//    how to obtain complete source code for the this software and any
//    accompanying software that uses the this software.  The source code
//    must either be included in the distribution or be available for no
//    more than the cost of distribution plus a nominal fee, and must be
//    freely redistributable under reasonable conditions.  For an
//    executable file, complete source code means the source code for all
//    modules of this software it contains.  It does not include source code
//    for modules or files that typically accompany the major components
//    of the operating system on which the executable file runs.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// NON-INFRINGEMENT, ARE DISCLAIMED.  IN NO EVENT SHALL ORACLE BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This software links against software from the Poco Project which is
// distributed under the Boost Software License 1.0. Please visit the
// following url for more information: pocoproject.org/license.html.

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
    loadConfiguration();
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
//TODO: put the url in
    helpFormatter.setHeader(
        "A web server that serves microservices, see url to github.");
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
        std::string statusURI =
            config().getString("microserver.settings.statusURI", "/status");
        ThreadPool::defaultPool().addCapacity(maxThreads);

        HTTPServerParams *pParams = new HTTPServerParams;
        pParams->setMaxQueued(maxQueued);
        pParams->setMaxThreads(maxThreads);

        ServerSocket svs(port);
        HTTPServer srv(new MicroServerRequestHandlerFactory(
                           requestHandlers, lazyLoading, statusURI),
                       svs, pParams);
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
  Logger &l = Logger::get("MicroServer");

  bool loadLibraryConfiguration(
      std::vector<RequestHandlerDefinition> &requestHandlers) {
    MicroServer &app = *this;
    bool noErrors = true;

    const std::string keyPrefix = {"microserver.library"};
    const std::string keyClassLabel = {"class"};
    const std::string keyPathLabel = {"path"};

    //Use ConfigurationView????
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
      } catch (Poco::NotFoundException ex) {
        noErrors = false;
        l.information("Configuration for library " + key +
                      " is not complete. MicroServer is not started.");
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
