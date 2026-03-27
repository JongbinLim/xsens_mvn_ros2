#ifndef XMEDEVICESTATUSARRAY_H
#define XMEDEVICESTATUSARRAY_H

#include <xstypes/xsarray.h>
#include <xstypes/pstdint.h>

#include "xmedllapi.h"
#ifdef __cplusplus
#include "xmedevicestatus.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmeDeviceStatusArrayDescriptor;

#ifndef __cplusplus
#define XmeDeviceStatusArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmeDeviceStatusArrayDescriptor)

struct XmeDeviceStatus;
XSARRAY_STRUCT(XmeDeviceStatusArray, struct XmeDeviceStatus);
typedef struct XmeDeviceStatusArray XmeDeviceStatusArray;

XME_DLL_API void XmeDeviceStatusArray_construct(XmeDeviceStatusArray* thisPtr, XsSize count, struct XmeDeviceStatus const* src);
#endif

XME_DLL_API int XmeDeviceStatusArray_isEquivalent(struct XmeDeviceStatusArray const* a, struct XmeDeviceStatusArray const* b);

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
struct XmeDeviceStatusArray : public XsArrayImpl<XmeDeviceStatus, g_XmeDeviceStatusArrayDescriptor, XmeDeviceStatusArray>
{
	//! \brief Constructs an XmeDeviceStatusArray
	inline explicit XmeDeviceStatusArray(XsSize sz = 0, XmeDeviceStatus const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmeDeviceStatusArray as a copy of \a other
	inline XmeDeviceStatusArray(XmeDeviceStatusArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmeDeviceStatusArray that references the data supplied in \a ref
	inline explicit XmeDeviceStatusArray(XmeDeviceStatus* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmeDeviceStatusArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmeDeviceStatusArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}

	//! \brief Compare the important parts of each XmeDeviceStatus for equality, returning true when they're equal
	bool isEquivalent(XmeDeviceStatusArray const& other) const
	{
		return 0 != XmeDeviceStatusArray_isEquivalent(this, &other);
	}

};
#endif

#endif
