/*
* ccWebServerFileDownloadPage.h
*
*  Created on: 2016. 8. 12.
*      Author: Mansoo Kim
*/

#pragma once

#include "ccWebServerPage.h"

namespace Luna {

class ccWebServerFileDownloadPage : public ccWebServerPage {
public:
    ccWebServerFileDownloadPage(const std::string& path) : ccWebServerPage(path) {}
    virtual ~ccWebServerFileDownloadPage() {}

public:
    virtual bool        is_download_page() { return true; }
};

}
