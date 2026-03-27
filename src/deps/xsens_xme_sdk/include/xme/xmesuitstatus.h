#ifndef XMESUITSTATUS_H
#define XMESUITSTATUS_H

#include "xmedllapi.h"
#include <xstypes/xsstring.h>
#include "xmedevicestatus.h"
#include "xmedevicestatusarray.h"
#include <xstypes/xsintarray.h>
#include <xstypes/xsdeviceid.h>

/*!	\addtogroup enums
	@{
*/
/*! \brief Describes the global status of a suit
*/
enum XmeHardwareStatus
{
	XHS_HardwareOk = 0,		//!< The suit is configured correctly
	XHS_Error,				//!< An error occurred during initialization
	XHS_NothingFound,		//!< No master device was found
	XHS_MissingMaster_deprecated,		//!< One master device is missing in a multi-master system, which are no longer supported
	XHS_MissingSensor,		//!< At least one usable Motion Tracker is missing, note that this could be because there are duplicates!
	XHS_SensorsSwapped_deprecated,		//!< Two Motion Trackers are swapped on the body. This can no longer be detected
	XHS_ObrMode,			//!< The system is configured for On Body Recording mode, preventing it from being initialized
	XHS_Invalid,			//!< Some invalid hardware was detected or the status is uninitialized
};
//! @}
typedef enum XmeHardwareStatus XmeHardwareStatus;

