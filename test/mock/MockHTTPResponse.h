//
// MockHTTPResponse.h
//
// Copyright (c) December 2015 Hans klabbers
//
#ifndef __MOCKHTTPRESPONSE_H__
#define __MOCKHTTPRESPONSE_H__

#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"

using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;

namespace microserver_mock {

class MockHTTPResponse : public HTTPServerResponse {
public:
  MOCK_METHOD2(redirect, void(const std::string &, HTTPStatus));
  MOCK_METHOD1(requireAuthentication, void(const std::string &));
  MOCK_METHOD0(send, std::ostream &());
  MOCK_METHOD2(sendBuffer, void(const void *, std::size_t));
  MOCK_METHOD0(sendContinue, void());
  MOCK_METHOD2(sendFile, void(const std::string &, const std::string &));
  MOCK_CONST_METHOD0(sent, bool());
};

} // end namespace microserver_mock
#endif // __MOCKHTTPRESPONSE_H__
