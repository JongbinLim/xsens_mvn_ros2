#ifndef XMEEULERPERMUTATION_H
#define XMEEULERPERMUTATION_H

/*!	\addtogroup enums
	@{
*/
/*! \brief Defines the order of the Euler rotations
	\details This order can influence when a gimbal lock occurs
*/
enum XmeEulerPermutation
{
	XEP_ZXY_YUp,	//!< Rotate around Z, then X, then Y, Y is the 'up' axis
	XEP_XZY_YUp		//!< Rotate around X, then Z, then Y, Y is the 'up' axis
};
//! @}
typedef enum XmeEulerPermutation XmeEulerPermutation;

#endif
