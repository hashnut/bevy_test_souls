// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildOperationCamp.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Common/Entry/GsUISimpleTwoTextEntry.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Popup/GsUIPopupWarp.h"
#include "UI/UIContent/Popup/GsUIPopupGuildCampBuffInfo.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Net/GsNetSendServiceWorld.h"
#include "Guild/GsGuildHelper.h"
#include "Guild/GsGuildBuildingData.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/GameObject/Camp/GsSchemaCampData.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"
#include "Classes/Engine/Texture2D.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "ContentsEvent/GsContentsEventTeleportCost.h"
#include "Currency/GsCostOption.h"
#include "Sequence/GsSequencePlayer.h"


void UGsUIGuildOperationCamp::BeginDestroy()
{
	if (_slotHelperConstructionBuff)
	{
		_slotHelperConstructionBuff->OnRefreshEntry.RemoveDynamic(this, 
			&UGsUIGuildOperationCamp::OnRefreshConstructionBuff);
	}
	_slotHelperConstructionBuff = nullptr;

	if (_slotHelperAreaBuff)
	{
		_slotHelperAreaBuff->OnRefreshEntry.RemoveDynamic(this,
			&UGsUIGuildOperationCamp::OnRefreshAreaBuff);
	}
	_slotHelperAreaBuff = nullptr;

	Super::BeginDestroy();
}

void UGsUIGuildOperationCamp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTooltipCamp->OnClicked.AddDynamic(this, &UGsUIGuildOperationCamp::OnClickTooltipCamp);
	_btnRemoveCamp->OnClicked.AddDynamic(this, &UGsUIGuildOperationCamp::OnClickRemoveCamp);
	_btnTipConstructionBuff->OnClicked.AddDynamic(this, &UGsUIGuildOperationCamp::OnClickTipConstructionBuff);
	_btnTipAreaBuff->OnClicked.AddDynamic(this, &UGsUIGuildOperationCamp::OnClickAreaBuff);
	_btnWarp->OnClicked.AddDynamic(this, &UGsUIGuildOperationCamp::OnClickWarp);

	_slotHelperConstructionBuff = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperConstructionBuff->Initialize(_entryWidgetClass, _panelConstructionBuffList);
	_slotHelperConstructionBuff->OnRefreshEntry.AddDynamic(this,
		&UGsUIGuildOperationCamp::OnRefreshConstructionBuff);

	_slotHelperAreaBuff = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperAreaBuff->Initialize(_entryWidgetClass, _panelAreaBuffList);
	_slotHelperAreaBuff->OnRefreshEntry.AddDynamic(this,
		&UGsUIGuildOperationCamp::OnRefreshAreaBuff);
}

void UGsUIGuildOperationCamp::NativeConstruct()
{
	Super::NativeConstruct();

	// TEXT: 수호탑 설치 완료 후 기사단원 모두에게 바로 적용되는 버프 효과
	FText textConstructionBuff;
	FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_ConstructionBuff"), textConstructionBuff);
	_tooltipConstructionBuff->SetDescText(textConstructionBuff);

	// TEXT: 수호탑 설치 완료 후 수호탑 근처 제한된 활성화 영역에 적용되는 강화 효과
	FText textAreaBuff;
	FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_AreaBuff"), textAreaBuff);
	_tooltipAreaBuff->SetDescText(textAreaBuff);
}

void UGsUIGuildOperationCamp::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	IGsInvalidateUIInterface::CheckInvalidate();

	// n초마다 갱신
	if (0 < _remainCheckTime)
	{
		_remainCheckTime -= InDeltaTime;
		if (0 >= _remainCheckTime)
		{
			UpdateRemainTime();
		}
	}
}

