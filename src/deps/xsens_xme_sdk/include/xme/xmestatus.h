#ifndef XMESTATUS_H
#define XMESTATUS_H

#include "xmesuitstatus.h"
#include "xmerecordingstate.h"

#ifdef __cplusplus
extern "C" {
#endif

struct XmeStatus;
typedef struct XmeStatus XmeStatus;

XME_DLL_API void XmeStatus_construct(XmeStatus* thisPtr);
XME_DLL_API void XmeStatus_destruct(XmeStatus* thisPtr);
XME_DLL_API void XmeStatus_copy(XmeStatus* copy, XmeStatus const* src);
XME_DLL_API void XmeStatus_swap(XmeStatus* a, XmeStatus* b);
XME_DLL_API int XmeStatus_empty(XmeStatus const* thisPtr);

#ifdef __cplusplus
} // extern "C"
#endif
/*!	\addtogroup enums
	@{
*/
/*! \brief Status flags, used as bits in a bitfield to describe the status of the system
*/
enum XmeStatusFlags
{
	XSF_Connected = 0x01,		//!< A suit is connected and properly initialized
	//XSF_Recording = 0x02,
	XSF_Calibrating = 0x04,		//!< A calibration procedure is in progress
	XSF_Scanning = 0x08,		//!< The XmeControl is scanning for a suit
	XSF_OutOfRange = 0x10,		//!< Suit is out of range, no data will be received while this flag is set
	XSF_FileOpen = 0x20,		//!< A file is open
	XSF_FileChanged = 0x40,		//!< The open file has unsaved changes
	XSF_Processing = 0x80,		//!< A file reprocessing operation is in progress
	XSF_ConfigMode = 0x800,			//!< The connected system is in Config mode and will not generate pose data
	XSF_MasterGnssData = 0x1000,	//!< The connected system is configured to record GNSS data on the master device
	XSF_ObrMode = 0x2000,			//!< The connected system is (being) configured to be in On Body Recording mode
};
//! @}
typedef enum XmeStatusFlags XmeStatusFlags;

/*! \brief Contains the full observable status of an XmeControl
*/
struct XmeStatus
{
#ifdef __cplusplus
	//! \brief Default constructor
	XmeStatus()
		: m_statusFlags(0)
		, m_frameCount(0)
		, m_framesProcessed(0)
		, m_recordingState(XRS_NotRecording)
	{}

	/*! \brief Initializing constructor
		\param flags \copydoc m_statusFlags
		\param fCount \copydoc m_frameCount
		\param fProcessed \copydoc m_framesProcessed
		\param recState \copydoc m_recordingState
		\param suit \copydoc m_suitStatus
	*/
	XmeStatus(int flags, int fCount, int fProcessed, XmeRecordingState recState, const XmeSuitStatus& suit)
		: m_statusFlags(flags)
		, m_frameCount(fCount)
		, m_framesProcessed(fProcessed)
		, m_recordingState(recState)
		, m_suitStatus(suit)
	{
	}

	/*! \brief Copy constructor, copies the contents from \a other */
	XmeStatus(const XmeStatus& other)
		: m_statusFlags(other.m_statusFlags)
		, m_frameCount(other.m_frameCount)
		, m_framesProcessed(other.m_framesProcessed)
		, m_recordingState(other.m_recordingState)
		, m_suitStatus(other.m_suitStatus)
	{
	}

	//! \brief Destructor
	~XmeStatus()
	{
	}

	/*! \brief Assignment operator, copies contents from \a other */
	XmeStatus& operator = (const XmeStatus& other)
	{
		XmeStatus_copy(this, &other);
		return *this;
	}

	/*! \brief Equivalence comparison, compares only important parts
		\details This comparison function doesn't compare battery level and wireless signal strength.
	*/
	bool isStatusEquivalent(const XmeStatus& other) const
	{
		return	m_statusFlags == other.m_statusFlags &&
			m_recordingState == other.m_recordingState &&
			m_suitStatus.isEquivalent(other.m_suitStatus);
	}

	/*! \brief Comparison operator, returns true when all fields are identical
		\sa isStatusEquivalent
	*/
	bool operator == (const XmeStatus& other) const
	{
		return	m_statusFlags == other.m_statusFlags &&
			m_recordingState == other.m_recordingState &&
			m_suitStatus == other.m_suitStatus;
	}

	//! \brief Return true when the object is empty
	inline bool empty() const
	{
		return !m_statusFlags;
	}

