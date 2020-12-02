//----------------------------------------------------------------------------------------------------------------------
//	CSceneItemButton.cpp			©2012 Stevo Brock		All rights reserved.
//----------------------------------------------------------------------------------------------------------------------

#include "CSceneItemButton.h"

#include "CKeyframeAnimationInfo.h"

//----------------------------------------------------------------------------------------------------------------------
// MARK: Local data

static	CString	sActionsInfoKey(OSSTR("actionInfo"));
static	CString	sAudioInfoKey(OSSTR("audioInfo"));
static	CString	sHitRadiusKey(OSSTR("hitRadius"));
static	CString	sDisabledKeyframeAnimationInfoKey(OSSTR("disabledAnimationInfo"));
static	CString	sDownKeyframeAnimationInfoKey(OSSTR("downAnimationInfo"));
static	CString	sStartTimeIntervalKey(OSSTR("startTime"));
static	CString	sUpKeyframeAnimationInfoKey(OSSTR("upAnimationInfo"));

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// MARK: - CSceneItemButtonInternals

class CSceneItemButtonInternals : public TCopyOnWriteReferenceCountable<CSceneItemButtonInternals> {
	public:
		CSceneItemButtonInternals() : TCopyOnWriteReferenceCountable(), mHitRadius(0.0) {}
		CSceneItemButtonInternals(const CSceneItemButtonInternals& other) :
			TCopyOnWriteReferenceCountable(),
					mActions(other.mActions), mAudioInfo(other.mAudioInfo),
							mUpKeyframeAnimationInfo(other.mUpKeyframeAnimationInfo),
							mDownKeyframeAnimationInfo(other.mDownKeyframeAnimationInfo),
							mDisabledKeyframeAnimationInfo(other.mDisabledKeyframeAnimationInfo),
							mHitRadius(other.mHitRadius), mStartTimeInterval(other.mStartTimeInterval)
			{}

		OI<CActions>				mActions;
		OI<CAudioInfo>				mAudioInfo;
		OI<CKeyframeAnimationInfo>	mUpKeyframeAnimationInfo;
		OI<CKeyframeAnimationInfo>	mDownKeyframeAnimationInfo;
		OI<CKeyframeAnimationInfo>	mDisabledKeyframeAnimationInfo;
		Float32						mHitRadius;
		OV<UniversalTimeInterval>	mStartTimeInterval;
};

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
// MARK: - CSceneItemButton

// MARK: Properties

CString	CSceneItemButton::mType(OSSTR("button"));

// MARK: Lifecycle methods

//----------------------------------------------------------------------------------------------------------------------
CSceneItemButton::CSceneItemButton() : CSceneItem()
//----------------------------------------------------------------------------------------------------------------------
{
	mInternals = new CSceneItemButtonInternals();
}

//----------------------------------------------------------------------------------------------------------------------
CSceneItemButton::CSceneItemButton(const CDictionary& info) : CSceneItem(info)
//----------------------------------------------------------------------------------------------------------------------
{
	mInternals = new CSceneItemButtonInternals();

	if (info.contains(sActionsInfoKey))
		mInternals->mActions = OI<CActions>(CActions(info.getDictionary(sActionsInfoKey)));
	if (info.contains(sAudioInfoKey))
		mInternals->mAudioInfo = OI<CAudioInfo>(CAudioInfo(info.getDictionary(sAudioInfoKey)));
	mInternals->mUpKeyframeAnimationInfo =
			OI<CKeyframeAnimationInfo>(CKeyframeAnimationInfo(info.getDictionary(sUpKeyframeAnimationInfoKey)));
	if (info.contains(sDownKeyframeAnimationInfoKey))
		mInternals->mDownKeyframeAnimationInfo =
				OI<CKeyframeAnimationInfo>(CKeyframeAnimationInfo(info.getDictionary(sDownKeyframeAnimationInfoKey)));
	if (info.contains(sDisabledKeyframeAnimationInfoKey))
		mInternals->mDisabledKeyframeAnimationInfo =
				OI<CKeyframeAnimationInfo>(
						CKeyframeAnimationInfo(info.getDictionary(sDisabledKeyframeAnimationInfoKey)));
	if (info.contains(sStartTimeIntervalKey))
		mInternals->mStartTimeInterval = OV<UniversalTimeInterval>(info.getFloat64(sStartTimeIntervalKey));
	mInternals->mHitRadius = info.getFloat32(sHitRadiusKey);
}

//----------------------------------------------------------------------------------------------------------------------
CSceneItemButton::CSceneItemButton(const CSceneItemButton& other) : CSceneItem(other)
//----------------------------------------------------------------------------------------------------------------------
{
	mInternals = other.mInternals->addReference();
}

//----------------------------------------------------------------------------------------------------------------------
CSceneItemButton::~CSceneItemButton()
//----------------------------------------------------------------------------------------------------------------------
{
	mInternals->removeReference();
}

