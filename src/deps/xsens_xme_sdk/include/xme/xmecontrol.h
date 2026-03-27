#ifndef GUARD_XMECONTROL_H
#define GUARD_XMECONTROL_H
#include "xmecalibrationresult.h"
#include "xmepoint.h"
#include "xmelocation.h"
#include "xmejoint.h"
#include "xmemarkerarray.h"
#include "xmeaidingarray.h"
#include "xmepointarray.h"
#include "xmejointarray.h"
#include "xmeextendedconfiguration.h"
#include "xmeobrrecordingstate.h"
#include "xmedef.h"
#include <xstypes/xsexception.h>
#include <xstypes/xsversion.h>
#include <xstypes/xsbytearray.h>
#include <xstypes/xsint64array.h>
#include <xstypes/xssyncsettingarray.h>
#include <xstypes/xsdeviceidarray.h>
#include <xstypes/xsmatrix.h>
#include <xstypes/xsquaternionarray.h>
#include <xstypes/xsresultvalue.h>
#include <xstypes/xstimestamp.h>
#include <xstypes/xshandid.h>
#include "xmestatus.h"
#include "xmepose.h"
#include "xmecallbackplainc.h"
#include "xmesuitsample.h"
#include "xmeposereference.h"
#include "xmeeulerpermutation.h"
#include "xmerecordingstate.h"
#include "xmeposequality.h"
#include "xmeobrmode.h"
#include "xmeexternalpositionoption.h"
#include "xmebiomechanicalmodel.h"
#ifdef __cplusplus
extern "C" {
#endif
/*! \addtogroup cinterface C Interface
	@{ */
struct XmeControl;
#ifndef __cplusplus
typedef struct XmeControl XmeControl;
#endif
XME_DLL_API struct XmeControl* XmeControl_construct(void);
XME_DLL_API void XmeControl_destruct(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::~XmeControl()*/
XME_DLL_API XsResultValue* XmeControl_creationResultCode(XsResultValue* returnValue);/*!< \copydoc XmeControl::creationResultCode()*/
XME_DLL_API void XmeControl_setFactory(struct XmeControl* thisPtr, void* opaque);/*!< \copydoc XmeControl::setFactory(void*)*/
XME_DLL_API void* XmeControl_getFactory(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::getFactory()*/
XME_DLL_API void XmeControl_disconnectHardware(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::disconnectHardware()*/
XME_DLL_API XsResultValue* XmeControl_lastResultValue(const struct XmeControl* thisPtr, XsResultValue* returnValue);/*!< \copydoc XmeControl::lastResultValue() const*/
XME_DLL_API XsString* XmeControl_lastResultText(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::lastResultText() const*/
XME_DLL_API XmeStatus* XmeControl_status(const struct XmeControl* thisPtr, XmeStatus* returnValue);/*!< \copydoc XmeControl::status() const*/
XME_DLL_API void XmeControl_setTempFilePath(struct XmeControl* thisPtr, const XsString* path);/*!< \copydoc XmeControl::setTempFilePath(const XsString&)*/
XME_DLL_API XsString* XmeControl_tempFileName(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::tempFileName() const*/
XME_DLL_API int XmeControl_batteryLevel(const struct XmeControl* thisPtr, const XsDeviceId* devId);/*!< \copydoc XmeControl::batteryLevel(const XsDeviceId&) const*/
XME_DLL_API int XmeControl_secondaryBatteryLevel(const struct XmeControl* thisPtr, const XsDeviceId* devId);/*!< \copydoc XmeControl::secondaryBatteryLevel(const XsDeviceId&) const*/
XME_DLL_API XsDeviceId* XmeControl_masterId(const struct XmeControl* thisPtr, XsDeviceId* returnValue);/*!< \copydoc XmeControl::masterId() const*/
XME_DLL_API XsDeviceIdArray* XmeControl_detectedDevices(XsDeviceIdArray* returnValue);/*!< \copydoc XmeControl::detectedDevices()*/
XME_DLL_API XsString* XmeControl_portName(const struct XmeControl* thisPtr, XsString* returnValue, const XsDeviceId* devId);/*!< \copydoc XmeControl::portName(const XsDeviceId&) const*/
XME_DLL_API XsString* XmeControl_hardwareId(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::hardwareId() const*/
XME_DLL_API void XmeControl_closeFile(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::closeFile()*/
XME_DLL_API void XmeControl_copyMvnFile(struct XmeControl* thisPtr, const XsString* filename, int firstFrame, int lastFrame, int skipFactor);/*!< \copydoc XmeControl::copyMvnFile(const XsString&,int,int,int)*/
XME_DLL_API int XmeControl_copyProgress(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::copyProgress() const*/
XME_DLL_API int XmeControl_isMvnReadOnly(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::isMvnReadOnly() const*/
XME_DLL_API XmePoseQuality XmeControl_poseQuality(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::poseQuality() const*/
XME_DLL_API XmePoseQuality XmeControl_processingPoseQuality(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::processingPoseQuality() const*/
XME_DLL_API void XmeControl_createMvnFile(struct XmeControl* thisPtr, const XsString* filename);/*!< \copydoc XmeControl::createMvnFile(const XsString&)*/
XME_DLL_API void XmeControl_openMvnFile(struct XmeControl* thisPtr, const XsString* filename);/*!< \copydoc XmeControl::openMvnFile(const XsString&)*/
XME_DLL_API XsByteArray* XmeControl_readMtbMetaData(XsByteArray* returnValue, const XsString* mtbFileName);/*!< \copydoc XmeControl::readMtbMetaData(const XsString&)*/
XME_DLL_API void XmeControl_openRawFile(struct XmeControl* thisPtr, const XsString* filename, const XsString* mvnFilename, const XsString* previousFileInSequence, const XsString* settingsFile, const XsString* scenarioOverride, const XsString* calibrationType, XmePoseQuality quality, int useGnssData);/*!< \copydoc XmeControl::openRawFile(const XsString&,const XsString&,const XsString&,const XsString&,const XsString&,const XsString&,XmePoseQuality,bool)*/
XME_DLL_API void XmeControl_saveFile(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::saveFile()*/
XME_DLL_API void XmeControl_saveAndCloseFile(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::saveAndCloseFile()*/
XME_DLL_API void XmeControl_swapTrackers(struct XmeControl* thisPtr, const XsIntArray* newIdList, const XsString* calibrationFile);/*!< \copydoc XmeControl::swapTrackers(const XsIntArray&,const XsString&)*/
XME_DLL_API void XmeControl_remapRecordedData(struct XmeControl* thisPtr, const XmeLocationArray* from, const XmeLocationArray* to);/*!< \copydoc XmeControl::remapRecordedData(const XmeLocationArray&,const XmeLocationArray&)*/
XME_DLL_API int XmeControl_recordedDataRemapping(const struct XmeControl* thisPtr, XmeLocationArray* from, XmeLocationArray* to);/*!< \copydoc XmeControl::recordedDataRemapping(XmeLocationArray&,XmeLocationArray&) const*/
XME_DLL_API int XmeControl_recordedDataRemapping_1(const struct XmeControl* thisPtr, const XmeLocation* from, XmeLocation* to);/*!< \copydoc XmeControl::recordedDataRemapping(const XmeLocation&,XmeLocation&) const*/
XME_DLL_API XsIntArray* XmeControl_getSegmentsWithTrackers(XsIntArray* returnValue, const XsString* filename);/*!< \copydoc XmeControl::getSegmentsWithTrackers(const XsString&)*/
XME_DLL_API void XmeControl_clearAllMarkers(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::clearAllMarkers()*/
XME_DLL_API XmeMarkerArray* XmeControl_markers(const struct XmeControl* thisPtr, XmeMarkerArray* returnValue);/*!< \copydoc XmeControl::markers() const*/
XME_DLL_API void XmeControl_setMarker(struct XmeControl* thisPtr, const XmeMarker* m);/*!< \copydoc XmeControl::setMarker(const XmeMarker&)*/
XME_DLL_API void XmeControl_setMarker_1(struct XmeControl* thisPtr, int frameNumber, const XsString* text);/*!< \copydoc XmeControl::setMarker(int,const XsString&)*/
XME_DLL_API XmeMarker* XmeControl_marker(const struct XmeControl* thisPtr, XmeMarker* returnValue, int frameNumber);/*!< \copydoc XmeControl::marker(int) const*/
XME_DLL_API void XmeControl_removeMarker(struct XmeControl* thisPtr, int frameNumber);/*!< \copydoc XmeControl::removeMarker(int)*/
XME_DLL_API XmeAidingArray* XmeControl_contacts(const struct XmeControl* thisPtr, XmeAidingArray* returnValue, int frameNumber);/*!< \copydoc XmeControl::contacts(int) const*/
XME_DLL_API XmePose* XmeControl_pose(const struct XmeControl* thisPtr, XmePose* returnValue, int frameNumber);/*!< \copydoc XmeControl::pose(int) const*/
XME_DLL_API void XmeControl_reprocessData(struct XmeControl* thisPtr, XmePoseQuality desiredQuality);/*!< \copydoc XmeControl::reprocessData(XmePoseQuality)*/
XME_DLL_API void XmeControl_setContacts(struct XmeControl* thisPtr, const XmeAidingArray* contacts);/*!< \copydoc XmeControl::setContacts(const XmeAidingArray&)*/
XME_DLL_API void XmeControl_startRecording(struct XmeControl* thisPtr, const XsTimeStamp* when);/*!< \copydoc XmeControl::startRecording(const XsTimeStamp&)*/
XME_DLL_API void XmeControl_stopRecording(struct XmeControl* thisPtr, const XsTimeStamp* when);/*!< \copydoc XmeControl::stopRecording(const XsTimeStamp&)*/
XME_DLL_API XmeKinematicState* XmeControl_centerOfMass(const struct XmeControl* thisPtr, XmeKinematicState* returnValue, int frameNumber);/*!< \copydoc XmeControl::centerOfMass(int) const*/
XME_DLL_API void XmeControl_startRecordingOnBody(struct XmeControl* thisPtr, const XsString* filename, const XsTimeStamp* when);/*!< \copydoc XmeControl::startRecordingOnBody(const XsString&,const XsTimeStamp&)*/
XME_DLL_API void XmeControl_stopRecordingOnBody(struct XmeControl* thisPtr, const XsTimeStamp* when);/*!< \copydoc XmeControl::stopRecordingOnBody(const XsTimeStamp&)*/
XME_DLL_API XmeObrRecordingState* XmeControl_getObrRecordingState(const struct XmeControl* thisPtr, XmeObrRecordingState* returnValue);/*!< \copydoc XmeControl::getObrRecordingState() const*/
XME_DLL_API int XmeControl_setBodypackConfigFile(struct XmeControl* thisPtr, const XsByteArray* settings);/*!< \copydoc XmeControl::setBodypackConfigFile(const XsByteArray&)*/
XME_DLL_API XsResultValue* XmeControl_getBodypackConfigFile(const struct XmeControl* thisPtr, XsResultValue* returnValue, XsByteArray* settings);/*!< \copydoc XmeControl::getBodypackConfigFile(XsByteArray&) const*/
XME_DLL_API int XmeControl_segmentCount(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::segmentCount() const*/
XME_DLL_API int XmeControl_segmentId(const struct XmeControl* thisPtr, const XsString* name);/*!< \copydoc XmeControl::segmentId(const XsString&) const*/
XME_DLL_API int XmeControl_segmentIndex(const struct XmeControl* thisPtr, const XsString* name);/*!< \copydoc XmeControl::segmentIndex(const XsString&) const*/
XME_DLL_API XsString* XmeControl_segmentName(const struct XmeControl* thisPtr, XsString* returnValue, int segmentId);/*!< \copydoc XmeControl::segmentName(int) const*/
XME_DLL_API XsStringArray* XmeControl_segmentNames(const struct XmeControl* thisPtr, XsStringArray* returnValue);/*!< \copydoc XmeControl::segmentNames() const*/
XME_DLL_API XsString* XmeControl_fingerTrackingSegmentName(const struct XmeControl* thisPtr, XsString* returnValue, XsHandId hand, int segmentId);/*!< \copydoc XmeControl::fingerTrackingSegmentName(XsHandId,int) const*/
XME_DLL_API int XmeControl_sensorCount(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::sensorCount() const*/
XME_DLL_API XsString* XmeControl_bodyDimensionDescription(const struct XmeControl* thisPtr, XsString* returnValue, const XsString* label);/*!< \copydoc XmeControl::bodyDimensionDescription(const XsString&) const*/
XME_DLL_API XsStringArray* XmeControl_bodyDimensionLabelList(const struct XmeControl* thisPtr, XsStringArray* returnValue);/*!< \copydoc XmeControl::bodyDimensionLabelList() const*/
XME_DLL_API double XmeControl_bodyDimensionValue(const struct XmeControl* thisPtr, const XsString* label);/*!< \copydoc XmeControl::bodyDimensionValue(const XsString&) const*/
XME_DLL_API double XmeControl_bodyDimensionValueEstimate(const struct XmeControl* thisPtr, const XsString* label);/*!< \copydoc XmeControl::bodyDimensionValueEstimate(const XsString&) const*/
XME_DLL_API void XmeControl_setBodyDimension(struct XmeControl* thisPtr, const XsString* label, double value);/*!< \copydoc XmeControl::setBodyDimension(const XsString&,double)*/
XME_DLL_API XmeBiomechanicalModel XmeControl_biomechanicalModel(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::biomechanicalModel() const*/
XME_DLL_API void XmeControl_setBiomechanicalModel(struct XmeControl* thisPtr, XmeBiomechanicalModel biomechModel);/*!< \copydoc XmeControl::setBiomechanicalModel(XmeBiomechanicalModel)*/
XME_DLL_API void XmeControl_abortCalibration(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::abortCalibration()*/
XME_DLL_API XsStringArray* XmeControl_calibrationLabelList(const struct XmeControl* thisPtr, XsStringArray* returnValue);/*!< \copydoc XmeControl::calibrationLabelList() const*/
XME_DLL_API XmePose* XmeControl_calibrationPose(const struct XmeControl* thisPtr, XmePose* returnValue);/*!< \copydoc XmeControl::calibrationPose() const*/
XME_DLL_API XmeCalibrationResult* XmeControl_calibrationResult(const struct XmeControl* thisPtr, XmeCalibrationResult* returnValue, const XsString* label);/*!< \copydoc XmeControl::calibrationResult(const XsString&) const*/
XME_DLL_API void XmeControl_importBhmFromMvn(struct XmeControl* thisPtr, const XsString* filename);/*!< \copydoc XmeControl::importBhmFromMvn(const XsString&)*/
XME_DLL_API void XmeControl_initializeCalibration(struct XmeControl* thisPtr, const XsString* calibrationLabel, const XsString* tmpFile);/*!< \copydoc XmeControl::initializeCalibration(const XsString&,const XsString&)*/
XME_DLL_API void XmeControl_calibrateFromFile(struct XmeControl* thisPtr, const XsString* calibrationLabel, const XsString* mvnFile, int firstFrame, int lastFrame);/*!< \copydoc XmeControl::calibrateFromFile(const XsString&,const XsString&,int,int)*/
XME_DLL_API void XmeControl_startCalibration(struct XmeControl* thisPtr, const XsTimeStamp* when);/*!< \copydoc XmeControl::startCalibration(const XsTimeStamp&)*/
XME_DLL_API void XmeControl_clearCalibration(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::clearCalibration()*/
XME_DLL_API void XmeControl_saveCurrentCalibration(struct XmeControl* thisPtr, const XsString* filename);/*!< \copydoc XmeControl::saveCurrentCalibration(const XsString&)*/
XME_DLL_API void XmeControl_loadCalibrations(struct XmeControl* thisPtr, const XsString* filename);/*!< \copydoc XmeControl::loadCalibrations(const XsString&)*/
XME_DLL_API int XmeControl_isRecordedPositionCalibrationPerformed(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::isRecordedPositionCalibrationPerformed() const*/
XME_DLL_API XsIntArray* XmeControl_calibrationRecordingTimesRemaining(const struct XmeControl* thisPtr, XsIntArray* returnValue);/*!< \copydoc XmeControl::calibrationRecordingTimesRemaining() const*/
XME_DLL_API int XmeControl_remainingTimeInCurrentCalibrationStage(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::remainingTimeInCurrentCalibrationStage() const*/
XME_DLL_API int XmeControl_durationOfCurrentCalibrationStage(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::durationOfCurrentCalibrationStage() const*/
XME_DLL_API void XmeControl_setReadyToApplyCalibration(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::setReadyToApplyCalibration()*/
XME_DLL_API XmeSuitSample* XmeControl_suitSample(const struct XmeControl* thisPtr, XmeSuitSample* returnValue, int frameNumber);/*!< \copydoc XmeControl::suitSample(int) const*/
XME_DLL_API void XmeControl_coldStart(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::coldStart()*/
XME_DLL_API void XmeControl_setPose(struct XmeControl* thisPtr, const int frameNumber, const XmePose* pose);/*!< \copydoc XmeControl::setPose(const int,const XmePose&)*/
XME_DLL_API void XmeControl_setSuitSample(struct XmeControl* thisPtr, int frameNumber, const XmeSuitSample* sam);/*!< \copydoc XmeControl::setSuitSample(int,const XmeSuitSample&)*/
XME_DLL_API void XmeControl_setSdiData(struct XmeControl* thisPtr, int frameNumber, const XmeSdiSampleArray* sam);/*!< \copydoc XmeControl::setSdiData(int,const XmeSdiSampleArray&)*/
XME_DLL_API void XmeControl_clearAiding(struct XmeControl* thisPtr, int frameNumber);/*!< \copydoc XmeControl::clearAiding(int)*/
XME_DLL_API XmeAidingArray* XmeControl_aiding(const struct XmeControl* thisPtr, XmeAidingArray* returnValue, int frameNumber);/*!< \copydoc XmeControl::aiding(int) const*/
XME_DLL_API void XmeControl_setAiding(struct XmeControl* thisPtr, int frameNumber, const XmeAidingArray* aidList);/*!< \copydoc XmeControl::setAiding(int,const XmeAidingArray&)*/
XME_DLL_API void XmeControl_setTimestampedAiding(struct XmeControl* thisPtr, const XsTimeStamp* ts, const XmeAidingArray* aiding);/*!< \copydoc XmeControl::setTimestampedAiding(const XsTimeStamp&,const XmeAidingArray&)*/
XME_DLL_API int XmeControl_customAiding(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::customAiding() const*/
XME_DLL_API XsString* XmeControl_externalPositionAidingSystem(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::externalPositionAidingSystem() const*/
XME_DLL_API XmeCalibrationQuality* XmeControl_externalAidingCalibrationQuality(const struct XmeControl* thisPtr, XmeCalibrationQuality* returnValue);/*!< \copydoc XmeControl::externalAidingCalibrationQuality() const*/
XME_DLL_API int XmeControl_startCameraAidingCalibration(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::startCameraAidingCalibration()*/
XME_DLL_API int XmeControl_abortCameraAidingCalibration(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::abortCameraAidingCalibration()*/
XME_DLL_API void XmeControl_abortProcessing(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::abortProcessing()*/
XME_DLL_API int XmeControl_realTimePoseMode(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::realTimePoseMode() const*/
XME_DLL_API void XmeControl_setRealTimePoseMode(struct XmeControl* thisPtr, int enabled);/*!< \copydoc XmeControl::setRealTimePoseMode(bool)*/
XME_DLL_API XsString* XmeControl_filename(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::filename() const*/
XME_DLL_API int XmeControl_scanMode(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::scanMode() const*/
XME_DLL_API double XmeControl_sampleRate(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::sampleRate() const*/
XME_DLL_API int XmeControl_maxFastAidingPerFrame(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::maxFastAidingPerFrame() const*/
XME_DLL_API XsTimeStamp* XmeControl_recordStart(const struct XmeControl* thisPtr, XsTimeStamp* returnValue);/*!< \copydoc XmeControl::recordStart() const*/
XME_DLL_API XsString* XmeControl_userComment(const struct XmeControl* thisPtr, XsString* returnValue, int index);/*!< \copydoc XmeControl::userComment(int) const*/
XME_DLL_API void XmeControl_setMaxFastAidingPerFrame(struct XmeControl* thisPtr, int aidingCount);/*!< \copydoc XmeControl::setMaxFastAidingPerFrame(int)*/
XME_DLL_API void XmeControl_setScanMode(struct XmeControl* thisPtr, int enabled);/*!< \copydoc XmeControl::setScanMode(bool)*/
XME_DLL_API void XmeControl_setConfigMode(struct XmeControl* thisPtr, int enabled);/*!< \copydoc XmeControl::setConfigMode(bool)*/
XME_DLL_API void XmeControl_setUserComment(struct XmeControl* thisPtr, int index, const XsString* text);/*!< \copydoc XmeControl::setUserComment(int,const XsString&)*/
XME_DLL_API void XmeControl_setUserColor(const struct XmeControl* thisPtr, const XsByteArray* color);/*!< \copydoc XmeControl::setUserColor(const XsByteArray&) const*/
XME_DLL_API void XmeControl_setSampleRate(struct XmeControl* thisPtr, double frequency);/*!< \copydoc XmeControl::setSampleRate(double)*/
XME_DLL_API XsIntArray* XmeControl_sensorSegmentsForConfiguration(const struct XmeControl* thisPtr, XsIntArray* returnValue, const XsString* config);/*!< \copydoc XmeControl::sensorSegmentsForConfiguration(const XsString&) const*/
XME_DLL_API void XmeControl_switchOffHardware(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::switchOffHardware()*/
XME_DLL_API void XmeControl_disableRadio(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::disableRadio()*/
XME_DLL_API XmeObrMode XmeControl_getObrMode(const struct XmeControl* thisPtr, const XsDeviceId* masterId);/*!< \copydoc XmeControl::getObrMode(const XsDeviceId&) const*/
XME_DLL_API int XmeControl_currentMasterDeviceInOBRMode(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::currentMasterDeviceInOBRMode() const*/
XME_DLL_API void XmeControl_setObrMode(struct XmeControl* thisPtr, const XsDeviceId* masterId, XmeObrMode obrMode);/*!< \copydoc XmeControl::setObrMode(const XsDeviceId&,XmeObrMode)*/
XME_DLL_API void XmeControl_setObrModeStatic(const XsDeviceId* masterId, XmeObrMode obrMode);/*!< \copydoc XmeControl::setObrModeStatic(const XsDeviceId&,XmeObrMode)*/
XME_DLL_API void XmeControl_setUseGnss(struct XmeControl* thisPtr, int enable);/*!< \copydoc XmeControl::setUseGnss(bool)*/
XME_DLL_API int XmeControl_gnssDataAvailable(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::gnssDataAvailable() const*/
XME_DLL_API int XmeControl_gnssDataUsed(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::gnssDataUsed() const*/
XME_DLL_API int XmeControl_externalPositionAvailable(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::externalPositionAvailable() const*/
XME_DLL_API XsStringArray* XmeControl_elementTypeList(XsStringArray* returnValue);/*!< \copydoc XmeControl::elementTypeList()*/
XME_DLL_API XsString* XmeControl_elementType(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::elementType() const*/
XME_DLL_API void XmeControl_setElementType(struct XmeControl* thisPtr, const XsString* label);/*!< \copydoc XmeControl::setElementType(const XsString&)*/
XME_DLL_API XsString* XmeControl_configuration(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::configuration() const*/
XME_DLL_API XsStringArray* XmeControl_configurationList(const struct XmeControl* thisPtr, XsStringArray* returnValue);/*!< \copydoc XmeControl::configurationList() const*/
XME_DLL_API void XmeControl_setConfiguration(struct XmeControl* thisPtr, const XsString* label);/*!< \copydoc XmeControl::setConfiguration(const XsString&)*/
XME_DLL_API void XmeControl_setExtendedConfiguration(struct XmeControl* thisPtr, const XmeExtendedConfiguration* config);/*!< \copydoc XmeControl::setExtendedConfiguration(const XmeExtendedConfiguration&)*/
XME_DLL_API XmeExtendedConfiguration* XmeControl_extendedConfiguration(const struct XmeControl* thisPtr, XmeExtendedConfiguration* returnValue);/*!< \copydoc XmeControl::extendedConfiguration() const*/
XME_DLL_API XmePoint* XmeControl_point(const struct XmeControl* thisPtr, XmePoint* returnValue, const XmeLocation* loc);/*!< \copydoc XmeControl::point(const XmeLocation&) const*/
XME_DLL_API XmePointArray* XmeControl_points(const struct XmeControl* thisPtr, XmePointArray* returnValue, const XsString* segmentName);/*!< \copydoc XmeControl::points(const XsString&) const*/
XME_DLL_API XmePointArray* XmeControl_points_1(const struct XmeControl* thisPtr, XmePointArray* returnValue, int segmentId);/*!< \copydoc XmeControl::points(int) const*/
XME_DLL_API XmePointArray* XmeControl_fingerTrackingPoints(const struct XmeControl* thisPtr, XmePointArray* returnValue, XsHandId hand, int segmentId);/*!< \copydoc XmeControl::fingerTrackingPoints(XsHandId,int) const*/
XME_DLL_API XmeJointArray* XmeControl_joints(const struct XmeControl* thisPtr, XmeJointArray* returnValue);/*!< \copydoc XmeControl::joints() const*/
XME_DLL_API XsString* XmeControl_userScenario(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::userScenario() const*/
XME_DLL_API XsStringArray* XmeControl_userScenarioList(const struct XmeControl* thisPtr, XsStringArray* returnValue, XmeExternalPositionOption xco, XmePoseQuality quality);/*!< \copydoc XmeControl::userScenarioList(XmeExternalPositionOption,XmePoseQuality) const*/
XME_DLL_API void XmeControl_setUserScenario(struct XmeControl* thisPtr, const XsString* label);/*!< \copydoc XmeControl::setUserScenario(const XsString&)*/
XME_DLL_API XsVector* XmeControl_pointOffset(const struct XmeControl* thisPtr, XsVector* returnValue, const XmeLocation* loc, const XsQuaternion* q);/*!< \copydoc XmeControl::pointOffset(const XmeLocation&,const XsQuaternion&) const*/
XME_DLL_API void XmeControl_setSensorSegmentId(const XsDeviceId* sensorId, int segmentId);/*!< \copydoc XmeControl::setSensorSegmentId(const XsDeviceId&,int)*/
XME_DLL_API void XmeControl_allowConnection(const XsDeviceId* masterId, const XsDeviceId* sensorId, int allow);/*!< \copydoc XmeControl::allowConnection(const XsDeviceId&,const XsDeviceId&,bool)*/
XME_DLL_API void XmeControl_setRadioChannel(const XsDeviceId* masterId, int channel);/*!< \copydoc XmeControl::setRadioChannel(const XsDeviceId&,int)*/
XME_DLL_API int XmeControl_getDesiredRadioChannel(const XsDeviceId* masterId);/*!< \copydoc XmeControl::getDesiredRadioChannel(const XsDeviceId&)*/
XME_DLL_API void XmeControl_setWhiteList(const XsDeviceId* masterId, const XsDeviceIdArray* allowed);/*!< \copydoc XmeControl::setWhiteList(const XsDeviceId&,const XsDeviceIdArray&)*/
XME_DLL_API void XmeControl_setBlackList(const XsDeviceId* masterId, const XsDeviceIdArray* blocked);/*!< \copydoc XmeControl::setBlackList(const XsDeviceId&,const XsDeviceIdArray&)*/
XME_DLL_API void XmeControl_setSystemCredentials(const XsString* sessionId, const XsString* systemId);/*!< \copydoc XmeControl::setSystemCredentials(const XsString&,const XsString&)*/
XME_DLL_API void XmeControl_setStealthMode(int enabled);/*!< \copydoc XmeControl::setStealthMode(bool)*/
XME_DLL_API int XmeControl_getStealthMode(void);/*!< \copydoc XmeControl::getStealthMode()*/
XME_DLL_API void XmeControl_disconnectUnusedSystems(void);/*!< \copydoc XmeControl::disconnectUnusedSystems()*/
XME_DLL_API void XmeControl_setLogAllLiveData(const XsString* path);/*!< \copydoc XmeControl::setLogAllLiveData(const XsString&)*/
XME_DLL_API XsString* XmeControl_getLogAllLiveDataPath(XsString* returnValue);/*!< \copydoc XmeControl::getLogAllLiveDataPath()*/
XME_DLL_API void XmeControl_setDeviceBufferSize(int size);/*!< \copydoc XmeControl::setDeviceBufferSize(int)*/
XME_DLL_API int XmeControl_getDeviceBufferSize(void);/*!< \copydoc XmeControl::getDeviceBufferSize()*/
XME_DLL_API void XmeControl_setUsbHubMode(XmeUsbHubMode mode);/*!< \copydoc XmeControl::setUsbHubMode(XmeUsbHubMode)*/
XME_DLL_API XsIntArray* XmeControl_determineSupportedUpdateRates(XsIntArray* returnValue, const XsDeviceIdArray* deviceIds, int numberOfTrackers, int withRetransmissions);/*!< \copydoc XmeControl::determineSupportedUpdateRates(const XsDeviceIdArray&,int,bool)*/
XME_DLL_API void XmeControl_repairMvnFile(struct XmeControl* thisPtr, const XsString* filename, const XsString* assistingFilename);/*!< \copydoc XmeControl::repairMvnFile(const XsString&,const XsString&)*/
XME_DLL_API void XmeControl_synchronizeClock(const XsTimeStamp* local, const XsTimeStamp* global);/*!< \copydoc XmeControl::synchronizeClock(const XsTimeStamp&,const XsTimeStamp&)*/
XME_DLL_API XsTimeStamp* XmeControl_globalTime(XsTimeStamp* returnValue, const XsTimeStamp* local);/*!< \copydoc XmeControl::globalTime(const XsTimeStamp&)*/
XME_DLL_API XsTimeStamp* XmeControl_localTime(XsTimeStamp* returnValue, const XsTimeStamp* global);/*!< \copydoc XmeControl::localTime(const XsTimeStamp&)*/
XME_DLL_API XsTimeStamp* XmeControl_nowGlobal(XsTimeStamp* returnValue);/*!< \copydoc XmeControl::nowGlobal()*/
XME_DLL_API XsTimeStamp* XmeControl_nowLocal(XsTimeStamp* returnValue);/*!< \copydoc XmeControl::nowLocal()*/
XME_DLL_API void XmeControl_setHardwareIdFilter(struct XmeControl* thisPtr, const XsDeviceIdArray* deviceList);/*!< \copydoc XmeControl::setHardwareIdFilter(const XsDeviceIdArray&)*/
XME_DLL_API XsDeviceIdArray* XmeControl_hardwareIdFilter(const struct XmeControl* thisPtr, XsDeviceIdArray* returnValue);/*!< \copydoc XmeControl::hardwareIdFilter() const*/
XME_DLL_API void XmeControl_setHardwareIdFilters(struct XmeControl* thisPtr, const XsDeviceIdArray* whiteList, const XsDeviceIdArray* blackList);/*!< \copydoc XmeControl::setHardwareIdFilters(const XsDeviceIdArray&,const XsDeviceIdArray&)*/
XME_DLL_API void XmeControl_hardwareIdFilters(const struct XmeControl* thisPtr, XsDeviceIdArray* whiteList, XsDeviceIdArray* blackList);/*!< \copydoc XmeControl::hardwareIdFilters(XsDeviceIdArray&,XsDeviceIdArray&) const*/
XME_DLL_API void XmeControl_reinitializeHardware(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::reinitializeHardware()*/
XME_DLL_API void XmeControl_abortFlushing(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::abortFlushing()*/
XME_DLL_API XsString* XmeControl_wifiSsid(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::wifiSsid() const*/
XME_DLL_API void XmeControl_setWifiSsid(struct XmeControl* thisPtr, const XsString* ssid);/*!< \copydoc XmeControl::setWifiSsid(const XsString&)*/
XME_DLL_API XsVersion* XmeControl_version(XsVersion* returnValue);/*!< \copydoc XmeControl::version()*/
XME_DLL_API const XsVersion* XmeControl_versions(void);/*!< \copydoc XmeControl::versions()*/
XME_DLL_API void XmeControl_clearCallbackHandlers(struct XmeControl* thisPtr);/*!< \copydoc XmeControl::clearCallbackHandlers()*/
XME_DLL_API void XmeControl_addCallbackHandler(struct XmeControl* thisPtr, XmeCallbackPlainC* cb);/*!< \copydoc XmeControl::addCallbackHandler(XmeCallbackPlainC*)*/
XME_DLL_API void XmeControl_removeCallbackHandler(struct XmeControl* thisPtr, XmeCallbackPlainC* cb);/*!< \copydoc XmeControl::removeCallbackHandler(XmeCallbackPlainC*)*/
XME_DLL_API XsQuaternion* XmeControl_convertQuatPoseReference(XsQuaternion* returnValue, const XsQuaternion* source, XmePoseReference sourceRef, XmePoseReference destinationRef, int segmentIndex);/*!< \copydoc XmeControl::convertQuatPoseReference(const XsQuaternion&,XmePoseReference,XmePoseReference,int)*/
XME_DLL_API XsMatrix* XmeControl_jointAngles(const struct XmeControl* thisPtr, XsMatrix* returnValue, const XmePose* pose, XmeEulerPermutation eulerPermutation, XmePoseReference poseRef);/*!< \copydoc XmeControl::jointAngles(const XmePose&,XmeEulerPermutation,XmePoseReference) const*/
XME_DLL_API XsVector3* XmeControl_jointAngleFromQuaternion(XsVector3* returnValue, const XsQuaternion* qDiff, int childSegmentIndex, XmeEulerPermutation eulerPermutation);/*!< \copydoc XmeControl::jointAngleFromQuaternion(const XsQuaternion&,int,XmeEulerPermutation)*/
XME_DLL_API XsMatrix* XmeControl_jointAnglesErgo(const struct XmeControl* thisPtr, XsMatrix* returnValue, const XmePose* pose, XmeEulerPermutation eulerPermutation, XmePoseReference poseRef);/*!< \copydoc XmeControl::jointAnglesErgo(const XmePose&,XmeEulerPermutation,XmePoseReference) const*/
XME_DLL_API XsSyncSettingArray* XmeControl_syncSettings(const struct XmeControl* thisPtr, XsSyncSettingArray* returnValue);/*!< \copydoc XmeControl::syncSettings() const*/
XME_DLL_API int XmeControl_setSyncSettings(struct XmeControl* thisPtr, const XsSyncSettingArray* settingList);/*!< \copydoc XmeControl::setSyncSettings(const XsSyncSettingArray&)*/
XME_DLL_API XsSyncSettingArray* XmeControl_supportedSyncSettings(const struct XmeControl* thisPtr, XsSyncSettingArray* returnValue);/*!< \copydoc XmeControl::supportedSyncSettings() const*/
XME_DLL_API XsVersion* XmeControl_xmeVersionUsedForProcessingOfOpenFile(const struct XmeControl* thisPtr, XsVersion* returnValue);/*!< \copydoc XmeControl::xmeVersionUsedForProcessingOfOpenFile() const*/
XME_DLL_API XsString* XmeControl_motionCloudFileUuid(const struct XmeControl* thisPtr, XsString* returnValue);/*!< \copydoc XmeControl::motionCloudFileUuid() const*/
XME_DLL_API void XmeControl_setMotionCloudFileUuid(struct XmeControl* thisPtr, const XsString* uuid);/*!< \copydoc XmeControl::setMotionCloudFileUuid(const XsString&)*/
XME_DLL_API XsSyncSettingArray* XmeControl_supportedSyncSettings_1(XsSyncSettingArray* returnValue, const XsDeviceId* deviceId);/*!< \copydoc XmeControl::supportedSyncSettings(const XsDeviceId&)*/
XME_DLL_API int XmeControl_supportsSyncSettings(const XsDeviceId* deviceId);/*!< \copydoc XmeControl::supportsSyncSettings(const XsDeviceId&)*/
XME_DLL_API int XmeControl_isCompatibleSyncSetting(const XsDeviceId* deviceId, const XsSyncSetting* setting1, const XsSyncSetting* setting2);/*!< \copydoc XmeControl::isCompatibleSyncSetting(const XsDeviceId&,const XsSyncSetting&,const XsSyncSetting&)*/
XME_DLL_API unsigned int XmeControl_syncSettingsTimeResolutionInMicroSeconds(const XsDeviceId* deviceId);/*!< \copydoc XmeControl::syncSettingsTimeResolutionInMicroSeconds(const XsDeviceId&)*/
XME_DLL_API void XmeControl_setFingerTrackingData(struct XmeControl* thisPtr, const XsQuaternionArray* fingerTrackingData, XsHandId hand, int synchronize, const XsTimeStamp* ts);/*!< \copydoc XmeControl::setFingerTrackingData(const XsQuaternionArray&,XsHandId,bool,const XsTimeStamp&)*/
XME_DLL_API int XmeControl_hasFingerTrackingData(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::hasFingerTrackingData() const*/
XME_DLL_API void XmeControl_setExternalGloveIds(struct XmeControl* thisPtr, int64_t leftGloveId, int64_t rightGloveId);/*!< \copydoc XmeControl::setExternalGloveIds(int64_t,int64_t)*/
XME_DLL_API void XmeControl_getExternalGloveIds(const struct XmeControl* thisPtr, int64_t* leftGloveId, int64_t* rightGloveId);/*!< \copydoc XmeControl::getExternalGloveIds(int64_t&,int64_t&) const*/
XME_DLL_API int64_t XmeControl_utcOffset(const struct XmeControl* thisPtr);/*!< \copydoc XmeControl::utcOffset() const*/
XME_DLL_API void XmeControl_copyTimeSyncInfo(void* tsync);/*!< \copydoc XmeControl::copyTimeSyncInfo(void*)*/
XME_DLL_API int64_t XmeControl_currentUtcOffset(void);/*!< \copydoc XmeControl::currentUtcOffset()*/
/*! @} */
#ifdef __cplusplus
} // extern "C"
#if 0	// set to 0 to prevent throwing exceptions when an error occurs
#define XmeControl_USES_EXCEPTIONS
#define XmeControl_THROW()	ExceptionThrower eThrow(this)
#else
#define XmeControl_THROW()	(void) 0
#endif
struct XmeControl {
#ifdef XmeControl_USES_EXCEPTIONS
	//! \brief throws an XsException if the lastResultValue() is not XRV_OK
	inline void throwOnError(void) const
	{
		if (lastResultValue() != XRV_OK)
			throw XsException(lastResultValue(), lastResultText());
	}

	//! \brief Throws an exception if there is an error when the object is destroyed
	class ExceptionThrower {
	public:
		//! \brief Constructor
		ExceptionThrower(XmeControl const* obj) : m_obj(obj) {}
		//! \brief Destructor, throws an exception when m_obj has an error
		~ExceptionThrower() noexcept(false) { m_obj->throwOnError(); }
	private:
		//! \brief The object to check for errors
		XmeControl const* m_obj;
	};
#endif

	//! \brief Construct a new XmeControl* object. Clean it up with the destruct() function or delete the object
	inline static XmeControl* construct(void)
	{
		return XmeControl_construct();
	}

	//! \brief Destruct a XmeControl object and free all memory allocated for it
	inline void destruct(void)
	{
		XmeControl_destruct(this);
	}

	inline static XsResultValue creationResultCode(void)
	{
		XsResultValue returnValue;
		return *XmeControl_creationResultCode(&returnValue);
	}

	inline void setFactory(void* opaque)
	{
		XmeControl_THROW();
		XmeControl_setFactory(this, opaque);
	}

	inline void* getFactory(void)
	{
		XmeControl_THROW();
		return XmeControl_getFactory(this);
	}

	inline void disconnectHardware(void)
	{
		XmeControl_THROW();
		XmeControl_disconnectHardware(this);
	}

	inline XsResultValue lastResultValue(void) const
	{
		XsResultValue returnValue;
		return *XmeControl_lastResultValue(this, &returnValue);
	}

	inline XsString lastResultText(void) const
	{
		XsString returnValue;
		return *XmeControl_lastResultText(this, &returnValue);
	}

	inline XmeStatus status(void) const
	{
		XmeControl_THROW();
		XmeStatus returnValue;
		return *XmeControl_status(this, &returnValue);
	}

	inline void setTempFilePath(const XsString& path)
	{
		XmeControl_THROW();
		XmeControl_setTempFilePath(this, &path);
	}

	inline XsString tempFileName(void) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_tempFileName(this, &returnValue);
	}

	inline int batteryLevel(const XsDeviceId& devId) const
	{
		XmeControl_THROW();
		return XmeControl_batteryLevel(this, &devId);
	}

	inline int secondaryBatteryLevel(const XsDeviceId& devId) const
	{
		XmeControl_THROW();
		return XmeControl_secondaryBatteryLevel(this, &devId);
	}

	inline XsDeviceId masterId(void) const
	{
		XmeControl_THROW();
		XsDeviceId returnValue;
		return *XmeControl_masterId(this, &returnValue);
	}

	inline static XsDeviceIdArray detectedDevices(void)
	{
		XsDeviceIdArray returnValue;
		return *XmeControl_detectedDevices(&returnValue);
	}

	inline XsString portName(const XsDeviceId& devId) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_portName(this, &returnValue, &devId);
	}

	inline XsString hardwareId(void) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_hardwareId(this, &returnValue);
	}

	inline void closeFile(void)
	{
		XmeControl_THROW();
		XmeControl_closeFile(this);
	}

	inline void copyMvnFile(const XsString& filename, int firstFrame = 0, int lastFrame = XME_LAST_AVAILABLE_FRAME, int skipFactor = 0)
	{
		XmeControl_THROW();
		XmeControl_copyMvnFile(this, &filename, firstFrame, lastFrame, skipFactor);
	}

	inline int copyProgress(void) const
	{
		XmeControl_THROW();
		return XmeControl_copyProgress(this);
	}

	inline bool isMvnReadOnly(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_isMvnReadOnly(this);
	}

	inline XmePoseQuality poseQuality(void) const
	{
		XmeControl_THROW();
		return XmeControl_poseQuality(this);
	}

	inline XmePoseQuality processingPoseQuality(void) const
	{
		XmeControl_THROW();
		return XmeControl_processingPoseQuality(this);
	}

	inline void createMvnFile(const XsString& filename)
	{
		XmeControl_THROW();
		XmeControl_createMvnFile(this, &filename);
	}

	inline void openMvnFile(const XsString& filename)
	{
		XmeControl_THROW();
		XmeControl_openMvnFile(this, &filename);
	}

	inline static XsByteArray readMtbMetaData(const XsString& mtbFileName)
	{
		XsByteArray returnValue;
		return *XmeControl_readMtbMetaData(&returnValue, &mtbFileName);
	}

	inline void openRawFile(const XsString& filename, const XsString& mvnFilename, const XsString& previousFileInSequence, const XsString& settingsFile = XsString(), const XsString& scenarioOverride = XsString(), const XsString& calibrationType = XsString(), XmePoseQuality quality = XPQ_Fast, bool useGnssData = true)
	{
		XmeControl_THROW();
		XmeControl_openRawFile(this, &filename, &mvnFilename, &previousFileInSequence, &settingsFile, &scenarioOverride, &calibrationType, quality, useGnssData);
	}

	inline void saveFile(void)
	{
		XmeControl_THROW();
		XmeControl_saveFile(this);
	}

	inline void saveAndCloseFile(void)
	{
		XmeControl_THROW();
		XmeControl_saveAndCloseFile(this);
	}

	inline void swapTrackers(const XsIntArray& newIdList, const XsString& calibrationFile)
	{
		XmeControl_THROW();
		XmeControl_swapTrackers(this, &newIdList, &calibrationFile);
	}

	inline void remapRecordedData(const XmeLocationArray& from, const XmeLocationArray& to)
	{
		XmeControl_THROW();
		XmeControl_remapRecordedData(this, &from, &to);
	}

	inline bool recordedDataRemapping(XmeLocationArray& from, XmeLocationArray& to) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_recordedDataRemapping(this, &from, &to);
	}

	inline bool recordedDataRemapping(const XmeLocation& from, XmeLocation& to) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_recordedDataRemapping_1(this, &from, &to);
	}

	inline static XsIntArray getSegmentsWithTrackers(const XsString& filename)
	{
		XsIntArray returnValue;
		return *XmeControl_getSegmentsWithTrackers(&returnValue, &filename);
	}

	inline void clearAllMarkers(void)
	{
		XmeControl_THROW();
		XmeControl_clearAllMarkers(this);
	}

	inline XmeMarkerArray markers(void) const
	{
		XmeControl_THROW();
		XmeMarkerArray returnValue;
		return *XmeControl_markers(this, &returnValue);
	}

	inline void setMarker(const XmeMarker& m)
	{
		XmeControl_THROW();
		XmeControl_setMarker(this, &m);
	}

	inline void setMarker(int frameNumber, const XsString& text)
	{
		XmeControl_THROW();
		XmeControl_setMarker_1(this, frameNumber, &text);
	}

	inline XmeMarker marker(int frameNumber) const
	{
		XmeControl_THROW();
		XmeMarker returnValue;
		return *XmeControl_marker(this, &returnValue, frameNumber);
	}

	inline void removeMarker(int frameNumber)
	{
		XmeControl_THROW();
		XmeControl_removeMarker(this, frameNumber);
	}

	inline XmeAidingArray contacts(int frameNumber) const
	{
		XmeControl_THROW();
		XmeAidingArray returnValue;
		return *XmeControl_contacts(this, &returnValue, frameNumber);
	}

	inline XmePose pose(int frameNumber) const
	{
		XmeControl_THROW();
		XmePose returnValue;
		return *XmeControl_pose(this, &returnValue, frameNumber);
	}

	inline void reprocessData(XmePoseQuality desiredQuality)
	{
		XmeControl_THROW();
		XmeControl_reprocessData(this, desiredQuality);
	}

	inline void setContacts(const XmeAidingArray& contacts)
	{
		XmeControl_THROW();
		XmeControl_setContacts(this, &contacts);
	}

	inline void startRecording(const XsTimeStamp& when = XsTimeStamp())
	{
		XmeControl_THROW();
		XmeControl_startRecording(this, &when);
	}

	inline void stopRecording(const XsTimeStamp& when = XsTimeStamp())
	{
		XmeControl_THROW();
		XmeControl_stopRecording(this, &when);
	}

	inline XmeKinematicState centerOfMass(int frameNumber) const
	{
		XmeControl_THROW();
		XmeKinematicState returnValue;
		return *XmeControl_centerOfMass(this, &returnValue, frameNumber);
	}

	inline void startRecordingOnBody(const XsString& filename, const XsTimeStamp& when = XsTimeStamp())
	{
		XmeControl_THROW();
		XmeControl_startRecordingOnBody(this, &filename, &when);
	}

	inline void stopRecordingOnBody(const XsTimeStamp& when = XsTimeStamp())
	{
		XmeControl_THROW();
		XmeControl_stopRecordingOnBody(this, &when);
	}

	inline XmeObrRecordingState getObrRecordingState(void) const
	{
		XmeControl_THROW();
		XmeObrRecordingState returnValue;
		return *XmeControl_getObrRecordingState(this, &returnValue);
	}

	inline bool setBodypackConfigFile(const XsByteArray& settings)
	{
		XmeControl_THROW();
		return 0 != XmeControl_setBodypackConfigFile(this, &settings);
	}

	inline XsResultValue getBodypackConfigFile(XsByteArray& settings) const
	{
		XmeControl_THROW();
		XsResultValue returnValue;
		return *XmeControl_getBodypackConfigFile(this, &returnValue, &settings);
	}

	inline int segmentCount(void) const
	{
		XmeControl_THROW();
		return XmeControl_segmentCount(this);
	}

	inline int segmentId(const XsString& name) const
	{
		XmeControl_THROW();
		return XmeControl_segmentId(this, &name);
	}

	inline int segmentIndex(const XsString& name) const
	{
		XmeControl_THROW();
		return XmeControl_segmentIndex(this, &name);
	}

	inline XsString segmentName(int segmentId) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_segmentName(this, &returnValue, segmentId);
	}

	inline XsStringArray segmentNames(void) const
	{
		XmeControl_THROW();
		XsStringArray returnValue;
		return *XmeControl_segmentNames(this, &returnValue);
	}

	inline XsString fingerTrackingSegmentName(XsHandId hand, int segmentId) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_fingerTrackingSegmentName(this, &returnValue, hand, segmentId);
	}

	inline int sensorCount(void) const
	{
		XmeControl_THROW();
		return XmeControl_sensorCount(this);
	}

	inline XsString bodyDimensionDescription(const XsString& label) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_bodyDimensionDescription(this, &returnValue, &label);
	}

	inline XsStringArray bodyDimensionLabelList(void) const
	{
		XmeControl_THROW();
		XsStringArray returnValue;
		return *XmeControl_bodyDimensionLabelList(this, &returnValue);
	}

	inline double bodyDimensionValue(const XsString& label) const
	{
		XmeControl_THROW();
		return XmeControl_bodyDimensionValue(this, &label);
	}

	inline double bodyDimensionValueEstimate(const XsString& label) const
	{
		XmeControl_THROW();
		return XmeControl_bodyDimensionValueEstimate(this, &label);
	}

	inline void setBodyDimension(const XsString& label, double value)
	{
		XmeControl_THROW();
		XmeControl_setBodyDimension(this, &label, value);
	}

	inline XmeBiomechanicalModel biomechanicalModel(void) const
	{
		XmeControl_THROW();
		return XmeControl_biomechanicalModel(this);
	}

	inline void setBiomechanicalModel(XmeBiomechanicalModel biomechModel)
	{
		XmeControl_THROW();
		XmeControl_setBiomechanicalModel(this, biomechModel);
	}

	inline void abortCalibration(void)
	{
		XmeControl_THROW();
		XmeControl_abortCalibration(this);
	}

	inline XsStringArray calibrationLabelList(void) const
	{
		XmeControl_THROW();
		XsStringArray returnValue;
		return *XmeControl_calibrationLabelList(this, &returnValue);
	}

	inline XmePose calibrationPose(void) const
	{
		XmeControl_THROW();
		XmePose returnValue;
		return *XmeControl_calibrationPose(this, &returnValue);
	}

	inline XmeCalibrationResult calibrationResult(const XsString& label) const
	{
		XmeControl_THROW();
		XmeCalibrationResult returnValue;
		return *XmeControl_calibrationResult(this, &returnValue, &label);
	}

	inline void importBhmFromMvn(const XsString& filename)
	{
		XmeControl_THROW();
		XmeControl_importBhmFromMvn(this, &filename);
	}

	inline void initializeCalibration(const XsString& calibrationLabel, const XsString& tmpFile = XsString())
	{
		XmeControl_THROW();
		XmeControl_initializeCalibration(this, &calibrationLabel, &tmpFile);
	}

	inline void calibrateFromFile(const XsString& calibrationLabel, const XsString& mvnFile, int firstFrame, int lastFrame)
	{
		XmeControl_THROW();
		XmeControl_calibrateFromFile(this, &calibrationLabel, &mvnFile, firstFrame, lastFrame);
	}

	inline void startCalibration(const XsTimeStamp& when = XsTimeStamp())
	{
		XmeControl_THROW();
		XmeControl_startCalibration(this, &when);
	}

	inline void clearCalibration(void)
	{
		XmeControl_THROW();
		XmeControl_clearCalibration(this);
	}

	inline void saveCurrentCalibration(const XsString& filename)
	{
		XmeControl_THROW();
		XmeControl_saveCurrentCalibration(this, &filename);
	}

	inline void loadCalibrations(const XsString& filename)
	{
		XmeControl_THROW();
		XmeControl_loadCalibrations(this, &filename);
	}

	inline bool isRecordedPositionCalibrationPerformed(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_isRecordedPositionCalibrationPerformed(this);
	}

	inline XsIntArray calibrationRecordingTimesRemaining(void) const
	{
		XmeControl_THROW();
		XsIntArray returnValue;
		return *XmeControl_calibrationRecordingTimesRemaining(this, &returnValue);
	}

	inline int remainingTimeInCurrentCalibrationStage(void) const
	{
		XmeControl_THROW();
		return XmeControl_remainingTimeInCurrentCalibrationStage(this);
	}

	inline int durationOfCurrentCalibrationStage(void) const
	{
		XmeControl_THROW();
		return XmeControl_durationOfCurrentCalibrationStage(this);
	}

	inline void setReadyToApplyCalibration(void)
	{
		XmeControl_THROW();
		XmeControl_setReadyToApplyCalibration(this);
	}

	inline XmeSuitSample suitSample(int frameNumber) const
	{
		XmeControl_THROW();
		XmeSuitSample returnValue;
		return *XmeControl_suitSample(this, &returnValue, frameNumber);
	}

	inline void coldStart(void)
	{
		XmeControl_THROW();
		XmeControl_coldStart(this);
	}

	inline void setPose(const int frameNumber, const XmePose& pose)
	{
		XmeControl_THROW();
		XmeControl_setPose(this, frameNumber, &pose);
	}

	inline void setSuitSample(int frameNumber, const XmeSuitSample& sam)
	{
		XmeControl_THROW();
		XmeControl_setSuitSample(this, frameNumber, &sam);
	}

	inline void setSdiData(int frameNumber, const XmeSdiSampleArray& sam)
	{
		XmeControl_THROW();
		XmeControl_setSdiData(this, frameNumber, &sam);
	}

	inline void clearAiding(int frameNumber)
	{
		XmeControl_THROW();
		XmeControl_clearAiding(this, frameNumber);
	}

	inline XmeAidingArray aiding(int frameNumber) const
	{
		XmeControl_THROW();
		XmeAidingArray returnValue;
		return *XmeControl_aiding(this, &returnValue, frameNumber);
	}

	inline void setAiding(int frameNumber, const XmeAidingArray& aidList)
	{
		XmeControl_THROW();
		XmeControl_setAiding(this, frameNumber, &aidList);
	}

	inline void setTimestampedAiding(const XsTimeStamp& ts, const XmeAidingArray& aiding)
	{
		XmeControl_THROW();
		XmeControl_setTimestampedAiding(this, &ts, &aiding);
	}

	inline bool customAiding(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_customAiding(this);
	}

	inline XsString externalPositionAidingSystem(void) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_externalPositionAidingSystem(this, &returnValue);
	}

	inline XmeCalibrationQuality externalAidingCalibrationQuality(void) const
	{
		XmeControl_THROW();
		XmeCalibrationQuality returnValue;
		return *XmeControl_externalAidingCalibrationQuality(this, &returnValue);
	}

	inline bool startCameraAidingCalibration(void)
	{
		XmeControl_THROW();
		return 0 != XmeControl_startCameraAidingCalibration(this);
	}

	inline bool abortCameraAidingCalibration(void)
	{
		XmeControl_THROW();
		return 0 != XmeControl_abortCameraAidingCalibration(this);
	}

	inline void abortProcessing(void)
	{
		XmeControl_THROW();
		XmeControl_abortProcessing(this);
	}

	inline bool realTimePoseMode(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_realTimePoseMode(this);
	}

	inline void setRealTimePoseMode(bool enabled)
	{
		XmeControl_THROW();
		XmeControl_setRealTimePoseMode(this, enabled);
	}

	inline XsString filename(void) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_filename(this, &returnValue);
	}

	inline bool scanMode(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_scanMode(this);
	}

	inline double sampleRate(void) const
	{
		XmeControl_THROW();
		return XmeControl_sampleRate(this);
	}

	inline int maxFastAidingPerFrame(void) const
	{
		XmeControl_THROW();
		return XmeControl_maxFastAidingPerFrame(this);
	}

	inline XsTimeStamp recordStart(void) const
	{
		XmeControl_THROW();
		XsTimeStamp returnValue;
		return *XmeControl_recordStart(this, &returnValue);
	}

	inline XsString userComment(int index) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_userComment(this, &returnValue, index);
	}

	inline void setMaxFastAidingPerFrame(int aidingCount)
	{
		XmeControl_THROW();
		XmeControl_setMaxFastAidingPerFrame(this, aidingCount);
	}

	inline void setScanMode(bool enabled)
	{
		XmeControl_THROW();
		XmeControl_setScanMode(this, enabled);
	}

	inline void setConfigMode(bool enabled)
	{
		XmeControl_THROW();
		XmeControl_setConfigMode(this, enabled);
	}

	inline void setUserComment(int index, const XsString& text)
	{
		XmeControl_THROW();
		XmeControl_setUserComment(this, index, &text);
	}

	inline void setUserColor(const XsByteArray& color) const
	{
		XmeControl_THROW();
		XmeControl_setUserColor(this, &color);
	}

	inline void setSampleRate(double frequency)
	{
		XmeControl_THROW();
		XmeControl_setSampleRate(this, frequency);
	}

	inline XsIntArray sensorSegmentsForConfiguration(const XsString& config) const
	{
		XmeControl_THROW();
		XsIntArray returnValue;
		return *XmeControl_sensorSegmentsForConfiguration(this, &returnValue, &config);
	}

	inline void switchOffHardware(void)
	{
		XmeControl_THROW();
		XmeControl_switchOffHardware(this);
	}

	inline void disableRadio(void)
	{
		XmeControl_THROW();
		XmeControl_disableRadio(this);
	}

	inline XmeObrMode getObrMode(const XsDeviceId& masterId) const
	{
		XmeControl_THROW();
		return XmeControl_getObrMode(this, &masterId);
	}

	inline bool currentMasterDeviceInOBRMode(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_currentMasterDeviceInOBRMode(this);
	}

	inline void setObrMode(const XsDeviceId& masterId, XmeObrMode obrMode)
	{
		XmeControl_THROW();
		XmeControl_setObrMode(this, &masterId, obrMode);
	}

	inline static void setObrModeStatic(const XsDeviceId& masterId, XmeObrMode obrMode)
	{
		XmeControl_setObrModeStatic(&masterId, obrMode);
	}

	inline void setUseGnss(bool enable)
	{
		XmeControl_THROW();
		XmeControl_setUseGnss(this, enable);
	}

	inline bool gnssDataAvailable(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_gnssDataAvailable(this);
	}

	inline bool gnssDataUsed(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_gnssDataUsed(this);
	}

	inline bool externalPositionAvailable(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_externalPositionAvailable(this);
	}

	inline static XsStringArray elementTypeList(void)
	{
		XsStringArray returnValue;
		return *XmeControl_elementTypeList(&returnValue);
	}

	inline XsString elementType(void) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_elementType(this, &returnValue);
	}

	inline void setElementType(const XsString& label)
	{
		XmeControl_THROW();
		XmeControl_setElementType(this, &label);
	}

	inline XsString configuration(void) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_configuration(this, &returnValue);
	}

	inline XsStringArray configurationList(void) const
	{
		XmeControl_THROW();
		XsStringArray returnValue;
		return *XmeControl_configurationList(this, &returnValue);
	}

	inline void setConfiguration(const XsString& label)
	{
		XmeControl_THROW();
		XmeControl_setConfiguration(this, &label);
	}

	inline void setExtendedConfiguration(const XmeExtendedConfiguration& config)
	{
		XmeControl_THROW();
		XmeControl_setExtendedConfiguration(this, &config);
	}

	inline XmeExtendedConfiguration extendedConfiguration(void) const
	{
		XmeControl_THROW();
		XmeExtendedConfiguration returnValue;
		return *XmeControl_extendedConfiguration(this, &returnValue);
	}

	inline XmePoint point(const XmeLocation& loc) const
	{
		XmeControl_THROW();
		XmePoint returnValue;
		return *XmeControl_point(this, &returnValue, &loc);
	}

	inline XmePointArray points(const XsString& segmentName) const
	{
		XmeControl_THROW();
		XmePointArray returnValue;
		return *XmeControl_points(this, &returnValue, &segmentName);
	}

	inline XmePointArray points(int segmentId) const
	{
		XmeControl_THROW();
		XmePointArray returnValue;
		return *XmeControl_points_1(this, &returnValue, segmentId);
	}

	inline XmePointArray fingerTrackingPoints(XsHandId hand, int segmentId) const
	{
		XmeControl_THROW();
		XmePointArray returnValue;
		return *XmeControl_fingerTrackingPoints(this, &returnValue, hand, segmentId);
	}

	inline XmeJointArray joints(void) const
	{
		XmeControl_THROW();
		XmeJointArray returnValue;
		return *XmeControl_joints(this, &returnValue);
	}

	inline XsString userScenario(void) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_userScenario(this, &returnValue);
	}

	inline XsStringArray userScenarioList(XmeExternalPositionOption xco = XEPO_Current, XmePoseQuality quality = XPQ_None) const
	{
		XmeControl_THROW();
		XsStringArray returnValue;
		return *XmeControl_userScenarioList(this, &returnValue, xco, quality);
	}

	inline void setUserScenario(const XsString& label)
	{
		XmeControl_THROW();
		XmeControl_setUserScenario(this, &label);
	}

	inline XsVector pointOffset(const XmeLocation& loc, const XsQuaternion& q) const
	{
		XmeControl_THROW();
		XsVector returnValue;
		return *XmeControl_pointOffset(this, &returnValue, &loc, &q);
	}

	inline static void setSensorSegmentId(const XsDeviceId& sensorId, int segmentId)
	{
		XmeControl_setSensorSegmentId(&sensorId, segmentId);
	}

	inline static void allowConnection(const XsDeviceId& masterId, const XsDeviceId& sensorId, bool allow)
	{
		XmeControl_allowConnection(&masterId, &sensorId, allow);
	}

	inline static void setRadioChannel(const XsDeviceId& masterId, int channel)
	{
		XmeControl_setRadioChannel(&masterId, channel);
	}

	inline static int getDesiredRadioChannel(const XsDeviceId& masterId)
	{
		return XmeControl_getDesiredRadioChannel(&masterId);
	}

	inline static void setWhiteList(const XsDeviceId& masterId, const XsDeviceIdArray& allowed)
	{
		XmeControl_setWhiteList(&masterId, &allowed);
	}

	inline static void setBlackList(const XsDeviceId& masterId, const XsDeviceIdArray& blocked)
	{
		XmeControl_setBlackList(&masterId, &blocked);
	}

	inline static void setSystemCredentials(const XsString& sessionId, const XsString& systemId = XsString())
	{
		XmeControl_setSystemCredentials(&sessionId, &systemId);
	}

	inline static void setStealthMode(bool enabled)
	{
		XmeControl_setStealthMode(enabled);
	}

	inline static bool getStealthMode(void)
	{
		return 0 != XmeControl_getStealthMode();
	}

	inline static void disconnectUnusedSystems(void)
	{
		XmeControl_disconnectUnusedSystems();
	}

	inline static void setLogAllLiveData(const XsString& path)
	{
		XmeControl_setLogAllLiveData(&path);
	}

	inline static XsString getLogAllLiveDataPath(void)
	{
		XsString returnValue;
		return *XmeControl_getLogAllLiveDataPath(&returnValue);
	}

	inline static void setDeviceBufferSize(int size)
	{
		XmeControl_setDeviceBufferSize(size);
	}

	inline static int getDeviceBufferSize(void)
	{
		return XmeControl_getDeviceBufferSize();
	}

	inline static void setUsbHubMode(XmeUsbHubMode mode)
	{
		XmeControl_setUsbHubMode(mode);
	}

	inline static XsIntArray determineSupportedUpdateRates(const XsDeviceIdArray& deviceIds, int numberOfTrackers, bool withRetransmissions)
	{
		XsIntArray returnValue;
		return *XmeControl_determineSupportedUpdateRates(&returnValue, &deviceIds, numberOfTrackers, withRetransmissions);
	}

	inline void repairMvnFile(const XsString& filename, const XsString& assistingFilename)
	{
		XmeControl_THROW();
		XmeControl_repairMvnFile(this, &filename, &assistingFilename);
	}

	inline static void synchronizeClock(const XsTimeStamp& local, const XsTimeStamp& global)
	{
		XmeControl_synchronizeClock(&local, &global);
	}

	inline static XsTimeStamp globalTime(const XsTimeStamp& local)
	{
		XsTimeStamp returnValue;
		return *XmeControl_globalTime(&returnValue, &local);
	}

	inline static XsTimeStamp localTime(const XsTimeStamp& global)
	{
		XsTimeStamp returnValue;
		return *XmeControl_localTime(&returnValue, &global);
	}

	inline static XsTimeStamp nowGlobal(void)
	{
		XsTimeStamp returnValue;
		return *XmeControl_nowGlobal(&returnValue);
	}

	inline static XsTimeStamp nowLocal(void)
	{
		XsTimeStamp returnValue;
		return *XmeControl_nowLocal(&returnValue);
	}

	inline void setHardwareIdFilter(const XsDeviceIdArray& deviceList)
	{
		XmeControl_THROW();
		XmeControl_setHardwareIdFilter(this, &deviceList);
	}

	inline XsDeviceIdArray hardwareIdFilter(void) const
	{
		XmeControl_THROW();
		XsDeviceIdArray returnValue;
		return *XmeControl_hardwareIdFilter(this, &returnValue);
	}

	inline void setHardwareIdFilters(const XsDeviceIdArray& whiteList, const XsDeviceIdArray& blackList)
	{
		XmeControl_THROW();
		XmeControl_setHardwareIdFilters(this, &whiteList, &blackList);
	}

	inline void hardwareIdFilters(XsDeviceIdArray& whiteList, XsDeviceIdArray& blackList) const
	{
		XmeControl_THROW();
		XmeControl_hardwareIdFilters(this, &whiteList, &blackList);
	}

	inline void reinitializeHardware(void)
	{
		XmeControl_THROW();
		XmeControl_reinitializeHardware(this);
	}

	inline void abortFlushing(void)
	{
		XmeControl_THROW();
		XmeControl_abortFlushing(this);
	}

	inline XsString wifiSsid(void) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_wifiSsid(this, &returnValue);
	}

	inline void setWifiSsid(const XsString& ssid)
	{
		XmeControl_THROW();
		XmeControl_setWifiSsid(this, &ssid);
	}

	inline static XsVersion version(void)
	{
		XsVersion returnValue;
		return *XmeControl_version(&returnValue);
	}

	inline static const XsVersion* versions(void)
	{
		return XmeControl_versions();
	}

	inline void clearCallbackHandlers(void)
	{
		XmeControl_THROW();
		XmeControl_clearCallbackHandlers(this);
	}

	inline void addCallbackHandler(XmeCallbackPlainC* cb)
	{
		XmeControl_THROW();
		XmeControl_addCallbackHandler(this, cb);
	}

	inline void removeCallbackHandler(XmeCallbackPlainC* cb)
	{
		XmeControl_THROW();
		XmeControl_removeCallbackHandler(this, cb);
	}

	inline static XsQuaternion convertQuatPoseReference(const XsQuaternion& source, XmePoseReference sourceRef, XmePoseReference destinationRef, int segmentIndex)
	{
		XsQuaternion returnValue;
		return *XmeControl_convertQuatPoseReference(&returnValue, &source, sourceRef, destinationRef, segmentIndex);
	}

	inline XsMatrix jointAngles(const XmePose& pose, XmeEulerPermutation eulerPermutation = XEP_ZXY_YUp, XmePoseReference poseRef = XPR_IsbPose) const
	{
		XmeControl_THROW();
		XsMatrix returnValue;
		return *XmeControl_jointAngles(this, &returnValue, &pose, eulerPermutation, poseRef);
	}

	inline static XsVector3 jointAngleFromQuaternion(const XsQuaternion& qDiff, int childSegmentIndex, XmeEulerPermutation eulerPermutation = XEP_ZXY_YUp)
	{
		XsVector3 returnValue;
		return *XmeControl_jointAngleFromQuaternion(&returnValue, &qDiff, childSegmentIndex, eulerPermutation);
	}

	inline XsMatrix jointAnglesErgo(const XmePose& pose, XmeEulerPermutation eulerPermutation = XEP_ZXY_YUp, XmePoseReference poseRef = XPR_IsbPose) const
	{
		XmeControl_THROW();
		XsMatrix returnValue;
		return *XmeControl_jointAnglesErgo(this, &returnValue, &pose, eulerPermutation, poseRef);
	}

	inline XsSyncSettingArray syncSettings(void) const
	{
		XmeControl_THROW();
		XsSyncSettingArray returnValue;
		return *XmeControl_syncSettings(this, &returnValue);
	}

	inline bool setSyncSettings(const XsSyncSettingArray& settingList)
	{
		XmeControl_THROW();
		return 0 != XmeControl_setSyncSettings(this, &settingList);
	}

	inline XsSyncSettingArray supportedSyncSettings(void) const
	{
		XmeControl_THROW();
		XsSyncSettingArray returnValue;
		return *XmeControl_supportedSyncSettings(this, &returnValue);
	}

	inline XsVersion xmeVersionUsedForProcessingOfOpenFile(void) const
	{
		XmeControl_THROW();
		XsVersion returnValue;
		return *XmeControl_xmeVersionUsedForProcessingOfOpenFile(this, &returnValue);
	}

	inline XsString motionCloudFileUuid(void) const
	{
		XmeControl_THROW();
		XsString returnValue;
		return *XmeControl_motionCloudFileUuid(this, &returnValue);
	}

	inline void setMotionCloudFileUuid(const XsString& uuid)
	{
		XmeControl_THROW();
		XmeControl_setMotionCloudFileUuid(this, &uuid);
	}

	inline static XsSyncSettingArray supportedSyncSettings(const XsDeviceId& deviceId)
	{
		XsSyncSettingArray returnValue;
		return *XmeControl_supportedSyncSettings_1(&returnValue, &deviceId);
	}

	inline static bool supportsSyncSettings(const XsDeviceId& deviceId)
	{
		return 0 != XmeControl_supportsSyncSettings(&deviceId);
	}

	inline static bool isCompatibleSyncSetting(const XsDeviceId& deviceId, const XsSyncSetting& setting1, const XsSyncSetting& setting2)
	{
		return 0 != XmeControl_isCompatibleSyncSetting(&deviceId, &setting1, &setting2);
	}

	inline static unsigned int syncSettingsTimeResolutionInMicroSeconds(const XsDeviceId& deviceId)
	{
		return XmeControl_syncSettingsTimeResolutionInMicroSeconds(&deviceId);
	}

	inline void setFingerTrackingData(const XsQuaternionArray& fingerTrackingData, XsHandId hand, bool synchronize = false, const XsTimeStamp& ts = XsTimeStamp())
	{
		XmeControl_THROW();
		XmeControl_setFingerTrackingData(this, &fingerTrackingData, hand, synchronize, &ts);
	}

	inline bool hasFingerTrackingData(void) const
	{
		XmeControl_THROW();
		return 0 != XmeControl_hasFingerTrackingData(this);
	}

	inline void setExternalGloveIds(int64_t leftGloveId, int64_t rightGloveId)
	{
		XmeControl_THROW();
		XmeControl_setExternalGloveIds(this, leftGloveId, rightGloveId);
	}

	inline void getExternalGloveIds(int64_t& leftGloveId, int64_t& rightGloveId) const
	{
		XmeControl_THROW();
		XmeControl_getExternalGloveIds(this, &leftGloveId, &rightGloveId);
	}

	inline int64_t utcOffset(void) const
	{
		XmeControl_THROW();
		return XmeControl_utcOffset(this);
	}

	inline static void copyTimeSyncInfo(void* tsync)
	{
		XmeControl_copyTimeSyncInfo(tsync);
	}

	inline static int64_t currentUtcOffset(void)
	{
		return XmeControl_currentUtcOffset();
	}

	//! \brief Destructor, calls destruct() function to clean up object
	~XmeControl()
	{
		XmeControl_destruct(this);
	}

	//! \brief overloaded delete operator to allow user to use delete instead of calling destruct() function
	void operator delete (void*)
	{
	}

private:
	XmeControl(); // Default constructor not implemented to prevent faulty memory allocation, use construct function instead
#ifndef SWIG
	void* operator new (size_t); //!< \brief new operator not implemented to prevent faulty memory allocation by user, use construct() function instead
	void* operator new[] (size_t); //!< \brief array new operator not implemented to prevent faulty memory allocation by user, use construct() function instead
	void operator delete[] (void*); //!< \brief array delete operator not implemented to prevent faulty memory deallocation by user, use destruct() function instead
#endif
};
#endif // __cplusplus
#endif // GUARD_XMECONTROL_H
