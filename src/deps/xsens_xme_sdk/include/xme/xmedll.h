#ifndef XMEDLL_H
#define XMEDLL_H

#include "xmedllapi.h"
#include <xstypes/xsversion.h>

#ifdef __cplusplus
extern "C" {
class Journaller;
#endif

XME_DLL_API void xmeGetDllVersion(XsVersion* version);
XME_DLL_API void xmeTerminate();
#ifndef SWIG
XME_DLL_API void xmeSetPaths(XsString const* settings, XsString const* userData, XsString const* logs);
XME_DLL_API void xmeSetThreadPoolInstance(void* instance);
#endif

#ifdef __cplusplus
XME_DLL_API void xmeSetLogger(Journaller* journaller);
}

inline XsVersion xmeGetDllVersion(void)
{
	XsVersion tmp;
	xmeGetDllVersion(&tmp);
	return tmp;
}

inline void xmeSetPaths(XsString const& settings, XsString const& userData, XsString const& logs)
{
	xmeSetPaths(&settings, &userData, &logs);
}
#endif

#endif
