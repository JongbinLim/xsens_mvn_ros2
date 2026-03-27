#ifndef XMEDEF_H
#define XMEDEF_H

#include "xmedllapi.h"

struct XsString;
#ifdef __cplusplus
	#include <xstypes/xsstring.h>
#endif

/*! \defgroup defines Constants, defines and macros
	\addtogroup defines
	@{
*/
#define XME_LAST_AVAILABLE_FRAME		-1	//!< \brief Indicates that the last available frame should be retrieved, instead of a specific frame number.
#define XME_SPECIAL_FRAME_TPOSE			-2	//!< \brief Indicates that a special tpose frame should be retrieved, instead of a specific frame number.
#define XME_SPECIAL_FRAME_IDENTITYPOSE	-3	//!< \brief Indicates that a special nullpose frame should be retrieved, instead of a specific frame number.

#define XME_CONTACT_NONE_ID				255		//!< \brief Special contact identifier for 'no contacts at all for this frame', use instead of segment ID \sa XME_CONTACT_NONE_IDX
#define XME_CONTACT_DELETE_ID			254		//!< \brief Special contact identifier for 'delete user defined contacts for this frame', use instead of segment ID \sa XME_CONTACT_DELETE_IDX

#define XME_CONTACT_NONE_IDX			(XME_CONTACT_NONE_ID-1)		//!< \brief Special contact identifier for 'no contacts at all for this frame', use instead of segment ID \sa XME_CONTACT_NONE_ID
#define XME_CONTACT_DELETE_IDX			(XME_CONTACT_DELETE_ID-1)	//!< \brief Special contact identifier for 'delete user defined contacts for this frame', use instead of segment ID \sa XME_CONTACT_DELETE_ID

#define XME_MAX_PROP_COUNT				4	//!< \brief The maximum number of props allowed per system

#define XME_AGE_MAX		32767				//!< \brief Maximum and 'invalid' age value for master data

/*!
	@}
*/

/*!	\addtogroup enums
	@{
*/
/*! \brief This is an enumerator that contains the indices of segments in the XmePose data structure.
	\details These values should be used when accessing data in an XmePose structure.
*/
enum XmeSegmentIndex
{
	XME_PELVIS_SEGMENT_INDEX				= 0,	//!< Segment index of the pelvis segment
	XME_L5_SEGMENT_INDEX					= 1,	//!< Segment index of the L5 segment
	XME_L3_SEGMENT_INDEX					= 2,	//!< Segment index of the L3 segment
	XME_T12_SEGMENT_INDEX					= 3,	//!< Segment index of the T12 segment
	XME_T8_SEGMENT_INDEX					= 4,	//!< Segment index of the T8 segment
	XME_NECK_SEGMENT_INDEX					= 5,	//!< Segment index of the neck segment
	XME_HEAD_SEGMENT_INDEX					= 6,	//!< Segment index of the head segment
	XME_RIGHT_SHOULDER_SEGMENT_INDEX		= 7,	//!< Segment index of the right shoulder segment
	XME_RIGHT_UPPER_ARM_SEGMENT_INDEX		= 8,	//!< Segment index of the right upper arm segment
	XME_RIGHT_FORE_ARM_SEGMENT_INDEX		= 9,	//!< Segment index of the right fore arm segment
	XME_RIGHT_HAND_SEGMENT_INDEX			= 10,	//!< Segment index of the right hand segment
	XME_LEFT_SHOULDER_SEGMENT_INDEX			= 11,	//!< Segment index of the left shoulder segment
	XME_LEFT_UPPER_ARM_SEGMENT_INDEX		= 12,	//!< Segment index of the left upper arm segment
	XME_LEFT_FORE_ARM_SEGMENT_INDEX			= 13,	//!< Segment index of the left fore arm segment
	XME_LEFT_HAND_SEGMENT_INDEX				= 14,	//!< Segment index of the left hand segment
	XME_RIGHT_UPPER_LEG_SEGMENT_INDEX		= 15,	//!< Segment index of the right upper leg segment
	XME_RIGHT_LOWER_LEG_SEGMENT_INDEX		= 16,	//!< Segment index of the right lower leg segment
	XME_RIGHT_FOOT_SEGMENT_INDEX			= 17,	//!< Segment index of the right foot segment
	XME_RIGHT_TOE_SEGMENT_INDEX				= 18,	//!< Segment index of the right toe segment
	XME_LEFT_UPPER_LEG_SEGMENT_INDEX		= 19,	//!< Segment index of the left upper leg segment
	XME_LEFT_LOWER_LEG_SEGMENT_INDEX		= 20,	//!< Segment index of the left lower leg segment
	XME_LEFT_FOOT_SEGMENT_INDEX				= 21,	//!< Segment index of the left foot segment
	XME_LEFT_TOE_SEGMENT_INDEX				= 22,	//!< Segment index of the left toe segment
	XME_PROP1_SEGMENT_INDEX					= 23,	//!< Segment index of the first prop segment
	XME_PROP2_SEGMENT_INDEX					= 24,	//!< Segment index of the second prop segment
	XME_PROP3_SEGMENT_INDEX					= 25,	//!< Segment index of the third prop segment
	XME_PROP4_SEGMENT_INDEX					= 26	//!< Segment index of the fourth prop segment
};

