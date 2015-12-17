//
// MicroServerRequestHandlerFactory.h
//
// Copyright (c) December 2015 Hans Klabbers
//
#ifndef __MICROSERVERREQUESTHANDLERFACTORY_H__
#define __MICROSERVERREQUESTHANDLERFACTORY_H__

#include "RequestHandlerDefinition.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Util/Application.h"

using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Util::Application;

class MicroServerRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
  MicroServerRequestHandlerFactory(
      std::vector<RequestHandlerDefinition> &requestHandlers, bool lazyLoading);

  HTTPRequestHandler *createRequestHandler(const HTTPServerRequest &request);

private:
  std::vector<RequestHandlerDefinition> &requestHandlers;
  ClassLoader<AbstractRequestHandler> classLoader;
  bool lazyLoading = true;
  Application& app = Application::instance();

  bool loadAllLibraries();
  bool loadLibrary(std::string libraryName);
};
#endif //__MICROSERVERREQUESTHANDLERFACTORY_H__