// MARK: CSceneItem methods

//----------------------------------------------------------------------------------------------------------------------
TArray<CDictionary> CSceneItemButton::getProperties() const
//----------------------------------------------------------------------------------------------------------------------
{
	// Setup
	TArray<CDictionary>	properties = CSceneItem::getProperties();

	// Add properties
	CDictionary	actionPropertyInfo;
	actionPropertyInfo.set(mPropertyTitleKey, CString(OSSTR("Actions")));
	actionPropertyInfo.set(mPropertyNameKey, sActionsInfoKey);
	actionPropertyInfo.set(mPropertyTypeKey, kSceneItemPropertyTypeActions);
	properties += actionPropertyInfo;

	CDictionary	audioInfoPropertyInfo;
	audioInfoPropertyInfo.set(mPropertyTitleKey, CString(OSSTR("Audio")));
	audioInfoPropertyInfo.set(mPropertyNameKey, sAudioInfoKey);
	audioInfoPropertyInfo.set(mPropertyTypeKey, kSceneItemPropertyTypeAudioInfo);
	properties += audioInfoPropertyInfo;

	CDictionary	upKeyframeAnimationInfoPropertyInfo;
	upKeyframeAnimationInfoPropertyInfo.set(mPropertyTitleKey, CString(OSSTR("Up Keyframe Animation")));
	upKeyframeAnimationInfoPropertyInfo.set(mPropertyNameKey, sUpKeyframeAnimationInfoKey);
	upKeyframeAnimationInfoPropertyInfo.set(mPropertyTypeKey, kSceneItemPropertyTypeKeyframeAnimationInfo);
	properties += upKeyframeAnimationInfoPropertyInfo;

	CDictionary	downKeyframeAnimationInfoPropertyInfo;
	downKeyframeAnimationInfoPropertyInfo.set(mPropertyTitleKey, CString(OSSTR("Down Keyframe Animation")));
	downKeyframeAnimationInfoPropertyInfo.set(mPropertyNameKey, sDownKeyframeAnimationInfoKey);
	downKeyframeAnimationInfoPropertyInfo.set(mPropertyTypeKey, kSceneItemPropertyTypeKeyframeAnimationInfo);
	properties += downKeyframeAnimationInfoPropertyInfo;

	CDictionary	disabledKeyframeAnimationInfoPropertyInfo;
	disabledKeyframeAnimationInfoPropertyInfo.set(mPropertyTitleKey, CString(OSSTR("Disabled Keyframe Animation")));
	disabledKeyframeAnimationInfoPropertyInfo.set(mPropertyNameKey, sDisabledKeyframeAnimationInfoKey);
	disabledKeyframeAnimationInfoPropertyInfo.set(mPropertyTypeKey, kSceneItemPropertyTypeKeyframeAnimationInfo);
	properties += disabledKeyframeAnimationInfoPropertyInfo;

	CDictionary	startTimeIntervalPropertyInfo;
	startTimeIntervalPropertyInfo.set(mPropertyTitleKey, CString(OSSTR("Start Time Interval")));
	startTimeIntervalPropertyInfo.set(mPropertyNameKey, sStartTimeIntervalKey);
	startTimeIntervalPropertyInfo.set(mPropertyTypeKey, kSceneItemPropertyTypeStartTimeInterval);
	properties += startTimeIntervalPropertyInfo;

	CDictionary	hitRadiusPropertyInfo;
	hitRadiusPropertyInfo.set(mPropertyTitleKey, CString(OSSTR("Hit Radius")));
	hitRadiusPropertyInfo.set(mPropertyNameKey, sHitRadiusKey);
	hitRadiusPropertyInfo.set(mPropertyTypeKey, kSceneItemPropertyTypePixelWidth);
	properties += hitRadiusPropertyInfo;

	return properties;
}

//----------------------------------------------------------------------------------------------------------------------
CDictionary CSceneItemButton::getInfo() const
//----------------------------------------------------------------------------------------------------------------------
{
	CDictionary	info = CSceneItem::getInfo();

	if (mInternals->mActions.hasInstance())
		info.set(sActionsInfoKey, mInternals->mActions->getInfo());
	if (mInternals->mAudioInfo.hasInstance())
		info.set(sAudioInfoKey, mInternals->mAudioInfo->getInfo());
	if (mInternals->mUpKeyframeAnimationInfo.hasInstance())
		info.set(sUpKeyframeAnimationInfoKey, mInternals->mUpKeyframeAnimationInfo->getInfo());
	if (mInternals->mDownKeyframeAnimationInfo.hasInstance())
		info.set(sDownKeyframeAnimationInfoKey, mInternals->mDownKeyframeAnimationInfo->getInfo());
	if (mInternals->mDisabledKeyframeAnimationInfo.hasInstance())
		info.set(sDisabledKeyframeAnimationInfoKey, mInternals->mDisabledKeyframeAnimationInfo->getInfo());
	if (mInternals->mStartTimeInterval.hasValue())
		info.set(sStartTimeIntervalKey, *mInternals->mStartTimeInterval);
	info.set(sHitRadiusKey, mInternals->mHitRadius);

	return info;
}

