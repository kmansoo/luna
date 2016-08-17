/*
* ccWebServerFileDownloadPage.h
*
*  Created on: 2016. 8. 12.
*      Author: Mansoo Kim
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERFILEDOWNLOADPAGE_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERFILEDOWNLOADPAGE_H_

#include "ccWebServerPage.h"

namespace Luna {

    class ccWebServerFileDownloadPage : public ccWebServerPage
    {
    public:
        ccWebServerFileDownloadPage(const std::string& path) : ccWebServerPage(path) {}
        virtual ~ccWebServerFileDownloadPage() {}

    public:
        virtual bool        isDownloadPage() { return true; }
    };
}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERFILEDOWNLOADPAGE_H_ */
