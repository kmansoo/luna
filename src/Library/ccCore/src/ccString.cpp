/*
 * ccString.cpp
 *
 *  Created on: 2015. 11. 8.
 *      Author: kmansoo
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "../ccString.h"

//  static function
static char            b64_encode(unsigned char u);
static unsigned char   b64_decode(char c);
static int             is_base64(char c);

ccString::ccString()
{
    // TODO Auto-generated constructor stub

}

ccString::ccString(const char* strInitData)
{
    if (strInitData != NULL)
        assign(strInitData);
}

ccString::ccString(const char* strFirst, const char* strLast)
{
    if (strFirst != NULL && strLast != NULL)
    {
        if (strFirst != strLast)
            assign(&*strFirst, strLast - strFirst);
    }
    else
    {

    }
}

ccString::ccString(const ccString& _Right)
{
    std::string::append(_Right);
}

ccString::~ccString()
{
    // TODO Auto-generated destructor stub
}


void  ccString::ReplaceStringInPlace(const ccString& search, const ccString& replace)
{
    ccString::replace(*this, search, replace);
}

unsigned long ccString::DecodeHex(int *pConvertedSize)
{
    return decodeStringToHex(c_str(), length(), pConvertedSize);
}


void ccString::MakeUpper()
{
    for (size_t nIndex = 0; nIndex < length(); nIndex++)
        at(nIndex) = toupper(at(nIndex));
}

void ccString::MakeLower()
{
    for (size_t nIndex = 0; nIndex < length(); nIndex++)
        at(nIndex) = tolower(at(nIndex));
}

void ccString::Trim()
{
    TrimLeft();
    TrimRight();
}

void ccString::TrimLeft()
{
    erase(0, find_first_not_of(' '));
}

void ccString::TrimRight()
{
    erase(find_last_not_of(' ') + 1);
}

//  static method
long ccString::decodeStringToHex(const char* strVal, int nLength, int* pConvertedSize)
{
    int nConvertedSize  = 0;
    int nOffset         = 0;
    int nStartPos       = 0;

    if (nLength == -1)
    {
        if (strVal == NULL)
            nLength = 0;
        else
            nLength = strlen(strVal);
    }

    for (int nIndex = 0; nIndex < nLength; nIndex++)
    {
        char c = *(strVal + nIndex);

        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
            nConvertedSize++;
        else
        {
            if (c == 'X' || c == 'x')
            {
                nStartPos = nIndex + 1;
                nConvertedSize = 0;
            }
            else
                return 0;
        }
    }

    if (pConvertedSize != NULL)
        *pConvertedSize = nConvertedSize;

    unsigned long result = 0;

    int nActualLength = nLength - nStartPos;

    for (int nIndex = 0; nIndex < nActualLength; nIndex++)
    {
        nOffset = nStartPos + (nActualLength - nIndex) - 1;

        if (nOffset >= nLength)
            break;

        char byChar = *(strVal + nOffset);

        int n = 1 << (nIndex * 4);

        if ((byChar >= '0' && byChar <= '9'))
            result += n * (byChar - '0');
        else
        {
            if ((byChar >= 'A' && byChar <= 'F'))
                result += n * (byChar - 'A' + 10);
            else
            {
                if ((byChar >= 'a' && byChar <= 'f'))
                    result += n * (byChar - 'a' + 10);
            }
        }
    }

    return result;
}

//  Supports up to 512 bytes
void ccString::format(std::string& destStr, const char* format, ...)
{
    char strFormatBuffer[CV_MAX_FORMAT_BUFFER_SIZE];

    va_list args;

    va_start(args, format);

    //  Guess we need no more than CV_MAX_FORMAT_BUFFER_SIZE bytes.
    int nAllocSize, size = CV_MAX_FORMAT_BUFFER_SIZE;

    strFormatBuffer[0] = '\0';

#ifdef _WIN32
    nAllocSize = _vsnprintf (strFormatBuffer, size, format, args);
#else
    nAllocSize = vsnprintf(strFormatBuffer, size, format, args);
#endif

    va_end(args);

    strFormatBuffer[nAllocSize] = '\0';

    destStr.assign(strFormatBuffer);
}

void ccString::replace(std::string& destStr, const ccString& search, const ccString& replace)
{
    size_t pos = 0;

    while ((pos = destStr.find(search, pos)) != ccString::npos)
    {
        destStr.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

/**
 * @fn char *base64_encode(int size, const unsigned char *src)
 * @brief Base64 encode and return size data in 'src'. The caller must free the
 * returned string.
 *
 * @param dest
 * @param src
 * @return
 */
