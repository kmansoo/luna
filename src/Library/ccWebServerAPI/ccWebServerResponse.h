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

using namespace std;

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
    void    Status(unsigned int code, const string& strStatusText);
    void    Status(unsigned int code, const string& strStatusText, const string& strExtInfo);

    void    ContentType(const string& type, bool bInsertSeparator = true);
    void    ContentType(const string& type, size_t size, bool bInsertSeparator = true);

    void    HeaderInfo(const string& type, size_t size, string& strExtInfo, bool bInsertSeparator = true);

    size_t  Printf(const char *fmt, ...);
    size_t  VPrintf(const char *fmt, va_list ap);
    size_t  Write(const string& buf);
    size_t  Write(const char* strBuf, size_t size);
    size_t  Write(istream& is);

    bool    NotFoundFile(const string& strURI);

protected:
    unsigned int    _uStatudCode;
    string          _strStatusText;
    string          _strExtInfo;

protected:
    bool    _bStatusSet;
    bool    _bContentTypeSet;
    bool    _bContentLengthSet;
    bool    _bHeadersSent;

};

#endif /* CCLIBRARY_CCWEBSERVERAPI_CCWEBSERVERRESPONSE_H_ */
