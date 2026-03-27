#ifndef XMEEXTENDEDCONFIGURATION_H
#define XMEEXTENDEDCONFIGURATION_H

#include "xmedllapi.h"
#include <xstypes/xsstring.h>
#include "xmepropdefinitionarray.h"
#include <xstypes/xshandid.h>
#include "xmeglovetype.h"

#ifdef __cplusplus
extern "C" {
#endif

struct XmeExtendedConfiguration;
typedef struct XmeExtendedConfiguration XmeExtendedConfiguration;

XME_DLL_API void XmeExtendedConfiguration_construct(XmeExtendedConfiguration* thisPtr);
XME_DLL_API void XmeExtendedConfiguration_copyConstruct(XmeExtendedConfiguration* thisPtr, XmeExtendedConfiguration const* src);
XME_DLL_API void XmeExtendedConfiguration_destruct(XmeExtendedConfiguration* thisPtr);
XME_DLL_API void XmeExtendedConfiguration_copy(XmeExtendedConfiguration* copy, XmeExtendedConfiguration const* src);
XME_DLL_API void XmeExtendedConfiguration_swap(XmeExtendedConfiguration* a, XmeExtendedConfiguration* b);

#ifdef __cplusplus
} // extern "C"
#endif

/*! \brief Contains extended status information of a (connected) device
*/
struct XmeExtendedConfiguration
{
	XsString m_configuration;					//!< The name of the basic configuration (FullBody, LowerBody, etc)
	XmePropDefinitionArray m_propDefinitions;	//!< Optional prop definition(s)
	XmeGloveType m_gloveDefinition;				//!< Optional glove definitions (XGT_NoGlove, XGT_ExternalGlove)

#ifdef __cplusplus
	//! \brief Default constructor
	XmeExtendedConfiguration()
		: m_configuration()
		, m_propDefinitions()
		, m_gloveDefinition(XGT_NoGlove)
	{
	}

	/*! \brief Initializing constructor
		\param config \copydoc m_configuration
		\param props \copydoc m_propDefinitions
		\param glove \copydoc m_gloveDefinition
	*/
	XmeExtendedConfiguration(XsString const& config, XmePropDefinitionArray const& props, XmeGloveType const& glove)
		: m_configuration(config)
		, m_propDefinitions(props)
		, m_gloveDefinition(glove)
	{
	}

	/*! \brief Initializing constructor
		\param config \copydoc m_configuration
		\param prop0 \copydoc m_propDefinitions
		\param prop1 \copydoc m_propDefinitions
		\param prop2 \copydoc m_propDefinitions
		\param prop3 \copydoc m_propDefinitions
		\param glove \copydoc m_gloveDefinition
	*/
	XmeExtendedConfiguration(XsString const& config, XmePropDefinition const& prop0 = XmePropDefinition(), XmePropDefinition const& prop1 = XmePropDefinition(), XmePropDefinition const& prop2 = XmePropDefinition(), XmePropDefinition const& prop3 = XmePropDefinition(), XmeGloveType const& glove = XGT_NoGlove)
		: m_configuration(config)
		, m_gloveDefinition(glove)
	{
		if (!prop0.m_uniqueName.empty())
		{
			m_propDefinitions.push_back(prop0);
			if (!prop1.m_uniqueName.empty())
			{
				m_propDefinitions.push_back(prop1);
				if (!prop2.m_uniqueName.empty())
				{
					m_propDefinitions.push_back(prop2);
					if (!prop3.m_uniqueName.empty())
						m_propDefinitions.push_back(prop3);
				}
			}
		}
	}

	//! \brief Copy constructor, copies contents from \a other
	XmeExtendedConfiguration(const XmeExtendedConfiguration& other)
		: m_configuration(other.m_configuration)
		, m_propDefinitions(other.m_propDefinitions)
		, m_gloveDefinition(other.m_gloveDefinition)
	{
	}

	//! \brief Destructor
	~XmeExtendedConfiguration()
	{
	}

	//! \brief Assignment operator, copies contents from \a other
	XmeExtendedConfiguration& operator = (const XmeExtendedConfiguration& other)
	{
		XmeExtendedConfiguration_copy(this, &other);
		return *this;
	}

	//! \brief Swap the contents of the object with \a other
	inline void swap(XmeExtendedConfiguration& other) noexcept
	{
		XmeExtendedConfiguration_swap(this, &other);
	}

	//! \brief Swap the contents of the \a first object with \a second
	inline friend void swap(XmeExtendedConfiguration& first, XmeExtendedConfiguration& second) noexcept
	{
		first.swap(second);
	}
#endif
};

#endif