// MARK: Instance methods

//----------------------------------------------------------------------------------------------------------------------
const OI<CActions>& CSceneItemButton::getActions() const
//----------------------------------------------------------------------------------------------------------------------
{
	return mInternals->mActions;
}

//----------------------------------------------------------------------------------------------------------------------
void CSceneItemButton::setActions(const OI<CActions>& actions)
//----------------------------------------------------------------------------------------------------------------------
{
	// Prepare to write
	mInternals = mInternals->prepareForWrite();

	// Update
	mInternals->mActions = actions;
}

//----------------------------------------------------------------------------------------------------------------------
const OI<CAudioInfo>& CSceneItemButton::getAudioInfo() const
//----------------------------------------------------------------------------------------------------------------------
{
	return mInternals->mAudioInfo;
}

//----------------------------------------------------------------------------------------------------------------------
void CSceneItemButton::setAudioInfo(const OI<CAudioInfo>& audioInfo)
//----------------------------------------------------------------------------------------------------------------------
{
	// Prepare to write
	mInternals = mInternals->prepareForWrite();

	// Update
	mInternals->mAudioInfo = audioInfo;
}

//----------------------------------------------------------------------------------------------------------------------
const OI<CKeyframeAnimationInfo>& CSceneItemButton::getUpKeyframeAnimationInfo() const
//----------------------------------------------------------------------------------------------------------------------
{
	return mInternals->mUpKeyframeAnimationInfo;
}

//----------------------------------------------------------------------------------------------------------------------
void CSceneItemButton::setUpKeyframeAnimationInfo(const OI<CKeyframeAnimationInfo>& keyframeAnimationInfo)
//----------------------------------------------------------------------------------------------------------------------
{
	// Prepare to write
	mInternals = mInternals->prepareForWrite();

	// Update
	mInternals->mUpKeyframeAnimationInfo = keyframeAnimationInfo;
}

//----------------------------------------------------------------------------------------------------------------------
const OI<CKeyframeAnimationInfo>& CSceneItemButton::getDownKeyframeAnimationInfo() const
//----------------------------------------------------------------------------------------------------------------------
{
	return mInternals->mDownKeyframeAnimationInfo;
}

//----------------------------------------------------------------------------------------------------------------------
void CSceneItemButton::setDownKeyframeAnimationInfo(const OI<CKeyframeAnimationInfo>& keyframeAnimationInfo)
//----------------------------------------------------------------------------------------------------------------------
{
	// Prepare to write
	mInternals = mInternals->prepareForWrite();

	// Update
	mInternals->mDownKeyframeAnimationInfo = keyframeAnimationInfo;
}

//----------------------------------------------------------------------------------------------------------------------
const OI<CKeyframeAnimationInfo>& CSceneItemButton::getDisabledKeyframeAnimationInfo() const
//----------------------------------------------------------------------------------------------------------------------
{
	return mInternals->mDisabledKeyframeAnimationInfo;
}

//----------------------------------------------------------------------------------------------------------------------
void CSceneItemButton::setDisabledKeyframeAnimationInfo(const OI<CKeyframeAnimationInfo>& keyframeAnimationInfo)
//----------------------------------------------------------------------------------------------------------------------
{
	// Prepare to write
	mInternals = mInternals->prepareForWrite();

	// Update
	mInternals->mDisabledKeyframeAnimationInfo = keyframeAnimationInfo;
}

//----------------------------------------------------------------------------------------------------------------------
const OV<UniversalTimeInterval>& CSceneItemButton::getStartTimeInterval() const
//----------------------------------------------------------------------------------------------------------------------
{
	return mInternals->mStartTimeInterval;
}

//----------------------------------------------------------------------------------------------------------------------
void CSceneItemButton::setStartTimeInterval(const OV<UniversalTimeInterval>& startTimeInterval)
//----------------------------------------------------------------------------------------------------------------------
{
	// Prepare to write
	mInternals = mInternals->prepareForWrite();

	// Update
	mInternals->mStartTimeInterval = startTimeInterval;
}

//----------------------------------------------------------------------------------------------------------------------
Float32 CSceneItemButton::getHitRadius() const
//----------------------------------------------------------------------------------------------------------------------
{
	return mInternals->mHitRadius;
}

//----------------------------------------------------------------------------------------------------------------------
void CSceneItemButton::setHitRadius(Float32 hitRadius)
//----------------------------------------------------------------------------------------------------------------------
{
	// Prepare to write
	mInternals = mInternals->prepareForWrite();

	// Update
	mInternals->mHitRadius = hitRadius;
}
