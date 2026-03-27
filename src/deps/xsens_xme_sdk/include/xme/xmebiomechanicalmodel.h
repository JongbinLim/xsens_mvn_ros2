#ifndef XMEBIOMECHMODEL_H
#define XMEBIOMECHMODEL_H

/*!	\addtogroup enums
	@{
*/
/*! \brief This enum contains the different choices in Biomechanical Models you have. */
enum XmeBiomechanicalModel
{
	Legacy = 0,		//!< Biomechanical model prior to Male/Female split
	Female = 1,		//!< Female biomechanical model for the biological female sex
	Male = 2,		//!< Male biomechanical model for the biological male sex
};
//! @}
typedef enum XmeBiomechanicalModel XmeBiomechanicalModel;

#endif
