//
// Error404RequestHandler.h
//
// Copyright (c) December 2015 Hans Klabbers
//
#ifndef MICROSERVER_LIB_REQUESTHANDLER_ERROR404REQUESTHANDLER_H
#define MICROSERVER_LIB_REQUESTHANDLER_ERROR404REQUESTHANDLER_H

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

class Error404RequestHandler : public HTTPRequestHandler {
public:
  void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response);
};
#endif // MICROSERVER_LIB_REQUESTHANDLER_ERROR404REQUESTHANDLER_H__