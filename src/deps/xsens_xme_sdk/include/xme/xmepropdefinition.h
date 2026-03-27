#ifndef XMEPROPDEFINITION_H
#define XMEPROPDEFINITION_H

#include "xmedllapi.h"
#include <xstypes/xsstring.h>
#include "xmelocation.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifndef __cplusplus
#define XMEPROPDEFINITION_INITIALIZER	{ -1, XSSTRING_INITIALIZER, XMELOCATION_INITIALIZER }
#endif

struct XmePropDefinition;
typedef struct XmePropDefinition XmePropDefinition;

XME_DLL_API void XmePropDefinition_construct(XmePropDefinition* thisPtr);
XME_DLL_API void XmePropDefinition_copyConstruct(XmePropDefinition* thisPtr, XmePropDefinition const* src);
XME_DLL_API void XmePropDefinition_destruct(XmePropDefinition* thisPtr);
XME_DLL_API void XmePropDefinition_copy(XmePropDefinition* thisPtr, XmePropDefinition const* src);
XME_DLL_API void XmePropDefinition_swap(XmePropDefinition* a, XmePropDefinition* b);
XME_DLL_API int XmePropDefinition_compare(XmePropDefinition const* a, XmePropDefinition const* b);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief The definition of a single prop
*/
struct XmePropDefinition
{
#ifdef __cplusplus
	//! Basic constructor
	inline explicit XmePropDefinition(const XsString& uniqueName = XsString(), XmeLocation const& _parentConnection = XmeLocation(), int segId = -1)
		: m_uniqueName(uniqueName)
		, m_parentConnection(_parentConnection)
		, m_segmentId(segId)
	{
	}

	//! Copy constructor
	inline XmePropDefinition(const XmePropDefinition& other)
		: m_uniqueName(other.m_uniqueName)
		, m_parentConnection(other.m_parentConnection)
		, m_segmentId(other.m_segmentId)
	{
	}

	//! Destructor
	inline ~XmePropDefinition() {}

	//! Clear the contents of the marker
	inline void clear()
	{
		*this = XmePropDefinition();
	}

	//! \brief Compare the two definitions and return true if they're equal
	inline bool operator == (XmePropDefinition const& rhs) const
	{
		return !XmePropDefinition_compare(this, &rhs);
	}

	// pod structure
#endif

	XsString m_uniqueName;	//!< The unique name of the prop. Each segment has a unique name and so each defined prop must also get a unique name.
	XmeLocation m_parentConnection;	//!< The location that the prop is connected to.
	int m_segmentId;		//!< The segment ID of the prop, ignored as input, valid as output.
};

#endif
