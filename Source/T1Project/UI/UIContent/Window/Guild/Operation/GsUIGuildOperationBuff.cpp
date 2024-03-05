// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildOperationBuff.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UILib/Base/GsUITooltip.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoCurrency.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildBuffListEntry.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"
#include "DataSchema/Guild/GsSchemaGuildBuilding.h"
#include "DataSchema/Guild/GsSchemaGuildBuildingLevelData.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Guild/GsGuildHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Net/GsNetSendServiceGuild.h"


void UGsUIGuildOperationBuff::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIGuildOperationBuff::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIGuildOperationBuff::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelScrollRoot);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIGuildOperationBuff::OnRefreshEntry);

	_toggleGroupBuffList.OnSelectChanged.BindUObject(this, &UGsUIGuildOperationBuff::OnClickBuffList);

	_btnActivateBuff->OnClickCurrencyButton.BindUObject(this, &UGsUIGuildOperationBuff::OnClickActivateBuff);
	_btnInfo->OnPressed.AddDynamic(this, &UGsUIGuildOperationBuff::OnPressInfo);
}

void UGsUIGuildOperationBuff::NativeConstruct()
{
	Super::NativeConstruct();

	// TEXT: 버프 적용
	FText textButtonName;
	FText::FindText(TEXT("GuildText"), TEXT("UI_Button_BuffStart"), textButtonName);
	_btnActivateBuff->SetButtonName(textButtonName);

	_tooltipInfo->Close();
}

void UGsUIGuildOperationBuff::NativeDestruct()
{
	_bIsAuthority = false;
	_dataList.Empty();
	_buffState = EGsGuildBuffStateType::None;
	_bIsTimer = false;
	_checkRemainSec = 0.f;
	_endTimeSec = 0;
	_popupActiveBuff = nullptr;
	_maxDurationSec = 0;

	Super::NativeDestruct();
}

void UGsUIGuildOperationBuff::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_bIsTimer)
	{
		_checkRemainSec -= InDeltaTime;
		if (0 >= _checkRemainSec)
		{
			SetUIRemainTime();
			_checkRemainSec = 1.f;
		}
	}
}

void UGsUIGuildOperationBuff::Show(bool bIsMyGuild, bool IsStackRecoverFlag)
{
	// 권한 체크
	_bIsAuthority = false;
	if (bIsMyGuild)
	{
		_bIsAuthority = FGsGuildHelper::CanManageBuilding(GGuild()->GetAuthority());
	}

	// 버프 리스트. 가장 처음 것을 선택
	SetUIBuffList(0);

	// 스크롤 위치 최상으로
	_scrollBox->ScrollToStart();

	// 기사단 운영비
	SetUIGuildAsset();

	// 버프 설명글 세팅
	FText textDesc;
	const FGsSchemaGuildConfig* configTable = FGsGuildHelper::GetGuildConfig();
	if (configTable)
	{
		// TEXT: 일시적으로 ... 다음 활성화까지 {0}이/가 필요합니다.
		FText textFormat;
		if (FText::FindText(TEXT("GuildText"), TEXT("UI_Buff_Text"), textFormat))
		{
			FString timeString;
			FTimespan timespan = FTimespan::FromMinutes(configTable->guildBuffReactivateMin);
			FGsTimeStringHelper::GetTimeString(timespan, timeString);
			
			textDesc = FText::Format(textFormat, FText::FromString(timeString));
		}
	}
	_textBlockDesc->SetText(textDesc);

	_tooltipInfo->Close();
}

void UGsUIGuildOperationBuff::Hide()
{
	if (FGsGuildManager* guildMgr = GGuild())
	{
		guildMgr->GetRedDotCheckData().UpdateBuffCheckTime();
		guildMgr->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Buffs);
	}
}

void UGsUIGuildOperationBuff::OnChangeMyAuthority()
{
	_bIsAuthority = FGsGuildHelper::CanManageBuilding(GGuild()->GetAuthority());

	// 내 권한이 바뀌었다 -> 버프 적용 버튼 사용가능 체크
	SetUIBuffDetail(GetSelectedBuildingTableData());

	// 버프 활성 권한을 잃었을 경우 창닫기
	if (false == _bIsAuthority)
	{
		// 창이 열려있으면 닫기
		//if (GUI()->IsActiveWidget(TEXT("PopupYesNoCurrency")))
		{
			if (_popupActiveBuff.IsValid())
			{
				_popupActiveBuff->Close();
				_popupActiveBuff = nullptr;
			}
		}
	}
}

