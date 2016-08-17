/*
 * ccWebServer.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_

#include <vector>
#include <string>
#include <memory>

#include "ccWebServerEventListener.h"
#include "ccWebServerPageDirectory.h"

namespace Luna {

class ccWebServer
{
public:
    ccWebServer(const std::string& name, const std::string& ports, const std::string& root_path, std::shared_ptr<ccWebServerPageDirectory> page_directory = nullptr);
    virtual ~ccWebServer();

public:
    virtual bool start();
    virtual bool stop();

    virtual void setUploadEvent(const std::string& path) {}

public:
    const std::string&  getName();

    bool    setOption(std::string const& name, std::string const& value);
    void    setListener(ccWebServerEventListener* pListener);

protected:
    //  this function performs like a initiation of web page or features for web server.
    virtual void initServer();

    const std::vector<std::string>& doGetOptions() {
        return options_;
    }

    std::shared_ptr<ccWebServerPage> findPage(const std::string& uri);
    std::shared_ptr<ccWebServerFileUploadPage> findFileUploadPage(const std::string& uri);
    std::shared_ptr<ccWebServerFileDownloadPage> findFileDownloadPage(const std::string& uri);

protected:
    std::string                 name_;
    std::string                 httpPorts_;
    std::string                 webRootPath_;
    ccWebServerEventListener*   eventListener_;
    std::vector<std::string>    options_;

    std::shared_ptr<ccWebServerPageDirectory> _pageDirectory;
};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVER_H_ */
