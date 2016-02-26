//
// Created by Hans Klabbers on 10-02-16.
//
#include "Error404RequestHandler.h"

void Error404RequestHandler::handleRequest(HTTPServerRequest &request,
                                           HTTPServerResponse &response) {
    // send error 400...
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
    std::ostream &ostr = response.send();
    ostr << "HTTP ERROR 404.";
    ostr.flush();
}
