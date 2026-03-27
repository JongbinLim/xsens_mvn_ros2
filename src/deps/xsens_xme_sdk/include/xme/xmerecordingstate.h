#ifndef XMERECORDINGSTATE_H
#define XMERECORDINGSTATE_H

/*!	\addtogroup enums
	@{
*/
/*! \brief Defines the recording state */
enum XmeRecordingState
{
	XRS_NotRecording,		//!< The system is not currently recording
	XRS_WaitingForStart,	//!< A recording is scheduled, but the system is waiting for it to start. This can be due to an external trigger or a delayed start
	XRS_Recording,			//!< The system is recording
	XRS_Flushing,			//!< The system is flushing retransmitted data after stopping recording
};
//! @}
typedef enum XmeRecordingState XmeRecordingState;

#endif
