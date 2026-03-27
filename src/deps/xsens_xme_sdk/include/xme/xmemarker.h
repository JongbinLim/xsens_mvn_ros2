#ifndef XMEMARKER_H
#define XMEMARKER_H

#include "xmedllapi.h"
#include <xstypes/xsstring.h>
#include "xmelocation.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifndef __cplusplus
#define XMEMARKER_INITIALIZER	{ -1, XSSTRING_INITIALIZER }
#endif

struct XmeMarker;
typedef struct XmeMarker XmeMarker;

XME_DLL_API void XmeMarker_construct(XmeMarker* thisPtr);
XME_DLL_API void XmeMarker_constructInit(XmeMarker* thisPtr, int frameNumber, XsString const* text);
XME_DLL_API void XmeMarker_copyConstruct(XmeMarker* thisPtr, XmeMarker const* src);
XME_DLL_API void XmeMarker_destruct(XmeMarker* thisPtr);
XME_DLL_API void XmeMarker_copy(XmeMarker* thisPtr, XmeMarker const* src);
XME_DLL_API int XmeMarker_equal(XmeMarker const* thisPtr, XmeMarker const* thatPtr);
XME_DLL_API void XmeMarker_swap(XmeMarker* a, XmeMarker* b);
XME_DLL_API int XmeMarker_empty(XmeMarker const* thisPtr);
XME_DLL_API int XmeMarker_valid(XmeMarker const* thisPtr);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief A marker, typically used to mark a special event in a file
*/
struct XmeMarker
{
#ifdef __cplusplus
	//! Basic constructor
	inline explicit XmeMarker(int frameNr = -1, const XsString& _text = XsString())
		: m_frameNumber(frameNr)
		, m_text(_text)
	{}

	//! Copy constructor
	inline XmeMarker(const XmeMarker& other)
		: m_frameNumber(other.m_frameNumber)
		, m_text(other.m_text)
	{}

	//! Destructor
	inline ~XmeMarker() {}

	//! Clear the contents of the marker
	inline void clear()
	{
		*this = XmeMarker();
	}

	//! Return the text of the marker
	inline const XsString& text() const
	{
		return m_text;
	}

	//! Return the frame number of the marker
	inline int frameNumber() const
	{
		return m_frameNumber;
	}

	//! Update the marker with new frame number and text
	inline void set(int frameNr, const XsString& _text)
	{
		m_frameNumber = frameNr;
		m_text = _text;
	}

	//! Returns true when the marker is considered empty
	inline bool empty() const
	{
		return 0 != XmeMarker_empty(this);
	}

	//! Returns true when the marker contains valid data
	inline bool valid() const
	{
		return 0 != XmeMarker_valid(this);
	}

	//! Comparison operator
	inline bool operator == (const XmeMarker& other) const
	{
		return 0 != XmeMarker_equal(this, &other);
	}

private:
	//! \protectedsection
#endif
	int m_frameNumber;	//!< The frame number of the marker
	XsString m_text;	//!< The text in the marker
};

#endif
