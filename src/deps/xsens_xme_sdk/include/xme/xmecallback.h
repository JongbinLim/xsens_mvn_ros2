#ifndef XMECALLBACK_H
#define XMECALLBACK_H

#include "xmecallbackplainc.h"

#ifdef __cplusplus

/*! \brief Structure that contains callback functions for the Xsens Mocap Engine
	\details When programming in C++, simply overload the callback that you want to use and supply
	your XmeCallback-inheriting class to one of the setCallback functions

	When programming in C, create an XmeCallbackPlainC structure and initialize each function pointer to
	the right function to call. The supplied first parameter is the address of the XmeCallbackPlainC
	object that you supplied to the addCallback function. If you do not wish to receive a specific
	callback in C, set the function pointer to 0.

	In both cases, the calling application remains in control of the XmeCallbackPlainC object and thus
	remains responsible for cleaning it up when the it is no longer necessary.

	\note Any callback function in %XmeCallback that is not overloaded will only be called once to
	minimize callback overhead.
*/
class XmeCallback : public XmeCallbackPlainC
{
public:
	/*! \brief Constructor */
	XmeCallback()
	{
		m_onAboutToBeDestroyed = sonAboutToBeDestroyed;
		m_onBufferOverflow = sonBufferOverflow;
		m_onCalibrationAborted = sonCalibrationAborted;
		m_onCalibrationComplete = sonCalibrationComplete;
		m_onCalibrationProcessed = sonCalibrationProcessed;
		m_onConfigurationChangeComplete = sonConfigurationChangeComplete;
		m_onConfigurationChangeFailed = sonConfigurationChangeFailed;
		m_onContactsUpdated = sonContactsUpdated;
		m_onProgressUpdate = sonProgressUpdate;
		m_onCopyFileError = sonCopyFileError;
		m_onCopyFileReady = sonCopyFileReady;
		m_onHardwareDisconnected = sonHardwareDisconnected;
		m_onHardwareError = sonHardwareError;
		m_onHardwareReady = sonHardwareReady;
		m_onLowBatteryLevel = sonLowBatteryLevel;
		m_onPoseReady = sonPoseReady;
		m_onProcessingAborted = sonProcessingAborted;
		m_onProcessingComplete = sonProcessingComplete;
		m_onProcessingProgress = sonProcessingProgress;
		m_onRecordingStateChanged = sonRecordingStateChanged;
		m_onHardwareWarning = sonHardwareWarning;
		m_onStatusUpdated = sonStatusUpdated;
		m_onProcessingWarning = sonProcessingWarning;
		m_onCalibrationRecordingSectionChanged = sonCalibrationRecordingSectionChanged;
	}

	/*! \brief Destructor
		\note Make sure that the object is removed from callback generating objects before destroying it!
	*/
	virtual ~XmeCallback() {}

protected:
	/*! \addtogroup Callbacks
		@{
	*/

