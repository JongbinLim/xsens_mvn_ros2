#ifndef XMEDEVICESTATUS_H
#define XMEDEVICESTATUS_H

#include "xmedllapi.h"
#include <xstypes/xsdeviceid.h>
#include <xstypes/xssimpleversion.h>

#ifdef __cplusplus
extern "C" {
#endif

struct XmeDeviceStatus;
typedef struct XmeDeviceStatus XmeDeviceStatus;

XME_DLL_API void XmeDeviceStatus_construct(XmeDeviceStatus* thisPtr);
XME_DLL_API void XmeDeviceStatus_copyConstruct(XmeDeviceStatus* thisPtr, XmeDeviceStatus const* src);
XME_DLL_API void XmeDeviceStatus_destruct(XmeDeviceStatus* thisPtr);
XME_DLL_API void XmeDeviceStatus_copy(XmeDeviceStatus* copy, XmeDeviceStatus const* src);
XME_DLL_API void XmeDeviceStatus_swap(XmeDeviceStatus* a, XmeDeviceStatus* b);
XME_DLL_API int XmeDeviceStatus_empty(XmeDeviceStatus const* thisPtr);
XME_DLL_API int XmeDeviceStatus_compare(XmeDeviceStatus const* a, XmeDeviceStatus const* b);
XME_DLL_API int XmeDeviceStatus_equivalenceComparison(XmeDeviceStatus const* a, XmeDeviceStatus const* b);

#ifdef __cplusplus
} // extern "C"
#endif

/*!	\addtogroup enums
	@{
*/

// note for developers: do not change values in this enum as its numerical values are recorded in files
/*! \brief Describes general state of a (expected) device
*/
enum XmeDeviceValidity
{
	// XDV_Placement = 0,		//!< Device is in the wrong place (this value is not used at the moment)
	XDV_Ok = 1,					//!< Device is ok and ready to use
	XDV_Unused = 2,				//!< Device is connected but data from it will not be used
	XDV_Duplicate = 3,			//!< The device has the same segment ID as another connected device, which is not allowed
	XDV_Empty = 4,				//!< The device has not yet been detected
	XDV_Rejected = 5,			//!< The device's connection has been rejected
	XDV_Unknown = 6,			//!< The device is in an unknown state
	XDV_RejectedDuplicate = 7,	//!< The device has a duplicate ID as another detected device, but its connection is rejected so this should not cause problems
	XDV_DuplicateId = 8,		//!< The device has the same (partial) device ID as another connected device, which is not allowed for bodypacks
};
//! @}
typedef enum XmeDeviceValidity XmeDeviceValidity;

/*! \brief Contains status information of a (connected) device
*/
struct XmeDeviceStatus
{
	XsDeviceId m_deviceId;			//!< The ID of the device
	int m_segmentId;				//!< The segment ID that the device is associated with
	XmeDeviceValidity m_validity;	//!< The validity of the device with respect to the desired configuration
	XsSimpleVersion m_firmwareVersion;	//!< The Firmware version of the device
	int m_batteryLevel;				//!< The battery level of the device in the range 1-100, -1 means that the device has no battery, 0 means that the battery level is currently unknown
	int m_secondaryBatteryLevel;	//!< The secondary battery level of the device in the range 1-100, -1 means that the device has no battery, 0 means that the battery level is currently unknown
	int m_radioQuality;				//!< Estimation of the radio signal quality, valid in the range 0..100, 100 being the best. A value of -1 indicates that no information is available.
#ifdef __cplusplus
	//! \brief Default constructor
	XmeDeviceStatus() : m_deviceId(0), m_segmentId(0), m_validity(XDV_Empty), m_firmwareVersion(0, 0, 0), m_batteryLevel(0), m_secondaryBatteryLevel(0), m_radioQuality(0)
	{}

	/*! \brief Initializing constructor
		\param devId \copydoc m_deviceId
		\param firmwareVersion \copydoc m_firmwareVersion
		\param segId \copydoc m_segmentId
		\param validity \copydoc m_validity
		\param battery \copydoc m_batteryLevel
		\param radioQuality \copydoc m_radioQuality
	*/
	XmeDeviceStatus(XsDeviceId const& devId, int segId, XmeDeviceValidity validity, XsSimpleVersion const& firmwareVersion, int battery, int radioQuality)
		: m_deviceId(devId)
		, m_segmentId(segId)
		, m_validity(validity)
		, m_firmwareVersion(firmwareVersion)
		, m_batteryLevel(battery)
		, m_secondaryBatteryLevel(0)
		, m_radioQuality(radioQuality)
	{
	}

	/*! \brief Initializing constructor
	\param devId \copydoc m_deviceId
	\param firmwareVersion \copydoc m_firmwareVersion
	\param segId \copydoc m_segmentId
	\param validity \copydoc m_validity
	\param battery \copydoc m_batteryLevel
	\param secondaryBattery \copydoc m_secondaryBatteryLevel
	\param radioQuality \copydoc m_radioQuality
*/
	XmeDeviceStatus(XsDeviceId const& devId, int segId, XmeDeviceValidity validity, XsSimpleVersion const& firmwareVersion, int battery, int secondaryBattery, int radioQuality)
		: m_deviceId(devId)
		, m_segmentId(segId)
		, m_validity(validity)
		, m_firmwareVersion(firmwareVersion)
		, m_batteryLevel(battery)
		, m_secondaryBatteryLevel(secondaryBattery)
		, m_radioQuality(radioQuality)
	{
	}

	//! \brief Copy constructor, copies contents from \a other
	XmeDeviceStatus(const XmeDeviceStatus& other)
		: m_deviceId(other.m_deviceId)
		, m_segmentId(other.m_segmentId)
		, m_validity(other.m_validity)
		, m_firmwareVersion(other.m_firmwareVersion)
		, m_batteryLevel(other.m_batteryLevel)
		, m_secondaryBatteryLevel(other.m_secondaryBatteryLevel)
		, m_radioQuality(other.m_radioQuality)
	{
	}

	//! \brief Destructor
	~XmeDeviceStatus()
	{
	}

	//! \brief Assignment operator, copies contents from \a other
	XmeDeviceStatus& operator = (const XmeDeviceStatus& other)
	{
		if (this == &other)
			return *this;
		m_deviceId = other.m_deviceId;
		m_segmentId = other.m_segmentId;
		m_validity = other.m_validity;
		m_firmwareVersion = other.m_firmwareVersion;
		m_batteryLevel = other.m_batteryLevel;
		m_secondaryBatteryLevel = other.m_secondaryBatteryLevel;
		m_radioQuality = other.m_radioQuality;
		return *this;
	}

	//! \copydoc XmeDeviceStatus_compare
	inline bool operator == (const XmeDeviceStatus& other) const
	{
		return !XmeDeviceStatus_compare(this, &other);
	}

	//! \copydoc XmeDeviceStatus_equivalenceComparison
	inline bool isEquivalent(const XmeDeviceStatus& other) const
	{
		return !XmeDeviceStatus_equivalenceComparison(this, &other);
	}

	//! \brief Return true when the object is empty
	inline bool empty() const
	{
		// master may be 0
		return !m_deviceId.isValid() || m_validity == XDV_Empty;
	}

	//! \brief Swap the contents of the object with \a other
	inline void swap(XmeDeviceStatus& other) noexcept
	{
		XmeDeviceStatus_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeDeviceStatus& first, XmeDeviceStatus& second) noexcept
	{
		first.swap(second);
	}

#endif
};

#endif
