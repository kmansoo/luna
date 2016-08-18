/*
* ccWebServerFileUploadPage.cpp
*
*  Created on: 2016. 8. 12.
*      Author: Mansoo Kim
*/

#include "../ccWebServerFileUploadPage.h"

namespace Luna {


    // class ccWebServerFileUploadAgent
    ccWebServerFileUploadAgent::ccWebServerFileUploadAgent(std::uint32_t id, std::string localFilePath, ccWebServerFileUploadPage* parent)
        : id_(id), parent_(parent), localFilePath_(localFilePath)
    {
        uploadFileLength_ = 0;
        receivedDataLength_ = 0;
    }

    ccWebServerFileUploadAgent::~ccWebServerFileUploadAgent() 
    {
        onFinishUpload();
    }

    bool ccWebServerFileUploadAgent::onFileInfo(const std::string& filename, size_t file_size)
    {
        std::string open_filename = filename;

        if (open_filename.length() == 0)
        {
            open_filename = localFilePath_;

            if (open_filename.length() == 0)
                open_filename = "noname_uploadfile.dat";
        }

        uploadFile_.open(open_filename, std::ios_base::binary);

        return uploadFile_.is_open();
    }

    bool ccWebServerFileUploadAgent::onReceiveData(const char* data, size_t size)
    {
        if (!uploadFile_.is_open())
            return false;

        uploadFile_.write(data, size);

        return true;
    }

    bool ccWebServerFileUploadAgent::onFinishUpload()
    {
        if (!uploadFile_.is_open())
            return false;

        uploadFile_.close();

        return true;
    }

    // class ccWebServerFileUploadPage
    ccWebServerFileUploadPage::ccWebServerFileUploadPage(const std::string& path, bool singleAgent) : ccWebServerPage(path), generate_id_(0x00)
    {
    }

    ccWebServerFileUploadPage::ccWebServerFileUploadPage(const std::string& path, const std::string& local_file_path, bool singleAgent) :
        ccWebServerPage(path), generate_id_(0x00), localFilePath_(local_file_path)
    {
    }

    ccWebServerFileUploadAgent* ccWebServerFileUploadPage::creteFileUploadAgent()
    {
        if (singleAgent_ && agents_.size() != 0)
            return nullptr;

        mtx_.lock();
        generate_id_++;
        mtx_.unlock();

        std::shared_ptr<ccWebServerFileUploadAgent> newAgent = std::make_shared<ccWebServerFileUploadAgent>(generate_id_, localFilePath_, this);

        agents_[generate_id_] = newAgent;

        return newAgent.get();
    }

    void ccWebServerFileUploadPage::destroyFileUploadAgent(ccWebServerFileUploadAgent* agent)
    {
        if (agent)
            agents_.erase(agent->getID());
    }
}