void UGsUIGuildOperationCamp::InvalidateAllInternal()
{
	FGsGuildManager* guildMgr = GGuild();

	// 주의: 추후 캠프가 여러개가 될 경우 개선 필요
	const FGsGuildCampData* campData = guildMgr->GetCampData(0);
	if (nullptr == campData)
	{
		_switcherCamp->SetActiveWidgetIndex(0);
		return;
	}

	if (GuildCampStateType::COMPLETE != campData->_campState)
	{
		_switcherCamp->SetActiveWidgetIndex(0);
		return;
	}

	// R1: 캠프 한개만 설치 가능하게 함. (이후 복수개 가능하도록 변경 예정)
	_campId = campData->_campId;
	_switcherCamp->SetActiveWidgetIndex(1);

	UpdateRemoveCampButton();	

	// 버프 정보 표시
	_constructionBuffTextList.Empty();
	_areaBuffTextList.Empty();
	const FGsSchemaCampData* tableCamp =
		UGsTableUtil::FindRowById<UGsTableCampData, FGsSchemaCampData>(campData->_campId);
	if (tableCamp)
	{
		TWeakObjectPtr<UGsUIGuildOperationCamp> weakThis(this);
		FSoftObjectPath path = tableCamp->imagePath;
		FGsUIHelper::AsyncLoadTextureFromPath(path, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
			{
				if (UGsUIGuildOperationCamp* StrongThis = weakThis.Get())
				{
					// 로드 끝난 후라 새로 로드하지 않고, 로드된 항목을 찾아옴
					UTexture2D* loadedTexture = Cast<UTexture2D>(inRes);
					if (loadedTexture)
					{
						StrongThis->SetCampImage(loadedTexture);
					}
				}
			}));

		FString strTitle = FString::Format(TEXT("{0} Lv.{1}"), { tableCamp->nameText.ToString(), campData->_level });
		_tbCampName->SetText(FText::FromString(strTitle));

		if (FGsEffectTextManager* effectTextMgr = GSEffectText())
		{
			int32 effectTextOption = 0;
			FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::SKIP_STAT_TIME, effectTextOption);

			int32 levelIndex = (0 < campData->_level) ? campData->_level - 1 : 0;
			if (tableCamp->constructionBuffList.IsValidIndex(levelIndex))
			{
				effectTextMgr->GetEffectTextListPassivity(tableCamp->constructionBuffList[levelIndex], 
					_constructionBuffTextList, effectTextOption);
			}

			if (tableCamp->areaBuffList.IsValidIndex(levelIndex))
			{
				// 레벨 1 고정. 레벨별 abnormality id가 전부 다름
				effectTextMgr->GetEffectTextListAbnormality(tableCamp->areaBuffList[levelIndex], _areaBuffTextList,
					1, effectTextOption);
				//effectTextMgr->GetEffectTextListAbnormality(tableCamp->areaBuffList[levelIndex], _areaBuffTextList, 
				//	campData->_level, effectTextOption);
			}
		}
	}

	_slotHelperConstructionBuff->RefreshAll(_constructionBuffTextList.Num());
	_slotHelperAreaBuff->RefreshAll(_areaBuffTextList.Num());

	// 캠프 종료시간 설정
	_endTimeSec = campData->_endTime;

	UpdateRemainTime();

	// 맵정보 저장
	_mapId = campData->_mapId;
	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(_mapId);
	if (mapData)
	{
		_tbMapName->SetText(mapData->MapStringId);
		_btnWarp->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_tbMapName->SetText(FText::GetEmpty());
		_btnWarp->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIGuildOperationCamp::Show(bool bIsMyGuild, bool IsStackRecoverFlag)
{
	InvalidateAll();
}

void UGsUIGuildOperationCamp::OnChangeGuildLevel()
{
	// 캠프가 이제 길드 건물과 관련이 없고, 레벨 변경 시 캠프 레벨도 엮여있으므로 체크
	InvalidateAll();
}

void UGsUIGuildOperationCamp::OnChangeMyAuthority()
{
	// 수호탑 해제 팝업 열려있을 경우 강제 닫기
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("PopupYesNo"));
	if (widget.IsValid())
	{
		widget->Close();
	}

	// 수호탑 해제 버튼 갱신
	UpdateRemoveCampButton();
}

void UGsUIGuildOperationCamp::OnCampDestruction()
{
	// 수호탑 해제 팝업 열려있을 경우 강제 닫기
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("PopupYesNo"));
	if (widget.IsValid())
	{
		widget->Close();
	}

	// 워프 팝업 열려있을 경우 강제 닫기
	TWeakObjectPtr<UGsUIWidgetBase> widgetWarp = GUI()->GetWidgetByKey(TEXT("PopupWarp"));
	if (widgetWarp.IsValid())
	{
		widgetWarp->Close();
	}

	InvalidateAll();
}

void UGsUIGuildOperationCamp::SetCampImage(class UTexture2D* InTexture)
{
	_imgCamp->SetBrushFromSoftTexture(InTexture);
}

void UGsUIGuildOperationCamp::UpdateRemainTime()
{
	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
	time_t remainTimeSec = 0;
	if (serverNowSec < _endTimeSec)
	{
		remainTimeSec = _endTimeSec - serverNowSec;
	}

	FString strTime;
	if (0 == remainTimeSec)
	{
		FGsTimeStringHelper::GetTimeStringHMS(FTimespan::FromSeconds(remainTimeSec), strTime, false);

		_remainCheckTime = 0.f;
	}
	else
	{
		FGsTimeStringHelper::GetTimeStringHMS(FTimespan::FromSeconds(remainTimeSec), strTime, true);

		// 다시 흘러가게 하기
		_remainCheckTime = 1.f;
	}	

	_tbRemainTime->SetText(FText::FromString(strTime));
}

void UGsUIGuildOperationCamp::UpdateRemoveCampButton()
{
	// 권한 있는 사람만 누를 수 있음 누르고 메시지 안내를 해야함? 다른 곳에선 그렇게 안하는데... 논의 필요
	_btnRemoveCamp->SetIsEnabled(FGsGuildHelper::CanManageCamp(GGuild()->GetAuthority()));
}

