#ifndef XMELICENSE_H
#define XMELICENSE_H

#include "xmedllapi.h"
#include <xstypes/xsstring.h>

#ifdef __cplusplus
extern "C" {
#endif

struct XmeLicense;
typedef struct XmeLicense XmeLicense;

XME_DLL_API void XmeLicense_construct();
XME_DLL_API void XmeLicense_host_construct(const char* license_host);
XME_DLL_API void XmeLicense_destruct(void);
XME_DLL_API int XmeLicense_isConstructed(void);
XME_DLL_API void XmeLicense_setFailureCallback(void (*func)(void));
XME_DLL_API void XmeLicense_setRecoveredCallback(void (*func)(void));
XME_DLL_API void XmeLicense_setPreferredLicense(const XsString* keyId, int productId, const XsString* serverAddress);
XME_DLL_API void XmeLicense_getPreferredLicense(XsString* keyId, int* productId, XsString* serverAddress);
XME_DLL_API int XmeLicense_isLicenseAvailable(void);
XME_DLL_API void XmeLicense_getCurrentLicense(XsString* name);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#ifdef __cplusplus
/*! \brief License management object
	\details This object will initialize the licensing component of XME during construction and will clean it up during destruction. Usually this object is created once in an application at the start of the main() function, before any XmeControl objects are created.

	The object encapsulates the C-interface functions XmeLicense_construct() and XmeLicense_destruct(), which can also be safely used. Use of this object is recommended, since it provides exception safety.
*/
struct XmeLicense
{
public:

	//! \copydoc XmeLicense_construct
	XmeLicense()
	{
		XmeLicense_construct();
	}

	//! \copydoc XmeLicense_construct
	explicit XmeLicense(const char* hostname)
	{
		XmeLicense_host_construct(hostname);
	}

	//! \copydoc XmeLicense_destruct
	~XmeLicense()
	{
		XmeLicense_destruct();
	}

	//! \copydoc XmeLicense_isConstructed
	inline static bool isConstructed()
	{
		return 0 != XmeLicense_isConstructed();
	}

	//! \copydoc XmeLicense_setFailureCallback
	static void setFailureCallback(void (*func)(void))
	{
		XmeLicense_setFailureCallback(func);
	}

	//! \copydoc XmeLicense_setRecoveredCallback
	static void setRecoveredCallback(void(*func)(void))
	{
		XmeLicense_setRecoveredCallback(func);
	}

	//! \copydoc XmeLicense_isLicenseAvailable
	static bool isLicenseAvailable()
	{
		return 0 != XmeLicense_isLicenseAvailable();
	}

	//! \copydoc XmeLicense_getCurrentLicense
	static XsString getCurrentLicense()
	{
		XsString rv;
		XmeLicense_getCurrentLicense(&rv);
		return rv;
	}
private:
	XmeLicense(XmeLicense const&) = delete;
	XmeLicense& operator = (XmeLicense const&) = delete;
};
#endif // __cplusplus
#endif
