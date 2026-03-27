#ifndef XMEAIDINGFLAGS_H
#define XMEAIDINGFLAGS_H

/*!	\addtogroup enums
	@{
*/
/*! \brief These flags the type of aiding.
	\details The type is several bits wide, not a single flag, so not all types can be freely combined.
	\sa XmeAiding
*/
enum XmeAidingFlags
{
	XAF_Invalid				= 0x00000000, //!< Administrative initialization value
	XAF_ExternalContact		= 0x00000001, //!< An external contact aiding, these can also be set with xmeSetExternalContacts

	/*! \brief A floor level aiding.

		param1[2] sets the new floor level for use in the current and all future frames.
		When used with XAF_No, automatic contact detection is disabled (for the current frame only).
	*/
	XAF_FloorLevel			= 0x00000002,
	XAF_ContactWithHeight	= 0x00000003, //!< An external contact aiding at a height given in param1[2]
	XAF_Position			= 0x00000004, //!< A position aiding

	XAF_AxisReset			= 0x00000005, //!< Reinitialization of the fusion engine with an axis redefinition so the avatar will look along the x-axis after the reinitialization. This reset will keep the position of the pelvis constant.
	XAF_ColdStart			= 0x00000006, //!< Reinitialization of the fusion engine without axis redefinition.
	XAF_AbsolutePosition	= 0x00000008, //!< A position aiding, using an absolute, immediate reposition of the character.

	XAF_Orientation			= 0x00000009, //!< An orientation aiding

	/*! \brief This value can be logically OR-ed with one of the other types to indicate a negation of that aiding.

		This can be used to disable a specific contact for example. See the other aiding types for more details.
	*/
	XAF_No					= 0x00800000,
	XAF_MaskBasic			= 0x00FFFFFF, //!< This mask can be applied to a received aiding type to get the basic type.
	XAF_UserDefined			= 0x80000000, //!< This bit indicates that the aiding was user defined and not automatically computed from available data
	XAF_Recorded			= 0x40000000, //!< This bit indicates that the aiding was recorded from an automated aiding source
	XAF_MaskFlags			= 0x0F000000,
	XAF_HotStartFlag		= 0x01000000, //!< This bit indicates that the aiding should reinitialize the fusion engine without resetting the position and heading.
	XAF_Flag2				= 0x02000000, //!< Reserved for future use.
	XAF_Flag3				= 0x04000000, //!< Reserved for future use.
	XAF_Flag4				= 0x08000000, //!< Reserved for future use.
};
//! @}
typedef enum XmeAidingFlags XmeAidingFlags;

#endif
