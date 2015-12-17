//
// MicroServerRequestHandlerFactory.cpp
//
// Copyright (c) December 2015 Hans Klabbers
//
#include "MicroServerRequestHandlerFactory.h"
#include "AbstractRequestHandler.h"
#include "Error400RequestHandler.h"
#include "Poco/Util/Application.h"
#include "Poco/ClassLoader.h"
#include "Poco/Manifest.h"
#include <iostream>

using Poco::Util::Application;
using Poco::ClassLoader;
using Poco::Manifest;

MicroServerRequestHandlerFactory::MicroServerRequestHandlerFactory(
    std::vector<RequestHandlerDefinition> &requestHandlers, bool lazyLoading)
    : requestHandlers(requestHandlers), lazyLoading(lazyLoading) {

  app.logger().information("requestHandlers.size(): " +
                           std::to_string(requestHandlers.size()));
  if (!lazyLoading) {
    loadAllLibraries()
        ? app.logger().information(
              "All configured libraries are loaded (lazyLoading is false).")
        : app.logger().error(
              "Error while loading all libraries (lazyLoading is false.");
  }
}

HTTPRequestHandler *MicroServerRequestHandlerFactory::createRequestHandler(
    const HTTPServerRequest &request) {
  app.logger().information("request.getURI(): " + request.getURI());
  for (auto requestHandler : requestHandlers) {
      app.logger().information("Loop: " + requestHandler.getUri());
    if (requestHandler.getUri().compare(request.getURI()) == 0) {
        app.logger().information("to here...");
        app.logger().information("path: " + requestHandler.getLibraryNameWithPathAndSuffix());
        bool libraryLoadedSuccessful = classLoader.isLibraryLoaded(requestHandler.getLibraryNameWithPathAndSuffix());
        std::string boolean = libraryLoadedSuccessful ? "true" : "false";
        app.logger().information("libraryLoadedSuccessful: " + boolean);
      if (!libraryLoadedSuccessful) {
        libraryLoadedSuccessful = loadLibrary(requestHandler.getLibraryNameWithPathAndSuffix());
        std::string boolean = libraryLoadedSuccessful ? "true" : "false";
        app.logger().information("libraryLoadedSuccessful-1: " + boolean);
      }
      if (libraryLoadedSuccessful) {
        // now we can serve the request by first loading the class
        try {
          app.logger().information("Trying to load class: " +
                                   requestHandler.getClassName());
          //Testing 123
          ClassLoader<AbstractRequestHandler>::Iterator it(classLoader.begin());
          ClassLoader<AbstractRequestHandler>::Iterator end(classLoader.end());
          for (; it != end; ++it)
          {
                std::cout << "lib path: " << it->first << std::endl;;
                Poco::Manifest<AbstractRequestHandler>::Iterator itMan(it->second->begin());
               Poco::Manifest<AbstractRequestHandler>::Iterator endMan(it->second->end());
              for (; itMan != endMan; ++itMan){
                  std::cout << itMan->name() << std::endl;
              }
          }
          // End Testing
          AbstractRequestHandler* provider =  classLoader.create(requestHandler.getClassName());
          return provider->getRequestHandler();
        } catch (Poco::NotFoundException &ex) {
          app.logger().error(ex.message());
        }
      }
      return 0;
    }
  }
  return new Error400RequestHandler(); //Foute aanvraag 400 Error.
}

bool MicroServerRequestHandlerFactory::loadAllLibraries() {
  bool noErrors = true;
  for (auto requestHandler : requestHandlers) {
    app.logger().information("---");
    if (!classLoader.isLibraryLoaded(requestHandler.getLibraryNameWithPathAndSuffix())) {
      loadLibrary(requestHandler.getLibraryNameWithPathAndSuffix());
    }
  }
  return noErrors;
}

bool MicroServerRequestHandlerFactory::loadLibrary(std::string libraryName) {
  bool noErrors = true;
  try {
    if (!classLoader.isLibraryLoaded(libraryName)) {
        app.logger().information("Loading library: " + libraryName);
      classLoader.loadLibrary(libraryName);
    }
  } catch (Poco::LibraryLoadException &ex) {
    app.logger().error("Error loading library " + libraryName + " (" +
                       ex.message() + ").");
    noErrors = false;
  }
  return noErrors;
}
