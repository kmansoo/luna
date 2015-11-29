#pragma once

#include "HSCoreAPI/HSObject.h"

class IHSWin32RgWebApiTransactionNotifier 
{
public:
    virtual void    OnTransactionRecveResponse(const CString& strResponse) = 0;
    virtual void    OnTransactionRequestTimeout() = 0;
};

/* ====================================================================
 *  File: IHSWin32RgWebApiTransactionNotifier.h
 *
 *  Desc:
 *      Win32±â¹Ý Multimedia Timer
 *
 *  Author:
 *      ±è¸¸¼ö(kmansoo@humaxdigital.com)
 *
 *  Date:
 *      2014.11.29
 */
