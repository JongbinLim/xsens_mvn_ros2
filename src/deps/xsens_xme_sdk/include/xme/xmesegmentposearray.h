#ifndef XMESEGMENTPOSEARRAY_H
#define XMESEGMENTPOSEARRAY_H

#include "xmedllapi.h"
#include <xstypes/xsarray.h>

#ifdef __cplusplus
#include "xmesegmentpose.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmeSegmentPoseArrayDescriptor;

#ifndef __cplusplus
#define XmeSegmentPoseArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmeSegmentPoseArrayDescriptor)

struct XmeSegmentPose;
XSARRAY_STRUCT(XmeSegmentPoseArray, struct XmeSegmentPose);
typedef struct XmeSegmentPoseArray XmeSegmentPoseArray;

XME_DLL_API void XmeSegmentPoseArray_construct(XmeSegmentPoseArray* thisPtr, XsSize count, struct XmeSegmentPose const* src);
#endif

#ifdef __cplusplus
} // extern "C"

struct XmeSegmentPoseArray : public XsArrayImpl<XmeSegmentPose, g_XmeSegmentPoseArrayDescriptor, XmeSegmentPoseArray>
{
	//! \brief Constructs an XmeSegmentPoseArray
	inline explicit XmeSegmentPoseArray(XsSize sz = 0, XmeSegmentPose const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmeSegmentPoseArray as a copy of \a other
	inline XmeSegmentPoseArray(XmeSegmentPoseArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmeSegmentPoseArray that references the data supplied in \a ref
	inline explicit XmeSegmentPoseArray(XmeSegmentPose* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmeSegmentPoseArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmeSegmentPoseArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}
};
#endif

#endif
