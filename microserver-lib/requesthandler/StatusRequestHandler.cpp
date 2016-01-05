//
// StatusRequestHandler.cpp
//
// Copyright (c) December 2015 Hans Klabbers
//
#include "StatusRequestHandler.h"

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using ::Poco::Net::HTTPResponse;

void StatusRequestHandler::handleRequest(HTTPServerRequest &request,
                                         HTTPServerResponse &response) {
  std::string result{
      "{HTTPStatus: 200, HTTPStatusDescription: OK, CurrentLoad: 1}"};
  response.setChunkedTransferEncoding(true);
  response.setContentType("application/json");
  response.setStatus(HTTPResponse::HTTP_OK);
  std::ostream &ostr = response.send();
  ostr << result;
  ostr.flush();

  return;
}

