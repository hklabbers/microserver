//
// MicroServerRequestHandlerFactory.h
//
// Copyright (c) December 2015 Hans Klabbers
//
#ifndef MICROSERVER_LIB_REQUESTHANDLER_MICROSERVERREQUESTHANDLERFACTORY_H
#define MICROSERVER_LIB_REQUESTHANDLER_MICROSERVERREQUESTHANDLERFACTORY_H

#include "RequestHandlerDefinition.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Logger.h"

using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Logger;

class MicroServerRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
  MicroServerRequestHandlerFactory(
      std::vector<RequestHandlerDefinition> &requestHandlers, bool lazyLoading,
      std::string loadURI);

  HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &request);

private:
  std::vector<RequestHandlerDefinition> &requestHandlers;
  ClassLoader<AbstractRequestHandler> classLoader;
  bool lazyLoading = true;
  std::string loadURI;
  Logger &l = Logger::get("requestHandler.MicroServerRequestHandlerFactory");

  bool loadAllLibraries();
  bool loadLibrary(std::string libraryName);
};
#endif //MICROSERVER_LIB_REQUESTHANDLER_MICROSERVERREQUESTHANDLERFACTORY_H
