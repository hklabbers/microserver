//
// Error400RequestHandler.cpp
//
// Copyright (c) December 2015 Hans Klabbers
//
#include "Error400RequestHandler.h"

void Error400RequestHandler::handleRequest(HTTPServerRequest &request,
                                           HTTPServerResponse &response) {
  // send error 400...
  response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
  std::ostream& ostr = response.send();
  ostr << "HTTP ERROR 400.";
  ostr.flush();
}
