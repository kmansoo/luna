#pragma once

#include <string>
#include <mutex>
#include <list>
#include <thread>

#include "afxtempl.h"
#include "afxmt.h"

//enum HS_SERIAL_HELPER_ERROR {
//	HS_SH_ERROR_PORT   = 0xFFFFFFFF,
//	HS_SH_ERROR_WRITE  = 0xFFFFFFFE,  
//	HS_SH_ERROR_READ   = 0xFFFFFFFC
//};
//
//class ccWin32SerialPortHelper;
//
//class IHSWin32SerialCallback
//{
//public:
//	IHSWin32SerialCallback() {}
//	virtual ~IHSWin32SerialCallback() {}
//
//public:
//	virtual void    OnHSPSerialPortReceive(ccWin32SerialPortHelper* pSH, DWORD dwLength) = 0;
//	virtual void    OnHSPSerialPortOccurError(ccWin32SerialPortHelper* pSH, DWORD dwErrorCode) = 0;
//};

namespace Luna {

class ccWin32SerialPortHelper
{
public:
    ccWin32SerialPortHelper();
    virtual ~ccWin32SerialPortHelper();

public:
    //	기본 선언 자료들
    // Flow control flags
    enum FLOW_CONTROL_FLAGS {
        FC_DTRDSR   = 0x01,
        FC_RTSCTS   = 0x02,
        FC_XONXOFF  = 0x04,
    };

    enum ASCII_CODES {
        ASCII_BEL   = 0x07,
        ASCII_BS    = 0x08,
        ASCII_LF    = 0x0A,
        ASCII_CR    = 0x0D,
        ASCII_XON   = 0x11,
        ASCII_XOFF  = 0x13,    
    };

    // Port
    enum	COM_PORT	{COM1 = 1, COM2, COM3, COM4, COM5, COM6, COM7};
    enum	PARITY_BIT	{PB_NONE, PB_OLD, PB_EVEN, PB_MARK, PB_SPACE};
    enum	BYTE_SIZE	{BS_SIZE4 = 4, BS_SIZE5, BS_SIZE6, BS_SIZE7, BS_SIZE8};
    enum	STOP_BITS	{SB_1_BITS, SB_1_5_BITS, SB_2_BITS};

    // Operations
public:
    BOOL 	IsOpenPort()	{return _bOpenPort;};

    BOOL 	Open(
        UINT		uComPort, 
        DWORD		dwRate, 
        PARITY_BIT	ParityBit = PB_NONE, 
        BYTE_SIZE	ByteSize = BS_SIZE8, 
        STOP_BITS	StopBits = SB_1_BITS);

    BOOL 	CheckValidPort(
        UINT        uComPort, 
        DWORD       dwRate,
        PARITY_BIT	ParityBit = PB_NONE, 
        BYTE_SIZE	ByteSize = BS_SIZE8, 
        STOP_BITS	StopBits = SB_1_BITS);

    void	Close();

    const std::string	&GetPortName();

    BOOL 	Putc(BYTE ch);
    BOOL 	Puts(const char *pData);

    BOOL 	Write(const void*, DWORD dwWriteSize);
    BOOL 	Write(const void *lpBuf, DWORD dwCount, DWORD *pBytesWritten);
    BOOL 	WriteString(std::string& str, DWORD *pBytesWritten = NULL);

    // 자료 읽기
    BOOL 	IsEmpty();
    CHAR	GetChar();
    DWORD	Read(void*, DWORD dwReadSize);

    void	ResetBuffer();

    void	SetReceiveEvent(BOOL  bEvent = true);

    //	Frame에러가 났을 경우에 처리한다.
    BOOL 	StartPollThread();
    BOOL 	StopPollThread();

protected:
    virtual void    DoSetupHardwareFlowControl(DCB& stDCB);

private:
    BOOL 	Open(
        UINT		uComPort, 
        DWORD		dwRate, 
        PARITY_BIT	ParityBit, 
        BYTE_SIZE	ByteSize, 
        STOP_BITS	StopBits);

    BOOL        SettingPort();
    BOOL 		CreateRWEvent();

protected:
    BOOL                    _bReceiveEvent;
    std::list<char>	        _sReceiveData;

    virtual void            ReadFromDevice();

private:
    std::thread _oPollThread;
    bool        _bIsStopThread;

    BOOL 	    _bOpenPort;
    BOOL 	    _bActiveThread;

    HANDLE		_hFile;
    HANDLE		_hReadEvent;
    OVERLAPPED	_osWrite, _osRead;

    CWinThread* _pWatchThread;
    CEvent		_evWatchThreadKillEvent;

    // 통신 설정
    std::string _strPort;
    DWORD		_dwBaudRate;
    PARITY_BIT	_ParityBit;
    BYTE_SIZE	_ByteSize;
    STOP_BITS	_StopBits;

    //	읽기 버퍼
    UINT		_uAllocBufferSize;
    char*       _pReadingBuf;

    std::mutex  _mtx;
    std::mutex  _mtx_buffer;
};

}