void UGsUIGuildOperationBuff::OnChangeCurrency(CurrencyType InType)
{
	if (CurrencyType::GUILD_ASSET == InType)
	{
		SetUIGuildAsset();

		// 버튼도 재화 체크
		if (_btnActivateBuff->IsVisible())
		{
			_btnActivateBuff->UpdateAmountTextColorLocalPlayer();
		}
	}
}

void UGsUIGuildOperationBuff::OnReceiveBuildingList()
{
	// 버프 리스트. 가장 처음 것을 선택
	SetUIBuffList(0);
}

void UGsUIGuildOperationBuff::OnUpdateBuilding(GuildBuildingId InBuildingId)
{
	// 해당 빌딩 상태가 바뀌었다. 전체 갱신
	SetUIBuffList(0);

	// 활성창 열고 있다면 바로 닫기
	//if (GUI()->IsActiveWidget(TEXT("PopupYesNoCurrency")))
	{
		if (_popupActiveBuff.IsValid())
		{
			_popupActiveBuff->Close();
			_popupActiveBuff = nullptr;
		}
	}
}

void UGsUIGuildOperationBuff::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGuildBuffListEntry* entry = Cast<UGsUIGuildBuffListEntry>(InEntry))
	{
		entry->SetData(_dataList[InIndex]);
	}
}

void UGsUIGuildOperationBuff::OnPressInfo()
{
	if (_tooltipInfo->IsOpen())
	{
		_tooltipInfo->Close();
	}
	else
	{
		_tooltipInfo->Open();
	}
}

void UGsUIGuildOperationBuff::OnClickBuffList(int32 InIndex)
{
	if (_dataList.IsValidIndex(InIndex))
	{
		SetUIBuffDetail(_dataList[InIndex]);
	}
	else
	{
		SetUIBuffDetail(nullptr);
	}
}

void UGsUIGuildOperationBuff::OnClickActivateBuff()
{
	FGsGuildManager* guildMgr = GGuild();

	// 권한 검사
	if (false == FGsGuildHelper::CanManageBuilding(guildMgr->GetAuthority()))
	{
		return;
	}

	const FGsSchemaGuildBuilding* tableData = GetSelectedBuildingTableData();
	if (nullptr == tableData)
	{
		return;
	}

	int32 buildingLevel = 0;
	if (const FGsGuildBuildingData* buildingData = guildMgr->GetBuildingData(tableData->id))
	{
		buildingLevel = buildingData->GetCurrBuildingLevel();
	}	
	
	const FGsSchemaGuildBuildingLevelData* levelData = FGsGuildHelper::GetBuildingLevelData(tableData, buildingLevel);
	if (nullptr == levelData)
	{
		return;
	}

	// 재화체크
	if (CurrencyType::NONE == levelData->buffActiveCostType)
	{
		return;
	}

	if (false == FGsCurrencyHelper::CheckCurrency(levelData->buffActiveCostType, levelData->buffActiveCost))
	{
		return;
	}

	GuildBuildingId buildingId = tableData->id;

	// 팝업열기
	_popupActiveBuff = GUI()->OpenAndGetWidget(TEXT("PopupYesNoCurrency"));
	if (_popupActiveBuff.IsValid())
	{
		UGsUIPopupYesNoCurrency* popup = Cast<UGsUIPopupYesNoCurrency>(_popupActiveBuff.Get());
		if (popup)
		{
			FText textContent;
			// TEXT: <pl2>Lv.{0} {1}</> 기사단 버프를 적용하시겠습니까?
			FText textFormat;
			if (FText::FindText(TEXT("GuildText"), TEXT("UI_BuffStart_Text"), textFormat))
			{
				textContent = FText::Format(textFormat, buildingLevel, tableData->nameText);
			}
			// TEXT:: 적용하기
			FText textYesButton;
			FText::FindText(TEXT("GuildText"), TEXT("UI_Button_Apply"), textYesButton);

			popup->SetCurrencyData(levelData->buffActiveCostType, levelData->buffActiveCost);
			popup->SetData(textContent, textYesButton, [buildingId](bool bIsOk)
				{
					if (bIsOk)
					{
						// 권한 검사
						if (FGsGuildHelper::CanManageBuilding(GGuild()->GetAuthority()))
						{
							FGsNetSendServiceGuild::SendReqGuildBuildingPassivity(buildingId);
						}
					}
				});
		}
	}
}

void UGsUIGuildOperationBuff::SetUIGuildAsset()
{
	FText textAmount = FText::AsNumber(FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::GUILD_ASSET));
	_textBlockGuildAsset->SetText(textAmount);
}

