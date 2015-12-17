//
// AbstractRequestHandler.h
//
// Copyright (c) December 2015 Hans Klabbers
//
#ifndef __ABSTRACTREQUESTHANDLER_H__
#define __ABSTRACTREQUESTHANDLER_H__

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
  virtual HTTPRequestHandler* getRequestHandler()=0; 
};
#endif // __ABSTRACTREQUESTHANDLER_H__
