//
// Created by Hans Klabbers on 10-02-16.
//
#ifndef MICROSERVER_REQUESTHANDLERDEFINITION_H
#define MICROSERVER_REQUESTHANDLERDEFINITION_H

#include "AbstractRequestHandler.h"
#include "Poco/ClassLoader.h"

#include <string>

using Poco::ClassLoader;

class RequestHandlerDefinition {
public:
    RequestHandlerDefinition(std::string uri, std::string libraryName,
                             std::string path, std::string suffix,
                             std::string className);
    std::string getUri() const;
    std::string getLibraryName() const;
    std::string getPath() const;
    std::string getClassName() const;
    std::string getLibraryNameWithPathAndSuffix() const;

private:
    std::string uri{};
    std::string libraryName{};
    std::string path{};
    std::string suffix{};
    std::string className{};
};
#endif //MICROSERVER_REQUESTHANDLERDEFINITION_H