/*! \brief This is an enumerator that contains the IDs of segments.
	\details
	These values are to be used when addressing a specific segment.
	This is the segment id that needs to be supplied to xmeGetMeshScaleFactors for example.
*/
enum XmeSegmentId
{
	XME_PELVIS_SEGMENT_ID			= (XME_PELVIS_SEGMENT_INDEX + 1),			//!< Segment id of the pelvis segment
	XME_L5_SEGMENT_ID				= (XME_L5_SEGMENT_INDEX + 1),				//!< Segment id of the L5 segment
	XME_L3_SEGMENT_ID				= (XME_L3_SEGMENT_INDEX + 1),				//!< Segment id of the L3 segment
	XME_T12_SEGMENT_ID				= (XME_T12_SEGMENT_INDEX + 1),			//!< Segment id of the T12 segment
	XME_T8_SEGMENT_ID				= (XME_T8_SEGMENT_INDEX + 1),				//!< Segment id of the T8 segment
	XME_NECK_SEGMENT_ID				= (XME_NECK_SEGMENT_INDEX + 1),			//!< Segment id of the neck segment
	XME_HEAD_SEGMENT_ID				= (XME_HEAD_SEGMENT_INDEX + 1),			//!< Segment id of the head segment
	XME_RIGHT_SHOULDER_SEGMENT_ID	= (XME_RIGHT_SHOULDER_SEGMENT_INDEX + 1),	//!< Segment id of the right shoulder segment
	XME_RIGHT_UPPER_ARM_SEGMENT_ID	= (XME_RIGHT_UPPER_ARM_SEGMENT_INDEX + 1), //!< Segment id of the right upper arm segment
	XME_RIGHT_FORE_ARM_SEGMENT_ID	= (XME_RIGHT_FORE_ARM_SEGMENT_INDEX + 1),	//!< Segment id of the right fore arm segment
	XME_RIGHT_HAND_SEGMENT_ID		= (XME_RIGHT_HAND_SEGMENT_INDEX + 1),		//!< Segment id of the right hand segment
	XME_LEFT_SHOULDER_SEGMENT_ID	= (XME_LEFT_SHOULDER_SEGMENT_INDEX + 1),	//!< Segment id of the left shoulder segment
	XME_LEFT_UPPER_ARM_SEGMENT_ID	= (XME_LEFT_UPPER_ARM_SEGMENT_INDEX + 1),	//!< Segment id of the left upper arm segment
	XME_LEFT_FORE_ARM_SEGMENT_ID	= (XME_LEFT_FORE_ARM_SEGMENT_INDEX + 1),	//!< Segment id of the left fore arm segment
	XME_LEFT_HAND_SEGMENT_ID		= (XME_LEFT_HAND_SEGMENT_INDEX + 1),		//!< Segment id of the left hand segment
	XME_RIGHT_UPPER_LEG_SEGMENT_ID	= (XME_RIGHT_UPPER_LEG_SEGMENT_INDEX + 1), //!< Segment id of the right upper leg segment
	XME_RIGHT_LOWER_LEG_SEGMENT_ID	= (XME_RIGHT_LOWER_LEG_SEGMENT_INDEX + 1), //!< Segment id of the right lower leg segment
	XME_RIGHT_FOOT_SEGMENT_ID		= (XME_RIGHT_FOOT_SEGMENT_INDEX + 1),		//!< Segment id of the right foot segment
	XME_RIGHT_TOE_SEGMENT_ID		= (XME_RIGHT_TOE_SEGMENT_INDEX + 1),		//!< Segment id of the right toe segment
	XME_LEFT_UPPER_LEG_SEGMENT_ID	= (XME_LEFT_UPPER_LEG_SEGMENT_INDEX + 1),	//!< Segment id of the left upper leg segment
	XME_LEFT_LOWER_LEG_SEGMENT_ID	= (XME_LEFT_LOWER_LEG_SEGMENT_INDEX + 1),	//!< Segment id of the left lower leg segment
	XME_LEFT_FOOT_SEGMENT_ID		= (XME_LEFT_FOOT_SEGMENT_INDEX + 1),		//!< Segment id of the left foot segment
	XME_LEFT_TOE_SEGMENT_ID			= (XME_LEFT_TOE_SEGMENT_INDEX + 1),		//!< Segment id of the left toe segment
	XME_PROP1_SEGMENT_ID			= (XME_PROP1_SEGMENT_INDEX + 1),			//!< Segment id of the first prop segment
	XME_PROP2_SEGMENT_ID			= (XME_PROP2_SEGMENT_INDEX + 1),			//!< Segment id of the second prop segment
	XME_PROP3_SEGMENT_ID			= (XME_PROP3_SEGMENT_INDEX + 1),			//!< Segment id of the third prop segment
	XME_PROP4_SEGMENT_ID			= (XME_PROP4_SEGMENT_INDEX + 1)			//!< Segment id of the fourth prop segment
};

