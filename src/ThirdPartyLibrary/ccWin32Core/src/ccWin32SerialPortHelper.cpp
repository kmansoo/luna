// ccWin32SerialPortHelper.cpp: implementation of the ccWin32SerialPortHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ccCore/ccString.h"

#include "ccWin32SerialPortHelper.h"

namespace Luna {

#define MAX_RETRY_COUNT	3600

//////////////////////////////////////////////////////////////////////////
// for defug
// #define	__START_READ_WRITE_DATA_MONITOR__
#ifdef	__START_READ_WRITE_DATA_MONITOR__
FILE *fp = NULL;
FILE *write_fp = NULL;
#endif
//////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
////	쓰레드와 통신을 하기 위함
//class HSWatchCommProcParam : public CObject
//{
//public:
//    HSWatchCommProcParam() {
//        _hPort = INVALID_HANDLE_VALUE;
//        _pSerialHelper = NULL;
//    }
//
//    virtual ~HSWatchCommProcParam() {};
//
//public:
//    HANDLE			        _hPort;
//    ccWin32SerialPortHelper*    _pSerialHelper;
//};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ccWin32SerialPortHelper::ccWin32SerialPortHelper()
{
    _bOpenPort          = FALSE;
    _bActiveThread      = FALSE;
    _pWatchThread       = NULL;

    _strPort            = "";
    _bReceiveEvent      = TRUE;

    _uAllocBufferSize   = 0x1000;
    _pReadingBuf        = new char[_uAllocBufferSize];

    _osWrite.hEvent     = INVALID_HANDLE_VALUE;
    _osRead.hEvent      = INVALID_HANDLE_VALUE;

    //	for	debuf
#ifdef _DEBUG
#ifdef	__START_READ_WRITE_DATA_MONITOR__
    if (fp == NULL)
        fp = fopen("serial_recv_data.data", "wb+");

    if (write_fp == NULL)
        write_fp = fopen("serial_send_data.data", "wb+");
#endif	
#endif	
}

ccWin32SerialPortHelper::~ccWin32SerialPortHelper()
{
    delete _pReadingBuf;

    Close();

    //delete _pQueue;
    //_csSendData.unlock();

    // for debug
#ifdef _DEBUG
#ifdef	__START_READ_WRITE_DATA_MONITOR__
    if (fp)
        fclose(fp);

    if (write_fp)
        fclose(write_fp);
#endif
#endif

}

////////////////////////////////////////////////////////////
// 포트를 연다.
BOOL ccWin32SerialPortHelper::checkValidPort(
    UINT uComPort, 
    DWORD dwRate, 
    PARITY_BIT	ParityBit, 
    BYTE_SIZE	ByteSize, 
    STOP_BITS	StopBits)
{
    if (_bOpenPort != FALSE) 
        return FALSE;

    // Port 설정
    ccString::format(_strPort, "//./COM%d", uComPort);

    _dwBaudRate	    = dwRate;
    _ParityBit		= ParityBit;
    _ByteSize		= ByteSize;
    _StopBits		= StopBits;

    _hFile = CreateFile(
        _strPort.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL, 
        open_EXISTING, 
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL);

    if (_hFile == INVALID_HANDLE_VALUE)  {
        CloseHandle(_hFile);
        return FALSE ;
    }

    // Settting
    SetCommMask(_hFile, EV_RXCHAR);	// A character was received and placed in the input buffer. 
    SetupComm(_hFile, 1024, 1024);		// 버퍼 설정.

    // 쓰레기 값 없앰.
    PurgeComm(_hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

    COMMTIMEOUTS	CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout		= 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier	= 0;
    CommTimeOuts.ReadTotalTimeoutConstant	= 1000;
    CommTimeOuts.WriteTotalTimeoutMultiplier= 0;
    CommTimeOuts.WriteTotalTimeoutConstant	= 1000;

    SetCommTimeouts(_hFile, &CommTimeOuts);

    if (settingPort() != TRUE)
        return FALSE;

    _bOpenPort = TRUE;

    return TRUE;
}

BOOL ccWin32SerialPortHelper::open(
    UINT		uComPort, 
    DWORD		dwRate, 
    PARITY_BIT	ParityBit, 
    BYTE_SIZE	ByteSize, 
    STOP_BITS	StopBits)
{
    if (_bOpenPort != FALSE) 
        return FALSE;

    // Port 설정
    ccString::format(_strPort, "//./COM%d", uComPort);
    _dwBaudRate	    = dwRate;
    _ParityBit		= ParityBit;
    _ByteSize		= ByteSize;
    _StopBits		= StopBits;

    _hFile = CreateFile(
        _strPort.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL, 
        open_EXISTING, 
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        NULL);

    if (_hFile == INVALID_HANDLE_VALUE)  {
        CloseHandle(_hFile);
        return FALSE ;
    }

    // Settting
    SetCommMask(_hFile, EV_RXCHAR);	// A character was received and placed in the input buffer. 
    SetupComm(_hFile, 1024, 1024);		// 버퍼 설정.

    // 쓰레기 값 없앰.
    PurgeComm(_hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

    COMMTIMEOUTS	CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout		= 0xFFFFFFFF;
    CommTimeOuts.ReadTotalTimeoutMultiplier	= 0;
    CommTimeOuts.ReadTotalTimeoutConstant	= 1000;
    CommTimeOuts.WriteTotalTimeoutMultiplier= 0;
    CommTimeOuts.WriteTotalTimeoutConstant	= 1000;

    SetCommTimeouts(_hFile, &CommTimeOuts);

    //
    if (settingPort() != TRUE)
        return FALSE;

    _bOpenPort = TRUE;

    //	쓰레드를 수행한다.
    StartPollThread();

    return TRUE;
}

BOOL	ccWin32SerialPortHelper::open(
    UINT		uComPort, 
    DWORD		dwRate, 
    PARITY_BIT	ParityBit, 
    BYTE_SIZE	ByteSize, 
    STOP_BITS	StopBits)
{
    return open(uComPort, dwRate, ParityBit, ByteSize, StopBits);
}

/*
*	Serail Port를 닫음
*/
void ccWin32SerialPortHelper::close()
{
    if (_bOpenPort)
    {
        // 쓰레기 값 없앰.
        PurgeComm(_hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

        if (_osWrite.hEvent != INVALID_HANDLE_VALUE)
            CloseHandle(_osWrite.hEvent);

        if (_osRead.hEvent != INVALID_HANDLE_VALUE)
            CloseHandle(_osRead.hEvent);

        if (_hFile != INVALID_HANDLE_VALUE)
            CloseHandle(_hFile);

        stopWatchThread();

        _bOpenPort      =	FALSE;
        _hFile		    =	INVALID_HANDLE_VALUE;

        _osWrite.hEvent = INVALID_HANDLE_VALUE;
        _osRead.hEvent  = INVALID_HANDLE_VALUE;
    }
}

void ccWin32SerialPortHelper::DoSetupHardwareFlowControl(DCB& stDCB)
{
    stDCB.fTXContinueOnXoff	= 0;
    stDCB.fOutxDsrFlow		= 0;
    stDCB.fOutxCtsFlow		= 0;

    stDCB.fDtrControl       = DTR_CONTROL_ENABLE;
    stDCB.fRtsControl       = DTR_CONTROL_ENABLE;

    stDCB.fInX              = TRUE;
    stDCB.fOutX             = TRUE;

    stDCB.XonChar           = ASCII_XON;
    stDCB.XoffChar          = ASCII_XOFF;

    stDCB.XonLim            = 100;
    stDCB.XoffLim           = 100;

    stDCB.fBinary           = TRUE;
    stDCB.fParity           = TRUE;
}

/*
*	포트를 설정한다.
*/
BOOL ccWin32SerialPortHelper::settingPort()
{
    //	최초 한번만 수행
    //	CreateFile이 성공적이여야 한다.
    if (_bOpenPort)
        return FALSE;
    //
    BOOL	bResult;
    DCB		stDCB;

    stDCB.DCBlength = sizeof(DCB);

    GetCommState(_hFile, &stDCB);

    // 이 부분을 수정. 
    stDCB.BaudRate	= _dwBaudRate;	// 전송 속도
    stDCB.ByteSize	= _ByteSize;	// 데이터 비트
    stDCB.Parity    = _ParityBit;	// 페리트 비트
    stDCB.StopBits	= _StopBits;	// 스톱	 비트

    DoSetupHardwareFlowControl(stDCB);

    if ( (bResult = SetCommState(_hFile, &stDCB)) == FALSE)
        CloseHandle(_hFile);

    return bResult;
}

/////////////////////////////
BOOL ccWin32SerialPortHelper::CreateRWEvent()
{
    // write
    memset(&_osWrite,	0,	sizeof(OVERLAPPED));
    _osWrite.hEvent = ::CreateEvent(NULL,TRUE,TRUE,NULL);

    if (_osWrite.hEvent == INVALID_HANDLE_VALUE) 
        return FALSE;

    // read
    memset(&_osRead,	0,	sizeof(OVERLAPPED));
    _osRead.hEvent = ::CreateEvent(NULL,TRUE,TRUE,NULL);

    if (_osRead.hEvent == INVALID_HANDLE_VALUE)
    {
        CloseHandle(_osWrite.hEvent);
        return FALSE;
    }

    return TRUE;
}

////////////////////////////////
// 자료가 들어오면 알려주는 역활
BOOL ccWin32SerialPortHelper::StartPollThread()
{
    if (!_bOpenPort)
        return FALSE;

    if (_bActiveThread)
        return FALSE;

    if (!CreateRWEvent()) 
        return FALSE;

    is_stop_thread_ = false;
    polling_thread_ = std::thread(std::bind(&ccWin32SerialPortHelper::DoPoll, this));


    HSWatchCommProcParam *pWTParemeter	= new HSWatchCommProcParam;

    //
    pWTParemeter->_hPort	        = (HANDLE) _hFile;
    pWTParemeter->_pSerialHelper	= this;

    _pWatchThread		= AfxBeginThread(
        (AFX_THREADPROC) ccWin32SerialPortHelper::WatchSerialProc, 
        (LPVOID)pWTParemeter);

    if (_pWatchThread == NULL) {
        delete pWTParemeter;
        pWTParemeter = NULL;
    }
    else
        _bActiveThread		= TRUE;

    return _bActiveThread;
}

/////////////////////////////
BOOL ccWin32SerialPortHelper::stopPollThread()
{
    if (_bActiveThread)
    {
        _bActiveThread = FALSE;

        SetCommMask(_hFile, 0);
        EscapeCommFunction(_hFile, CLRDTR);
        PurgeComm(_hFile, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

        _evWatchThreadKillEvent.SetEvent();

        if (WaitForSingleObject(_pWatchThread->m_hThread, 1000) == WAIT_TIMEOUT)
            TerminateThread(_pWatchThread->m_hThread, 0);

        //  SetCommMask(_hFile, EV_RXCHAR);
        return TRUE;
    }

    return FALSE;
}

////////////////////////////
// 자료 읽기
BOOL ccWin32SerialPortHelper::isEmpty()
{
    HSAutoLocking al(&_mtx_buffer);

    if (_sReceiveData.GetCount() == 0)
        return TRUE;

    return FALSE;
}

char ccWin32SerialPortHelper::getChar()
{
    if (isEmpty())
        return 0;

    char ch = 0;

    _mtx_buffer.lock();
    _sReceiveData.remove(ch);
    _mtx_buffer.unlock();

    return ch;
}

DWORD ccWin32SerialPortHelper::read(void*pBuf, DWORD dwReadSize)
{
    _mtx_buffer.lock();

    dwReadSize = ((int)dwReadSize > _sReceiveData.GetCount()) ?  _sReceiveData.GetCount() : dwReadSize;

    char *pCharBufType = (char *)pBuf;

    for (DWORD i = 0; i < dwReadSize; i++)
        _sReceiveData.remove(pCharBufType[i]);

    _mtx_buffer.unlock();

    return dwReadSize;
}


/////////////////////////////
// 자료 송신 및 수신

void ccWin32SerialPortHelper::resetBuffer()
{
    _mtx_buffer.lock();
    _sReceiveData.clear();
    _mtx_buffer.unlock();

    //	_pQueue->RemoveAll();
}

void ccWin32SerialPortHelper::setReceiveEvent(BOOL bEvent)
{
    _bReceiveEvent = bEvent;
}

BOOL ccWin32SerialPortHelper::putc(BYTE ch)
{
    return Write(&ch, sizeof(BYTE));
}

BOOL ccWin32SerialPortHelper::puts(const char *pData)
{
    return Write(pData, strlen(pData));
}

BOOL ccWin32SerialPortHelper::write(const void *lpBuf, DWORD dwCount)
{
    if (!_bActiveThread)
        return FALSE;

    BOOL	bResult ;
    DWORD	dwBytesWritten;

    bResult = WriteFile(_hFile, lpBuf, dwCount, &dwBytesWritten, &_osWrite);

#ifdef _DEBUG
#ifdef	__START_READ_WRITE_DATA_MONITOR__
    if (write_fp != NULL)
        fwrite(lpBuf, dwCount, 1, write_fp);
#endif
#endif

    if (!bResult)
    {
        int nRetryCount = 0;

        //  컴포트에 데이타를 제대로 써넣지 못했을경우이다.
        bResult = FALSE;

        while (!bResult)
        {
            bResult = GetOverlappedResult((HANDLE)_hFile, &_osWrite, &dwBytesWritten, FALSE);

            if (bResult == FALSE)
            {
                Sleep(1);

                if (nRetryCount++ > 1000)
                {
                    //if (_pListener)
                    //    _pListener->OnHSPSerialPortOccurError(this, HS_SH_ERROR_WRITE);

                    return FALSE;
                }
            }
        }
    }

    return bResult;
}


BOOL ccWin32SerialPortHelper::write(const void *lpBuf, DWORD dwCount, DWORD *pBytesWritten)
{
    if (!_bActiveThread)
        return FALSE;

    BOOL	bResult ;
    DWORD	dwBytesWritten;
    int     nRetryCount = 0;

    bResult = WriteFile(_hFile, lpBuf, dwCount, &dwBytesWritten, &_osWrite);

    if (!bResult)
    {
        int i   = 0;
        bResult = FALSE;

        while (!bResult)
        {
            bResult = GetOverlappedResult((HANDLE)_hFile, &_osWrite, &dwBytesWritten, TRUE);

            if (bResult == FALSE)
            {
                Sleep(1);

                if (nRetryCount++ > 1000)
                {
                    //if (_pListener)
                    //    _pListener->OnHSPSerialPortOccurError(this, HS_SH_ERROR_WRITE);

                    return FALSE;
                }
            }
        }
    }

    if (pBytesWritten != NULL)
        *pBytesWritten = dwBytesWritten;

    return bResult;
}

BOOL ccWin32SerialPortHelper::writeString(std::string& str, DWORD *pBytesWritten)
{
    return write(str.c_str(), str.length(), pBytesWritten);
}

void ccWin32SerialPortHelper::readFromDevice()
{
    if (!_bActiveThread)	
    {	
        //TRACE(" # ccWin32SerialPortHelper::Read() : 데이터 읽지 못함\n");
        return;
    }

    BOOL	fReadStat;
    COMSTAT	ComStat;
    DWORD	dwErrorFlags;
    DWORD	dwLength;
    DWORD	dwLength2;

    WaitForSingleObject(_osRead.hEvent, 1000);
    ClearCommError(_hFile, &dwErrorFlags, &ComStat);

    dwLength = ComStat.cbInQue;

    if (dwLength == 0)
        return;

    if (_uAllocBufferSize < dwLength)
    {
        delete _pReadingBuf;

        _uAllocBufferSize = dwLength;
        _pReadingBuf = new char[_uAllocBufferSize];
    }

    if (_pReadingBuf == NULL)	
        return;

    fReadStat = ReadFile(_hFile, _pReadingBuf, dwLength, &dwLength2, &_osRead);

    _mtx_buffer.lock();
    for (DWORD i = 0; i < dwLength; i++)
        _sReceiveData.push_back(_pReadingBuf[i]);
    _mtx_buffer.unlock();

#ifdef _DEBUG
#ifdef	__START_READ_WRITE_DATA_MONITOR__
    if (fp != NULL)
        fwrite(_pReadingBuf, ComStat.cbInQue, 1, fp);
#endif
#endif

    if (_bReceiveEvent)
    {
        //if (_pListener)
        //    _pListener->OnHSPSerialPortReceive(this, dwLength);
    }

    //goto Retry;
}

/////////////////////////////////
void ccWin32SerialPortHelper::watchSerialProc(LPVOID pParam)
{
    HSWatchCommProcParam wcppParam;
    DWORD		dwEvtMask;
    OVERLAPPED	os;
    BOOL		bConnect = FALSE;

    HSWatchCommProcParam *pWCPParem = (HSWatchCommProcParam *)pParam;

    wcppParam._hPort		=	pWCPParem->_hPort;
    wcppParam._pSerialHelper=	pWCPParem->_pSerialHelper;

    delete pWCPParem;

    if (pParam == NULL || pWCPParem->_pSerialHelper == NULL)
        return;

    hsp_memset(&os, 0, sizeof(OVERLAPPED));

    os.hEvent = CreateEvent(NULL,		// no security
        TRUE,		// explicit reset req
        FALSE,		// initial event reset
        NULL);		// no name


    if (os.hEvent == NULL) 	
    {
        MessageBox(NULL, _T("Can't create event for thread."), _T("Communication"), MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    if (!SetCommMask(wcppParam._hPort, EV_RXCHAR))	
    {
#ifdef _DEBUG
        TRACE(_T("[SerialHelper] 문제가 발생...\n"));
#endif
        if (wcppParam._pSerialHelper->_pListener)
            wcppParam._pSerialHelper->_pListener->OnHSPSerialPortOccurError(wcppParam._pSerialHelper, 0x01);

        return;
    }

    while (true)
    {
        dwEvtMask	= 0;

        if (::WaitForSingleObject(wcppParam._pSerialHelper->_evWatchThreadKillEvent.m_hObject, 0) == WAIT_OBJECT_0)
            break;

        WaitCommEvent(wcppParam._hPort, &dwEvtMask, NULL);

        if (::WaitForSingleObject(wcppParam._pSerialHelper->_evWatchThreadKillEvent.m_hObject, 0) == WAIT_OBJECT_0)
            break;

        if ( (dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
            wcppParam._pSerialHelper->ReadFromDevice();
    }

#ifdef _DEBUG
    TRACE(" # Terminated WatchThread for Serial.\n");
#endif
}

}