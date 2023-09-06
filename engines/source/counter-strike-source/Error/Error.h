
#ifndef ERROR_H
#define ERROR_H

class CErrorHandler
{
public:
	CErrorHandler( const char* szFile, const char* szFunction, unsigned int iLine, const char* szMessage, ... );
	CErrorHandler( const char* szMessage, ... );
	virtual ~CErrorHandler();

private:
	void Pop();

private:
	char m_errorMessage[2048];
};

#ifndef ERR
	#if SB_DEBUG
		#define ERR( message, ... ) { CErrorHandler gErr_##__LINE__##( __FILE__, __FUNCTION__, __LINE__, message, __VA_ARGS__ ); }
	#else
		#define ERR( message, ... ) { CErrorHandler gErr_##__LINE__##( message, __VA_ARGS__ ); }
	#endif // SB_DEBUG
#endif // ERR

#endif // ERROR_H