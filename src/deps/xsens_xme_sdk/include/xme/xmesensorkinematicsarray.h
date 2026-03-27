#ifndef XMESENSORKINEMATICSARRAY_H
#define XMESENSORKINEMATICSARRAY_H

#include <xstypes/xsarray.h>
#include <xstypes/pstdint.h>

#include "xmedllapi.h"
#ifdef __cplusplus
#include "xmesensorkinematics.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmeSensorKinematicsArrayDescriptor;

#ifndef __cplusplus
#define XmeSensorKinematicsArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmeSensorKinematicsArrayDescriptor)

struct XmeSensorKinematics;
XSARRAY_STRUCT(XmeSensorKinematicsArray, struct XmeSensorKinematics);
typedef struct XmeSensorKinematicsArray XmeSensorKinematicsArray;

XME_DLL_API void XmeSensorKinematicsArray_construct(XmeSensorKinematicsArray* thisPtr, XsSize count, struct XmeSensorKinematics const* src);
#endif

#ifdef __cplusplus
} // extern "C"

struct XmeSensorKinematicsArray : public XsArrayImpl<XmeSensorKinematics, g_XmeSensorKinematicsArrayDescriptor, XmeSensorKinematicsArray>
{
	//! \brief Constructs an XmeSensorKinematicsArray
	inline explicit XmeSensorKinematicsArray(XsSize sz = 0, XmeSensorKinematics const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmeSensorKinematicsArray as a copy of \a other
	inline XmeSensorKinematicsArray(XmeSensorKinematicsArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmeSensorKinematicsArray that references the data supplied in \a ref
	inline explicit XmeSensorKinematicsArray(XmeSensorKinematics* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmeSensorKinematicsArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmeSensorKinematicsArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}
};
#endif

#endif