#ifdef __cplusplus
extern "C" {
#endif

struct XmeSuitStatus;
typedef struct XmeSuitStatus XmeSuitStatus;

XME_DLL_API void XmeSuitStatus_construct(XmeSuitStatus* thisPtr);
XME_DLL_API void XmeSuitStatus_destruct(XmeSuitStatus* thisPtr);
XME_DLL_API void XmeSuitStatus_copy(XmeSuitStatus* copy, XmeSuitStatus const* src);
XME_DLL_API void XmeSuitStatus_swap(XmeSuitStatus* a, XmeSuitStatus* b);
XME_DLL_API int XmeSuitStatus_empty(XmeSuitStatus const* thisPtr);
XME_DLL_API const char* XmeHardwareStatus_toString(XmeHardwareStatus hardwareStatus);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Contains status information about a suit
*/
struct XmeSuitStatus
{
#ifdef __cplusplus
	//! \brief Default constructor
	XmeSuitStatus()
		: m_hardwareStatus(XHS_Invalid)
		, m_wirelessChannel(-1)
	{
	}

	/*! \brief Initializing constructor
		\param hardwareStatus			\copydoc m_hardwareStatus
		\param hardwareStatusText		\copydoc m_hardwareStatusText
		\param masterDeviceId			The device ID of the master device
		\param masterBatteryLevel		The battery level of the master device
		\param wirelessChannel			The wireless channel configured for the first master (or 0 if no wireless channnel)
		\param lastProblematicDeviceId	The device ID of the last Tracker that caused a hardware error
		\param sensors					\copydoc m_sensors
		\param missingSensors			\copydoc m_missingSensors
		\param masterConnectionStrength	The wireless connection strength of the first master
	*/
	XmeSuitStatus(XmeHardwareStatus hardwareStatus,
		const XsString& hardwareStatusText,
		const XsDeviceId& masterDeviceId,
		int masterBatteryLevel,
		int wirelessChannel,
		const XsDeviceId& lastProblematicDeviceId,
		const XmeDeviceStatusArray& sensors,
		const XsIntArray& missingSensors,
		int masterConnectionStrength)
		: m_hardwareStatus(hardwareStatus)
		, m_hardwareStatusText(hardwareStatusText)
		, m_sensors(sensors)
		, m_missingSensors(missingSensors)
		, m_wirelessChannel(wirelessChannel)
		, m_lastProblematicDeviceId(lastProblematicDeviceId)
	{
		if (masterDeviceId.isValid())
			m_masterDevice = XmeDeviceStatus(masterDeviceId, 0, XDV_Ok, XsSimpleVersion(), masterBatteryLevel, masterConnectionStrength);
		else
			m_masterDevice = XmeDeviceStatus();
	}

	/*! \brief Copy constructor
		\param other The object to copy from
	*/
	XmeSuitStatus(const XmeSuitStatus& other)
		: m_hardwareStatus(other.m_hardwareStatus)
		, m_hardwareStatusText(other.m_hardwareStatusText)
		, m_masterDevice(other.m_masterDevice)
		, m_sensors(other.m_sensors)
		, m_missingSensors(other.m_missingSensors)
		, m_wirelessChannel(other.m_wirelessChannel)
		, m_lastProblematicDeviceId(other.m_lastProblematicDeviceId)
	{
	}

	//! \brief Destructor
	~XmeSuitStatus()
	{}

	/*! \brief Assignment operator
		\param other The object to copy from
		\return A reference to this
	*/
	XmeSuitStatus& operator = (const XmeSuitStatus& other)
	{
		XmeSuitStatus_copy(this, &other);
		return *this;
	}

	/*! \brief full comparison operator
		\param other The object to compare with
		\return true if all fields are identical
		\sa isEquivalent
	*/
	bool operator == (const XmeSuitStatus& other) const
	{
		return	m_hardwareStatus == other.m_hardwareStatus &&
			m_hardwareStatusText == other.m_hardwareStatusText &&
			m_masterDevice == other.m_masterDevice &&
			m_sensors == other.m_sensors &&
			m_missingSensors == other.m_missingSensors &&
			m_wirelessChannel == other.m_wirelessChannel &&
			m_lastProblematicDeviceId == other.m_lastProblematicDeviceId;
	}

	/*! \brief Equivalence comparison
		\param other The object to compare with
		\return true if all fields are equivalent. This means battery levels and radio strengths are not compared
	*/
	bool isEquivalent(const XmeSuitStatus& other) const
	{
		return	m_hardwareStatus == other.m_hardwareStatus &&
			m_hardwareStatusText == other.m_hardwareStatusText &&
			m_masterDevice.isEquivalent(other.m_masterDevice) &&
			m_sensors.isEquivalent(other.m_sensors) &&
			m_missingSensors == other.m_missingSensors &&
			m_wirelessChannel == other.m_wirelessChannel &&
			m_lastProblematicDeviceId == other.m_lastProblematicDeviceId;
	}

	/*! \brief Returns true when the status is considered to be empty
		\return true when the status is considered to be empty
	*/
	inline bool empty() const
	{
		return m_hardwareStatus == XHS_Invalid;
	}

	/*! \brief Clear the contents of the object
	*/
	inline void clear()
	{
		*this = XmeSuitStatus();
	}

	/*! \brief Swap the contents with \a other
		\param other The object to swap contents with
	*/
	inline void swap(XmeSuitStatus& other) noexcept
	{
		XmeSuitStatus_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeSuitStatus& first, XmeSuitStatus& second) noexcept
	{
		first.swap(second);
	}

	//private:	pod structure, all direct access allowed
#endif

	XmeHardwareStatus m_hardwareStatus;		//!< The global status of the hardware, whether it connected properly
	XsString m_hardwareStatusText;			//!< A textual description of m_hardwareStatus
	XmeDeviceStatus m_masterDevice;			//!< The status of the master device
	XmeDeviceStatusArray m_sensors;			//!< A list of the detected Motion Trackers.
	XsIntArray m_missingSensors;			//!< The list of segment IDs for which the Tracker is missing
	int m_wirelessChannel;					//!< The wireless channel configured for the master device. When set to -1, no wireless channel can be configured.
	XsDeviceId m_lastProblematicDeviceId;	//!< The device ID of the Motion Tracker that failed during the last scan. This is not 100% reliable, but can be used to detect faulty wiring in some cases.
};

#endif
