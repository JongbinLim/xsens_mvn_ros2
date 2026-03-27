#ifndef XMEAIDING_H
#define XMEAIDING_H

#include "xmedllapi.h"
#include <xstypes/xsvector3.h>
#include <xstypes/xsquaternion.h>
#include "xmelocation.h"
#include "xmeaidingflags.h"

#if defined(__cplusplus) && !defined(DOXYGEN)
extern "C" {
#endif

struct XmeAiding;
typedef struct XmeAiding XmeAiding;

XME_DLL_API void XmeAiding_construct(XmeAiding* thisPtr);
XME_DLL_API void XmeAiding_copyConstruct(XmeAiding* thisPtr, XmeAiding const* src);
XME_DLL_API void XmeAiding_destruct(XmeAiding* thisPtr);
XME_DLL_API void XmeAiding_copy(XmeAiding* copy, XmeAiding const* src);
XME_DLL_API void XmeAiding_swap(XmeAiding* a, XmeAiding* b);
XME_DLL_API int XmeAiding_empty(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_equal(XmeAiding const* a, XmeAiding const* b);

// getters
XME_DLL_API int XmeAiding_isContact(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isFloorLevel(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isDisableContactDetection(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isContactWithHeight(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isPosition(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isOrientation(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isAbsolutePosition(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isColdStart(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isAxisReset(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isNoContact(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isUserDefined(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_isRecorded(XmeAiding const* thisPtr);
XME_DLL_API int XmeAiding_hasFlagSet(XmeAiding const* thisPtr, XmeAidingFlags flag);

XME_DLL_API void XmeAiding_position(XmeAiding const* thisPtr, XsVector* returnValue);
XME_DLL_API void XmeAiding_orientationHelical(XmeAiding const* thisPtr, XsVector* returnValue);
XME_DLL_API void XmeAiding_orientationQuaternion(XmeAiding const* thisPtr, XsQuaternion* returnValue);
XME_DLL_API void XmeAiding_sd(XmeAiding const* thisPtr, XsVector* returnValue);
XME_DLL_API double XmeAiding_height(XmeAiding const* thisPtr);

// setters
XME_DLL_API void XmeAiding_setContact(XmeAiding* thisPtr, XmeLocation const* loc);
XME_DLL_API void XmeAiding_setNoContact(XmeAiding* thisPtr, XmeLocation const* loc);
XME_DLL_API void XmeAiding_setFloorLevel(XmeAiding* thisPtr, double height);
XME_DLL_API void XmeAiding_setDisableContactDetection(XmeAiding* thisPtr);
XME_DLL_API void XmeAiding_setContactWithHeight(XmeAiding* thisPtr, XmeLocation const* loc, double height, double sd);
XME_DLL_API void XmeAiding_setPosition(XmeAiding* thisPtr, XmeLocation const* loc, XsVector const* pos, XsVector const* sd);
XME_DLL_API void XmeAiding_setAbsolutePosition(XmeAiding* thisPtr, XmeLocation const* loc, XsVector const* pos);
XME_DLL_API void XmeAiding_setOrientationHelical(XmeAiding* thisPtr, XmeLocation const* loc, XsVector const* helical, XsVector const* sd);
XME_DLL_API void XmeAiding_setOrientationQuaternion(XmeAiding* thisPtr, XmeLocation const* loc, XsQuaternion const* quat, XsVector const* sd);
XME_DLL_API void XmeAiding_setSd(XmeAiding* thisPtr, XsVector const* sd);
XME_DLL_API void XmeAiding_setColdStart(XmeAiding* thisPtr);
XME_DLL_API void XmeAiding_setHotStart(XmeAiding* thisPtr);
XME_DLL_API void XmeAiding_setAxisReset(XmeAiding* thisPtr);
XME_DLL_API void XmeAiding_setUserDefined(XmeAiding* thisPtr, int yes);
XME_DLL_API void XmeAiding_setRecorded(XmeAiding* thisPtr, int yes);
XME_DLL_API void XmeAiding_setFlag(XmeAiding* thisPtr, XmeAidingFlags flag, int yes);

#if defined(__cplusplus) && !defined(DOXYGEN)
} // extern "C"
#endif

struct XmeAiding
{
#ifdef __cplusplus
	//! \brief Default constructor, constructs an 'invalid' object
	inline XmeAiding()
		: m_frameNumber(-1)
		, m_type(XAF_Invalid)
	{
		m_param.zero();
		m_extra.zero();
	}

	/*! \brief Initializing constructor
		\param typeFlags The flags describing the type of the aiding \sa XmeAidingFlags
		\param loc The body location where the aiding should be applied
		\param param_ The first parameter of the aiding as a 3-component vector. This usually specifies a position, field or distance
		\param extra_ The second parameter of the aiding as a 3-component vector. This usually specifies an accuracy measure of the first parameter
		\param frame The frame at which the aiding should be applied
	*/
	inline XmeAiding(int typeFlags, XmeLocation const& loc = XmeLocation::noLocation(), XsVector const& param_ = XsVector3::zero3(), XsVector const& extra_ = XsVector3::zero3(), int frame = 0)
		: m_frameNumber(frame)
		, m_type(typeFlags)
		, m_location(loc)
		, m_param(param_)
		, m_extra(extra_)
	{
	}

	//! \brief Returns true if the object refers to body location \a p
	inline bool operator == (XmeLocation const& p) const
	{
		return m_location == p;
	}

	//! \brief Returns true if the object contains the same type of aiding for the same location as \a that
	inline bool operator == (XmeAiding const& that) const
	{
		return (m_type & XAF_MaskBasic) == (that.m_type & XAF_MaskBasic)
			&& m_location == that.m_location;
	}

	//! \brief Compare object with \a b so we can order them by frame number, segment number and then by point number.
	inline bool operator < (XmeAiding const& b) const
	{
		return m_frameNumber < b.m_frameNumber ||
			(m_frameNumber == b.m_frameNumber &&
				m_location < b.m_location);
	}

	//! \brief Returns true when the object is empty/invalid
	inline bool empty() const
	{
		return 0 != XmeAiding_empty(this);
	}

	//! \brief Clear the object, making it invalid
	inline void clear()
	{
		*this = XmeAiding();
	}

	//! \brief Swap the contents of the object with \a other
	inline void swap(XmeAiding& other) noexcept
	{
		XmeAiding_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeAiding& first, XmeAiding& second) noexcept
	{
		first.swap(second);
	}

	// getters
	//! \brief Returns true if the object specifies a kind of contact
	inline bool isContact() const
	{
		return 0 != XmeAiding_isContact(this);
	}
	//! \brief Returns true if the object specifies a new floor level
	inline bool isFloorLevel() const
	{
		return 0 != XmeAiding_isFloorLevel(this);
	}
	//! \brief Returns true if the object specifies that contact detection should be disabled (for some segment)
	inline bool isDisableContactDetection() const
	{
		return 0 != XmeAiding_isDisableContactDetection(this);
	}
	//! \brief Returns true if the object specifies a contact with a given height
	inline bool isContactWithHeight() const
	{
		return 0 != XmeAiding_isContactWithHeight(this);
	}
	//! \brief Returns true if the object specifies a position with a margin of error
	inline bool isPosition() const
	{
		return 0 != XmeAiding_isPosition(this);
	}
	//! \brief Returns true if the object specifies a position with no margin of error
	inline bool isAbsolutePosition() const
	{
		return 0 != XmeAiding_isAbsolutePosition(this);
	}
	//! \brief Returns true if the object specifies that the aiding is an orientation item
	inline bool isOrientation() const
	{
		return 0 != XmeAiding_isOrientation(this);
	}
	//! \brief Returns true if the object specifies that a cold start should be done
	inline bool isColdStart() const
	{
		return 0 != XmeAiding_isColdStart(this);
	}
	//! \brief Returns true if the object specifies that an Axis Reset should be done
	inline bool isAxisReset() const
	{
		return 0 != XmeAiding_isAxisReset(this);
	}
	//! \brief Returns true if the object specifies that the specified segment should not be considered for contact detection
	inline bool isNoContact() const
	{
		return 0 != XmeAiding_isNoContact(this);
	}
	//! \brief Returns true if the object specifies a user defined aiding. This will get automatically set when calling XmeControl::setAiding
	inline bool isUserDefined() const
	{
		return 0 != XmeAiding_isUserDefined(this);
	}
	//! \brief Returns true if the object specifies that the aiding was added during recording as opposed to during (post-)processing. This kind of aiding will not be automatically cleaned during reprocessing.
	inline bool isRecorded() const
	{
		return 0 != XmeAiding_isRecorded(this);
	}
	//! \brief Returns true if aiding has the specified flag set.
	inline bool hasFlagSet(XmeAidingFlags flag) const
	{
		return 0 != XmeAiding_hasFlagSet(this, flag);
	}
	//! \brief Returns the body location where the aiding should be applied
	inline XmeLocation const& location() const
	{
		return m_location;
	}
	//! \brief Set the location where the aiding should be applied
	inline XmeAiding& setLocation(XmeLocation const& loc)
	{
		m_location = loc;
		return *this;
	}

	//! \copydoc XmeAiding_position
	inline XsVector position() const
	{
		XsVector3 tmp;
		XmeAiding_position(this, &tmp);
		return tmp;
	}

	//! \copydoc XmeAiding_orientationHelical
	inline XsVector orientationHelical() const
	{
		XsVector3 tmp;
		XmeAiding_orientationHelical(this, &tmp);
		return tmp;
	}

	//! \copydoc XmeAiding_orientationQuaternion
	inline XsQuaternion orientationQuaternion() const
	{
		XsQuaternion tmp;
		XmeAiding_orientationQuaternion(this, &tmp);
		return tmp;
	}

	//! \copydoc XmeAiding_sd
	inline XsVector sd() const
	{
		XsVector tmp;
		XmeAiding_sd(this, &tmp);
		return tmp;
	}

	//! \copydoc XmeAiding_height
	inline double height() const
	{
		return XmeAiding_height(this);
	}
	//! \brief Return the frame number where the aiding was/should be applied
	inline int frameNumber() const
	{
		return m_frameNumber;
	}
	//! \brief Set the frame number where the aiding should be applied
	inline XmeAiding& setFrameNumber(int frame)
	{
		m_frameNumber = frame;
		return *this;
	}

	/*! \brief Return the type of the aiding
		\details This returns the full type, which is a mixed bit-field. If you wish to see if the type matches a
		specific type, it is recommended to use the specific is... functions instead. If you wish to duplicate the type
		for constructing another object, this is a useful function.
		\sa basicType
	*/
	inline int type() const
	{
		return m_type;
	}
	/*! \brief Return the basic type of the aiding
		\details This returns the basic type, which will match one of the values in XmeAidingFlags, except
		for XAF_UserDefined and XAF_Recorded.
		This function is intended to be used in a switch statement.
		If you wish to see if the type matches a specific type, it is recommended to use the specific is... functions instead.
		\sa type
	*/
	inline int basicType() const
	{
		return m_type & XAF_MaskBasic;
	}
	//! \brief Return the first parameter of the aiding as a 3-component vector. This is usually the value of the aiding.
	inline XsVector const& param() const
	{
		return m_param;
	}
	//! \brief Return the second parameter of the aiding as a 3-component vector. This is usually the uncertainty of the value.
	inline XsVector const& extra() const
	{
		return m_extra;
	}

	/*! \brief Directly set the first parameter of the aiding as a 3-component vector.
		\details This is usually the value of the aiding. It is recommended to use the dedicated set... functions to
		specify the full aiding in one call.
		\param p The new value
	*/
	inline XmeAiding& setParam(XsVector const& p)
	{
		m_param = p;
		return *this;
	}
	/*! \brief Directly set the second parameter of the aiding as a 3-component vector.
		\details This is usually the uncertainty of the value. It is recommended to use the dedicated set... functions to
		specify the full aiding in one call.
		\param e The new value
	*/
	inline XmeAiding& setExtra(XsVector const& e)
	{
		m_extra = e;
		return *this;
	}

	// setters
	//! \copydoc XmeAiding_setContact
	inline XmeAiding& setContact(XmeLocation const& loc)
	{
		XmeAiding_setContact(this, &loc);
		return *this;
	}
	//! \copydoc XmeAiding_setNoContact
	inline XmeAiding& setNoContact(XmeLocation const& loc)
	{
		XmeAiding_setNoContact(this, &loc);
		return *this;
	}
	//! \copydoc XmeAiding_setFloorLevel
	inline XmeAiding& setFloorLevel(double height)
	{
		XmeAiding_setFloorLevel(this, height);
		return *this;
	}
	//! \copydoc XmeAiding_setDisableContactDetection
	inline XmeAiding& setDisableContactDetection()
	{
		XmeAiding_setDisableContactDetection(this);
		return *this;
	}
	//! \copydoc XmeAiding_setContactWithHeight
	inline XmeAiding& setContactWithHeight(XmeLocation const& loc, double height, double sd)
	{
		XmeAiding_setContactWithHeight(this, &loc, height, sd);
		return *this;
	}
	//! \copydoc XmeAiding_setPosition
	inline XmeAiding& setPosition(XmeLocation const& loc, XsVector const& pos, XsVector const& sd)
	{
		XmeAiding_setPosition(this, &loc, &pos, &sd);
		return *this;
	}
	//! \copydoc XmeAiding_setAbsolutePosition
	inline XmeAiding& setAbsolutePosition(XmeLocation const& loc, XsVector const& pos)
	{
		XmeAiding_setAbsolutePosition(this, &loc, &pos);
		return *this;
	}
	//! \copydoc XmeAiding_setOrientationHelical
	inline XmeAiding& setOrientation(XmeLocation const& loc, XsVector const& helical, XsVector const& sd)
	{
		XmeAiding_setOrientationHelical(this, &loc, &helical, &sd);
		return *this;
	}
	//! \copydoc XmeAiding_setOrientationQuaternion
	inline XmeAiding& setOrientation(XmeLocation const& loc, XsQuaternion const& quat, XsVector const& sd)
	{
		XmeAiding_setOrientationQuaternion(this, &loc, &quat, &sd);
		return *this;
	}
	//! \copydoc XmeAiding_setSd
	inline XmeAiding& setSd(XsVector const& sd)
	{
		XmeAiding_setSd(this, &sd);
		return *this;
	}
	//! \copydoc XmeAiding_setColdStart
	inline XmeAiding& setColdStart()
	{
		XmeAiding_setColdStart(this);
		return *this;
	}
	//! \copydoc XmeAiding_setHotStart
	inline XmeAiding& setHotStart()
	{
		XmeAiding_setHotStart(this);
		return *this;
	}
	//! \copydoc XmeAiding_setAxisReset
	inline XmeAiding& setAxisReset()
	{
		XmeAiding_setAxisReset(this);
		return *this;
	}

	//! \copydoc XmeAiding_setUserDefined
	inline XmeAiding& setUserDefined(bool yes = true)
	{
		XmeAiding_setUserDefined(this, yes ? 1 : 0);
		return *this;
	}
	//! \copydoc XmeAiding_setRecorded
	inline XmeAiding& setRecorded(bool yes = true)
	{
		XmeAiding_setRecorded(this, yes ? 1 : 0);
		return *this;
	}
	//! \copydoc XmeAiding_setFlag
	inline XmeAiding& setFlag(XmeAidingFlags flag, bool yes = true)
	{
		XmeAiding_setFlag(this, flag, yes ? 1 : 0);
		return *this;
	}

protected:
#endif
	int m_frameNumber;		//!< the frame number where the aiding was/should be applied
	int m_type;				//!< the full type of the aiding \sa type()
	XmeLocation m_location;	//!< the body location where the aiding should be applied
	XsVector3 m_param;		//!< the first parameter of the aiding as a 3-component vector. This is usually the value of the aiding.
	XsVector3 m_extra;		//!< the second parameter of the aiding as a 3-component vector. This is usually the uncertainty of the value.
};

#endif
