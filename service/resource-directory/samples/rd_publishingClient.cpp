//******************************************************************
//
// Copyright 2015 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <iostream>

#include "OCPlatform.h"
#include "OCApi.h"
#include "oic_string.h"

#include "rd_client.h"

using namespace OC;

OCResourceHandle g_curResource_t = NULL;
OCResourceHandle g_curResource_l = NULL;
char rdAddress[MAX_ADDR_STR_SIZE];
uint16_t rdPort;

bool waitRdDiscovery = false;

OCEntityHandlerResult entityHandlerCb(std::shared_ptr<OCResourceRequest> request)
{
    std::cout << "Inside eneityHandlerCb" << std::endl;

    OCEntityHandlerResult ehResult = OC_EH_ERROR;

    if(request)
    {
        std::cout << "Request for uri: " << request->getResourceUri() << std::endl;
        std::string requestType = request->getRequestType();
        int requestFlag = request->getRequestHandlerFlag();

        if(requestFlag & RequestHandlerFlag::RequestFlag)
        {
            std::cout << "\t\trequestFlag : Request\n";
            auto pResponse = std::make_shared<OC::OCResourceResponse>();
            pResponse->setRequestHandle(request->getRequestHandle());
            pResponse->setResourceHandle(request->getResourceHandle());

            // Check for query params (if any)
            QueryParamsMap queries = request->getQueryParameters();

            if (!queries.empty())
            {
                std::cout << "\nQuery processing upto entityHandler" << std::endl;
            }
            for (auto it : queries)
            {
                std::cout << "Query key: " << it.first << " value : " << it.second
                        << std:: endl;
            }

            OCRepresentation rep;
            std::string name("Mark's Light");
            rep.setValue("state", true);
            rep.setValue("power", (int)100);
            rep.setValue("name", name);


            // If the request type is GET
            if(requestType == "GET")
            {
                /*if(!initialized)
                {
                    OCStackResult res = OCPlatform::bindTypeToResource(request->getResourceHandle(),
                                                        "oic.r.light.dimming");
                    if(res != OC_STACK_OK)
                    {
                        std::cout << "Error binding type" << std::endl;
                    }
                }*/

            }

            if(requestType == "POST")
            {
                std::cout << "RequestType is POST" << std::endl;

            }

            if(requestType == "PUT")
            {
                std::cout << "requestType is PUT" << std::endl;
            }

            pResponse->setErrorCode(200);
            pResponse->setResponseResult(OC_EH_OK);
            pResponse->setResourceRepresentation(rep);
            if(OC_STACK_OK == OCPlatform::sendResponse(pResponse))
            {
                ehResult = OC_EH_OK;
            }
        }
    }
    return ehResult;
}


void registerLocalResources()
{
    std::string resourceURI_light = "/a/light";
    std::string resourceTypeName_light = "oic.d.light";
    std::string resourceInterface = DEFAULT_INTERFACE;
    uint8_t resourceProperty = OC_DISCOVERABLE | OC_OBSERVABLE;

    EntityHandler cb = std::bind(&entityHandlerCb, std::placeholders::_1);

    OCStackResult result = OCPlatform::registerResource(g_curResource_l,
                                          resourceURI_light,
                                          resourceTypeName_light,
                                          resourceInterface,
                                          cb,
                                          resourceProperty);


    if (OC_STACK_OK != result)
    {
        throw std::runtime_error(
            std::string("Device Resource failed to start") + std::to_string(result));
    }
}

void printHelp()
{
    std::cout << std::endl;
    std::cout << "********************************************" << std::endl;
    std::cout << "*  method Type : 1 - Discover RD           *" << std::endl;
    std::cout << "*  method Type : 2 - Publish               *" << std::endl;
    std::cout << "*  method Type : 3 - Update                *" << std::endl;
    std::cout << "*  method Type : 4 - Delete                *" << std::endl;
    std::cout << "*  method Type : 5 - Status                *" << std::endl;
    std::cout << "********************************************" << std::endl;
    std::cout << std::endl;
}

int biasFactorCB(char addr[MAX_ADDR_STR_SIZE], uint16_t port)
{
    OICStrcpy(rdAddress, MAX_ADDR_STR_SIZE, addr);
    rdPort = port;
    std::cout << "RD Address is : " <<  addr << ":" << port << std::endl;
    waitRdDiscovery = true;
    return 0;
}

int main()
{
    int in;
    PlatformConfig cfg;

    OCPlatform::Configure(cfg);

    std::cout << "Created Platform..." << std::endl;

    try
    {
        registerLocalResources();
    }
    catch (std::runtime_error e)
    {
        std::cout << "Caught OCException [Code: " << e.what() << std::endl;
    }

    while (1)
    {
        sleep(2);

        if (g_curResource_l == NULL)
        {
            std::cout << "Fail\n";
            continue;
        }

        std::cout << "Trying to discover " << std::endl;
        try
        {
            OCRDDiscover(biasFactorCB);
            while(!waitRdDiscovery)
            {
                std::cout << "Retrying.." << std::endl;
                OCRDDiscover(biasFactorCB);
                sleep(1);
            }

            std::cout << "Found RD!" << std::endl;

            while(OCProcess() == OC_STACK_OK)
            {
                sleep(0.1);
            }
            return 0;

        }
        catch (OCException e)
        {
            std::cout << "Caught OCException [Code: " << e.code() << " Reason: " << e.reason() << std::endl;
        }
    }
    return 0;
}
