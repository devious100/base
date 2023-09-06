

#include "Error.h"
#include "..\Include.h"

CErrorHandler::CErrorHandler( const char* szFile, const char* szFunction, unsigned int iLine, const char* szMessage, ... )
{
	va_list		va_alist;
	char		szBuf[2048];

	va_start( va_alist, szMessage );
	_vsnprintf( szBuf, sizeof( szBuf ), szMessage, va_alist );
	va_end( va_alist );

	sprintf( m_errorMessage, "SourceBase encountered an error!\n\nOrigin of the error was:\nFile:\t%s\nFunction:\t%s\nLine:\t%d\nMessage:\t%s\n\nPress ABORT to quit, press RETRY to debug, or press IGNORE to continue", szFile, szFunction, iLine, szBuf );
}

CErrorHandler::CErrorHandler( const char* szMessage, ... )
{
	va_list		va_alist;

	va_start( va_alist, szMessage );
	_vsnprintf( m_errorMessage, sizeof( m_errorMessage ), szMessage, va_alist );
	va_end( va_alist );
}

CErrorHandler::~CErrorHandler()
{
	Pop();
}

void CErrorHandler::Pop()
{
	int iID = 0;

#if SB_DEBUG
	iID = MessageBox( NULL, m_errorMessage, "SourceBase Error", MB_ICONSTOP|MB_ABORTRETRYIGNORE|MB_TOPMOST );

	if( iID == IDABORT )
		ExitProcess(0);
	else if( iID == IDRETRY )
		__debugbreak();
#else
	iID = MessageBox( NULL, m_errorMessage, "SourceBase Error", MB_ICONSTOP|MB_TOPMOST );
#endif
}