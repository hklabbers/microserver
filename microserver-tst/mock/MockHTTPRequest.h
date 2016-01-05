//
// MockHTTPRequest.h
//
// Copyright (c) December Hans Klabbers
//
#ifndef __MOCKHTTPREQUEST_H__
#define __MOCKHTTPREQUEST_H__

#include "gmock/gmock.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"

using Poco::Net::SocketAddress;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;

namespace microserver_mock {

class MockHTTPRequest : public HTTPServerRequest {
public:
  MOCK_CONST_METHOD0(clientAddress, const SocketAddress &());
  MOCK_CONST_METHOD0(expectContinue, bool());
  MOCK_CONST_METHOD0(response, HTTPServerResponse &());
  MOCK_CONST_METHOD0(serverAddress, const SocketAddress &());
  MOCK_CONST_METHOD0(serverParams, const HTTPServerParams &());
  MOCK_METHOD0(stream, std::istream &());
};

} // end namespace microserver_mock
#endif //__MOCKHTTPREQUEST_H__
