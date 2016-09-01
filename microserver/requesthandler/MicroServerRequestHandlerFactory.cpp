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

#include "MicroServerRequestHandlerFactory.h"
#include "AbstractRequestHandler.h"
#include "Error404RequestHandler.h"
#include "StatusRequestHandler.h"
#include "Poco/Util/Application.h"
#include "Poco/ClassLoader.h"
#include "Poco/Manifest.h"
#include "boost/algorithm/string/predicate.hpp"

#include <iostream>

using Poco::ClassLoader;
using Poco::Manifest;

MicroServerRequestHandlerFactory::MicroServerRequestHandlerFactory(
        std::vector<RequestHandlerDefinition> &requestHandlers, bool lazyLoading, std::string loadURI)
        : requestHandlers(requestHandlers), lazyLoading(lazyLoading), loadURI(loadURI) {

    if (!lazyLoading) {
        loadAllLibraries() ? l.information("All configured libraries are loaded (lazyLoading is false).") : l.error(
                "Error while loading all libraries (lazyLoading is false.");
    }
}

HTTPRequestHandler *MicroServerRequestHandlerFactory::createRequestHandler(const HTTPServerRequest &request) {
    l.information("request.getURI(): " + request.getURI() + " from: " + request.clientAddress().toString());

    if (boost::starts_with(request.getURI(), loadURI)) {
        return new StatusRequestHandler();
    }

    for (auto requestHandler : requestHandlers) {
        if (boost::starts_with(request.getURI(), requestHandler.getUri())) {
            l.debug("path: " + requestHandler.getLibraryNameWithPathAndSuffix());
            auto libraryLoadedSuccessful = classLoader.isLibraryLoaded(
                    requestHandler.getLibraryNameWithPathAndSuffix());
            if (!libraryLoadedSuccessful) {
                libraryLoadedSuccessful = loadLibrary(requestHandler.getLibraryNameWithPathAndSuffix());
            }
            if (libraryLoadedSuccessful) {
                try {
                    l.debug("Trying to load class: " + requestHandler.getClassName());
                    AbstractRequestHandler *provider = classLoader.create(requestHandler.getClassName());
                    return provider->getRequestHandler(request.getURI());
                } catch (Poco::NotFoundException &ex) {
                    l.error(ex.message());
                }
            }
        }
    }
    return new Error404RequestHandler();
}

bool MicroServerRequestHandlerFactory::loadAllLibraries() {
    bool noErrors {true};
    for (auto requestHandler : requestHandlers) {
        if (!classLoader.isLibraryLoaded(requestHandler.getLibraryNameWithPathAndSuffix())) {
            noErrors = loadLibrary(requestHandler.getLibraryNameWithPathAndSuffix());
            if (!noErrors) {
                l.error("Failed to load a library, micorserver stops loading libraries.");
                break;
            }
        }
    }
    return noErrors;
}

bool MicroServerRequestHandlerFactory::loadLibrary(std::string libraryName) {
    bool noErrors {true};
    try {
        if (!classLoader.isLibraryLoaded(libraryName)) {
            l.information("Loading library: " + libraryName);
            classLoader.loadLibrary(libraryName);
        }
    } catch (Poco::LibraryLoadException &ex) {
        l.error("Error loading library " + libraryName + " (" + ex.message() + ").");
        noErrors = false;
    }
    return noErrors;
}
