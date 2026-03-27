#ifndef XSPLATFORM_H
#define XSPLATFORM_H

#include "xstypesconfig.h"

#ifdef _WIN32
	/// microsoft / windows
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
	#endif
	#include <Windows.h>

	#define XsIoHandle HANDLE

#else
	/// gcc / linux
	#include <stdint.h>

	typedef int32_t XsIoHandle;

#endif

#endif