void UGsUIGuildOperationBuff::SetUIBuffList(int32 InIndex)
{
	_dataList.Empty();
	
	// 일단 존재하는 건물부터 담는다
	GuildBuildingId activeBuildingId = INVALID_GUILD_BUILDING_ID;
	const TArray<FGsGuildBuildingData> buildingList = GGuild()->GetBuildingList();
	for (const FGsGuildBuildingData& building : buildingList)
	{
		if (building._tableData &&
			GuildStatType::GUILD_BUFF == building._tableData->guildStatType)
		{
			_dataList.Emplace(building._tableData);
		}

		if (building.IsBuffActive())
		{
			activeBuildingId = building._id;
		}
	}

	// 활성화 버프가 있으면 제일 앞으로 정렬, 2순위는 순서대로
	_dataList.Sort([activeBuildingId](const FGsSchemaGuildBuilding& A, const FGsSchemaGuildBuilding& B)
		{
			// 1순위 활성화 중
			if (A.id == activeBuildingId && B.id != activeBuildingId)
			{
				return true;
			}
			else if (A.id != activeBuildingId && B.id == activeBuildingId)
			{
				return false;
			}
			else
			{
				// 2순위 건물 id 작은 순
				if (A.id < B.id)
				{
					return true;
				}
			}

			return false;
		});

	// 나머지 건물 담기
	TArray<const FGsSchemaGuildBuilding*> tempList;
	if (const UGsTable* table = FGsSchemaGuildBuilding::GetStaticTable())
	{
		table->GetAllRows(tempList);
	}

	for (const FGsSchemaGuildBuilding* iter : tempList)
	{
		if (GuildStatType::GUILD_BUFF == iter->guildStatType)
		{
			if (false == IsDataExist(iter->id))
			{
				_dataList.Emplace(iter);
			}
		}
	}
	
	// 버프 리스트 세팅
	_slotHelper->RefreshAll(_dataList.Num());

	_toggleGroupBuffList.Clear();
	_toggleGroupBuffList.AddToggleWidgetByParentPanel(_panelScrollRoot);
	_toggleGroupBuffList.SetSelectedIndex(InIndex, true); // 강제로 1회 호출되게 함
}

