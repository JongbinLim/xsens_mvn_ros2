#ifndef XMEMARKERARRAY_H
#define XMEMARKERARRAY_H

#include <xstypes/xsarray.h>
#include <xstypes/pstdint.h>

#include "xmedllapi.h"
#ifdef __cplusplus
#include "xmemarker.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmeMarkerArrayDescriptor;

#ifndef __cplusplus
#define XmeMarkerArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmeMarkerArrayDescriptor)

struct XmeMarker;
XSARRAY_STRUCT(XmeMarkerArray, struct XmeMarker);
typedef struct XmeMarkerArray XmeMarkerArray;

XME_DLL_API void XmeMarkerArray_construct(XmeMarkerArray* thisPtr, XsSize count, struct XmeMarker const* src);
#endif

#ifdef __cplusplus
} // extern "C"

struct XmeMarkerArray : public XsArrayImpl<XmeMarker, g_XmeMarkerArrayDescriptor, XmeMarkerArray>
{
	//! \brief Constructs an XmeMarkerArray
	inline explicit XmeMarkerArray(XsSize sz = 0, XmeMarker const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmeMarkerArray as a copy of \a other
	inline XmeMarkerArray(XmeMarkerArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmeMarkerArray that references the data supplied in \a ref
	inline explicit XmeMarkerArray(XmeMarker* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmeMarkerArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmeMarkerArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}
};
#endif

#endif
