#ifndef XMEJOINTARRAY_H
#define XMEJOINTARRAY_H

#include <xstypes/xsarray.h>
#include <xstypes/pstdint.h>

#include "xmedllapi.h"
#ifdef __cplusplus
#include "xmejoint.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmeJointArrayDescriptor;

#ifndef __cplusplus
#define XmeJointArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmeJointArrayDescriptor)

struct XmeJoint;
XSARRAY_STRUCT(XmeJointArray, struct XmeJoint);
typedef struct XmeJointArray XmeJointArray;

XME_DLL_API void XmeJointArray_construct(XmeJointArray* thisPtr, XsSize count, struct XmeJoint const* src);
#endif

#ifdef __cplusplus
} // extern "C"

struct XmeJointArray : public XsArrayImpl<XmeJoint, g_XmeJointArrayDescriptor, XmeJointArray>
{
	//! \brief Constructs an XmeJointArray
	inline explicit XmeJointArray(XsSize sz = 0, XmeJoint const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmeJointArray as a copy of \a other
	inline XmeJointArray(XmeJointArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmeJointArray that references the data supplied in \a ref
	inline explicit XmeJointArray(XmeJoint* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmeJointArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmeJointArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}
};
#endif

#endif