void UGsUIGuildOperationBuff::SetUIBuffDetail(const FGsSchemaGuildBuilding* InData)
{
	if (nullptr == InData)
	{
		// 화면을 가림
		_panelDetailInfoRoot->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	_panelDetailInfoRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 이름
	_textBlockTitle->SetText(InData->nameText);

	FGsGuildManager* guildMgr = GGuild();

	// 버프 지속시간 표현
	int32 buildingLevel = 0;
	const FGsGuildBuildingData* buildingData = guildMgr->GetBuildingData(InData->id);
	if (buildingData)
	{
		_buffDurationSec = buildingData->GetBuffDuration();
		buildingLevel = buildingData->GetCurrBuildingLevel();
	}
	
	FString strBuffDuration;
	FGsTimeStringHelper::GetTimeString(FTimespan::FromSeconds(_buffDurationSec), strBuffDuration);
	_textBlockBuffTime->SetText(FText::FromString(strBuffDuration));

	// 상태에 따라 바뀌는 UI들 세팅
	const FGsSchemaGuildBuildingLevelData* levelData = FGsGuildHelper::GetBuildingLevelData(InData, buildingLevel);
	SetUIBuffState(buildingData, levelData);
}

void UGsUIGuildOperationBuff::SetUIBuffState(const FGsGuildBuildingData* InData,
	const FGsSchemaGuildBuildingLevelData* InLevelData)
{
	FGsGuildManager* guildMgr = GGuild();
	bool bIsExistActiveBuff = guildMgr->IsExistActiveBuff(); // 활성화 중인 버프가 있는가??
	time_t reactiveTime = guildMgr->GetBuffReactiveTime();

	_buffState = EGsGuildBuffStateType::None;

	// 건설된 건물인가?
	if (InData &&
		InData->IsBuildingOpened())
	{
		// 현재 활성화 중인 버프인가?
		if (InData->IsBuffActive())
		{
			_buffState = EGsGuildBuffStateType::Active;
		}
		else
		{
			// 버프가 활성화 중이 아닐때만 재사용 대기시간 표시
			if (false == bIsExistActiveBuff)
			{
				time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
				if (reactiveTime > serverNowSec)
				{
					// 재사용 대기 시간이 지나지 않음
					_buffState = EGsGuildBuffStateType::WaitReactivate;
				}
			}
		}
	}
	else
	{
		_buffState = EGsGuildBuffStateType::NotOpened;
	}

	// 재사용 대기시간 중엔 보이지 않게 함
	_panelTitleRoot->SetVisibility((EGsGuildBuffStateType::WaitReactivate == _buffState) ?
		ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	// 활성 중에만 이미지에 불 들어옴
	_switcherImageState->SetActiveWidgetIndex((EGsGuildBuffStateType::Active == _buffState) ? 1 : 0);


	// 버튼 및 타이머 설정
	switch (_buffState)
	{
	case EGsGuildBuffStateType::NotOpened:
	{
		_switcherButtonState->SetActiveWidgetIndex(EGsGuildBuffStateType::None);
		_bIsTimer = false;
		_btnActivateBuff->SetVisibility(ESlateVisibility::Hidden);
	}
	break;
	case EGsGuildBuffStateType::None:
	{
		_switcherButtonState->SetActiveWidgetIndex(EGsGuildBuffStateType::None);
		_bIsTimer = false;

		bool bButtonVisible = false;
		if (InData && InLevelData)
		{
			_btnActivateBuff->SetData(InLevelData->buffActiveCostType, InLevelData->buffActiveCost);
			_btnActivateBuff->UpdateAmountTextColorLocalPlayer();

			// 권한이 있거나, 활성화 중인 버프가 없을때만 보여줌
			if (_bIsAuthority &&
				false == bIsExistActiveBuff)
			{
				bButtonVisible = true;
			}
		}
		_btnActivateBuff->SetVisibility(bButtonVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	break;
	case EGsGuildBuffStateType::Active:
	{
		_switcherButtonState->SetActiveWidgetIndex(EGsGuildBuffStateType::Active);
		if (InData)
		{
			_endTimeSec = InData->_passivityList[0]._buffEndTime;
			_bIsTimer = true;
			_checkRemainSec = 1.f;
			_maxDurationSec = _buffDurationSec;
		}		
	}
	break;
	case EGsGuildBuffStateType::WaitReactivate:
	{
		_switcherButtonState->SetActiveWidgetIndex(EGsGuildBuffStateType::WaitReactivate);
		_endTimeSec = reactiveTime;		
		_bIsTimer = true;
		_checkRemainSec = 1.f;

		if (const FGsSchemaGuildConfig* config = FGsGuildHelper::GetGuildConfig())
		{
			// 분이므로 초로 변환을 위해 60 곱해줌
			_maxDurationSec = config->guildBuffReactivateMin * 60;
		}
		else
		{
			_maxDurationSec = reactiveTime;
		}
	}
	break;	
	}

	// 첫 시간 갱신
	if (_bIsTimer)
	{
		SetUIRemainTime();
	}
}

void UGsUIGuildOperationBuff::SetUIRemainTime()
{
	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
	time_t remainSec = 0;
	if (serverNowSec < _endTimeSec)
	{
		remainSec = _endTimeSec - serverNowSec;
	}	

	if (0 == remainSec)
	{
		_bIsTimer = false;
		_checkRemainSec = 0.f;
	}

	// 버프 지속시간보다 남은시간이 클 수 없음
	time_t durationSec = static_cast<time_t>(_maxDurationSec);
	if (durationSec < remainSec)
	{
		remainSec = durationSec;
	}

	FString strRemainTime;
	FGsTimeStringHelper::GetTimeStringCeil(FTimespan::FromSeconds(remainSec), strRemainTime);

	if (EGsGuildBuffStateType::Active == _buffState)
	{	
		_textBlockBuffRemainTime->SetText(FText::FromString(strRemainTime));
	}
	else if (EGsGuildBuffStateType::WaitReactivate == _buffState)
	{
		_textBlockReactivateRemainTime->SetText(FText::FromString(strRemainTime));
	}

	if (false == _bIsTimer)
	{
		// 화면 갱신
		SetUIBuffDetail(GetSelectedBuildingTableData());
	}
}

const FGsSchemaGuildBuilding* UGsUIGuildOperationBuff::GetSelectedBuildingTableData() const
{
	int32 selectedIndex = _toggleGroupBuffList.GetSelectedIndex();
	if (_dataList.IsValidIndex(selectedIndex))
	{
		return _dataList[selectedIndex];
	}
	
	return nullptr;
}

bool UGsUIGuildOperationBuff::IsDataExist(GuildBuildingId InBuildingId)
{
	for (const FGsSchemaGuildBuilding* iter : _dataList)
	{
		if (InBuildingId == iter->id)
		{
			return true;
		}
	}

	return false;
}
