#ifndef XMEJOINT_H
#define XMEJOINT_H

#include "xmedllapi.h"
#include <xstypes/xsstring.h>
#include "xmelocation.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifndef __cplusplus
#define XMEJOINT_INITIALIZER	{ XSSTRING_INITIALIZER, XMELOCATION_INITIALIZER, XMELOCATION_INITIALIZER }
#endif

struct XmeJoint;
typedef struct XmeJoint XmeJoint;

XME_DLL_API void XmeJoint_construct(XmeJoint* thisPtr);
XME_DLL_API void XmeJoint_copyConstruct(XmeJoint* copy, XmeJoint const* src);
XME_DLL_API void XmeJoint_destruct(XmeJoint* thisPtr);
XME_DLL_API void XmeJoint_copy(XmeJoint* copy, XmeJoint const* src);
XME_DLL_API int XmeJoint_equal(XmeJoint const* thisPtr, XmeJoint const* thatPtr);
XME_DLL_API void XmeJoint_swap(XmeJoint* a, XmeJoint* b);
XME_DLL_API int XmeJoint_empty(XmeJoint const* thisPtr);
XME_DLL_API int XmeJoint_valid(XmeJoint const* thisPtr);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Defines a joint between two body segments.
*/
struct XmeJoint
{
#ifdef __cplusplus
	//! \brief basic constructor, creates an empty XmeJoint
	inline XmeJoint() {}

	//! \brief initializing constructor
	inline XmeJoint(const XsString& _name, const XmeLocation& parent, const XmeLocation& child)
		: m_name(_name)
		, m_parentConnection(parent)
		, m_childConnection(child)
	{}

	//! \brief Copy constructor
	inline XmeJoint(const XmeJoint& other)
		: m_name(other.m_name)
		, m_parentConnection(other.m_parentConnection)
		, m_childConnection(other.m_childConnection)
	{}

	//! \brief Destructor
	inline ~XmeJoint() {}

	//! \brief Clear the contents of the XmeJoint
	inline void clear()
	{
		*this = XmeJoint();
	}

	//! \brief Return the name of the joint
	inline const XsString& name() const
	{
		return m_name;
	}

	//! \brief Return where the joint is attached to the parent segment
	inline const XmeLocation& parentConnection() const
	{
		return m_parentConnection;
	}

	//! \brief Return where the joint is attached to the child segment
	inline const XmeLocation& childConnection() const
	{
		return m_childConnection;
	}

	//! \brief Update the values in the XmeJoint
	inline void set(const XsString& _name, const XmeLocation& parent, const XmeLocation& child)
	{
		m_name = _name;
		m_parentConnection = parent;
		m_childConnection = child;
	}

	//! \brief Returns true when the XmeJoint does not contain useful information
	inline bool empty() const
	{
		return m_name.empty() && m_parentConnection.empty() && m_childConnection.empty();
	}

	//! \brief Returns true when the XmeJoint is fully initialized
	inline bool valid() const
	{
		return !m_name.empty() && !m_parentConnection.empty() && !m_childConnection.empty();
	}

	//! \brief Comparison operator
	inline bool operator == (const XmeJoint& other) const
	{
		return	m_parentConnection == other.m_parentConnection &&
			m_childConnection == other.m_childConnection &&
			m_name == other.m_name;
	}

private:
	//! \protectedsection
#endif
	XsString m_name;				//!< The name of the joint, usually starting with j
	XmeLocation m_parentConnection;	//!< The 'parent' segment + point.
	XmeLocation m_childConnection;	//!< The 'child' segment + point. The point is usually the segment origin here.
};

#endif
