#ifndef XMEPROCESSINGSTAGE_H
#define XMEPROCESSINGSTAGE_H

/*!	\addtogroup enums
	@{
*/
/*! \brief Defines the processing stage of a frame range
	\details This is used in %XmeCallback::onProcessingIntermediate and %XmeCallbackPlainC::m_onProcessingIntermediate
*/
enum XmeProcessingStage
{
	XPS_Read	= 0,	//!< Reading data and doing preliminary processing for the solver. During this stage the frame range will grow until the solver has read enough data to start processing.
	XPS_Solve	= 1,	//!< The solver does its processing. During this stage the frame range will be constant and the progress will go from 0 to 100. This does not reflect which frames have been processed within the range, as all frames are being updated simlutaneously.
	XPS_Write	= 2,	//!< Writing the results back to file. During this stage the frame range will be constant and the progress will go from 0 to 100, indicating which frames have been committed to the file. When a progress of 100 percent is reported, all frames for the range have been written to file.

	XPS_Count_			//!< The number of stages, can be used to ensure dynamic handling of this enum is backwards compatible
};
//! @}
typedef enum XmeProcessingStage XmeProcessingStage;

#endif
