#ifndef XMEGLOVETYPE_H
#define XMEGLOVETYPE_H

/*!	\addtogroup enums
	@{
*/

#define XME_FINGER_TRACKING_SEGMENT_COUNT	20 //!< \brief the number of segments per hand

/*! \brief This is an enumerator that contains the IDs of the glove types.
	\details These values are to be used when addressing a specific glove types.
	External glove can be any third party glove as long as it supplies the right data structure.
*/

//// MVN-7009 MVN-7013
// -->	"Xsens glove is for our glove in the future."
enum XmeGloveType
{
	XGT_NoGlove = 0,		//!< No glove / finger tracking available
	XGT_ExternalGlove = 1	//!< An external glove will provide data to XME
};
//! @}

typedef enum XmeGloveType XmeGloveType;

/*! \cond XS_INTERNAL */
/*!	\addtogroup enums
	@{
*/
/*! \brief This is an enumerator that contains the indices of the finger segments as they are defined in XmeControl::setFingerTrackingData(XsQuaternionArray const& fingerTrackingData, ..etc)
	\details These values should be used when creating or reading from the quaternion array.
*/
enum XmeFingerSegmentIndex
{
	XME_CARPUS_SEGMENT_INDEX = 0,
	XME_FIRST_MC_SEGMENT_INDEX,
	XME_FIRST_PP_SEGMENT_INDEX,
	XME_FIRST_DP_SEGMENT_INDEX,
	XME_SECOND_MC_SEGMENT_INDEX,
	XME_SECOND_PP_SEGMENT_INDEX,
	XME_SECOND_MP_SEGMENT_INDEX,
	XME_SECOND_DP_SEGMENT_INDEX,
	XME_THIRD_MC_SEGMENT_INDEX,
	XME_THIRD_PP_SEGMENT_INDEX,
	XME_THIRD_MP_SEGMENT_INDEX,
	XME_THIRD_DP_SEGMENT_INDEX,
	XME_FOURTH_MC_SEGMENT_INDEX,
	XME_FOURTH_PP_SEGMENT_INDEX,
	XME_FOURTH_MP_SEGMENT_INDEX,
	XME_FOURTH_DP_SEGMENT_INDEX,
	XME_FIFTH_MC_SEGMENT_INDEX,
	XME_FIFTH_PP_SEGMENT_INDEX,
	XME_FIFTH_MP_SEGMENT_INDEX,
	XME_FIFTH_DP_SEGMENT_INDEX
};
//! @}
/*! \endcond */

#endif
