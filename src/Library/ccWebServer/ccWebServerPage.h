/*
 * ccWebServerPage.h
 *
 *  Created on: 2016. 8. 12.
 *      Author: Mansoo Kim
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERPAGE_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERPAGE_H_

#include <string>
#include <memory>

#include "ccWebServerRequest.h"
#include "ccWebServerResponse.h"

namespace Luna {

    class ccWebServerPage
    {
    public:
        ccWebServerPage(const std::string& path) { _strPath = path; }
        ccWebServerPage(const std::string& path, const std::string& name) { _strPath = path; _strFileName = name; }
        virtual ~ccWebServerPage() {}

    public:
        const std::string&  getPath() const { return _strPath; }
        const std::string&  getFileName() const { return _strFileName; }
        const std::string&  geMimeType() const { return _strMimeType; }

    public:
        virtual bool        isUploadPage() { return false; }
        virtual bool        isDownloadPage() { return false; }

        virtual bool        process(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response) { return false; }
        virtual const char* getContent() { return NULL; }
        virtual size_t      getContentLength() const { return 0; }

    protected:
        std::string     _strPath;
        std::string     _strFileName;
        std::string     _strMimeType;
    };
}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERPAGE_H_ */
