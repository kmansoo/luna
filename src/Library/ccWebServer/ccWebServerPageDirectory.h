/*
* ccWebServerPageDirectory.h
*
*  Created on: 2016. 8. 12.
*      Author: Mansoo Kim
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERPAGEDIRECTORY_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERPAGEDIRECTORY_H_

#include <unordered_map>

#include "ccWebServerPage.h"
#include "ccWebServerRequest.h"
#include "ccWebServerResponse.h"
#include "ccWebServerFileUploadPage.h"
#include "ccWebServerFileDownloadPage.h"

namespace Luna {
    class ccWebServer;

    class ccWebServerPageDirectory
    {
    public:
        ccWebServerPageDirectory();
        virtual ~ccWebServerPageDirectory();

    public:
        void    init(ccWebServer& server);

        bool    registerPage(std::shared_ptr<ccWebServerPage> page);
        bool    unregisterPage(const std::string& path);

        bool    processRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response);

        std::shared_ptr<ccWebServerPage> findPage(const std::string& uri);
        std::shared_ptr<ccWebServerFileUploadPage> findFileUploadPage(const std::string& uri);
        std::shared_ptr<ccWebServerFileDownloadPage> findFileDownloadPage(const std::string& uri);

    private:
        std::unordered_map<std::string, std::shared_ptr<ccWebServerPage>>   pages_;
    };
}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERPAGEDIRECTORY_H_ */
