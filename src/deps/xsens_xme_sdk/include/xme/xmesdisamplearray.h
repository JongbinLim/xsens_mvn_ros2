#ifndef XMESDISAMPLEARRAY_H
#define XMESDISAMPLEARRAY_H

#include <xstypes/xsarray.h>
#include <xstypes/pstdint.h>

#include "xmedllapi.h"
#ifdef __cplusplus
#include "xmesdisample.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmeSdiSampleArrayDescriptor;

#ifndef __cplusplus
#define XmeSdiSampleArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmeSdiSampleArrayDescriptor)

struct XmeSdiSample;
XSARRAY_STRUCT(XmeSdiSampleArray, struct XmeSdiSample);
typedef struct XmeSdiSampleArray XmeSdiSampleArray;

XME_DLL_API void XmeSdiSampleArray_construct(XmeSdiSampleArray* thisPtr, XsSize count, struct XmeSdiSample const* src);
#endif

#ifdef __cplusplus
} // extern "C"

struct XmeSdiSampleArray : public XsArrayImpl<XmeSdiSample, g_XmeSdiSampleArrayDescriptor, XmeSdiSampleArray>
{
	//! \brief Constructs an XmeSdiSampleArray
	inline explicit XmeSdiSampleArray(XsSize sz = 0, XmeSdiSample const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmeSdiSampleArray as a copy of \a other
	inline XmeSdiSampleArray(XmeSdiSampleArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmeSdiSampleArray that references the data supplied in \a ref
	inline explicit XmeSdiSampleArray(XmeSdiSample* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmeSdiSampleArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmeSdiSampleArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}
};
#endif

#endif
