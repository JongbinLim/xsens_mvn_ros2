#ifndef XMEPROPDEFINITIONARRAY_H
#define XMEPROPDEFINITIONARRAY_H

#include "xmedllapi.h"
#include <xstypes/xsarray.h>

#ifdef __cplusplus
#include "xmepropdefinition.h"
extern "C" {
#endif

extern XsArrayDescriptor const XME_DLL_API g_XmePropDefinitionArrayDescriptor;

#ifndef __cplusplus
#define XmePropDefinitionArray_INITIALIZER	XSARRAY_INITIALIZER(&g_XmePropDefinitionArrayDescriptor)

struct XmePropDefinition;
XSARRAY_STRUCT(XmePropDefinitionArray, struct XmePropDefinition);
typedef struct XmePropDefinitionArray XmePropDefinitionArray;

XME_DLL_API void XmePropDefinitionArray_construct(XmePropDefinitionArray* thisPtr, XsSize count, struct XmePropDefinition const* src);
#endif

#ifdef __cplusplus
} // extern "C"

struct XmePropDefinitionArray : public XsArrayImpl<XmePropDefinition, g_XmePropDefinitionArrayDescriptor, XmePropDefinitionArray>
{
	//! \brief Constructs an XmePropDefinitionArray
	inline explicit XmePropDefinitionArray(XsSize sz = 0, XmePropDefinition const* src = 0)
		: ArrayImpl(sz, src)
	{
	}

	//! \brief Constructs an XmePropDefinitionArray as a copy of \a other
	inline XmePropDefinitionArray(XmePropDefinitionArray const& other)
		: ArrayImpl(other)
	{
	}

	//! \brief Constructs an XmePropDefinitionArray that references the data supplied in \a ref
	inline explicit XmePropDefinitionArray(XmePropDefinition* ref, XsSize sz, XsDataFlags flags /* = XSDF_None */)
		: ArrayImpl(ref, sz, flags)
	{
	}

	//! \brief Constructs an XmePropDefinitionArray with the array bound by the supplied iterators \a beginIt and \a endIt
	template <typename Iterator>
	inline XmePropDefinitionArray(Iterator beginIt, Iterator endIt)
		: ArrayImpl(beginIt, endIt)
	{
	}
};
#endif

#endif
