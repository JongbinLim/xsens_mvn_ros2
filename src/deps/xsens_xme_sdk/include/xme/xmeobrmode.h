#ifndef XMEOBRMODE_H
#define XMEOBRMODE_H

/*!	\addtogroup enums
	@{
*/
/*! \brief Defines the Obr mode */
enum XmeObrMode
{
	XOBRM_Disabled = 0,			//!< Obr is disabled (live)
	XOBRM_Enabled = 1,			//!< Obr is enabled
	XOBRM_EnabledRemote = 2,	//!< Remote Obr (ROBR) is on

	XOBRM_Unknown = -1			//!< Obr mode is unknown
};
//! @}
typedef enum XmeObrMode XmeObrMode;

#endif
