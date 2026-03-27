#ifndef XMEKINEMATICSTATEARRAY_H
#define XMEKINEMATICSTATEARRAY_H

#include <xstypes/xsarray.h>
#include <xstypes/pstdint.h>

#include "xmedllapi.h"
#ifdef __cplusplus
#include "xmekinematicstate.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmeKinematicStateArrayDescriptor;

#ifndef __cplusplus
#define XmeKinematicStateArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmeKinematicStateArrayDescriptor)

struct XmeKinematicState;
XSARRAY_STRUCT(XmeKinematicStateArray, struct XmeKinematicState);
typedef struct XmeKinematicStateArray XmeKinematicStateArray;

XME_DLL_API void XmeKinematicStateArray_construct(XmeKinematicStateArray* thisPtr, XsSize count, struct XmeKinematicState const* src);
#endif

#ifdef __cplusplus
} // extern "C"

struct XmeKinematicStateArray : public XsArrayImpl<XmeKinematicState, g_XmeKinematicStateArrayDescriptor, XmeKinematicStateArray>
{
	//! \brief Constructs an XmeKinematicStateArray
	inline explicit XmeKinematicStateArray(XsSize sz = 0, XmeKinematicState const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmeKinematicStateArray as a copy of \a other
	inline XmeKinematicStateArray(XmeKinematicStateArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmeKinematicStateArray that references the data supplied in \a ref
	inline explicit XmeKinematicStateArray(XmeKinematicState* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmeKinematicStateArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmeKinematicStateArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}
};
#endif

#endif
