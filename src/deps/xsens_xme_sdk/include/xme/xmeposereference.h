#ifndef XMEPOSEREFERENCE_H
#define XMEPOSEREFERENCE_H

/*!	\addtogroup enums
	@{
*/
/*! \brief Defines the reference pose to use
	\details The reference pose defines the orientations of segments when all quaternions are set to identity
*/
enum XmePoseReference
{
	XPR_IsbPose,		//!< The ISB pose is similar to an n-pose, but with with the hand-palms forward
	XPR_AnatomicalPose,	//!< The Anatomical pose is similar to the n-pose (hands straight down besides the body, palms towards the body)
	XPR_TPose,			//!< The T-pose is the classic T pose. This is the default reference for MVN data
};
//! @}
typedef enum XmePoseReference XmePoseReference;

#endif
