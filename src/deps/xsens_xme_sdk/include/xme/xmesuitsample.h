#ifndef XMESUITSAMPLE_H
#define XMESUITSAMPLE_H

#include "xmedllapi.h"
#include <xstypes/pstdint.h>
#include "xmesdisamplearray.h"
#include "xmesensorkinematicsarray.h"
#include "xmegnssdata.h"

#ifdef __cplusplus
extern "C" {
#endif

struct XmeSuitSample;
typedef struct XmeSuitSample XmeSuitSample;

XME_DLL_API void XmeSuitSample_construct(XmeSuitSample* thisPtr);
XME_DLL_API void XmeSuitSample_destruct(XmeSuitSample* thisPtr);
XME_DLL_API void XmeSuitSample_copy(XmeSuitSample* copy, XmeSuitSample const* src);
XME_DLL_API void XmeSuitSample_copyConstruct(XmeSuitSample* thisPtr, XmeSuitSample const* src);
XME_DLL_API void XmeSuitSample_swap(XmeSuitSample* a, XmeSuitSample* b);
XME_DLL_API int XmeSuitSample_empty(XmeSuitSample const* thisPtr);
XME_DLL_API int XmeSuitSample_compare(XmeSuitSample const* a, XmeSuitSample const* b);
XME_DLL_API void XmeSuitSample_setGnssData(XmeSuitSample* thisPtr, XmeGnssData const* data);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Contains measured data from all Motion Trackers in a single suit
*/
struct XmeSuitSample
{
#ifdef __cplusplus
	//! \brief Default constructor
	XmeSuitSample()
		: m_relativeTime(0)
		, m_absoluteTime(0)
		, m_sampleCounter(0)
		, m_gnssData(nullptr)
	{
	}

	//! \brief Copy constructor, copies contents of \a other
	XmeSuitSample(const XmeSuitSample& other)
		: m_sdi(other.m_sdi)
		, m_sensorKinematics(other.m_sensorKinematics)
		, m_relativeTime(other.m_relativeTime)
		, m_absoluteTime(other.m_absoluteTime)
		, m_sampleCounter(other.m_sampleCounter)
		, m_gnssData(nullptr)
	{
		if (other.m_gnssData)
			XmeSuitSample_setGnssData(this, other.m_gnssData);
	}

	//! \brief Destructor
	~XmeSuitSample()
	{
		XmeSuitSample_destruct(this);
	}

	//! \brief Assignment operator, copies contents from \a other
	XmeSuitSample& operator = (const XmeSuitSample& other)
	{
		XmeSuitSample_copy(this, &other);
		return *this;
	}

	//! \copydoc XmeSuitSample_empty
	bool empty() const
	{
		return 0 != XmeSuitSample_empty(this);
	}

	//! \brief Clear the object
	void clear()
	{
		*this = XmeSuitSample();
	}

	//! \brief Swap the contents with \a other
	void swap(XmeSuitSample& other) noexcept
	{
		XmeSuitSample_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeSuitSample& first, XmeSuitSample& second) noexcept
	{
		first.swap(second);
	}

	/*! \brief Return true if the \a other is identical*/
	bool operator == (const XmeSuitSample& other) const
	{
		return !XmeSuitSample_compare(this, &other);
	}

	/*! \brief Return true if the \a other is not identical*/
	bool operator != (const XmeSuitSample& other) const
	{
		return !!XmeSuitSample_compare(this, &other);
	}

public:
	//! Returns the time since start of recording of this sample
	inline int64_t relativeTime() const
	{
		return m_relativeTime;
	}
	//! Sets the time since start of recording of this sample \note This function does not enforce consistency with the absolute time
	inline void setRelativeTime(int64_t t)
	{
		m_relativeTime = t;
	}
	//! Returns the absolute time of this sample
	inline int64_t absoluteTime() const
	{
		return m_absoluteTime;
	}
	//! Sets the absolute time of this sample \note This function does not enforce consistency with the relative time
	inline void setAbsoluteTime(int64_t t)
	{
		m_absoluteTime = t;
	}
	//! Returns the sample counter of this sample
	inline int64_t sampleCounter() const
	{
		return m_sampleCounter;
	}
	//! Sets the sample counter of this sample
	inline void setSampleCounter(int64_t t)
	{
		m_sampleCounter = t;
	}

	//! Returns an array of SDI sensor data for all Motion Trackers.
	inline XmeSdiSampleArray& sdiData()
	{
		return m_sdi;
	}
	//! Returns an array of SDI sensor data for all Motion Trackers.
	inline XmeSdiSampleArray const& sdiData() const
	{
		return m_sdi;
	}
	//! Returns an array of processed sensor data for all Motion Trackers.
	inline XmeSensorKinematicsArray& sensorKinematics()
	{
		return m_sensorKinematics;
	}
	//! Returns an array of processed sensor data for all Motion Trackers.
	inline XmeSensorKinematicsArray const& sensorKinematics() const
	{
		return m_sensorKinematics;
	}
	//! Returns the GNSS data. The returned object is always valid, but may be empty.
	inline XmeGnssData const& gnssData() const
	{
		static const XmeGnssData noData = XmeGnssData();
		if (m_gnssData)
			return *m_gnssData;
		else
			return noData;
	}
	//! Returns the GNSS data. If it does not exist yet, an empty GNSS data item will be created.
	inline XmeGnssData& gnssData()
	{
		static const XmeGnssData noData = XmeGnssData();
		if (!m_gnssData)
			XmeSuitSample_setGnssData(this, &noData);
		return *m_gnssData;
	}
	//! Returns whether the object contains GNSS data.
	inline bool hasGnssData() const
	{
		return !!m_gnssData;
	}
	
protected:
#endif
	XmeSdiSampleArray m_sdi;		//!< An array of SDI sensor data for all Motion Trackers
	XmeSensorKinematicsArray m_sensorKinematics;	//!< An array of processed sensor data for all Motion Trackers
	int64_t m_relativeTime;			//!< The time since start of recording of this sample
	int64_t m_absoluteTime;			//!< The absolute time of this sample
	int64_t m_sampleCounter;		//!< The sample counter of this sample
	XmeGnssData* m_gnssData;		//!< GNSS Data (optional)
};

#endif
