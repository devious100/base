#ifndef CRXHeaderH
#define CRXHeaderH

#define _USING_CXR

#ifndef _USING_CXR
#define _CXR(x) x

#else

#include <string>
using namespace std;


#define _CXR(x) __CXRDecrypt(x)

string __CXRDecrypt(const char *pIn);
int __CXRDecrypt(string* x, const char *pIn);

#endif

#endif