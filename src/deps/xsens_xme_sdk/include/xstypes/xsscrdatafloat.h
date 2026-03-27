#ifndef XSSCRDATAFLOAT_H
#define XSSCRDATAFLOAT_H

#include "pstdint.h"
#include "xsfloatvector.h"

#ifndef __cplusplus
	#define XSSCRDATAFLOAT_INITIALIZER {XSFLOATVECTOR_INITIALIZER, XSFLOATVECTOR_INITIALIZER, XSFLOATVECTOR_INITIALIZER, {0, 0, 0, 0}}
#endif

/*! \brief Container for raw sensor measurement data
	\details This structure contains raw measurement data from the sensors on the device.
	This data is unscaled, the bias has not been subtracted and no error correction has been applied.
*/
struct XsScrDataFloat
{
	XsFloatVector3	m_acc;	//!< The raw accelerometer data
	XsFloatVector3	m_gyr;	//!< The raw gyroscope data
	XsFloatVector3	m_mag;	//!< The raw magnetometer data
	float			m_temp;	//!< The temperature data
};
typedef struct XsScrDataFloat XsScrDataFloat;

#endif
