//
// Error400RequestHandler.h
//
// Copyright (c) December 2015 Hans Klabbers
//
#ifndef __ERROR400REQUESTHANDLER_H__
#define __ERROR400REQUESTHANDLER_H__

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerSession;

class Error400RequestHandler : public HTTPRequestHandler {
public:
  void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response);
};
#endif // __ERROR400REQUESTHANDLER_H__
