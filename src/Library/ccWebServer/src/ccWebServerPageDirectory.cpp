/*
 * ccWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include "../ccWebServerPageDirectory.h"
#include "../ccWebServer.h"

namespace Luna {

    ccWebServerPageDirectory::ccWebServerPageDirectory()
    {
        // TODO Auto-generated constructor stub

    }

    ccWebServerPageDirectory::~ccWebServerPageDirectory()
    {
        // TODO Auto-generated destructor stub
    }

    void ccWebServerPageDirectory::init(ccWebServer& server)
    {
        for (auto it : pages_)
        {
            if (it.second->isUploadPage())
                server.setUploadEvent(it.second->getPath());
        }
    }

    bool ccWebServerPageDirectory::registerPage(std::shared_ptr<ccWebServerPage> page)
    {
        if (page == nullptr)
            return false;

        auto item = pages_.find(page->getPath());

        if (item != pages_.end())
            return false;

        pages_[page->getPath()] = page;

        return true;
    }

    bool ccWebServerPageDirectory::unregisterPage(const std::string& path)
    {
        auto item = pages_.find(path);

        if (item == pages_.end())
            return false;

        pages_.erase(path);

        return true;
    }

    bool ccWebServerPageDirectory::processRequest(std::shared_ptr<ccWebServerRequest> request, std::shared_ptr<ccWebServerResponse> response)
    {
        return false;
    }

    std::shared_ptr<ccWebServerPage> ccWebServerPageDirectory::findPage(const std::string& uri)
    {
        auto item = pages_.find(uri);

        if (item == pages_.end())
            return nullptr;

        return item->second;
    }

    std::shared_ptr<ccWebServerFileUploadPage> ccWebServerPageDirectory::findFileUploadPage(const std::string& uri)
    {
        auto item = pages_.find(uri);

        if (item == pages_.end())
            return nullptr;

        return  std::dynamic_pointer_cast<ccWebServerFileUploadPage>(item->second);
    }

    std::shared_ptr<ccWebServerFileDownloadPage> ccWebServerPageDirectory::findFileDownloadPage(const std::string& uri)
    {
        auto item = pages_.find(uri);

        if (item == pages_.end())
            return nullptr;

        return std::dynamic_pointer_cast<ccWebServerFileDownloadPage>(item->second);
    }
}
