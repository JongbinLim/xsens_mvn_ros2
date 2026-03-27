#ifndef XMEPOINT_H
#define XMEPOINT_H

#include "xmedllapi.h"
#include <xstypes/xsstring.h>
#include <xstypes/xsvector3.h>
#include "xmelocation.h"

#ifdef __cplusplus
extern "C" {
#endif

struct XmePoint;
typedef struct XmePoint XmePoint;

XME_DLL_API void XmePoint_construct(XmePoint* thisPtr);
XME_DLL_API void XmePoint_copyConstruct(XmePoint* thisPtr, XmePoint const* src);
XME_DLL_API void XmePoint_destruct(XmePoint* thisPtr);
XME_DLL_API void XmePoint_copy(XmePoint* copy, XmePoint const* src);
XME_DLL_API int XmePoint_equal(XmePoint const* thisPtr, XmePoint const* thatPtr);
XME_DLL_API void XmePoint_swap(XmePoint* a, XmePoint* b);
XME_DLL_API int XmePoint_empty(XmePoint const* thisPtr);

#ifdef __cplusplus
} // extern "C"
#endif

/*!	\addtogroup enums
	@{
*/
/*! Flags that describe some what the point can be used for */
enum XmePointFlags
{
	XPF_NoFlags				= 0,	//!< No flags set
	XPF_PossibleContact		= 1,	//!< The point is considered as a potential contact during contact detection
	XPF_AnatomicalLandmark	= 2		//!< The point is external to the body and can be used as an anatomical landmark for measurements. (as opposed to points that are inside joints)
};
//! @}
typedef enum XmePointFlags XmePointFlags;

/*! \brief A point on the body
*/
struct XmePoint
{
#ifdef __cplusplus
	//! Default constructor
	XmePoint() : m_flags(XPF_NoFlags)
	{
		m_offset.setZero();
	}

	//! Initializing constructor
	XmePoint(const XsString& nm, const XmeLocation& loc, const XsVector& offs, int flags = XPF_NoFlags)
		: m_name(nm)
		, m_location(loc)
		, m_flags(flags)
	{
		assert(offs.size() == 3);
		m_offset = offs;
	}

	//! The name of the point
	inline const XsString& name() const
	{
		return m_name;
	}

	//! The offset of the point with respect to the unrotated segment (so when the segment's orientation is a unit quaaternion)
	inline const XsVector3& offset() const
	{
		return m_offset;
	}

	//! The location (segment ID + point ID) of this point
	inline const XmeLocation& location() const
	{
		return m_location;
	}

	/*! \brief Some flags that describe what the point can be used for \sa XmePointFlags
		\return The flags
	*/
	inline int flags() const
	{
		return m_flags;
	}

	//! Returns whether the point is considered as a potential contact during contact detection
	inline bool possibleContact() const
	{
		return !!(m_flags & XPF_PossibleContact);
	}

	//! Returns whether the point is external to the body and can be used as an anatomical landmark for measurements. (as opposed to points that are inside joints)
	inline bool anatomicalLandmark() const
	{
		return !!(m_flags & XPF_AnatomicalLandmark);
	}

	//! Comparison operator
	inline bool operator == (const XmePoint& that) const
	{
		return !!XmePoint_equal(this, &that);
	}

	//! Comparison operator
	inline bool operator != (const XmePoint& that) const
	{
		return !XmePoint_equal(this, &that);
	}

	/*! \brief Update the point
		\param nm The new name
		\param loc The new location
		\param offs The new offset with respect to the segment origin
		\param flags The new flags \sa XmePointFlags
	*/
	inline void set(const XsString& nm, const XmeLocation& loc, const XsVector& offs, int flags = XPF_NoFlags)
	{
		m_name = nm;
		m_location = loc;
		assert(offs.empty() || offs.size() == 3);
		m_offset = offs;
		m_flags = flags;
	}

	/*! \brief Make the point a possible contact \sa possibleContact()
		\param yes Set to true to mark this point as a possible contact
	*/
	inline void setPossibleContact(bool yes)
	{
		if (yes)
			m_flags |= XPF_PossibleContact;
		else
			m_flags &= ~XPF_PossibleContact;
	}

	/*! \brief Make the point an anatomical landmark \sa anatomicalLandmark()
		\param yes Set to true to mark this point as an anatomical landmark
	*/
	inline void setAnatomicalLandmark(bool yes)
	{
		if (yes)
			m_flags |= XPF_AnatomicalLandmark;
		else
			m_flags &= ~XPF_AnatomicalLandmark;
	}

	/*! \brief Returns true when the point is considered to be invalid
		\return true when the point is considered to be invalid
	*/
	inline bool empty() const
	{
		return m_location.empty() || m_name.empty();
	}

	//! Clear the contents of the point, making it invalid
	inline void clear()
	{
		*this = XmePoint();
	}

	//! Swap the contents with another XmePoint
	inline void swap(XmePoint& other) noexcept
	{
		m_name.swap(other.m_name);
		m_location.swap(other.m_location);
		m_offset.swap(other.m_offset);
		int tmp = other.m_flags;
		other.m_flags = m_flags;
		m_flags = tmp;
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmePoint& first, XmePoint& second) noexcept
	{
		first.swap(second);
	}

private:
	//! \protectedsection
#endif
	XsString m_name;		//!< The name of the point
	XmeLocation m_location;	//!< The location (segment ID + point ID) of this point
	XsVector3 m_offset;		//!< The offset of the point with respect to the unrotated segment (so when the segment's orientation is a unit quaaternion)
	int m_flags;			//!< Some flags that describe what the point can be used for \sa XmePointFlags
};

#endif

