#ifndef XMEKINEMATICSTATE_H
#define XMEKINEMATICSTATE_H

#include "xmedllapi.h"
#include <xstypes/xsvector3.h>
#include <xstypes/xsquaternion.h>

#ifdef __cplusplus
extern "C" {
#endif

struct XmeKinematicState;
#ifndef __cplusplus
typedef struct XmeKinematicState XmeKinematicState;
#endif

XME_DLL_API void XmeKinematicState_construct(XmeKinematicState* thisPtr);
XME_DLL_API void XmeKinematicState_copyConstruct(XmeKinematicState* thisPtr, XmeKinematicState const* src);
XME_DLL_API void XmeKinematicState_destruct(XmeKinematicState* thisPtr);
XME_DLL_API void XmeKinematicState_copy(XmeKinematicState* copy, XmeKinematicState const* src);
XME_DLL_API void XmeKinematicState_swap(XmeKinematicState* a, XmeKinematicState* b);
XME_DLL_API int XmeKinematicState_empty(XmeKinematicState const* thisPtr);
XME_DLL_API int XmeKinematicState_compare(XmeKinematicState const* a, XmeKinematicState const* b);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Contains the kinematic state of a single kinematic object
	\details This is the full kinematic state including accelerations and velocities as opposed to just
	the position and orientation that XmeSegmentPose gives.
	\sa XmeSegmentPose
*/
struct XmeKinematicState
{
#ifdef __cplusplus
	//! \brief Default constructor
	XmeKinematicState()
	{
		clear();
	}

	/*! \brief Initializing constructor
		\param ori \copydoc m_orientation
		\param pos \copydoc m_position
		\param vel \copydoc m_velocity
		\param acc \copydoc m_acceleration
		\param angvel \copydoc m_angularVelocity
		\param angacc \copydoc m_angularAcceleration
	*/
	XmeKinematicState(const XsQuaternion& ori,
		const XsVector& pos,
		const XsVector& vel,
		const XsVector& acc,
		const XsVector& angvel,
		const XsVector& angacc)
		: m_orientation(ori)
		, m_position(pos)
		, m_velocity(vel)
		, m_acceleration(acc)
		, m_angularVelocity(angvel)
		, m_angularAcceleration(angacc)
	{
	}

	/*! \brief Copy constructor, copies the contents of \a other */
	XmeKinematicState(const XmeKinematicState& other)
		: m_orientation(other.m_orientation)
		, m_position(other.m_position)
		, m_velocity(other.m_velocity)
		, m_acceleration(other.m_acceleration)
		, m_angularVelocity(other.m_angularVelocity)
		, m_angularAcceleration(other.m_angularAcceleration)
	{
	}

	//! \brief Destructor
	~XmeKinematicState() {}

	/*! \brief Full setter, updates all contained values
		\param ori \copydoc m_orientation
		\param pos \copydoc m_position
		\param vel \copydoc m_velocity
		\param acc \copydoc m_acceleration
		\param angvel \copydoc m_angularVelocity
		\param angacc \copydoc m_angularAcceleration
	*/
	inline void set(const XsQuaternion& ori,
		const XsVector& pos,
		const XsVector& vel,
		const XsVector& acc,
		const XsVector& angvel,
		const XsVector& angacc)
	{
		m_orientation = ori;
		m_position = pos;
		m_velocity = vel;
		m_acceleration = acc;
		m_angularVelocity = angvel;
		m_angularAcceleration = angacc;
	}

	//! \brief Return true if the object is empty (orientation is identity and position is zero)
	inline bool empty() const
	{
		return m_orientation == XsQuaternion::identity() && m_position[0] == 0.0 && m_position[1] == 0.0 && m_position[2] == 0.0;
	}

	//! \brief Clear the object
	inline void clear()
	{
		m_orientation = XsQuaternion::identity();
		m_position.zero();
		m_velocity.zero();
		m_acceleration.zero();
		m_angularVelocity.zero();
		m_angularAcceleration.zero();
	}

	//! \brief Swap the contents with \a other
	inline void swap(XmeKinematicState& other) noexcept
	{
		XmeKinematicState_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeKinematicState& first, XmeKinematicState& second) noexcept
	{
		first.swap(second);
	}

	//! \brief Comparison operator, returns true when the contents are identical
	inline bool operator == (const XmeKinematicState& other) const
	{
		return	m_orientation == other.m_orientation &&
			m_position == other.m_position &&
			m_velocity == other.m_velocity &&
			m_acceleration == other.m_acceleration &&
			m_angularVelocity == other.m_angularVelocity &&
			m_angularAcceleration == other.m_angularAcceleration;
	}

	//private:	pod structure, all direct access allowed
#endif
	XsQuaternion m_orientation;			//!< The orientation of the segment with respect to the world
	XsVector3 m_position;				//!< The position of the segment with respect to the world
	XsVector3 m_velocity;				//!< The velocity of the segment with respect to the world
	XsVector3 m_acceleration;			//!< The acceleration of the segment with respect to the world
	XsVector3 m_angularVelocity;		//!< The angular velocity of the segment with respect to the world
	XsVector3 m_angularAcceleration;	//!< The angular acceleration of the segment with respect to the world
};
#endif