	//! \copybrief m_onAboutToBeDestroyed
	virtual void onAboutToBeDestroyed(XmeControl* dev)
	{
		(void) dev;
		m_onAboutToBeDestroyed = 0;
	}
	//! \copybrief m_onBufferOverflow
	virtual void onBufferOverflow(XmeControl* dev)
	{
		(void) dev;
		m_onBufferOverflow = 0;
	}
	//! \copybrief m_onCalibrationAborted
	virtual void onCalibrationAborted(XmeControl* dev)
	{
		(void) dev;
		m_onCalibrationAborted = 0;
	}
	//! \copybrief m_onCalibrationComplete
	virtual void onCalibrationComplete(XmeControl* dev)
	{
		(void) dev;
		m_onCalibrationComplete = 0;
	}
	//! \copybrief m_onCalibrationProcessed
	virtual void onCalibrationProcessed(XmeControl* dev)
	{
		(void) dev;
		m_onCalibrationProcessed = 0;
	}
	//! \copybrief m_onConfigurationChangeComplete
	virtual void onConfigurationChangeComplete(XmeControl* dev)
	{
		(void) dev;
		m_onConfigurationChangeComplete = 0;
	}
	//! \copybrief m_onConfigurationChangeFailed
	virtual void onConfigurationChangeFailed(XmeControl* dev)
	{
		(void) dev;
		m_onConfigurationChangeFailed = 0;
	}
	//! \copybrief m_onContactsUpdated
	virtual void onContactsUpdated(XmeControl* dev)
	{
		(void) dev;
		m_onContactsUpdated = 0;
	}
	//! \copybrief m_onProgressUpdate
	virtual void onProgressUpdate(XmeControl* dev, int percentage, XsString const* category)
	{
		(void) dev;
		(void) percentage;
		(void) category;
		m_onProgressUpdate = 0;
	}
	//! \copybrief m_onCopyFileError
	virtual void onCopyFileError(XmeControl* dev)
	{
		(void) dev;
		m_onCopyFileError = 0;
	}
	//! \copybrief m_onCopyFileReady
	virtual void onCopyFileReady(XmeControl* dev)
	{
		(void) dev;
		m_onCopyFileReady = 0;
	}
	//! \copybrief m_onHardwareDisconnected
	virtual void onHardwareDisconnected(XmeControl* dev)
	{
		(void) dev;
		m_onHardwareDisconnected = 0;
	}
	//! \copybrief m_onHardwareError
	virtual void onHardwareError(XmeControl* dev)
	{
		(void) dev;
		m_onHardwareError = 0;
	}
	//! \copybrief m_onHardwareReady
	virtual void onHardwareReady(XmeControl* dev)
	{
		(void) dev;
		m_onHardwareReady = 0;
	}
	//! \copybrief m_onLowBatteryLevel
	virtual void onLowBatteryLevel(XmeControl* dev)
	{
		(void) dev;
		m_onLowBatteryLevel = 0;
	}
	//! \copybrief m_onPoseReady
	virtual void onPoseReady(XmeControl* dev)
	{
		(void) dev;
		m_onPoseReady = 0;
	}
	//! \copybrief m_onProcessingAborted
	virtual void onProcessingAborted(XmeControl* dev)
	{
		(void) dev;
		m_onProcessingAborted = 0;
	}
	//! \copybrief m_onProcessingComplete
	virtual void onProcessingComplete(XmeControl* dev)
	{
		(void) dev;
		m_onProcessingComplete = 0;
	}
	//! \copybrief m_onProcessingProgress
	virtual void onProcessingProgress(XmeControl* dev, XmeProcessingStage stage, int firstFrame, int lastFrame)
	{
		(void) dev;
		(void) stage;
		(void) firstFrame;
		(void) lastFrame;
		m_onProcessingProgress = 0;
	}
	//! \copybrief m_onRecordingStateChanged
	virtual void onRecordingStateChanged(XmeControl* dev, XmeRecordingState newState)
	{
		(void) dev;
		(void) newState;
		m_onRecordingStateChanged = 0;
	}
	//! \copybrief m_onHardwareWarning
	virtual void onHardwareWarning(XmeControl* dev, XsResultValue resultValue, XsString const* additionalMessage)
	{
		(void) dev;
		(void) resultValue;
		(void)additionalMessage;
		m_onHardwareWarning = 0;
	}
	//! \copybrief m_onStatusUpdated
	virtual void onStatusUpdated(XmeControl* dev)
	{
		(void) dev;
		m_onStatusUpdated = 0;
	}
	//! \copybrief m_onProcessingWarning
	virtual void onProcessingWarning(XmeControl* dev, int frameNr, int segmentIndex, XsString const* warning)
	{
		(void) dev;
		(void) frameNr;
		(void) segmentIndex;
		(void) warning;
		m_onProcessingWarning = 0;
	}
	//! \copybrief m_onCalibrationRecordingSectionChanged
	virtual void onCalibrationRecordingSectionChanged(XmeControl* dev, XmeCalibrationRecordingSection calibSection)
	{
		(void)dev;
		(void)calibSection;
		m_onCalibrationRecordingSectionChanged = 0;
	}
	//! @}

private:
	static void sonAboutToBeDestroyed(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onAboutToBeDestroyed(dev);
	}
	static void sonBufferOverflow(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onBufferOverflow(dev);
	}
	static void sonCalibrationAborted(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onCalibrationAborted(dev);
	}
	static void sonCalibrationComplete(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onCalibrationComplete(dev);
	}
	static void sonCalibrationProcessed(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onCalibrationProcessed(dev);
	}
	static void sonConfigurationChangeComplete(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onConfigurationChangeComplete(dev);
	}
	static void sonConfigurationChangeFailed(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onConfigurationChangeFailed(dev);
	}
	static void sonContactsUpdated(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onContactsUpdated(dev);
	}
	static void sonProgressUpdate(XmeCallbackPlainC* cb, XmeControl* dev, int percentage, XsString const* category)
	{
		((XmeCallback*)cb)->onProgressUpdate(dev, percentage, category);
	}
	static void sonCopyFileError(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onCopyFileError(dev);
	}
	static void sonCopyFileReady(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onCopyFileReady(dev);
	}
	static void sonHardwareDisconnected(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onHardwareDisconnected(dev);
	}
	static void sonHardwareError(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onHardwareError(dev);
	}
	static void sonHardwareReady(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onHardwareReady(dev);
	}
	static void sonLowBatteryLevel(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onLowBatteryLevel(dev);
	}
	static void sonPoseReady(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onPoseReady(dev);
	}
	static void sonProcessingComplete(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onProcessingComplete(dev);
	}
	static void sonProcessingAborted(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onProcessingAborted(dev);
	}
	static void sonProcessingProgress(XmeCallbackPlainC* cb, XmeControl* dev, XmeProcessingStage stage, int firstFrame, int lastFrame)
	{
		((XmeCallback*)cb)->onProcessingProgress(dev, stage, firstFrame, lastFrame);
	}
	static void sonRecordingStateChanged(XmeCallbackPlainC* cb, XmeControl* dev, XmeRecordingState newState)
	{
		((XmeCallback*)cb)->onRecordingStateChanged(dev, newState);
	}
	static void sonHardwareWarning(XmeCallbackPlainC* cb, XmeControl* dev, XsResultValue resultValue, XsString const* additionalMessage)
	{
		((XmeCallback*)cb)->onHardwareWarning(dev, resultValue, additionalMessage);
	}
	static void sonStatusUpdated(XmeCallbackPlainC* cb, XmeControl* dev)
	{
		((XmeCallback*)cb)->onStatusUpdated(dev);
	}
	static void sonProcessingWarning(XmeCallbackPlainC* cb, XmeControl* dev, int frameNr, int segmentIndex, XsString const* warning)
	{
		((XmeCallback*)cb)->onProcessingWarning(dev, frameNr, segmentIndex, warning);
	}
	static void sonCalibrationRecordingSectionChanged(XmeCallbackPlainC* cb, XmeControl* dev, XmeCalibrationRecordingSection calibSection)
	{
		((XmeCallback*)cb)->onCalibrationRecordingSectionChanged(dev, calibSection);
	}
};

#endif

#endif
