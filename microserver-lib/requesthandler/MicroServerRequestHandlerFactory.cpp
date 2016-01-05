//
// MicroServerRequestHandlerFactory.cpp
//
// Copyright (c) December 2015 Hans Klabbers
//
#include "MicroServerRequestHandlerFactory.h"
#include "AbstractRequestHandler.h"
#include "Error404RequestHandler.h"
#include "StatusRequestHandler.h"
#include "Poco/Util/Application.h"
#include "Poco/ClassLoader.h"
#include "Poco/Manifest.h"
#include "boost/algorithm/string/predicate.hpp"

#include <iostream>

using Poco::ClassLoader;
using Poco::Manifest;

MicroServerRequestHandlerFactory::MicroServerRequestHandlerFactory(
    std::vector<RequestHandlerDefinition> &requestHandlers, bool lazyLoading,
    std::string loadURI)
    : requestHandlers(requestHandlers), lazyLoading(lazyLoading),
      loadURI(loadURI) {

  if (!lazyLoading) {
    loadAllLibraries()
        ? l.information(
              "All configured libraries are loaded (lazyLoading is false).")
        : l.error("Error while loading all libraries (lazyLoading is false.");
  }
}

HTTPRequestHandler *MicroServerRequestHandlerFactory::createRequestHandler(
    const HTTPServerRequest &request) {
  l.information("request.getURI(): " + request.getURI());

  if (boost::starts_with(request.getURI(), loadURI)) {
    return new StatusRequestHandler();
  }

  for (auto requestHandler : requestHandlers) {
    if (boost::starts_with(request.getURI(), requestHandler.getUri())) {
      l.information("path: " +
                    requestHandler.getLibraryNameWithPathAndSuffix());
      bool libraryLoadedSuccessful = classLoader.isLibraryLoaded(
          requestHandler.getLibraryNameWithPathAndSuffix());
      std::string boolean = libraryLoadedSuccessful ? "true" : "false";
      l.information("libraryLoadedSuccessful: " + boolean);
      if (!libraryLoadedSuccessful) {
        libraryLoadedSuccessful =
            loadLibrary(requestHandler.getLibraryNameWithPathAndSuffix());
        std::string boolean = libraryLoadedSuccessful ? "true" : "false";
        l.information("libraryLoadedSuccessful-1: " + boolean);
      }
      if (libraryLoadedSuccessful) {
        try {
          l.information("Trying to load class: " +
                        requestHandler.getClassName());
          AbstractRequestHandler *provider =
              classLoader.create(requestHandler.getClassName());
          return provider->getRequestHandler(request.getURI());
        } catch (Poco::NotFoundException &ex) {
          l.error(ex.message());
        }
      }
      return 0;
    }
  }
  return new Error404RequestHandler();
}

bool MicroServerRequestHandlerFactory::loadAllLibraries() {
  bool noErrors = true;
  for (auto requestHandler : requestHandlers) {
    if (!classLoader.isLibraryLoaded(
            requestHandler.getLibraryNameWithPathAndSuffix())) {
      loadLibrary(requestHandler.getLibraryNameWithPathAndSuffix());
    }
  }
  return noErrors;
}

bool MicroServerRequestHandlerFactory::loadLibrary(std::string libraryName) {
  bool noErrors = true;
  try {
    if (!classLoader.isLibraryLoaded(libraryName)) {
      l.information("Loading library: " + libraryName);
      classLoader.loadLibrary(libraryName);
    }
  } catch (Poco::LibraryLoadException &ex) {
    l.error("Error loading library " + libraryName + " (" + ex.message() +
            ").");
    noErrors = false;
  }
  return noErrors;
}
