#ifndef XSSENSORRANGES_H
#define XSSENSORRANGES_H

#include "xstypesconfig.h"
#include "xsstring.h"
#include "xsdeviceid.h"

#ifdef __cplusplus
extern "C" {
#endif

enum HardwareManufacturerType
{
	HMT_MT	= 0,
	HMT_None
};
typedef enum HardwareManufacturerType HardwareManufacturerType;

XSTYPES_DLL_API void findHardwareTypeC(const XsDeviceId* deviceId, XsString* resultValue);
XSTYPES_DLL_API HardwareManufacturerType findHardwareManufacturerC(const XsDeviceId* deviceId);

XSTYPES_DLL_API double accelerometerRangeC(const XsDeviceId* deviceId);
XSTYPES_DLL_API double gyroscopeRangeC(const XsDeviceId* deviceId);

XSTYPES_DLL_API double actualAccelerometerRangeC(const XsDeviceId* deviceId);
XSTYPES_DLL_API double actualGyroscopeRangeC(const XsDeviceId* deviceId);

#ifdef __cplusplus
}

inline static XsString findHardwareType(const XsDeviceId& deviceId)
{
	XsString rv;
	findHardwareTypeC(&deviceId, &rv);
	return rv;
}
inline static HardwareManufacturerType findHardwareManufacturer(const XsDeviceId& deviceId)
{
	return findHardwareManufacturerC(&deviceId);
}
inline static double accelerometerRange(const XsDeviceId& deviceId)
{
	return accelerometerRangeC(&deviceId);
}
inline static double gyroscopeRange(const XsDeviceId& deviceId)
{
	return gyroscopeRangeC(&deviceId);
}
inline static double actualAccelerometerRange(const XsDeviceId& deviceId)
{
	return actualAccelerometerRangeC(&deviceId);
}
inline static double actualGyroscopeRange(const XsDeviceId& deviceId)
{
	return actualGyroscopeRangeC(&deviceId);
}
#endif

#endif
