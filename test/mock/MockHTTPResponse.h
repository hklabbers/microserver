// Copyright (c) March 2016 Hans Klabbers
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Redistributions in any form must be accompanied by information on
//    how to obtain complete source code for this software and any
//    accompanying software that uses this software.  The source code
//    must either be included in the distribution or be available for no
//    more than the cost of distribution plus a nominal fee, and must be
//    freely redistributable under reasonable conditions.  For an
//    executable file, complete source code means the source code for all
//    modules of this software it contains.  It does not include source code
//    for modules or files that typically accompany the major components
//    of the operating system on which the executable file runs.
//
// THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// NON-INFRINGEMENT, ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
// IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This software links against software from the Poco Project which is
// distributed under the Boost Software License 1.0. Please visit the
// following url for more information: pocoproject.org/license.html.

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

        MOCK_METHOD0(send, std::ostream & ());

        MOCK_METHOD2(sendBuffer, void(const void *, std::size_t));

        MOCK_METHOD0(sendContinue, void());

        MOCK_METHOD2(sendFile, void(const std::string &, const std::string &));

        MOCK_CONST_METHOD0(sent, bool());
    };

} // end namespace microserver_mock
#endif // __MOCKHTTPRESPONSE_H__
