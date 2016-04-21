//
// Copyright (c) March 2016 Hans Klabbers
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
//    how to obtain complete source code for this software and any
//    accompanying software that uses this software.  The source code
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
// NON-INFRINGEMENT, ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This software links against software from the Poco Project which is
// distributed under the Boost Software License 1.0. Please visit the
// following url for more information: pocoproject.org/license.html.

#include "microserver/requesthandler/RequestHandlerDefinition.h"
#include "microserver/requesthandler/MicroServerRequestHandlerFactory.h"
#include "Poco/Net/SecureServerSocket.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/ConfigurationView.h"
#include "boost/filesystem.hpp"
#include <iostream>

using Poco::Net::SecureServerSocket;
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
    MicroServer() { }

    ~MicroServer() { }

protected:
    void initialize(Application &self) {
        loadConfigurationFromFile();
        ServerApplication::initialize(self);
    }

    void uninitialize() { ServerApplication::uninitialize(); }

    void defineOptions(OptionSet &options) {
        ServerApplication::defineOptions(options);

        options.addOption(
                Option("help", "h", "display help information on command line arguments").required(false).repeatable(
                        false));

        options.addOption(
                Option("config", "c", "configuration file location").required(false).repeatable(false).argument(
                        "file"));
    }

    void handleOption(const std::string &name, const std::string &value) {
        ServerApplication::handleOption(name, value);

        if (name == "help") {
            helpRequested = true;
            return;
        }

        if (name == "config") {
            loadConfigfileFromLocation = true;
            configfileLocation = value;
            return;
        }
    }

    void displayHelp() {
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("A web server that serves microservices, see https://github.com/hklabbers/microserver.");
        helpFormatter.format(std::cout);
    }

    int main(const std::vector<std::string> &args) {
        if (helpRequested) {
            displayHelp();
            return Application::EXIT_OK;
        }

        if (loadLibraryConfiguration(requestHandlers)) {
            auto port = (unsigned short) config().getInt("microrSserver.settings.port", 9980);
            auto maxQueued = config().getInt("microserver.settings.maxQueued", 100);
            auto maxThreads = config().getInt("microserver.settings.maxThreads", 16);
            auto statusURI = config().getString("microserver.settings.statusURI", "/status");
            ThreadPool::defaultPool().addCapacity(maxThreads);

            HTTPServerParams *pParams = new HTTPServerParams;
            pParams->setMaxQueued(maxQueued);
            pParams->setMaxThreads(maxThreads);

            SecureServerSocket svs(port);
            HTTPServer srv(new MicroServerRequestHandlerFactory(requestHandlers, lazyLoading, statusURI), svs, pParams);
            srv.start();
            waitForTerminationRequest();
            srv.stop();
            return Application::EXIT_OK;
        } else {
            return Application::EXIT_CONFIG;
        }
    }

private:
    bool helpRequested{false};
    bool loadConfigfileFromLocation{false};
    std::string configfileLocation{};
    std::vector<RequestHandlerDefinition> requestHandlers;
    bool lazyLoading{false};
    Logger &l = Logger::get("MicroServer");

    void loadConfigurationFromFile() {
        using boost::filesystem::exists;
        std::string configfile;
        if (loadConfigfileFromLocation) {
            if (exists(configfileLocation)) {
                configfile = configfileLocation;
            } else {
                l.error("Configuration file(" + configfileLocation + ") not found.");
                return;
            }
        } else {
            std::string execName{this->commandName()};
            if (exists(execName + ".properties"))
                configfile = execName + ".properties";
            else if (exists("/etc/" + execName + ".properties"))
                configfile = "/etc/" + execName + ".properties";
            else if (exists("/etc/" + execName + "/" + execName + ".properties"))
                configfile = "/etc/" + execName + "/" + execName + ".properties";
            else {
                l.error("Configuration file not found.");
                return;
            }
        }
        loadConfiguration(configfile);
        l.information("Loaded configuration file: " + configfile);
    }

    bool loadLibraryConfiguration(std::vector<RequestHandlerDefinition> &requestHandlers) {
        MicroServer &app = *this;
        bool noErrors = true;

        const std::string keyPrefix = {"microserver.library"};
        const std::string keyClassLabel = {"class"};
        const std::string keyPathLabel = {"path"};

        Poco::Util::AbstractConfiguration::Keys libraryConfigKeys;
        app.config().keys(keyPrefix, libraryConfigKeys);
        for (auto key : libraryConfigKeys) {
            std::string libraryKey = {keyPrefix + "." + key};
            std::string classNameKey = {libraryKey + "." + keyClassLabel};
            std::string pathKey = {libraryKey + "." + keyPathLabel};
            try {
                const auto uriValue = app.config().getString(libraryKey);
                const auto classNameValue = app.config().getString(classNameKey);
                const auto pathValue = app.config().getString(pathKey);
                const RequestHandlerDefinition requestHandler = {uriValue, key, pathValue,
                                                                 Poco::SharedLibrary::suffix(), classNameValue};
                requestHandlers.push_back(requestHandler);
            } catch (Poco::NotFoundException ex) {
                noErrors = false;
                l.information("Configuration for library " + key + " is not complete. MicroServer is not started.");
            }
        }
        lazyLoading = app.config().getBool("microserver.settings.lazyLoading"); //, false);
        return noErrors;
    }

};

int main(int argc, char **argv) {
    MicroServer app;
    return app.run(argc, argv);
}
