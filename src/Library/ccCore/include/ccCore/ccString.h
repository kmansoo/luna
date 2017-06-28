/*
 * ccString.h
 *
 *  Created on: 2015. 11. 8.
 *      Author: Mansoo Kim(kmansoo@gmail.com)
 */

#ifndef CCLIBRARY_CCCOREAPI_CCSTRING_H_
#define CCLIBRARY_CCCOREAPI_CCSTRING_H_

#include <string>
#include <vector>

namespace Luna {

class ccString : public std::string {
public:
    ccString();
    ccString(const char* strInitData);
    ccString(const char* strFirst, const char* strLast);
    ccString(const ccString& _Right);
    ccString(const std::string& _Right);

    virtual ~ccString();

public:
    ccString& operator=(const ccString& _Right) {   // assign _Right
        std::string::assign(_Right.c_str());

        return *this;
    }

    ccString& operator=(const std::string& _Right) {   // assign _Right
        std::string::assign(_Right.c_str());

        return *this;
    }

    ccString& operator=(const char *_Ptr) {   // assign [_Ptr, <null>)
        std::string::assign(_Ptr);

        return *this;
    }

    ccString& operator=(char _Ch) {   // assign 1 * _Ch
        std::string::assign(1, _Ch);

        return *this;
    }

public:
    void            replace_string_in_place(const ccString& search, const ccString& replace);

    void            make_upper();
    void            make_lower();

    void            trim();
    void            trim_left();
    void            trim_right();

    unsigned long   decode_hex(int *length = NULL);

public:
    // Supports up to 512 bytes
    static  void    format(std::string& destStr, const char* format, ...);
    static void     replace(std::string& destStr, const ccString& search, const ccString& replace);

    static  long    decode_string_to_hex(const char* strVal, int nLength = -1, int* pConvertedSize = NULL);
    static  bool    encode_base64(std::string& destStr, std::string& src);
    static  bool    decode_base64(std::string& destStr, std::string& src);

    // 2017.6.28, Mansoo(mansoo.kim@icloud.com), Added this method to support spliting string using token
    static std::vector<std::string> splitToken(const std::string& source, char token_char);

private:
    enum CONST_VALUE {
        kMaxFormatBufferSize = 512
    };
};

};

#endif /* CCLIBRARY_CCCOREAPI_CCSTRING_H_ */
