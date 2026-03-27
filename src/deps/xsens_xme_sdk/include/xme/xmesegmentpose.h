#ifndef XMESEGMENTPOSE_H
#define XMESEGMENTPOSE_H

#include "xmedllapi.h"
#include <xstypes/xsvector3.h>
#include <xstypes/xsquaternion.h>

#ifdef __cplusplus
extern "C" {
#endif

struct XmeSegmentPose;
#ifndef __cplusplus
typedef struct XmeSegmentPose XmeSegmentPose;
#endif

XME_DLL_API void XmeSegmentPose_construct(XmeSegmentPose* thisPtr);
XME_DLL_API void XmeSegmentPose_copyConstruct(XmeSegmentPose* thisPtr, XmeSegmentPose const* src);
XME_DLL_API void XmeSegmentPose_destruct(XmeSegmentPose* thisPtr);
XME_DLL_API void XmeSegmentPose_copy(XmeSegmentPose* copy, XmeSegmentPose const* src);
XME_DLL_API void XmeSegmentPose_swap(XmeSegmentPose* a, XmeSegmentPose* b);
XME_DLL_API int XmeSegmentPose_compare(XmeSegmentPose const* a, XmeSegmentPose const* b);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Contains the pose of a single object
	\details This is just an orientation and position as opposed to a full XmeKinematicState
	\sa XmeKinematicState
*/
struct XmeSegmentPose
{
#ifdef __cplusplus
	/*! \brief Initializing constructor
		\param ori \copydoc m_orientation
		\param pos \copydoc m_position
	*/
	XmeSegmentPose(const XsQuaternion& ori = XsQuaternion::identity(),
		const XsVector& pos = XsVector3::zero3())
		: m_orientation(ori)
		, m_position(pos)
	{
	}

	/*! \brief Copy constructor, copies the contents of \a other */
	XmeSegmentPose(const XmeSegmentPose& other)
		: m_orientation(other.m_orientation)
		, m_position(other.m_position)
	{
	}

	//! \brief Destructor
	~XmeSegmentPose() {}


	/*! \brief Full setter, updates all contained values
		\param ori \copydoc m_orientation
		\param pos \copydoc m_position
	*/
	inline void set(const XsQuaternion& ori, const XsVector& pos)
	{
		m_orientation = ori;
		m_position = pos;
	}


	//! \brief Clear the object
	inline void clear()
	{
		m_orientation = XsQuaternion::identity();
		m_position.zero();
	}

	//! \brief Swap the contents with \a other
	inline void swap(XmeSegmentPose& other) noexcept
	{
		XmeSegmentPose_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeSegmentPose& first, XmeSegmentPose& second) noexcept
	{
		first.swap(second);
	}

	//! \brief Comparison operator, returns true when the contents are identical
	inline bool operator == (const XmeSegmentPose& other) const
	{
		return	m_orientation == other.m_orientation &&
			m_position == other.m_position;
	}

#endif
	XsQuaternion m_orientation;		//!< The orientation of the segment with respect to the world
	XsVector3 m_position;			//!< The position of the segment with respect to the world

};

#endif
