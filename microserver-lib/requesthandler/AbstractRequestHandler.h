//
// AbstractRequestHandler.h
//
// Copyright (c) December 2015 Hans Klabbers
//
#ifndef MICROSERVER_LIB_REQUESTHANDLER_ABSTRACTREQUESTHANDLER_H
#define MICROSERVER_LIB_REQUESTHANDLER_ABSTRACTREQUESTHANDLER_H

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPRequestHandler.h"

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPRequestHandler;

class AbstractRequestHandler {
public:
  AbstractRequestHandler();
  ~AbstractRequestHandler();
  virtual HTTPRequestHandler *getRequestHandler(std::string uri) = 0;
};
#endif // MICROSERVER_LIB_REQUESTHANDLER_ABSTRACTREQUESTHANDLER_H
