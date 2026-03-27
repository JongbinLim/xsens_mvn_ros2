#ifndef XMELOCATIONARRAY_H
#define XMELOCATIONARRAY_H

#include <xstypes/xsarray.h>
#include <xstypes/pstdint.h>

#include "xmedllapi.h"
#ifdef __cplusplus
#include "xmelocation.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmeLocationArrayDescriptor;

#ifndef __cplusplus
#define XmeLocationArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmeLocationArrayDescriptor)

struct XmeLocation;
XSARRAY_STRUCT(XmeLocationArray, struct XmeLocation);
typedef struct XmeLocationArray XmeLocationArray;

XME_DLL_API void XmeLocationArray_construct(XmeLocationArray* thisPtr, XsSize count, struct XmeLocation const* src);
#endif
#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
struct XmeLocationArray : public XsArrayImpl<XmeLocation, g_XmeLocationArrayDescriptor, XmeLocationArray>
{
	//! \brief Constructs an XmeLocationArray
	inline explicit XmeLocationArray(XsSize sz = 0, XmeLocation const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmeLocationArray as a copy of \a other
	inline XmeLocationArray(XmeLocationArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmeLocationArray that references the data supplied in \a ref
	inline explicit XmeLocationArray(XmeLocation* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmeLocationArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmeLocationArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}
};
#endif

#endif