	//! \brief Clears the object
	inline void clear()
	{
		*this = XmeStatus();
	}

	//! \brief Swap the contents of the object with \a other
	inline void swap(XmeStatus& other) noexcept
	{
		XmeStatus_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeStatus& first, XmeStatus& second) noexcept
	{
		first.swap(second);
	}

	//! \brief Return all status flags \details It is recommended to use the is... functions instead to check status. \sa XmeStatusFlags
	inline int statusFlags() const
	{
		return m_statusFlags;
	}
	//! \brief Set the status flags
	inline void setStatusFlags(int flags)
	{
		m_statusFlags = flags;
	}

	//! \copydoc m_frameCount
	inline int frameCount() const
	{
		return m_frameCount;
	}
	//! \brief Set the frame count
	inline void setFrameCount(int count)
	{
		m_frameCount = count;
	}

	//! \copydoc m_framesProcessed
	inline int framesProcessed() const
	{
		return m_framesProcessed;
	}
	//! \brief Set the number of frames that have been processed so far by a reprocessing operation
	inline void setFramesProcessed(int frames)
	{
		m_framesProcessed = frames;
	}

	//! \copydoc m_suitStatus
	inline const XmeSuitStatus& suitStatus() const
	{
		return m_suitStatus;
	}
	//! \brief Set the status of the connected or recorded suit
	inline void setSuitStatus(const XmeSuitStatus& suit)
	{
		m_suitStatus = suit;
	}

	/*! \brief Return true when a suit is connected and properly initialized */
	inline bool isConnected() const
	{
		return !!(m_statusFlags & XSF_Connected);
	}
	/*! \brief Return true when the system is recording */
	inline bool isRecording() const
	{
		return m_recordingState == XRS_Recording;
	}
	/*! \brief Return true when the system is flushing */
	inline bool isFlushing() const
	{
		return m_recordingState == XRS_Flushing;
	}
	/*! \brief Return true when the system is flushing or recording*/
	inline bool isRecordingOrFlushing() const
	{
		return m_recordingState == XRS_Recording || m_recordingState == XRS_Flushing;
	}
	/*! \brief Return true when a calibration is in progress */
	inline bool isCalibrating() const
	{
		return !!(m_statusFlags & XSF_Calibrating);
	}
	/*! \brief Return true when the system is scanning for a suit */
	inline bool isScanning() const
	{
		return !!(m_statusFlags & XSF_Scanning);
	}
	/*! \brief Return true when a suit is connected but currently out of wireless range */
	inline bool isOutOfRange() const
	{
		return !!(m_statusFlags & XSF_OutOfRange);
	}
	/*! \brief Return true when a file is open, either for recording or reading / reprocessing */
	inline bool isFileOpen() const
	{
		return !!(m_statusFlags & XSF_FileOpen);
	}
	/*! \brief Return true when the open file has unsaved changes */
	inline bool isFileChanged() const
	{
		return !!(m_statusFlags & XSF_FileChanged);
	}
	/*! \brief Return true when the system is processing motion capture data */
	inline bool isProcessing() const
	{
		return !!(m_statusFlags & XSF_Processing);
	}
	/*! \brief Return true when the hardware is in Config mode */
	inline bool isConfigMode() const
	{
		return !!(m_statusFlags & XSF_ConfigMode);
	}
	/*! \brief Return true when the hardware is configured to use master data */
	inline bool isMasterGnssDataEnabled() const
	{
		return !!(m_statusFlags & XSF_MasterGnssData);
	}
	/*! \brief Return true when the hardware is configured to do On Body Recording */
	inline bool isInObrMode() const
	{
		return m_suitStatus.m_hardwareStatus == XHS_ObrMode || !!(m_statusFlags & XSF_ObrMode);
	}
	/*! \brief Return the current recording state */
	XmeRecordingState recordingState() const
	{
		return m_recordingState;
	}

private:
	//! \protectedsection
	friend struct XmeControl;
#endif
	int m_statusFlags;		//!< A bitfield with status flags. \sa XmeStatusFlags
	int m_frameCount;		//!< The frame count of the currently open file
	int m_framesProcessed;	//!< The number of frames that have been processed so far by a reprocessing operation
	XmeRecordingState m_recordingState;	//!< The state of the recording
	XmeSuitStatus m_suitStatus;			//!< The status of the connected or recorded suit
};

#endif
