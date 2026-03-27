#ifndef XMECALIBRATIONRESULT_H
#define XMECALIBRATIONRESULT_H

#include "xmedllapi.h"
#include <xstypes/xsstring.h>
#include <xstypes/xsstringarray.h>

#ifdef __cplusplus
extern "C" {
#endif

struct XmeCalibrationResult;
#ifndef __cplusplus
typedef struct XmeCalibrationResult XmeCalibrationResult;
#endif

XME_DLL_API void XmeCalibrationResult_construct(XmeCalibrationResult* thisPtr);
XME_DLL_API void XmeCalibrationResult_destruct(XmeCalibrationResult* thisPtr);
XME_DLL_API void XmeCalibrationResult_copy(XmeCalibrationResult* copy, XmeCalibrationResult const* src);
XME_DLL_API void XmeCalibrationResult_swap(XmeCalibrationResult* a, XmeCalibrationResult* b);
XME_DLL_API int XmeCalibrationResult_empty(XmeCalibrationResult const* thisPtr);

#ifdef __cplusplus
} // extern "C"
#endif

/*!	\addtogroup enums
	@{
*/
/*! \brief An idication of the estimated quality of the calibration */
enum XmeCalibrationQuality
{
	XCalQ_Unknown		= 0	//!< Unknown quality or not yet performed
	, XCalQ_Good		= 1	//!< The calibration procedure detected no problems
	, XCalQ_Acceptable	= 2	//!< The calibration procedure detected some anomalies, but nothing serious
	, XCalQ_Poor		= 3	//!< The calibration procedure detected some serious issues, redoing the calibration is recommended
	, XCalQ_Failed		= 4	//!< The calibration procedure could not make sense of the recorded data
};
//! @}
typedef enum XmeCalibrationQuality XmeCalibrationQuality;

/*! \brief A detailed description of the result of a calibration operation
*/
struct XmeCalibrationResult
{
#ifdef __cplusplus
	//! \brief Default constructor, creates a result with an XCalQ_Unknown quality
	XmeCalibrationResult()
		: m_consistency(0.0)
		, m_quality(XCalQ_Unknown)
	{
	}

	/*! \brief Initializing constructor
		\param nm The name of the calibration routine
		\param cons The internal consistency, should be in the range 0-1
		\param qual The estimated quality of the calibration results
		\param warn An optional list of strings describing potential problems that were detected during processing
		\param type The type of calibration performed
	*/
	XmeCalibrationResult(const XsString& nm, double cons, XmeCalibrationQuality qual, const XsStringArray& warn = XsStringArray(), const XsString& type = XsString())
		: m_name(nm)
		, m_consistency(cons)
		, m_quality(qual)
		, m_warnings(warn)
		, m_calibrationType(type)
	{
	}

	/*! \brief Destructor
	*/
	~XmeCalibrationResult()
	{
	}

	/*! \brief true when the object does not contain useful data
	*/
	inline bool empty() const
	{
		return 0 != XmeCalibrationResult_empty(this);
	}

	/*! \brief Clear the object
	*/
	inline void clear()
	{
		*this = XmeCalibrationResult();
	}

	/*! \brief Swap the contents with another XmeCalibrationResult
		\param other The object to swap the contents with
	*/
	inline void swap(XmeCalibrationResult& other) noexcept
	{
		XmeCalibrationResult_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeCalibrationResult& first, XmeCalibrationResult& second) noexcept
	{
		first.swap(second);
	}

	/*! \brief Comparison operator
		\details Check all fields for equality and return true if they are all equal
		\param other The object to compare with
		\return true if all fields are equal
	*/
	bool operator == (XmeCalibrationResult const& other) const
	{
		return	m_name == other.m_name &&
			m_consistency == other.m_consistency &&
			m_quality == other.m_quality &&
			m_warnings == other.m_warnings &&
			m_calibrationType == other.m_calibrationType;
	}

	// private: pod structure
#endif
	XsString m_name;					//!< The name of the calibration procedure that was performed (e.g. NPose)
	double m_consistency;				//!< An indication of the internal consistency of the calibration procedure
	XmeCalibrationQuality m_quality;	//!< An indication of the global quality of the calibration
	XsStringArray m_warnings;			//!< A list of warnings (may be empty). Note that the presence of a warning does not always indicate that a serious issue occurred.
	XsString m_calibrationType;			//!< Type of calibration performed (Static, Dynamic or Extended)
};

#endif
