//#ifdef R3_ARCAS_NOT_USE
//#include "GsUIPopupExpBuff.h"
//#include "Management/ScopeGlobal/GsGameDataManager.h"
//#include "Management/ScopeGlobal/GsUIManager.h"
//#include "Management/ScopeGame/GsFairyManager.h"
//#include "Management/ScopeGame/GsGameObjectManager.h"
//#include "Management/GsMessageHolder.h"
//#include "Message/MessageParam/GsUIMessageParam.h"
//#include "Message/MessageParam/GsGameObjectMessageParam.h"
//#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
//#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
//
//#include "Net/GsNetSendServiceWorld.h"
//#include "Item/GsItemConsumable.h"
//#include "Item/GsItemManager.h"
//#include "Management/ScopeGlobal/GsOptionManager.h"
//
//#include "UI/UIControlLib/ContentWidget/GsButton.h"
//#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
//#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
//#include "UI/UIContent/Helper/GsUIHelper.h"
//#include "UI/UIContent/Helper/GsTimeStringHelper.h"
//
//#include "ProgressBar.h"
//#include "WidgetSwitcher.h"
//#include "TextBlock.h"
//
//void UGsUIPopupExpBuff::NativeOnInitialized()
//{
//	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupExpBuff::OnClosePopup);
//
//	Super::NativeOnInitialized();
//}
//
//void UGsUIPopupExpBuff::NativeConstruct()
//{
//	_msgUIHandle = GMessage()->GetUI().AddUObject(MessageUI::BLESS_ARCAS_POINT, this, &UGsUIPopupExpBuff::UpdateBlessInfo);
//	_msgObjectHandler = GMessage()->GetGameObject().AddUObject(MessageGameObject::ABNORMALITY_UPDATE, 
//		this, &UGsUIPopupExpBuff::UpdateAbnormalityInfo);
//
//	_btnUseFairyBless->SetIndexInGroup(0);
//	_btnUseFairyBless->SetToggleCallback([this](int32 index, bool isSelected) {
//		_btnUseFairyBless->SetIsSelected(isSelected);
//		OnUseFairyBless(isSelected);
//	});
//
//	_btnUseArcasBless->OnClicked.AddDynamic(this, &UGsUIPopupExpBuff::OnUseArcasBless);
//	_textMeditationTime->SetText(FText::GetEmpty());
//
//	FText::FindText(TEXT("ArcasText"), TEXT("Remain_Time_Meditation"), _meditationTimeFormat);
//	FText::FindText(TEXT("ArcasText"), TEXT("Remain_Time_Protection"), _protectionTimeFormat);
//
//	UpdateAbnormalityInfo(nullptr);
//
//	Super::NativeConstruct();
//}
//
//void UGsUIPopupExpBuff::NativeDestruct()
//{
//	GMessage()->GetUI().Remove(_msgUIHandle);
//	GMessage()->GetGameObject().Remove(_msgObjectHandler);
//
//	_btnUseFairyBless->SetToggleCallback(nullptr);
//	_btnUseArcasBless->OnClicked.RemoveDynamic(this, &UGsUIPopupExpBuff::OnUseArcasBless);
//
//	Super::NativeDestruct();
//}
//
//void UGsUIPopupExpBuff::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//	Super::NativeTick(MyGeometry, InDeltaTime);
//
//	if (0 < _checkRemainTime)
//	{
//		_checkRemainTime -= InDeltaTime;
//		if (0 >= _checkRemainTime)
//		{
//			UpdateRemainText();
//		}
//	}
//}
//
//void UGsUIPopupExpBuff::UpdateAbnormalityInfo(const IGsMessageParam* InParam)
//{
//	_endTime = 0.f;
//	_checkRemainTime = 0.f;
//
//	if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer)))
//	{
//		if (auto handler = player->GetAbnormalityHandler())
//		{
//			if (handler->IsAbnormalityEffectType(AbnormalityEffectSubType::UTIL_ARCAS_FAIRY_BUFF))
//			{
//				TArray<FGsAbnormalityData*> curAbnormalities;
//				handler->GetAbnormalityDataList(curAbnormalities, false);
//				if (auto iter = curAbnormalities.FindByPredicate([](FGsAbnormalityData* e) {
//					return e->GetEffectType() == AbnormalityEffectSubType::UTIL_ARCAS_FAIRY_BUFF; }))
//				{
//					_endTime = (*iter)->GetEndAppTime();
//					_checkRemainTime = 1;
//				}
//			}
//		}
//	}
//
//	UpdateBlessInfo(nullptr);
//}
//
//void UGsUIPopupExpBuff::UpdateBlessInfo(const IGsMessageParam* InParam)
//{
//	float maxArcasPoint = MAX_ARCAS_POINT;
//	const auto blessData = GGameData()->GetUserData()->GetBlessData();
//
//	float progress = float(blessData.mBlessPoint) / maxArcasPoint;
//	_blessProgress->SetPercent(progress);
//	_btnUseFairyBless->SetIsEnabled((blessData.mBlessPoint > 0 || _endTime > 0) && !blessData.mIsOnArcasBless && blessData.mMeditationTime == 0.f);
//	_btnUseFairyBless->SetIsSelected(blessData.mIsOnFairyBless);
//
//	_useProtectArcas->SetActiveWidgetIndex(_endTime > 0 ? 1 : 0);
//	_useArcasBless->SetActiveWidgetIndex(blessData.mIsOnArcasBless ? 1 : 0);
//	_btnUseArcasBless->SetIsEnabled(blessData.mBlessPoint > 0 && !blessData.mIsOnArcasBless && blessData.mMeditationTime == 0.f);
//
//	_textMeditationTime->SetText(FText::GetEmpty());
//	if (_endTime > 0 || blessData.mMeditationTime > 0)
//	{
//		UpdateRemainText();
//	}
//}
//
//void UGsUIPopupExpBuff::OnForcedClose()
//{
//	Close();
//}
//
//void UGsUIPopupExpBuff::UpdateRemainText()
//{
//	const auto blessData = GGameData()->GetUserData()->GetBlessData();
//	if (_endTime > 0)
//	{	
//		float remainTime = _endTime - FApp::GetCurrentTime();
//		if (0.f >= remainTime)
//		{
//			_checkRemainTime = 0.f;
//			_endTime = 0.f;
//			_textProtectionTime->SetText(FText::GetEmpty());
//			return;
//		}
//
//		_checkRemainTime = 1.f;
//		
//		FString strTime;
//		FGsTimeStringHelper::GetTimeString(FTimespan::FromSeconds(remainTime), strTime);
//		_textProtectionTime->SetText(FText::FromString(strTime));		
//
//		/*_textProtectionTime->SetText(FText::FromString(FString::Format(*(_protectionTimeFormat.ToString()),
//			{ protectRemainTime.GetHour(), protectRemainTime.GetMinute(), protectRemainTime.GetSecond() })));*/
//	}
//	else
//	{
//		_endTime = 0.f;
//		_checkRemainTime = 0.f;
//
//		_textProtectionTime->SetText(FText::GetEmpty());
//	}
//
//	if (blessData.mMeditationTime > 0)
//	{
//		FDateTime meditationRemainTime(blessData.mMeditationTime * ETimespan::TicksPerSecond);
//		meditationRemainTime -= (FDateTime::UtcNow() - blessData.mMeditaionPacketTime);
//
//		auto remainSpanTime = blessData.mMeditationTime - (FDateTime::UtcNow() - blessData.mMeditaionPacketTime).GetTotalSeconds();
//		FTimespan mediTimeSpan = FTimespan::FromSeconds(remainSpanTime);
//
//		FString timeString;
//		FGsTimeStringHelper::GetTimeString(mediTimeSpan, timeString);
//
//		_textMeditationTime->SetText(FText::FromString(timeString));
//
//		/*_textMeditationTime->SetText(FText::FromString(FString::Format(*(_meditationTimeFormat.ToString()),
//			{ meditationRemainTime.GetHour(), meditationRemainTime.GetMinute(), meditationRemainTime.GetSecond() })));*/
//	}
//}
//
//void UGsUIPopupExpBuff::OnUseFairyBless(bool select)
//{
//	const auto blessData = GGameData()->GetUserData()->GetBlessData();
//	if (!blessData.mIsOnArcasBless)
//	{
//		if (blessData.mIsOnFairyBless != select)
//		{
//			FGsNetSendServiceWorld::SendReqUpdateArcasBuff(!blessData.mIsOnFairyBless, blessData.mIsOnArcasBless);
//		}
//	}
//}
//
//void UGsUIPopupExpBuff::OnUseArcasBless()
//{
//	FText msg;
//	FText::FindText(TEXT("ArcasText"), TEXT("Arcas_Bless_Popup_Text"), msg);
//
//	FText yesText;
//	FText::FindText(TEXT("ArcasText"), TEXT("Arcas_Bless_Popup_Accept"), yesText);
//
//	FText noText;
//	FText::FindText(TEXT("ArcasText"), TEXT("Arcas_Bless_Popup_Refuse"), noText);
//
//	TWeakObjectPtr<UGsUIPopupYesNo> weak_popup = Cast<UGsUIPopupYesNo>(GUI()->OpenAndGetWidget(TEXT("PopupYesNo")));
//	if (auto popup = weak_popup.Get())
//	{
//		popup->SetData(msg, yesText, noText, [](bool yes) {
//			if (yes)
//			{
//				const auto blessData = GGameData()->GetUserData()->GetBlessData();
//				if(blessData.mBlessPoint > 0 && blessData.mMeditationTime == 0.f) //계속 열어놓고 있는 경우 패킷을 보내지 않도록
//					FGsNetSendServiceWorld::SendReqUpdateArcasBuff(blessData.mIsOnFairyBless, true);
//			}
//		});
//	}
//}
//
//void UGsUIPopupExpBuff::OnClosePopup()
//{
//	Close();
//}
//
//#endif