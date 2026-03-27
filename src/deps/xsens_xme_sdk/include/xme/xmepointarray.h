#ifndef XMEPOINTARRAY_H
#define XMEPOINTARRAY_H

#include <xstypes/xsarray.h>
#include <xstypes/pstdint.h>

#include "xmedllapi.h"
#ifdef __cplusplus
#include "xmepoint.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmePointArrayDescriptor;

#ifndef __cplusplus
#define XmePointArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmePointArrayDescriptor)

struct XmePoint;
XSARRAY_STRUCT(XmePointArray, struct XmePoint);
typedef struct XmePointArray XmePointArray;

XME_DLL_API void XmePointArray_construct(XmePointArray* thisPtr, XsSize count, struct XmePoint const* src);
#endif
#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
struct XmePointArray : public XsArrayImpl<XmePoint, g_XmePointArrayDescriptor, XmePointArray>
{
	//! \brief Constructs an XmePointArray
	inline explicit XmePointArray(XsSize sz = 0, XmePoint const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmePointArray as a copy of \a other
	inline XmePointArray(XmePointArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmePointArray that references the data supplied in \a ref
	inline explicit XmePointArray(XmePoint* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmePointArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmePointArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}
};
#endif

#endif