void UGsUIGuildOperationCamp::OnClickTooltipCamp()
{
	int32 tabIndex = 0;
	if (const FGsSchemaCampData* tableCamp =
		UGsTableUtil::FindRowById<UGsTableCampData, FGsSchemaCampData>(_campId))
	{
		tabIndex = tableCamp->infoUITabIndex;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupGuildCampBuffInfo"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildCampBuffInfo* popup = Cast<UGsUIPopupGuildCampBuffInfo>(widget.Get()))
		{
			uint8 campLevel = 0;
			if (const FGsGuildDataMine* myGuildData = GGuild()->GetMyGuildData())
			{
				campLevel = myGuildData->_campLevel;
			}

			popup->SetData(campLevel, tabIndex);
		}
	}
}

void UGsUIGuildOperationCamp::OnClickRemoveCamp()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNo"));
	if (false == widget.IsValid())
	{
		return;
	}

	// 디자인 확인해볼 것
	UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
	if (nullptr == popup)
	{
		return;
	}

	CampId campId = _campId;

	// TEXT: 수호탑을 해체하시겠습니까? 주의! 해체 시 적용 중인 수호탑의 효과는 즉시 제거됩니다.
	FText textDesc;
	FText::FindText(TEXT("GuildText"), TEXT("UI_Camp_Desc_Destruction_Popup"), textDesc);	
	popup->SetData(textDesc, [campId](bool bIsOk)
		{
			if (bIsOk)
			{
				FGsNetSendServiceWorld::SendCancelCamp(campId);
			}
		});
}

void UGsUIGuildOperationCamp::OnClickTipConstructionBuff()
{
	_tooltipConstructionBuff->ToggleOpenClose();
}

void UGsUIGuildOperationCamp::OnClickAreaBuff()
{
	_tooltipAreaBuff->ToggleOpenClose();
}

void UGsUIGuildOperationCamp::OnClickWarp()
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupWarp"));
	if (false == widget.IsValid())
	{
		return;
	}
	
	UGsUIPopupWarp* popup = Cast<UGsUIPopupWarp>(widget.Get());
	if (nullptr == popup)
	{
		return;
	}

	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(_mapId);
	if (nullptr == mapData)
	{
		return;
	}

	const FGsSchemaMapBalance* mapBalanceData = mapData->balanceId.GetRow();
	if (nullptr == mapBalanceData)
	{
		return;
	}

	// 시네마틱 플레이 중이면 불가
	if (UGsSequencePlayer::IsSequencePlaying())
	{
		FText findText;
		FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	CampId campId = _campId;
	TSharedRef<FGsCostPackage> CostPackage(FGsCostPackage::CreateSimple(mapBalanceData->cost.costCurrencyType, mapBalanceData->cost.costCurrencyAmount));

	TSharedPtr<FGsCostOption> CostOption = FGsContentsEventTeleportCost::CreateActivatedTeleportCostOption();
	if (CostOption.IsValid())
	{
		// 텔레포트 비용 할인 객체가 있을 경우, 추가
		CostPackage->AddCostOption(CostOption.ToSharedRef());
	}

	popup->SetData(mapData->MapStringId.ToString(), 
		MoveTemp(CostPackage),
		[campId]()
		{
			// 시네마틱 플레이 중이면 불가
			if (UGsSequencePlayer::IsSequencePlaying())
			{
				FText findText;
				FText::FindText(TEXT("NetText"), TEXT("WARP_ERROR_CINEMATIC_CANCLE"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return;
			}

			// 캠프 워프 패킷 보내기
			FGsNetSendServiceWorld::SendWarpGuildCamp(campId);
		});
}

void UGsUIGuildOperationCamp::OnRefreshConstructionBuff(int32 InIndex, UWidget* InEntry)
{
	if (_constructionBuffTextList.IsValidIndex(InIndex))
	{
		if (UGsUISimpleTwoTextEntry* entry = Cast<UGsUISimpleTwoTextEntry>(InEntry))
		{
			entry->SetTextFront(_constructionBuffTextList[InIndex].Key);
			entry->SetTextBack(_constructionBuffTextList[InIndex].Value);
		}
	}
}

void UGsUIGuildOperationCamp::OnRefreshAreaBuff(int32 InIndex, UWidget* InEntry)
{
	if (_areaBuffTextList.IsValidIndex(InIndex))
	{
		if (UGsUISimpleTwoTextEntry* entry = Cast<UGsUISimpleTwoTextEntry>(InEntry))
		{
			entry->SetTextFront(_areaBuffTextList[InIndex].Key);
			entry->SetTextBack(_areaBuffTextList[InIndex].Value);
		}
	}
}
