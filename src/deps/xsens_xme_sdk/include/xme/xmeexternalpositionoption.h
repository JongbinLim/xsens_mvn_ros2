#ifndef XMEEXTERNALPOSITIONOPTION_H
#define XMEEXTERNALPOSITIONOPTION_H

/*!	\addtogroup enums
	@{
*/
/*! \brief Defines configuration options for use of external position data */
enum XmeExternalPositionOption
{
	XEPO_Current = 0,		//!< Use current configuration
	XEPO_Normal = 1,			//!< Use normal configuration (ie. no GNSS or external position data)
	XEPO_Gnss = 2,			//!< Use GNSS enabled configuration
	XEPO_ExtPos = 3,			//!< Use External Position enabled configuration
};
//! @}
typedef enum XmeExternalPositionOption XmeExternalPositionOption;

#endif
