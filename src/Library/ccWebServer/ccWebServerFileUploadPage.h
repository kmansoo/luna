/*
* ccWebServerFileUploadPage.h
*
*  Created on: 2016. 8. 12.
*      Author: Mansoo Kim
*/

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERFILEUPLOADLOADPAGE_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERFILEUPLOADLOADPAGE_H_

#include <mutex>
#include <unordered_map>

#include "ccWebServerPage.h"

#include <fstream>

namespace Luna {

    class ccWebServerFileUploadPage;

    class ccWebServerFileUploadAgent 
    {
    public:
        ccWebServerFileUploadAgent(std::uint32_t id, std::string localFilePath, ccWebServerFileUploadPage* parent);
        ~ccWebServerFileUploadAgent();

    public:
        std::uint32_t   getID() { return id_; }
        ccWebServerFileUploadPage* getParent() { return parent_; }

        bool    onFileInfo(const std::string& filename, size_t file_size);
        bool    onReceiveData(const char* data, size_t size);
        bool    onFinishUpload();

    protected:
        std::uint32_t   id_;
        ccWebServerFileUploadPage*  parent_;

        std::string     localFilePath_;

        std::string     uploadFilename_;
        std::uint32_t   uploadFileLength_;
        std::uint32_t   receivedDataLength_;

        std::ofstream   uploadFile_;
    };

    class ccWebServerFileUploadPage : public ccWebServerPage
    {
    public:
        ccWebServerFileUploadPage(const std::string& path, bool singleAgent = true);
        ccWebServerFileUploadPage(const std::string& path, const std::string& local_file_path, bool singleAgent = true);

        virtual ~ccWebServerFileUploadPage() {}

    public:
        virtual bool    isUploadPage() { return true; }

    public:
        ccWebServerFileUploadAgent* creteFileUploadAgent();
        void                        destroyFileUploadAgent(ccWebServerFileUploadAgent* agent);

    private:
        bool singleAgent_;

        std::mutex mtx_;
        std::uint32_t generate_id_;
        std::string localFilePath_;

        std::unordered_map<std::uint32_t, std::shared_ptr<ccWebServerFileUploadAgent>> agents_;
    };

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERFILEUPLOADLOADPAGE_H_ */
