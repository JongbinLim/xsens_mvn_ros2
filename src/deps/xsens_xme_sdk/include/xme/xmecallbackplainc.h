#ifndef XMECALLBACKPLAINC_H
#define XMECALLBACKPLAINC_H

#include <xstypes/pstdint.h>
#include <xstypes/xsresultvalue.h>
#include "xmerecordingstate.h"
#include "xmeprocessingstage.h"
#include "xmecalibrationrecordingsection.h"

#ifndef __cplusplus
	#define XMECALLBACK_INITIALIZER		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
#endif

struct XmeControl;
struct XsString;

/*! \brief Structure that contains callback functions for the Xsens Mocap Engine
	\details When using C++, please use the overloaded class XmeCallback instead.

	This structure contains pointers to functions that will be called by XME when certain
	events occur. To use it in C, set any callback you do not wish to use to 0 and put a valid
	function pointer in the others. Then pass the object to an XmeControl object's
	addCallbackHandler function.

	\note XME does not copy the structure contents and does not take ownership of it. So make sure it
	is allocated on the heap or at least removed from wherever it was added by calling
	removeCallbackHandler before it is destroyed.
*/

typedef struct XmeCallbackPlainC
{
	/*! \defgroup Callbacks Callback functions.
		\addtogroup Callbacks
		@{
	*/
	void (*m_onAboutToBeDestroyed)(struct XmeCallbackPlainC* cb, XmeControl* dev);						//!< \brief Indicates that \a dev has just entered its destructor
	void (*m_onBufferOverflow)(struct XmeCallbackPlainC* cb, XmeControl* dev);					//!< \brief Indicates that the sample data buffer is completely full. This is worse than the XME_ET_LAG_WARNING event, since it is almost guaranteed that data has been lost or will be lost. The user should close some applications to reserve more CPU time for the XME process and redo the recording.
	void (*m_onCalibrationAborted)(struct XmeCallbackPlainC* cb, XmeControl* dev);				//!< \brief Indicates that the calibration has been aborted for some reason.
	void (*m_onCalibrationComplete)(struct XmeCallbackPlainC* cb, XmeControl* dev);				//!< \brief Indicates that calibration data has been processed, no more calibration processing will be done
	void (*m_onCalibrationProcessed)(struct XmeCallbackPlainC* cb, XmeControl* dev);			//!< \brief Indicates that calibration data has been processed, more processing may happen
	void (*m_onConfigurationChangeComplete)(struct XmeCallbackPlainC* cb, XmeControl* dev);		//!< \brief Indicates that some of the configuration successfully changed.
	void (*m_onConfigurationChangeFailed)(struct XmeCallbackPlainC* cb, XmeControl* dev);		//!< \brief Indicates that a request for changing the configuration was made, but it's failed.
	void (*m_onContactsUpdated)(struct XmeCallbackPlainC* cb, XmeControl* dev);						//!< \brief Indicates that the external contacts have been updated successfully.
	void (*m_onProgressUpdate)(struct XmeCallbackPlainC* cb, XmeControl* dev, int percentage, XsString const* category);	//!< \brief Indicates the progress in a long duration task
	void (*m_onCopyFileError)(struct XmeCallbackPlainC* cb, XmeControl* dev);						//!< \brief Indicates that an error occurred while trying to make a copy of the current MVN file.
	void (*m_onCopyFileReady)(struct XmeCallbackPlainC* cb, XmeControl* dev);						//!< \brief Indicates that the requested copy of the current MVN file has been successfully made.
	void (*m_onHardwareDisconnected)(struct XmeCallbackPlainC* cb, XmeControl* dev);		//!< \brief Indicates that the hardware has been successfully disconnected.
	void (*m_onHardwareError)(struct XmeCallbackPlainC* cb, XmeControl* dev);				//!< \brief Indicates that there is something wrong with the hardware or that the hardware could not be located or initialized during a hardware scan.
	void (*m_onHardwareReady)(struct XmeCallbackPlainC* cb, XmeControl* dev);				//!< \brief Indicates that the hardware is ready after a hardware scan.
	void (*m_onLowBatteryLevel)(struct XmeCallbackPlainC* cb, XmeControl* dev);				//!< \brief Indicates that the power level of at least one of the devices is dangerously low and that the batteries should be replaced or recharged. This event takes place at 10% power.
	void (*m_onPoseReady)(struct XmeCallbackPlainC* cb, XmeControl* dev);					//!< \brief Indicates that a new pose has been computed
	void (*m_onProcessingAborted)(struct XmeCallbackPlainC* cb, XmeControl* dev);								//!< \brief Indicates that processing of all the data in the current file has been aborted
	void (*m_onProcessingComplete)(struct XmeCallbackPlainC* cb, XmeControl* dev);							//!< \brief Indicates that processing of all the data in the current file has completed
	void (*m_onProcessingProgress)(struct XmeCallbackPlainC* cb, XmeControl* dev, XmeProcessingStage stage, int firstFrame, int lastFrame);		//!< \brief Indicates that a range of frames of data is now in a certain processing stage. \details The callback is triggered only during reprocessing of recorded data. The update applies to a range of frames. The \a stage describes what has been done with the supplied frame range and \a progress describes how far along the process of the current stage is. \param stage The stage of processing that the frames are in (see %XmeProcessingStage for details). \param firstFrame The first frame of the range that is being processed. \param lastFrame The last frame of the range that is being processed.
	void (*m_onRecordingStateChanged)(struct XmeCallbackPlainC* cb, XmeControl* dev, XmeRecordingState newState);	//!< \brief Indicates that recording of data to the open MVN file has begun or ended.
	void (*m_onHardwareWarning)(struct XmeCallbackPlainC* cb, XmeControl* dev, XsResultValue resultValue, XsString const* additionalMessage);	//!< \brief Indicates that a hardware warning \a resultValue has occurred for device \a dev with additionalMessage \a additionalMessage
	void (*m_onStatusUpdated)(struct XmeCallbackPlainC* cb, XmeControl* dev);				//!< \brief Indicates that something in the system status has changed. This is only triggered when a status change is not covered by one of the other callbacks.
	void (*m_onProcessingWarning)(struct XmeCallbackPlainC* cb, XmeControl* dev, int frameNr, int segmentIndex, XsString const* warning);	//!< \brief Indicates that some inconsistency was detected during processing
	void (*m_onCalibrationRecordingSectionChanged)(struct XmeCallbackPlainC* cb, XmeControl* dev, XmeCalibrationRecordingSection calibSection); //!< \brief Indicates that the section during calibration recording changed
	//! @}

#ifdef __cplusplus
	// Make sure that this struct is not used in C++ (except as base class for XmeCallback)
	friend class XmeCallback;
protected:
	XmeCallbackPlainC()
		: m_onAboutToBeDestroyed(nullptr)
		, m_onBufferOverflow(nullptr)
		, m_onCalibrationAborted(nullptr)
		, m_onCalibrationComplete(nullptr)
		, m_onCalibrationProcessed(nullptr)
		, m_onConfigurationChangeComplete(nullptr)
		, m_onConfigurationChangeFailed(nullptr)
		, m_onContactsUpdated(nullptr)
		, m_onProgressUpdate(nullptr)
		, m_onCopyFileError(nullptr)
		, m_onCopyFileReady(nullptr)
		, m_onHardwareDisconnected(nullptr)
		, m_onHardwareError(nullptr)
		, m_onHardwareReady(nullptr)
		, m_onLowBatteryLevel(nullptr)
		, m_onPoseReady(nullptr)
		, m_onProcessingAborted(nullptr)
		, m_onProcessingComplete(nullptr)
		, m_onProcessingProgress(nullptr)
		, m_onRecordingStateChanged(nullptr)
		, m_onHardwareWarning(nullptr)
		, m_onStatusUpdated(nullptr)
		, m_onProcessingWarning(nullptr)
		, m_onCalibrationRecordingSectionChanged(nullptr)
	{
	}
	~XmeCallbackPlainC() throw() {}
private:
	XmeCallbackPlainC(XmeCallbackPlainC const&);
	XmeCallbackPlainC& operator = (XmeCallbackPlainC const&);

#endif
} XmeCallbackPlainC;

#endif
