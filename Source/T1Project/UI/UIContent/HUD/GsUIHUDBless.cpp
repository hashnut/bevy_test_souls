//#ifdef R3_ARCAS_NOT_USE
//#include "GsUIHUDBless.h"
//#include "Shared/Shared/SharedInclude/SharedTypes.h"
//#include "Management/ScopeGlobal/GsUIManager.h"
//#include "Management/ScopeGlobal/GsGameDataManager.h"
//#include "Management/ScopeGame/GsGameObjectManager.h"
//#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
//#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
//#include "Management/GsMessageHolder.h"
//#include "Message/MessageParam/GsGameObjectMessageParam.h"
//
//#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
//#include "TextBlock.h"
//#include "WidgetSwitcher.h"
//#include "Components/Image.h"
//#include "Color.h"
//
//void UGsUIHUDBless::NativeOnInitialized()
//{
//	_btnBless->OnClicked.AddDynamic(this, &UGsUIHUDBless::OnClickBlesState);
//	_points = { _textPoint, _textPointDisable, _textPointFairy };
//
//	Super::NativeOnInitialized();
//}
//
//void UGsUIHUDBless::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	RegisterMessage();
//}
//
//void UGsUIHUDBless::NativeDestruct()
//{
//	UnRegisterMessage();
//
//	Super::NativeDestruct();
//}
//
//void UGsUIHUDBless::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(MyGeometry, InDeltaTime);
//
//	const auto& blessData = GGameData()->GetUserData()->GetBlessData();
//	if (blessData.mMeditationTime > 0.f && blessData.mMeditaionPacketTime.GetTicks() > 0)
//	{
//		_blessSwitcher->SetActiveWidgetIndex(_switchIndex[BlessHUDState::MEDITAION]);
//
//		if (blessData.mMeditaionPacketTime.GetTicks() > 0)
//		{
//			FDateTime meditaionTime(blessData.mMeditationTime * ETimespan::TicksPerSecond);
//			meditaionTime -= (FDateTime::UtcNow() - blessData.mMeditaionPacketTime);
//
//			_remainMeditation->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d:%02d"),
//				meditaionTime.GetHour(), meditaionTime.GetMinute(), meditaionTime.GetSecond())));
//
//			if (meditaionTime.GetTicks() < 0)
//			{
//				GGameData()->SetEndMeditation();
//				OnChangedBlessPoint(blessData.mBlessPoint);
//			}
//		}
//	}
//}
//
//void UGsUIHUDBless::InvalidateAllInternal()
//{
//	if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
//	{
//		if (auto handler = player->GetAbnormalityHandler())
//		{
//			_isProtection = handler->IsAbnormalityEffectType(AbnormalityEffectSubType::UTIL_ARCAS_FAIRY_BUFF);
//		}
//	}
//
//	const auto& blessData = GGameData()->GetUserData()->GetBlessData();
//	OnChangedBlessPoint(blessData.mBlessPoint);
//
//	Super::InvalidateAllInternal();
//}
//
//void UGsUIHUDBless::SetHUDMode(EGsUIHUDMode InMode)
//{
//	if (EGsUIHUDMode::CAMERA == InMode)
//	{
//		_btnBless->SetVisibility(ESlateVisibility::Collapsed);
//	}
//	else
//	{
//		_btnBless->SetVisibility(ESlateVisibility::Visible);
//	}
//}
//
//void UGsUIHUDBless::RegisterMessage()
//{
//	MGameObject& msgGameObject = GMessage()->GetGameObject();
//	_msgHandlerObject = msgGameObject.AddUObject(MessageGameObject::ABNORMALITY_UPDATE,
//		this, &UGsUIHUDBless::OnChangedProtection);
//}
//
//void UGsUIHUDBless::UnRegisterMessage()
//{
//	MGameObject& msgGameObject = GMessage()->GetGameObject();
//	msgGameObject.Remove(_msgHandlerObject);
//}
//
//void UGsUIHUDBless::OnChangedProtection(const IGsMessageParam* InParam)
//{
//	bool isProtection = _isProtection;
//
//	const FGsGameObjectMessageParamAbnormality* param = InParam->Cast<const FGsGameObjectMessageParamAbnormality>();
//	if (param && param->_paramOwner && EGsGameObjectType::LocalPlayer == param->_paramOwner->GetObjectType())
//	{
//		if (auto player = Cast<UGsGameObjectLocalPlayer>(param->_paramOwner))
//		{
//			if (auto handler = player->GetAbnormalityHandler())
//			{
//				isProtection = handler->IsAbnormalityEffectType(AbnormalityEffectSubType::UTIL_ARCAS_FAIRY_BUFF);
//			}
//		}
//	}
//
//	if (isProtection != _isProtection)
//	{
//		_isProtection = isProtection;
//
//		const auto& blessData = GGameData()->GetUserData()->GetBlessData();
//		OnChangedBlessPoint(blessData.mBlessPoint);
//	}
//}
//
//void UGsUIHUDBless::OnChangedBlessPoint(uint32 point)
//{	
//	for (auto textbox : _points)
//	{
//		textbox->SetText(FText::AsNumber(point));
//		textbox->SetVisibility(point > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
//	}
//
//	const auto blessData = GGameData()->GetUserData()->GetBlessData();
//
//	if (blessData.mMeditationTime > 0.f)
//	{
//		_blessSwitcher->SetActiveWidgetIndex(_switchIndex[BlessHUDState::MEDITAION]);
//
//		if (blessData.mMeditaionPacketTime.GetTicks() > 0)
//		{
//			FDateTime meditaionTime(blessData.mMeditationTime * ETimespan::TicksPerSecond);
//			meditaionTime -= (FDateTime::UtcNow() - blessData.mMeditaionPacketTime);
//
//			_remainMeditation->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d:%02d"),
//				meditaionTime.GetHour(), meditaionTime.GetMinute(), meditaionTime.GetSecond())));
//		}
//	}
//	else if (blessData.mBlessPoint > 0)
//	{	
//		_blessSwitcher->SetActiveWidgetIndex(blessData.mIsOnArcasBless ? _switchIndex[BlessHUDState::ARCASS_BLESS] : 
//			blessData.mIsOnFairyBless ? _switchIndex[BlessHUDState::FAIRY_BLESS] : _switchIndex[BlessHUDState::NONE]);
//	}
//	else
//	{
//		//protection 상태일때는 페어리 버프가 적용된다.
//		_blessSwitcher->SetActiveWidgetIndex(blessData.mIsOnFairyBless && _isProtection ?
//			_switchIndex[BlessHUDState::FAIRY_BLESS] : _switchIndex[BlessHUDState::NONE]);
//	}
//}
//
//void UGsUIHUDBless::OnClickBlesState()
//{
//	const auto& blessData = GGameData()->GetUserData()->GetBlessData();
//	if (blessData.mMeditaionPacketTime.GetTicks() > 0)
//	{
//		FDateTime meditaionTime(blessData.mMeditationTime * ETimespan::TicksPerSecond);
//		meditaionTime -= (FDateTime::UtcNow() - blessData.mMeditaionPacketTime);
//
//		if (meditaionTime.GetTicks() < 0)
//		{
//			GGameData()->SetEndMeditation();
//		}
//	}
//
//	GUI()->Open(TEXT("PopupEXPBuff"));
//}
//#endif