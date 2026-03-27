#ifndef XMEAIDINGARRAY_H
#define XMEAIDINGARRAY_H

#include <xstypes/xsarray.h>
#include <xstypes/pstdint.h>

#include "xmedllapi.h"
#ifdef __cplusplus
#include "xmeaiding.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmeAidingArrayDescriptor;

#ifndef __cplusplus
#define XmeAidingArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmeAidingArrayDescriptor)

struct XmeAiding;
XSARRAY_STRUCT(XmeAidingArray, struct XmeAiding);
typedef struct XmeAidingArray XmeAidingArray;

XME_DLL_API void XmeAidingArray_construct(XmeAidingArray* thisPtr, XsSize count, struct XmeAiding const* src);
#endif

#ifdef __cplusplus
} // extern "C"

struct XmeAidingArray : public XsArrayImpl<XmeAiding, g_XmeAidingArrayDescriptor, XmeAidingArray>
{
	//! \brief Constructs an XmeAidingArray
	inline explicit XmeAidingArray(XsSize sz = 0, XmeAiding const* src = 0)
		: XsArrayImpl<XmeAiding, g_XmeAidingArrayDescriptor, XmeAidingArray>(sz, src)
	{
	}

	//! \brief Constructs an XmeAidingArray as a copy of \a other
	inline XmeAidingArray(XmeAidingArray const& other)
		: XsArrayImpl<XmeAiding, g_XmeAidingArrayDescriptor, XmeAidingArray>(other)
	{
	}

	//! \brief Constructs an XmeAidingArray that references the data supplied in \a ref
	inline explicit XmeAidingArray(XmeAiding* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: XsArrayImpl<XmeAiding, g_XmeAidingArrayDescriptor, XmeAidingArray>(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmeAidingArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmeAidingArray(Iterator beginIt, Iterator endIt)
		: XsArrayImpl<XmeAiding, g_XmeAidingArrayDescriptor, XmeAidingArray>(beginIt, endIt)
	{
	}
};
#endif

#endif
