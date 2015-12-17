//
// RequestHandlerDefinition.cpp
//
// Copyright (c) November 2015 Hans Klabbers
//
#include "RequestHandlerDefinition.h"

RequestHandlerDefinition::RequestHandlerDefinition(std::string uri,
                                                   std::string libraryName,
                                                   std::string path,
                                                   std::string suffix,
                                                   std::string className)
    : uri(uri), libraryName(libraryName), path(path), suffix(suffix),
      className(className) {}

std::string RequestHandlerDefinition::getUri() const { return uri; }

std::string RequestHandlerDefinition::getLibraryName() const {
  return libraryName;
}

std::string RequestHandlerDefinition::getPath() const { return path; }

std::string RequestHandlerDefinition::getClassName() const { return className; }

std::string RequestHandlerDefinition::getLibraryNameWithPathAndSuffix() const {
  std::string returnStr{};
  returnStr.append(path).append("lib").append(libraryName).append(suffix);
  return returnStr;
}

std::string RequestHandlerDefinition::toString() const {
  std::string formattedString = {"Uri: "};
  formattedString.append(uri)
      .append(", LibraryName: ")
      .append(libraryName)
      .append(", Path: ")
      .append(path)
      .append(", Suffix: ")
      .append(suffix)
      .append(", ClassName: ")
      .append(className);
  return formattedString;
}
