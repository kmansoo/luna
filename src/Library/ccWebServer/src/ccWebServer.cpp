/*
 * ccWebServer.cpp
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#include "../ccWebServer.h"

namespace Luna {

    ccWebServer::ccWebServer(const std::string& name, const std::string& ports, const std::string& root_path, std::shared_ptr<ccWebServerPageDirectory> page_directory) :
        name_(name),
        httpPorts_(ports),
        webRootPath_(root_path),
        eventListener_(NULL),
        _pageDirectory(page_directory)
    {
        // TODO Auto-generated constructor stub

    }

    ccWebServer::~ccWebServer()
    {
        // TODO Auto-generated destructor stub
    }

    bool ccWebServer::start()
    {
        return false;
    }

    bool ccWebServer::stop()
    {
        return false;
    }

    void ccWebServer::initServer()
    {
        if (_pageDirectory)
            _pageDirectory->init(*this);
    }

    const std::string&  ccWebServer::getName()
    {
        return name_;
    }

    bool ccWebServer::setOption(std::string const& name, std::string const& value)
    {
        options_.push_back(name);
        options_.push_back(value);

        return true;
    }

    void ccWebServer::setListener(ccWebServerEventListener* pListener)
    {
        eventListener_ = pListener;
    }

    std::shared_ptr<ccWebServerPage> ccWebServer::findPage(const std::string& uri)
    {
        if (!_pageDirectory)
            return nullptr;

        return _pageDirectory->findPage(uri);
    }

    std::shared_ptr<ccWebServerFileUploadPage> ccWebServer::findFileUploadPage(const std::string& uri)
    {
        if (!_pageDirectory)
            return nullptr;

        return _pageDirectory->findFileUploadPage(uri);
    }

    std::shared_ptr<ccWebServerFileDownloadPage> ccWebServer::findFileDownloadPage(const std::string& uri)
    {
        if (!_pageDirectory)
            return nullptr;

        return _pageDirectory->findFileDownloadPage(uri);
    }
}