bool ccString::encodeBase64(std::string& dest, std::string& src)
{
    if (src.length() == 0)
        return false;

    int size = src.length();

    dest.clear();

    for (int i = 0; i < size; i += 3)
    {
        unsigned char b1=0, b2=0, b3=0, b4=0, b5=0, b6=0, b7=0;
        b1= src[i];

        if(i+1<size) b2= src[i+1];
        if(i+2<size) b3= src[i+2];

        b4= b1>>2;
        b5= ((b1&0x3)<<4)|(b2>>4);
        b6= ((b2&0xf)<<2)|(b3>>6);
        b7= b3&0x3f;

        dest += b64_encode(b4);
        dest += b64_encode(b5);

        if(i+1<size) dest += b64_encode(b6);
        else dest += '=';

        if(i+2<size) dest += b64_encode(b7);
        else dest += '=';
    }

    return true;
}


/**
 * @fn int base64_decode(unsigned char *dest, const char *src)
 * @brief Decode the base64 encoded string 'src' into the memory pointed to by 'dest'.
 *
 * @param dest
 * @param src
 * @return
 */
bool ccString::decodeBase64(std::string& dest, std::string& src)
{
    if (src.length() == 0)
        return false;

    int size = src.length();

    dest.clear();

    for (int k=0; k<size; k+=4)
    {
        char c1='A', c2='A', c3='A', c4='A';
        unsigned char b1=0, b2=0, b3=0, b4=0;
        c1= src[k];

        if(k+1<size) c2= src[k+1];
        if(k+2<size) c3= src[k+2];
        if(k+3<size) c4= src[k+3];

        if(!is_base64(c1) || !is_base64(c1) || !is_base64(c1) || !is_base64(c1))
            return false;

        b1= b64_decode(c1);
        b2= b64_decode(c2);
        b3= b64_decode(c3);
        b4= b64_decode(c4);

        dest += ((b1<<2)|(b2>>4) );

        if(c3 != '=') dest += (((b2&0xf)<<4)|(b3>>2) );
        if(c4 != '=') dest += (((b3&0x3)<<6)|b4 );
    }

    return true;
}


/**
 * @fn static char b64_encode(unsigned char u)
 * @brief Base64 encode one byte
 *
 * @param u
 * @return
 */
static char b64_encode(unsigned char u)
{
    if(u < 26)  return 'A'+u;
    if(u < 52)  return 'a'+(u-26);
    if(u < 62)  return '0'+(u-52);
    if(u == 62) return '+';
    return '/';
}


/**
 * @fn static unsigned char b64_decode(char c)
 * @brief Decode a single base64 character.
 *
 * @param c
 * @return
 */
static unsigned char b64_decode(char c)
{
    if(c >= 'A' && c <= 'Z') return(c - 'A');
    if(c >= 'a' && c <= 'z') return(c - 'a' + 26);
    if(c >= '0' && c <= '9') return(c - '0' + 52);
    if(c == '+')             return 62;
    return 63;
}


/**
 * @fn static int is_base64(char c)
 * @brief Return TRUE if 'c' is a valid base64 character, otherwise FALSE
 *
 * @param c
 * @return
 */
static int is_base64(char c)
{
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') || (c == '+')             ||
        (c == '/')             || (c == '=')) {
            return 1;
    }
    return 0;
}

