/*
 * ccWebServerResponse.h
 *
 *  Created on: 2015. 11. 7.
 *      Author: kmansoo
 */

#ifndef CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERRESPONSE_H_
#define CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERRESPONSE_H_

#include <stdarg.h>

#include <string>
#include <iostream>

namespace Luna {

class ccWebServerResponse
{
public:
    ccWebServerResponse();
    virtual ~ccWebServerResponse();

public:
    enum ConstValue {
        kMaxBufferSize  =   8192
    };

protected:
    virtual size_t DoWriteContentToConnector(const char* strBuf, size_t size) {return 0;}

protected:
    void    DoCheckHeadersSent();
    size_t  DoPrintf(const char *fmt, ...);

public:
    void    Status(unsigned int code, const std::string& strStatusText);
    void    Status(unsigned int code, const std::string& strStatusText, const std::string& strExtInfo);

    void    ContentType(const std::string& type, bool bInsertSeparator = true);
    void    ContentType(const std::string& type, size_t size, bool bInsertSeparator = true);

    void    HeaderInfo(const std::string& type, size_t size, std::string& strExtInfo, bool bInsertSeparator = true);

    size_t  Printf(const char *fmt, ...);
    size_t  VPrintf(const char *fmt, va_list ap);
    size_t  Write(const std::string& buf);
    size_t  Write(const char* strBuf, size_t size);
    size_t  Write(std::istream& is);

    bool    NotFoundFile(const std::string& strURI);

protected:
    unsigned int    _uStatudCode;
    std::string     _strStatusText;
    std::string     _strExtInfo;

protected:
    bool    _bStatusSet;
    bool    _bContentTypeSet;
    bool    _bContentLengthSet;
    bool    _bHeadersSent;

};

}

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERRESPONSE_H_ */
