/*
 * ccRESTfulWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include <iostream>
#include <functional>

#include "ccJsonParserAPI/json/value.h"
#include "ccJsonParserAPI/json/writer.h"

#include "../ccRESTfulChatting.h"

ccRESTfulChatting::ccRESTfulChatting()
{
    // TODO Auto-generated constructor stub
    AddAPI(std::string("/list"), std::bind(&ccRESTfulChatting::list, this, std::placeholders::_1, std::placeholders::_2));
    AddAPI(std::string("/session"), std::bind(&ccRESTfulChatting::session, this, std::placeholders::_1, std::placeholders::_2));
    AddAPI(std::string("/session_member"), std::bind(&ccRESTfulChatting::session_member, this, std::placeholders::_1, std::placeholders::_2));
    AddAPI(std::string("/session_message"), std::bind(&ccRESTfulChatting::session_message, this, std::placeholders::_1, std::placeholders::_2));
}

ccRESTfulChatting::~ccRESTfulChatting()
{
    // TODO Auto-generated destructor stub
}

bool ccRESTfulChatting::list(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    std::cout << "enter ccRESTfulChatting::list, query_string=" << pRequest->GetQueryString() << endl;

    switch (pRequest->GetMethod())
    {
    case ccWebServerRequest::HttpMethod_Get:
        {
            pResponse->Status(200, string("OK)"));

            Json::Value oResponseRPC;
            Json::Value oUserList;
            Json::StyledWriter oWriter;
            
            oResponseRPC["jsonrpc"] = "2.0";
            oResponseRPC["id"]      = 1;

            oUserList["count"] = 2;
            oUserList["info"][0]["id"] = "Neo";
            oUserList["info"][0]["Name"] = "Kim Mansoo";
            oUserList["info"][1]["id"] = "AppleHJ";
            oUserList["info"][1]["Name"] = "Kim Hyunjun";

            oResponseRPC["user_list"] = oUserList;

            std::string strJsonRPC2 = oWriter.write(oResponseRPC);

            pResponse->ContentType("application/javascript", strJsonRPC2.length());
            pResponse->Write(strJsonRPC2);

            return true;
        }

        break;

    case ccWebServerRequest::HttpMethod_Post:
        break;
    }

    return false;
}

bool ccRESTfulChatting::session(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    std::cout << "enter ccRESTfulChatting::session, query_string=" << pRequest->GetQueryString() << endl;

    return false;
}

bool ccRESTfulChatting::session_member(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    std::cout << "enter ccRESTfulChatting::session_member, query_string=" << pRequest->GetQueryString() << endl;

    return false;
}

bool ccRESTfulChatting::session_message(std::shared_ptr<ccWebServerRequest> pRequest, std::shared_ptr<ccWebServerResponse> pResponse)
{
    std::cout << "enter ccRESTfulChatting::session_message, query_string=" << pRequest->GetQueryString() << endl;

    return false;
}
