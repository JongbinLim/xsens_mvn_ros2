#ifndef XMESENSORKINEMATICS_H
#define XMESENSORKINEMATICS_H

#include "xmedllapi.h"
#include <xstypes/xsvector3.h>
#include <xstypes/xsquaternion.h>

#ifdef __cplusplus
extern "C" {
#endif

struct XmeSensorKinematics;
typedef struct XmeSensorKinematics XmeSensorKinematics;

void XmeSensorKinematics_construct(XmeSensorKinematics* thisPtr);
void XmeSensorKinematics_copyConstruct(XmeSensorKinematics* thisPtr, XmeSensorKinematics const* src);
void XmeSensorKinematics_destruct(XmeSensorKinematics* thisPtr);
void XmeSensorKinematics_copy(XmeSensorKinematics* copy, XmeSensorKinematics const* thisPtr);
void XmeSensorKinematics_swap(XmeSensorKinematics* a, XmeSensorKinematics* b);
int XmeSensorKinematics_empty(XmeSensorKinematics const* thisPtr);
int XmeSensorKinematics_compare(XmeSensorKinematics const* a, XmeSensorKinematics const* b);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Contains sample data as recorded by a Motion Tracker in inertial format
*/
struct XmeSensorKinematics
{
#ifdef __cplusplus
	/*! \brief Default constructor */
	XmeSensorKinematics()
	{
		// m_orientation = XsQuaternion::identity(); this would make the object not empty
		m_acc.zero();
		m_gyr.zero();
		m_mag.zero();
		m_posG.zero();
		m_velG.zero();
		m_accG.zero();
	}

	/*! \brief Initializing constructor
		\param acc \copydoc m_acc
		\param gyr \copydoc m_gyr
		\param mag \copydoc m_mag
		\param q \copydoc m_q
		\param posG \copydoc m_posG
		\param velG \copydoc m_velG
		\param accG \copydoc m_accG
	*/
	XmeSensorKinematics(const XsVector& acc,
		const XsVector& gyr,
		const XsVector& mag,
		const XsQuaternion& q,
		const XsVector& posG,
		const XsVector& velG,
		const XsVector& accG)
		: m_acc(acc)
		, m_gyr(gyr)
		, m_mag(mag)
		, m_q(q)
		, m_posG(posG)
		, m_velG(velG)
		, m_accG(accG)
	{
	}

	//! \brief Copy constructor, copies contents of \a other
	XmeSensorKinematics(const XmeSensorKinematics& other)
		: m_acc(other.m_acc)
		, m_gyr(other.m_gyr)
		, m_mag(other.m_mag)
		, m_q(other.m_q)
		, m_posG(other.m_posG)
		, m_velG(other.m_velG)
		, m_accG(other.m_accG)
	{
	}

	//! \brief Destructor
	~XmeSensorKinematics()
	{}

	/*! \brief Full setter
		\details Updates all values
		\param acc \copydoc m_acc
		\param gyr \copydoc m_gyr
		\param mag \copydoc m_mag
		\param q \copydoc m_q
		\param posG \copydoc m_posG
		\param velG \copydoc m_velG
		\param accG \copydoc m_accG
	*/
	inline void set(const XsVector& acc,
		const XsVector& gyr,
		const XsVector& mag,
		const XsQuaternion& q,
		const XsVector& posG,
		const XsVector& velG,
		const XsVector& accG)
	{
		m_acc = acc;
		m_gyr = gyr;
		m_mag = mag;
		m_q = q;
		m_posG = posG;
		m_velG = velG;
		m_accG = accG;
	}

	//! \brief Returns true if the object is empty (m_q is empty)
	inline bool empty() const
	{
		return m_q.empty();
	}

	//! \brief Clear the object
	inline void clear()
	{
		*this = XmeSensorKinematics();
	}

	//! \brief Swap the contents with \a other
	inline void swap(XmeSensorKinematics& other) noexcept
	{
		m_acc.swap(other.m_acc);
		m_gyr.swap(other.m_gyr);
		m_mag.swap(other.m_mag);
		m_q.swap(other.m_q);
		m_posG.swap(other.m_posG);
		m_velG.swap(other.m_velG);
		m_accG.swap(other.m_accG);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeSensorKinematics& first, XmeSensorKinematics& second) noexcept
	{
		first.swap(second);
	}

	/*! \brief Return true if the \a other is numerically equal to this */
	inline bool operator == (const XmeSensorKinematics& other) const
	{
		return	m_acc == other.m_acc &&
			m_gyr == other.m_gyr &&
			m_mag == other.m_mag &&
			m_q == other.m_q &&
			m_posG == other.m_posG &&
			m_velG == other.m_velG &&
			m_accG == other.m_accG;
	}

	//private:	pod structure, all direct access allowed
#endif
	XsVector3 m_acc;	//!< The acceleration data in m/s2. Not available will return zero.
	XsVector3 m_gyr;	//!< The gyroscope (angular velocity) data in deg/s2. Not available will return zero.
	XsVector3 m_mag;	//!< The magnetometer data in arbitrary units

	XsQuaternion m_q;	//!< The computed orientation of the Motion Tracker
	XsVector3 m_posG;	//!< An estimate position of the Motion Tracker in the global frame in m
	XsVector3 m_velG;	//!< An estimate velocity of the Motion Tracker in the global frame in m/s
	XsVector3 m_accG;	//!< The free acceleration data of the Motion Tracker in m/s2
};

#endif
