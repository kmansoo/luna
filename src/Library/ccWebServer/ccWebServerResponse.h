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
    virtual size_t doWriteContentToConnector(const char* strBuf, size_t size) {return 0;}

protected:
    void    doCheckHeadersSent();
    size_t  doPrintf(const char *fmt, ...);

public:
    void    status(unsigned int code, const std::string& strStatusText);
    void    status(unsigned int code, const std::string& strStatusText, const std::string& strExtInfo);

    void    contentType(const std::string& type, bool bInsertSeparator = true);
    void    contentType(const std::string& type, size_t size, bool bInsertSeparator = true);

    void    headerInfo(const std::string& type, size_t size, std::string& strExtInfo, bool bInsertSeparator = true);

    size_t  printf(const char *fmt, ...);
    size_t  vprintf(const char *fmt, va_list ap);
    size_t  write(const std::string& buf);
    size_t  write(const char* strBuf, size_t size);
    size_t  write(std::istream& is);

    bool    notFoundFile(const std::string& strURI);

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
