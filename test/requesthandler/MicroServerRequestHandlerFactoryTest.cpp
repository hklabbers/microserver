//
// MicroServerRequestHandlerFactoryTest.cpp
//
// Copyright (c) December 2015 Hans Klabbers
//
#include "../mock/MockHTTPRequest.h"
#include "../mock/MockHTTPResponse.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../microserver/requesthandler/RequestHandlerDefinition.h"
#include "../../microserver/requesthandler/MicroServerRequestHandlerFactory.h"

using microserver_mock::MockHTTPRequest;
using microserver_mock::MockHTTPResponse;
using testing::DefaultValue;
using testing::NiceMock;

namespace MicroServerRequestHandlerFactoryTest {

TEST(MicroServerRequestHandlerFactoryTest, Error404RequestHandler) {
  std::vector<RequestHandlerDefinition> requestHandlers;
  RequestHandlerDefinition requestHandler{
      "ThisIsATestUri", "ThisIsATestLibraryName", "ThisIsATestPath",
      "ThisIsATestSuffix", "ThisIsATestClassname"};
  requestHandlers.push_back(requestHandler);
  MicroServerRequestHandlerFactory microserverRequestHandlerFactory{
      requestHandlers, true, "/status"};

  MockHTTPRequest mockHTTPRequest;
  NiceMock<MockHTTPResponse> mockHTTPResponse;
  std::stringstream defaultStream;
  DefaultValue<std::ostream &>::Set(defaultStream);

  HTTPRequestHandler *httpRequestHandler =
      microserverRequestHandlerFactory.createRequestHandler(mockHTTPRequest);
  httpRequestHandler->handleRequest(mockHTTPRequest, mockHTTPResponse);
  EXPECT_EQ(HTTPResponse::HTTPStatus::HTTP_NOT_FOUND,
            mockHTTPResponse.getStatus());
}

} // end namespace MicroServerRequestHandlerFactoryTest
