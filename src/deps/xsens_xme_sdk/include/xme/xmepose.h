#ifndef XMEPOSE_H
#define XMEPOSE_H

#include <xstypes/pstdint.h>
#include "xmekinematicstatearray.h"
#include "xmelocationarray.h"
#include "xmesegmentposearray.h"
#include <xstypes/xsvector3.h>

#ifdef __cplusplus
extern "C" {
#endif

struct XmePose;
#ifndef __cplusplus
typedef struct XmePose XmePose;
#endif

XME_DLL_API void XmePose_construct(XmePose* thisPtr);
XME_DLL_API void XmePose_destruct(XmePose* thisPtr);
XME_DLL_API void XmePose_copy(XmePose* copy, XmePose const* src);
XME_DLL_API void XmePose_swap(XmePose* a, XmePose* b);
XME_DLL_API int XmePose_empty(XmePose const* thisPtr);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Contains a single computed pose of the system
*/
struct XmePose
{
#ifdef __cplusplus
	//! \brief Default constructor
	XmePose()
		: m_frameNumber(0)
		, m_relativeTime(0)
		, m_absoluteTime(0)
	{
	}

	//! \brief Copy constructor, copies the contents of \a other
	XmePose(XmePose const& other)
		: m_frameNumber(0)
		, m_relativeTime(0)
		, m_absoluteTime(0)
	{
		XmePose_copy(this, &other);
	}

	/*! \brief Initializing constructor
		\param frame \copydoc m_frameNumber
		\param tRel	\copydoc m_relativeTime
		\param tAbs	\copydoc m_absoluteTime
		\param segs	\copydoc m_segmentStates
		\param contacts	\copydoc m_contacts
	*/
	XmePose(int32_t frame, int64_t tRel, int64_t tAbs, const XmeKinematicStateArray& segs, const XmeLocationArray& contacts)
		: m_frameNumber(frame)
		, m_relativeTime(tRel)
		, m_absoluteTime(tAbs)
		, m_segmentStates(segs)
		, m_contacts(contacts)
	{
	}

	/*! \brief Initializing constructor
		\param frame \copydoc m_frameNumber
		\param tRel	\copydoc m_relativeTime
		\param tAbs	\copydoc m_absoluteTime
		\param segs	\copydoc m_segmentStates
		\param contacts	\copydoc m_contacts
		\param leftHandOut \copydoc m_leftHandOutput
		\param rightHandOut \copydoc m_rightHandOutput
	*/
	XmePose(int32_t frame, int64_t tRel, int64_t tAbs, const XmeKinematicStateArray& segs, const XmeLocationArray& contacts, const XmeSegmentPoseArray& leftHandOut, const XmeSegmentPoseArray& rightHandOut)
		: m_frameNumber(frame)
		, m_relativeTime(tRel)
		, m_absoluteTime(tAbs)
		, m_segmentStates(segs)
		, m_contacts(contacts)
		, m_leftHandOutput(leftHandOut)
		, m_rightHandOutput(rightHandOut)
	{
	}

	/*! \brief Initializing constructor
		\details Initializes the pose with room for data of \a segmentCount segments
		\param segmentCount The desired number of segments in the pose
	*/
	explicit XmePose(XsSize segmentCount)
		: m_frameNumber(0)
		, m_relativeTime(0)
		, m_absoluteTime(0)
		, m_segmentStates(segmentCount)
		, m_contacts(0)
		, m_leftHandOutput(0)
		, m_rightHandOutput(0)
	{
	}

	//! \copydoc XmePose_empty
	inline bool empty() const
	{
		return 0 != XmePose_empty(this);
	}

	//! \brief Clear the pose, removing all segment data
	inline void clear()
	{
		*this = XmePose();
	}

	//! \brief Swap the contents of the pose with \a other
	inline void swap(XmePose& other) noexcept
	{
		XmePose_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmePose& first, XmePose& second) noexcept
	{
		first.swap(second);
	}

	//! \brief Assignment operator, copy the contents of \a other
	XmePose& operator = (XmePose const& other)
	{
		XmePose_copy(this, &other);
		return *this;
	}

	//! \brief Comparison operator, returns true when all contents are identical
	bool operator == (XmePose const& other) const
	{
		return m_frameNumber == other.m_frameNumber &&
			m_relativeTime == other.m_relativeTime &&
			m_absoluteTime == other.m_absoluteTime &&
			m_segmentStates == other.m_segmentStates &&
			m_contacts == other.m_contacts &&
			m_leftHandOutput == other.m_leftHandOutput &&
			m_rightHandOutput == other.m_rightHandOutput;
	}

	// private: pod structure
#endif
	int32_t m_frameNumber;		//!< The frame number of this pose
	int64_t m_relativeTime;		//!< The time since start of recording of this pose
	int64_t m_absoluteTime;		//!< The absolute time of recording of this pose
	XmeKinematicStateArray m_segmentStates;	//!< The actual Newtonian pose data of all the segments
	XmeLocationArray m_contacts;			//!< A list of detected "contacts with the environment"
	XmeSegmentPoseArray m_leftHandOutput;	//!< A list of the left hand segment data calculated from the glove input and xsens hand sensor
	XmeSegmentPoseArray m_rightHandOutput;	//!< A list of the right hand segment data calculated from the glove input and xsens hand sensor
};

#endif
