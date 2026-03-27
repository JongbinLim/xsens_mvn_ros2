#ifndef XMELOCATION_H
#define XMELOCATION_H

#include "xmedllapi.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifndef __cplusplus
#define XMELOCATION_INITIALIZER	{ -1, -1 }
#endif

struct XmeLocation;
typedef struct XmeLocation XmeLocation;

XME_DLL_API int XmeLocation_empty(XmeLocation const* thisPtr);
XME_DLL_API int XmeLocation_compare(XmeLocation const* thisPtr, XmeLocation const* thatPtr);
XME_DLL_API void XmeLocation_clear(XmeLocation* thisPtr);
XME_DLL_API void XmeLocation_swap(XmeLocation* a, XmeLocation* b);

#ifdef __cplusplus
} // extern "C"
#endif

struct XmeLocation
{
#ifdef __cplusplus
	/*! \brief (Default) constructor */
	inline XmeLocation()
		: m_segmentIndex(-1)
		, m_pointIndex(-1)
	{}

	/*! \brief Copy constructor */
	inline XmeLocation(const XmeLocation& loc)
		: m_segmentIndex(loc.m_segmentIndex)
		, m_pointIndex(loc.m_pointIndex)
	{}

	/*! \brief assignment operator */
	inline XmeLocation& operator = (const XmeLocation& loc)
	{
		m_segmentIndex = loc.m_segmentIndex;
		m_pointIndex = loc.m_pointIndex;
		return *this;
	}

	//! \brief Create a new location from a set of segment and point IDs (1-based) \sa fromIndices
	inline static XmeLocation fromIds(int segId, int pointId)
	{
		return XmeLocation(segId - 1, pointId - 1);
	}
	//! \brief Create a new location from a set of segment and point indices (0-based) \sa fromIds
	inline static XmeLocation fromIndices(int segIndex, int pointIndex)
	{
		return XmeLocation(segIndex, pointIndex);
	}
	//! \brief Create a new location from a previously serialized integer value
	inline static XmeLocation fromSerialized(int serialized)
	{
		return serialized < 0 ? XmeLocation() : XmeLocation(serialized / 256, serialized & 0xFF);
	}
	//! \brief Create a new location from a previously serialized integer value
	inline int serialized() const
	{
		return (m_segmentIndex < 0) ? -1 : m_segmentIndex * 256 + m_pointIndex;
	}

	//! \brief Return the segment ID (1-based)
	inline int segmentId() const
	{
		return m_segmentIndex + 1;
	}
	//! \brief Return the point ID (1-based)
	inline int pointId() const
	{
		return m_pointIndex + 1;
	}
	//! \brief Return the segment index (0-based)
	inline int segmentIndex() const
	{
		return m_segmentIndex;
	}
	//! \brief Return the point ID (0-based)
	inline int pointIndex() const
	{
		return m_pointIndex;
	}

	//! \brief Set the segment ID (1-based)
	inline void setSegmentId(int id)
	{
		m_segmentIndex = id - 1;
	}
	//! \brief Set the segment index (0-based)
	inline void setSegmentIndex(int idx)
	{
		m_segmentIndex = idx;
	}
	//! \brief Set the point ID (1-based)
	inline void setPointId(int id)
	{
		m_pointIndex = id - 1;
	}
	//! \brief Set the point index (0-based)
	inline void setPointIndex(int idx)
	{
		m_pointIndex = idx;
	}

	//! \brief Set the segment and point ID (1-based)
	inline void setIds(int segId, int pntId)
	{
		m_segmentIndex = segId - 1;
		m_pointIndex = pntId - 1;
	}
	//! \brief Set the segment and point indices (0-based)
	inline void setIndices(int segIdx, int pntIdx)
	{
		m_segmentIndex = segIdx;
		m_pointIndex = pntIdx;
	}

	/*! \brief Return an empty 'no location' object
	*/
	static XmeLocation noLocation()
	{
		return XmeLocation(-1, -1);
	}

	//! \brief Returns true when there is no location in the object
	inline bool empty() const
	{
		return m_segmentIndex < 0 && m_pointIndex < 0;
	}

	//! \brief Clear the object
	inline void clear()
	{
		m_segmentIndex = -1;
		m_pointIndex = -1;
	}

	//! \brief Swap the contents of the object with \a other
	inline void swap(XmeLocation& other) noexcept
	{
		XmeLocation tmp;
		tmp.m_segmentIndex = m_segmentIndex;
		m_segmentIndex = other.m_segmentIndex;
		other.m_segmentIndex = tmp.m_segmentIndex;

		tmp.m_pointIndex = m_pointIndex;
		m_pointIndex = other.m_pointIndex;
		other.m_pointIndex = tmp.m_pointIndex;
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeLocation& first, XmeLocation& second) noexcept
	{
		first.swap(second);
	}

	//! \brief Return true when the objects are identical
	inline bool operator == (const XmeLocation& loc) const
	{
		return m_segmentIndex == loc.m_segmentIndex && m_pointIndex == loc.m_pointIndex;
	}

	//! \brief Return true when the objects are not identical
	inline bool operator != (const XmeLocation& loc) const
	{
		return !operator == (loc);
	}

	/*! \brief Return true if this location includes \a loc
		\details When one of the IDs is 0, this is interpreted as 'any' segment/point. In this way, XmeLocation(1,0)
		is said to contain XmeLocation(1,3).
	*/
	bool contains(const XmeLocation& loc) const
	{
		return	(m_segmentIndex == -1 || m_segmentIndex == loc.m_segmentIndex) &&
			(m_pointIndex == -1 || m_pointIndex == loc.m_pointIndex);
	}

	/*! \brief Returns true if this location is greater than \a loc
	*/
	bool operator > (const XmeLocation& loc) const
	{
		return XmeLocation_compare(this, &loc) > 0;
	}

	/*! \brief Returns true if this location is less than \a loc
	*/
	bool operator < (const XmeLocation& loc) const
	{
		return XmeLocation_compare(this, &loc) < 0;
	}

protected:
	/*! \brief Initializing constructor */
	inline XmeLocation(int segIdx, int pntIdx)
		: m_segmentIndex(segIdx)
		, m_pointIndex(pntIdx)
	{}

#endif
	int m_segmentIndex;	//!< the segment index (0-based)
	int m_pointIndex;	//!< the point index (0-based)
};

#endif
