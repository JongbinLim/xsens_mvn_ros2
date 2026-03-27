#ifndef XMESDISAMPLE_H
#define XMESDISAMPLE_H

#include "xmedllapi.h"
#include <xstypes/xsvector3.h>
#include <xstypes/xsquaternion.h>

#ifdef __cplusplus
extern "C" {
#endif

struct XmeSdiSample;
typedef struct XmeSdiSample XmeSdiSample;

XME_DLL_API void XmeSdiSample_construct(XmeSdiSample* thisPtr);
XME_DLL_API void XmeSdiSample_destruct(XmeSdiSample* thisPtr);
XME_DLL_API void XmeSdiSample_copy(XmeSdiSample* copy, XmeSdiSample const* src);
XME_DLL_API void XmeSdiSample_copyConstruct(XmeSdiSample* thisPtr, XmeSdiSample const* src);
XME_DLL_API void XmeSdiSample_swap(XmeSdiSample* a, XmeSdiSample* b);
XME_DLL_API int XmeSdiSample_empty(XmeSdiSample const* thisPtr);
XME_DLL_API int XmeSdiSample_compare(XmeSdiSample const* a, XmeSdiSample const* b);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Contains sample data as recorded by a Motion Tracker in SDI format
*/
struct XmeSdiSample
{
#ifdef __cplusplus
	//! \brief Default constructor
	XmeSdiSample()
		: m_dataFlags(0)
		, m_clipBits(0)
		, m_dq(0.0, 0.0, 0.0, 0.0)
		, m_barometer(0.0)
	{
		m_dv.zero();
		m_mag.zero();
	}

	//! \brief Copy constructor, copies contents of \a other
	XmeSdiSample(const XmeSdiSample& other)
		: m_dataFlags(other.m_dataFlags)
		, m_clipBits(other.m_clipBits)
		, m_dq(other.m_dq)
		, m_dv(other.m_dv)
		, m_mag(other.m_mag)
		, m_barometer(other.m_barometer)
	{
	}

	//! \brief Destructor
	~XmeSdiSample()
	{}

	//! \brief Assignment operator, copies contents of \a other
	XmeSdiSample& operator = (const XmeSdiSample& other)
	{
		XmeSdiSample_copy(this, &other);
		return *this;
	}

	//! \brief Return true when the object contains no data
	bool empty() const
	{
		return 0 != XmeSdiSample_empty(this);
	}

	//! \brief Clear the object
	void clear()
	{
		*this = XmeSdiSample();
	}

	//! \brief Swap the contents with \a other
	void swap(XmeSdiSample& other) noexcept
	{
		XmeSdiSample_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeSdiSample& first, XmeSdiSample& second) noexcept
	{
		first.swap(second);
	}

	/*! \brief Return true if the \a other is identical to this */
	bool operator == (const XmeSdiSample& other) const
	{
		return !XmeSdiSample_compare(this, &other);
	}

private:
	//! \brief Some flags to describe the contents of the structure
	enum Flags : uint16_t
	{
		BarometerAvailable	= 0x01,	//!< Indicates that barometer data is available
		InterpolatedSample	= 0x02,	//!< Indicates that the sample was computed by interpolation from a larger interval
		MagnetometerOld		= 0x04,	//!< Indicates that the magnetometer data is old
		Retransmitted		= 0x08,	//!< Indicates that the data was received from a retransmission. This does not affect the quality of the recording stream. The flag is for diagnostic purposes only.
	};

	//! \brief Set or clear one of the flags
	inline void setFlag(const Flags flag, bool yes)
	{
		m_dataFlags = (m_dataFlags & ~static_cast<uint16_t>(flag)) | (yes ? static_cast<uint16_t>(flag) : 0);
	}
	//! \brief Return true if the requested flag is set
	inline bool flagIsSet(const Flags flag) const
	{
		return !!(m_dataFlags & static_cast<uint16_t>(flag));
	}

public:
	//! Returns true when barometer data is available
	bool isBarometerAvailable() const
	{
		return flagIsSet(Flags::BarometerAvailable);
	}
	//! Sets whether barometer data is available
	void setBarometerAvailable(bool yes)
	{
		setFlag(Flags::BarometerAvailable, yes);
	}
	//! Returns true when the sample was interpolated from a larger interval
	bool isInterpolatedSample() const
	{
		return flagIsSet(Flags::InterpolatedSample);
	}
	//! Set whether the sample was interpolated from a larger interval
	void setInterpolatedSample(bool yes)
	{
		setFlag(Flags::InterpolatedSample, yes);
	}
	//! Returns true when the magnetometer data is old, false when it's new
	bool isMagnetometerOld() const
	{
		return flagIsSet(Flags::MagnetometerOld);
	}
	//! Set if the magnetometer data is old
	void setMagnetometerOld(bool yes)
	{
		setFlag(Flags::MagnetometerOld, yes);
	}
	//! Returns true when the retransmitted flag is set
	bool isRetransmitted() const
	{
		return flagIsSet(Flags::Retransmitted);
	}
	//! Set if the sample was retransmitted or not. When switched to true, the InterpolatedSample flag should usually be disabled.
	void setRetransmitted(bool yes)
	{
		setFlag(Flags::Retransmitted, yes);
	}

	//private:	pod structure, all direct access allowed
#endif
	uint16_t m_dataFlags;	//!< Flags that describe the validity of parts of the structure. \sa isBarometerAvailable() \sa isInterpolatedSample() \sa isMagnetometerOld()
	uint16_t m_clipBits;	//!< Clipping information: low 3 bits are accelerometer clipping (X-Y-Z channel), the next 3 are for gyroscope clipping (X-Y-Z channel), the next 3 are for magnetometer clipping (X-Y-Z channel)
	XsQuaternion m_dq;		//!< SDI delta-orientation. Not available will return the identity quaternion.
	XsVector3 m_dv;			//!< SDI delta-velocity in m/s. Not available will return zero.
	XsVector3 m_mag;		//!< The magnetometer data in arbitrary units
	XsReal m_barometer;		//!< The barometer data in Pascal
};

#endif
