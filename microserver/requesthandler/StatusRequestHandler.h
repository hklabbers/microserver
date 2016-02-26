//
// Created by Hans Klabbers on 10-02-16.
//
#ifndef MICROSERVER_STATUSREQUESTHANDLER_H
#define MICROSERVER_STATUSREQUESTHANDLER_H

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

class StatusRequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest &request, HTTPServerResponse &response);
};

#endif //MICROSERVER_STATUSREQUESTHANDLER_H