/*! \brief These enum values can be logically or-ed with a m_clipBits field for detailed analysis
*/
enum XmeClipBits
{
	XME_CLIP_ACC_X = 0x0001,		//!< Accelerometer X channel clipped
	XME_CLIP_ACC_Y = 0x0002,		//!< Accelerometer Y channel clipped
	XME_CLIP_ACC_Z = 0x0004,		//!< Accelerometer Z channel clipped

	XME_CLIP_GYR_X = 0x0008,		//!< Gyroscope X channel clipped
	XME_CLIP_GYR_Y = 0x0010,		//!< Gyroscope Y channel clipped
	XME_CLIP_GYR_Z = 0x0020,		//!< Gyroscope Z channel clipped

	XME_CLIP_MAG_X = 0x0040,		//!< Magnetometer X channel clipped
	XME_CLIP_MAG_Y = 0x0080,		//!< Magnetometer Y channel clipped
	XME_CLIP_MAG_Z = 0x0100,		//!< Magnetometer Z channel clipped

	XME_CLIP_ACC_ANY = (XME_CLIP_ACC_X | XME_CLIP_ACC_Y | XME_CLIP_ACC_Z),		//!< Any of the accelerometer channels clipped
	XME_CLIP_GYR_ANY = (XME_CLIP_GYR_X | XME_CLIP_GYR_Y | XME_CLIP_GYR_Z),		//!< Any of the gyroscope channels clipped
	XME_CLIP_MAG_ANY = (XME_CLIP_MAG_X | XME_CLIP_MAG_Y | XME_CLIP_MAG_Z),		//!< Any of the magnetometer channels clipped

	XME_CLIP_ANY = (XME_CLIP_ACC_ANY | XME_CLIP_GYR_ANY | XME_CLIP_MAG_ANY)		//!< Any of the channels clipped
};

/*! \brief These values define how XME should handle the USB hub functionality of discovered Awinda Stations */
enum XmeUsbHubMode
{
	XUHM_Default,				//!< Default operation, don't touch the USB Hub state at all
	XUHM_Enable,				//!< Enable the USB hub of any Awinda Station that is discovered
	XUHM_Disable,				//!< Disable the USB hub of any Awinda Station that is discovered
	XUHM_DisableWhileRunning,	//!< Disable the USB hub of any Awinda Station that is discovered but enable them when there is no Live XmeControl anymore
};

//! @}

typedef enum XmeSegmentIndex XmeSegmentIndex;
typedef enum XmeSegmentId XmeSegmentId;
typedef enum XmeClipBits XmeClipBits;
typedef enum XmeUsbHubMode XmeUsbHubMode;

#ifdef __cplusplus
extern "C" {
#endif

XME_DLL_API void xmeSegmentIndexToString(XmeSegmentIndex idx, struct XsString* returnValue);
XME_DLL_API void xmeSegmentIdToString(XmeSegmentId id, struct XsString* returnValue);

#ifdef __cplusplus
} // extern "C"

namespace XmeSegment
{
/*! \copydoc xmeSegmentIndexToString
*/
inline XsString indexToString(XmeSegmentIndex idx)
{
	XsString s;
	xmeSegmentIndexToString(idx, &s);
	return s;
}

/*! \copydoc xmeSegmentIdToString
*/
inline XsString idToString(XmeSegmentId id)
{
	XsString s;
	xmeSegmentIdToString(id, &s);
	return s;
}
}

namespace XmeClipping
{
//! Return whether the supplied \a clipBits indicate that any of the accelerometer channels clipped
inline bool isAccClipped(uint16_t clipBits)
{
	return !!(clipBits & XME_CLIP_ACC_ANY);
}
//! Return whether the supplied \a clipBits indicate that any of the gyroscope channels clipped
inline bool isGyrClipped(uint16_t clipBits)
{
	return !!(clipBits & XME_CLIP_GYR_ANY);
}
//! Return whether the supplied \a clipBits indicate that any of the magnetometer channels clipped
inline bool isMagClipped(uint16_t clipBits)
{
	return !!(clipBits & XME_CLIP_MAG_ANY);
}
//! Return whether the supplied \a clipBits indicate that any of the channels clipped
inline bool isAnyClipped(uint16_t clipBits)
{
	return !!(clipBits & XME_CLIP_ANY);
}
}
#endif

#endif
