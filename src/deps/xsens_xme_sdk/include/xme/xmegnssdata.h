#ifndef XMEGNSSDATA_H
#define XMEGNSSDATA_H

#include "xmedllapi.h"
#include <xstypes/xsrawgnsspvtdata.h>
#include "xmedef.h"
#include <xstypes/xsvector3.h>

#ifdef __cplusplus
extern "C" {
#endif

struct XmeGnssData;
typedef struct XmeGnssData XmeGnssData;

#ifndef SIT
XME_DLL_API void XmeGnssData_construct(XmeGnssData* thisPtr);
XME_DLL_API void XmeGnssData_destruct(XmeGnssData* thisPtr);
XME_DLL_API void XmeGnssData_swap(XmeGnssData* a, XmeGnssData* b);
XME_DLL_API void XmeGnssData_copy(XmeGnssData* thisPtr, XmeGnssData const* other);
XME_DLL_API int XmeGnssData_empty(XmeGnssData const* thisPtr);
XME_DLL_API int XmeGnssData_compare(XmeGnssData const* a, XmeGnssData const* b);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Contains sample data as recorded by a master device, such as an Xsens Bodypack
*/
struct XmeGnssData
{
#ifdef __cplusplus
	//! \brief Default contructor, constructs an empty object
	XmeGnssData()
		: m_gnss()
		, m_gnssAge(XME_AGE_MAX)
		, m_latLonAlt(0.0, 0.0, 0.0)
	{
	}

	/*! \brief Initializing constructor
		\param gnss \copydoc m_gnss
		\param gnssAge \copydoc m_gnssAge
		\param lla \copydoc m_latLonAlt
	*/
	XmeGnssData(XsRawGnssPvtData const& gnss, int gnssAge, XsVector const& lla = XsVector3::zero3())
		: m_gnss(gnss)
		, m_gnssAge((int16_t) gnssAge)
		, m_latLonAlt(lla)
	{
	}

	//! \brief Destructor
	~XmeGnssData()
	{
	}

	//! \brief Clear the object
	inline void clear()
	{
		*this = XmeGnssData();
	}

	//! \brief Swap the contents of the object with \a other
	inline void swap(XmeGnssData& other) noexcept
	{
		XmeGnssData_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeGnssData& first, XmeGnssData& second) noexcept
	{
		first.swap(second);
	}

	/*! \brief Return true if the \a other is numerically equal to this, considering only available data */
	inline bool operator == (const XmeGnssData& other) const
	{
		if (haveGnss() && (m_gnssAge != other.m_gnssAge || !(m_gnss == other.m_gnss)))
			return false;

		return m_latLonAlt == other.m_latLonAlt;
	}

	/*! \brief Return true if the GNSS data is available */
	inline bool haveGnss() const
	{
		return m_gnssAge < XME_AGE_MAX;
	}

	//! \brief Return true if the object is empty
	inline bool empty() const
	{
		return !haveGnss();
	}

	//private:	mostly pod structure, all direct access allowed
#endif

	XsRawGnssPvtData m_gnss;	//!< GNSS data
	int16_t		m_gnssAge;		//!< Age of the GNSS data (in samples), will be 0 in recordings when the data is valid, but larger for live systems. When a drop in age with respect to the previous frame occurs, new data is available. When less than 0, the m_gnss field should not be used yet as the data is from the future. When set to XME_AGE_MAX the data should not be considered valid at all.
	XsVector3	m_latLonAlt;	//!< Latitude, Longitude, Altitude output values of Pelvis segment (if available)
};

#endif
