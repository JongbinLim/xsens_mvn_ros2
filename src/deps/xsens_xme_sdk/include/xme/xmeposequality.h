#ifndef XMEPOSEQUALITY_H
#define XMEPOSEQUALITY_H

/*!	\addtogroup enums
	@{
*/

/*! \brief This enum describes the quality of pose data in a file */
enum XmePoseQuality
{
	XPQ_None = 0,		//!< File only contains raw sample data. Typical for OBR recordings that have not yet been fully processed.
	XPQ_Live = 1,		//!< File contains data as it was recorded, retransmitted data is not incorporated in solution
	XPQ_Fast = 2,		//!< File is processed using the live engine, retransmitted data is incorporated, giving a better result than XPQ_Live
	XPQ_Full = 3,		//!< File is processed using full solver, resulting in the best quality. Please note that for some scenarios XPQ_Fast and XPQ_Full are identical, in which case XPQ_Full will typically be returned.
};
//! @}
typedef enum XmePoseQuality XmePoseQuality;

#endif
