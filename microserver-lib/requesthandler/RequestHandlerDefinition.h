
// RequestHandlerDefinition.h
//
// Copyright (c) December 2015 Hans Klabbers
//

#ifndef MICROSERVER_LIB_REQUESTHANDLER_REQUESTHANDLERDEFINITION_H
#define MICROSERVER_LIB_REQUESTHANDLER_REQUESTHANDLERDEFINITION_H

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

#endif /* MICROSERVER_LIB_REQUESTHANDLER_REQUESTHANDLERDEFINITION_H */
