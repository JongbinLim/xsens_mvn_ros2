#ifndef XMEOBRRECORDINGSTATE_H
#define XMEOBRRECORDINGSTATE_H

#include "xmerecordingstate.h"
#include <xstypes/xstimestamp.h>
#include <xstypes/xsdeviceid.h>

/*! \brief The state of remote OBR recording for a single system */
struct XmeObrRecordingState
{
	XsDeviceId m_deviceId;			//!< The unique device ID of the system
	XmeRecordingState m_recState;	//!< The recording state of the system
	XsTimeStamp m_remainingTime;	//!< The time remaining for on body recordings
	int m_batteryLevel;				//!< Battery level in percentage
	int m_radioQuality;				//!< An estimate of the radio communication quality
	XsString m_systemName;			//!< The user-customizable name of the system
	int m_clockSyncAccuracy;		//!< The accuracy of the Bodypack's clock synchronization in milliseconds. If this value is negative, the clock is not synchronized

	/*! \brief Default constructor */
	XmeObrRecordingState()
		: m_deviceId(0)
		, m_recState(XRS_NotRecording)
		, m_remainingTime(0)
		, m_batteryLevel(0)
		, m_radioQuality(0)
		, m_systemName(XsString())
		, m_clockSyncAccuracy(-1)
	{}

	/*! \brief Device constructor, initializes deviceId field to \a deviceId */
	XmeObrRecordingState(XsDeviceId const& deviceId)
		: m_deviceId(deviceId)
		, m_recState(XRS_NotRecording)
		, m_remainingTime(0)
		, m_batteryLevel(0)
		, m_radioQuality(0)
		, m_systemName(XsString())
		, m_clockSyncAccuracy(-1)
	{}
};

#endif
