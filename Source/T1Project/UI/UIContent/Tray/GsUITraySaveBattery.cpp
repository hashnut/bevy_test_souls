// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITraySaveBattery.h"
#include "GameFlow/GameContents/ContentsGame/SaveBattery/GsSaveBatteryHandler.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsSaveBatteryManager.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsPKBookManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "Reward/GsRewardHelper.h"

#include "TimerManager.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemSimple.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/HUD/GsUIHUDPlayerInfo.h"
#include "UI/UIContent/HUD/GsUIHUDAbnormality.h"
#include "UI/UIContent/HUD/GsUIHUDSystemInfo.h"
#include "UI/UIContent/HUD/GsUIHUDConsumable.h"
#include "UI/UIContent/HUD/GsUIHUDSpiritShot.h"
#include "UI/UIContent/HUD/GsUIHUDCounterAttackButton.h"
#include "UI/UIContent/Tray/SaveBattery/GsUISaveBatteryMode.h"
#include "UI/UIContent/Tray/SaveBattery/GsUIInventorySaveBattery.h"
#include "UI/UIContent/Tray/GsUITrayTickerMessage.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Window/PKBook/GsUIPKTargetDetectionDataEntry.h"

#include "Data/GsDataContainManager.h"
#include "DataSchema/Map/GsSchemaMapData.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"

#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Stat/GsStatInfo.h"


void UGsUITraySaveBattery::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOffLinePlay->OnClicked.AddDynamic(this, &UGsUITraySaveBattery::OnClickOfflinePlay);
	_btnDetectionTarget->OnClicked.AddDynamic(this, &UGsUITraySaveBattery::OnClickDetectionTarget);

	// 수동 해제 길이
	FVector2D viewSize;
	GEngine->GameViewport->GetViewportSize(viewSize);

	_closeDistance = viewSize.X / 100.f * GData()->GetGlobalData()->_saveBatteryCloseDistanceRate;

	_slotHelperDetectionTargetData = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperDetectionTargetData->Initialize(_subClassOfDetectionTargetDataEntry, _scrollBoxDetectionTargetSet);
	_slotHelperDetectionTargetData->OnRefreshEntry.AddDynamic(this, &UGsUITraySaveBattery::OnRefreshEntryDetectionTarget);
}

void UGsUITraySaveBattery::NativeConstruct()
{
	Super::NativeConstruct();

	FText::FindText(TEXT("SaveBatteryText"), TEXT("SaveBattery_DungeonRemainingTime"), _remainTimeFormatText);

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->AddUIScaleFlag(EGsUIScaleFlags::UI_SCALE_SAVE_BATTERY);
	}

	FGsSaveBatteryManager* manager = GSSaveBattery();
	if (nullptr == manager)
		return;

	// 부활 등으로 지역이 바뀐 뒤에 절전 모드가 다시 열렸을 때
	// 기존 데이터를 그대로 사용한다.
	if (false == manager->IsReStart())
	{
		manager->ResetData();
		_animationState = AnimationType::Idle;
	}
	else
	{
		manager->SetReStart(false);
		_animationState = AnimationType::Dead;
	}

	// 캐릭터 상태 바
	_playerInfoUI->SetSaveBatteryMode();

	// 상태 이상 표시
	_abnormalityUI->SetSaveBatteryMode();

	// 귀환 주문서 초기화
	_returnScrollId = 0;
	if (UGsItemManager* itemManager = GItem())
	{
		if (const FGsSchemaItemCommon* returnScrollData = itemManager->FindItemSchemaDataBySubCategory(ItemCategorySub::WARP_TO_TOWN_SCROLL))
		{
			_returnScrollId = returnScrollData->id;
		}
	}

	InitializeReturnScroll();

	// 절전 모드 진행 시간 타이머 등록
	_startTime = manager->GetStartTime();
	GetWorld()->GetTimerManager().SetTimer(_timerHandler, this, &UGsUITraySaveBattery::UpdateTime, _updateSec, true);

	GetWorld()->GetTimerManager().SetTimer(_remainTimerHandler, this, &UGsUITraySaveBattery::UpdateRemainTime, _updateSec, true);
	GetWorld()->GetTimerManager().PauseTimer(_remainTimerHandler);

	_startPoint = FVector2D::ZeroVector;
	_endPoint = FVector2D::ZeroVector;
}

void UGsUITraySaveBattery::NativeDestruct()
{
	if (nullptr != _saveBatteryHandler)
	{
		_saveBatteryHandler->Exit();
	}

	// 귀환 주문서 제거
	_iconSelectorReturn->RemoveAllChildren();
	_iconReturn = nullptr;

	//타이머 해지
	GetWorld()->GetTimerManager().ClearTimer(_timerHandler);
	GetWorld()->GetTimerManager().ClearTimer(_remainTimerHandler);

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->RemoveUIScaleFlag(EGsUIScaleFlags::UI_SCALE_SAVE_BATTERY);
	}

	Super::NativeDestruct();
}

void UGsUITraySaveBattery::InvalidateAllInternal()
{
	if (nullptr == _iconReturn)
	{
		InitializeReturnScroll();
	}

	// 캐릭터 상태 바
	_playerInfoUI->InvalidateAll();

	// 상태 이상 표시	
	_abnormalityUI->InvalidateAll();

	// 현재 캐릭터 위치 표기
	InvalidateMapName();

	// 절전 모드 애니메이션 및 캐릭터 행동 표시
	InvalidateMode();

	// 시스템 정보 표시
	_systemInfoUI->InvalidateAll();

	// 물약, 천마석, 귀환 주문서
	_consumableUI->InvalidateAll();
	_spiritShotUI->InvalidateAll();

	InvalidateReturnScroll();

	// 타이머
	UpdateTime();
	UpdateRemainTime();

	// 획득 골드 및 경험치
	OnIncreaseGold(0);
	OnIncreasePlayerExp(0);

	// 획득 아이템
	_invenUI->InvalidateAll();

	// 인벤토리 무게 및 슬롯 정보
	InvalidateItemWeightRate();
	InvalidateInventoryCount();

	_counterAttackButton->UpdateVisible();

	// 오프라인 플레이 버튼
	InvalidateOfflinePlay();

	// 경계 대상 정보
	InvalidateDetectionTarget();
}

FReply UGsUITraySaveBattery::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITraySaveBattery::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (FVector2D::ZeroVector == _startPoint)
	{
		_startPoint = InGestureEvent.GetScreenSpacePosition();
	}

	return FReply::Handled();
}

FReply UGsUITraySaveBattery::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (nullptr != _saveBatteryHandler)
	{
		if (FVector2D::ZeroVector != _startPoint)
		{
			_endPoint = InGestureEvent.GetScreenSpacePosition();
			float distance = FVector2D::Distance(_startPoint, _endPoint);

			_startPoint = FVector2D::ZeroVector;
			_endPoint = FVector2D::ZeroVector;

			if (_closeDistance < distance)
			{
				_saveBatteryHandler->OnClose();
			}
		}
	}

	return FReply::Handled();
}

void UGsUITraySaveBattery::OnManagerTick(float InDeltaTime)
{
	// 보일 때만 틱이 돌도록
	if (false == IsVisible())
	{
		return;
	}

	IGsInvalidateUIInterface::CheckInvalidate();

	_playerInfoUI->UpdateManagerTick(InDeltaTime);
	_abnormalityUI->UpdateManagerTick(InDeltaTime);
	_systemInfoUI->UpdateManagerTick(InDeltaTime);
	_consumableUI->UpdateManagerTick(InDeltaTime);
	_spiritShotUI->UpdateManagerTick(InDeltaTime);
	_counterAttackButton->UpdateManagerTick(InDeltaTime);
}

void UGsUITraySaveBattery::InitializeReturnScroll()
{
	if (nullptr == _iconReturn)
	{
		_iconReturn = Cast<UGsUIIconItemSimple>(_iconSelectorReturn->CreateIcon());
		_iconSelectorReturn->AddChild(_iconReturn);

		if (false == _iconReturn->OnClickSlot.IsBoundToObject(this))
		{
			_iconReturn->OnClickSlot.AddUObject(this, &UGsUITraySaveBattery::OnClickReturn);
		}
	}

	if (_iconReturn == nullptr)
		return;

	_iconReturn->SetNotOwnItem(_returnScrollId, 0);
	_iconReturn->SetActiveOnEffect(false);
	_iconReturn->SetOnChangeDisplayDetailWndType(UGsUIIconItem::NotUseDetailWndType);
}

void UGsUITraySaveBattery::InvalidateMapName()
{
	int32 mapId = GLevel()->GetCurrentLevelId();
	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
	if (mapData == nullptr)
		return;

	if (mapData->MapStringId.IsEmpty())
		return;

	_textBlockMap->SetText(mapData->MapStringId);
}

void UGsUITraySaveBattery::InvalidateMode()
{
	UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == localPlayer)
		return;

	UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(localPlayer);
	if (nullptr == creature)
		return;

	// 사망 상태 - 사망 애니메이션
	if (creature->IsDeadState() || AnimationType::Dead == _animationState)
	{
		_animationState = AnimationType::Dead;
		if (nullptr != _iconReturn)
		{
			_iconReturn->SetImageGrayscale(true);
		}
	}
	// 전투 상태 - 전투 애니메이션
	else if (creature->IsBattleMode())
	{
		_animationState = AnimationType::Battle;
	}
	// 대기 상태 - 휴식 애니메이션
	else
	{
		_animationState = AnimationType::Idle;
	}

	//_switcherMode->SetActiveWidgetIndex(_animationState);
	_modeTextUI->SetMode(_animationState);
}

void UGsUITraySaveBattery::OnLocalPlayerDie()
{
	GSSaveBattery()->SetReStart(true);
	_animationState = AnimationType::Dead;
	//_switcherMode->SetActiveWidgetIndex(_animationState);
	_modeTextUI->SetMode(_animationState);

	// 버튼 비활성화
	if (nullptr != _iconReturn)
	{
		_iconReturn->SetImageGrayscale(true);
	}

	InvalidateOfflinePlay();
}

void UGsUITraySaveBattery::InvalidateReturnScroll()
{
	if (nullptr == _iconReturn)
		return;

	_iconReturn->SetAllOwnedItemCountDisplay();

	bool isDialogOpened = GUI()->IsActiveWidget(TEXT("NpcDialog"));
	if (0 < _iconReturn->GetAmount() && AnimationType::Dead != _animationState && !isDialogOpened)
	{
		_iconReturn->SetImageGrayscale(false);
	}
	else
	{
		_iconReturn->SetImageGrayscale(true);
	}
}

void UGsUITraySaveBattery::InvalidateItemWeightRate()
{
	float weightPercent = FGsItemHelper::GetWeightPercent(GItem()->GetOwnedItemWeight());
	OnUpdateItemWeight(weightPercent);
}

void UGsUITraySaveBattery::InvalidateInventoryCount()
{
	int64 ownedItemCount = 0;
	int64 invenSlotMaxCount = 0;
	if (UGsItemManager* itemManager = GItem())
	{
		ownedItemCount = itemManager->GetOwnedItemCount();
	}
	if (UGsInventoryManager* inventoryManager = GSInventory())
	{
		invenSlotMaxCount = inventoryManager->GetInvenSlotMaxCount();
	}

	// 수량 텍스트 및 색상 설정
	FText slotCountText;
	FGsUIStringHelper::GetTwoValueSlashedText(ownedItemCount, invenSlotMaxCount, slotCountText);
	EGsUIColorType colorType = (ownedItemCount > invenSlotMaxCount) ? EGsUIColorType::DEFAULT_LACK : EGsUIColorType::DEFAULT_ENOUGH;

	_textBlockSlotCount->SetText(slotCountText);
	_textBlockSlotCount->SetColorAndOpacity(FGsUIColorHelper::GetColor(colorType));
}

void UGsUITraySaveBattery::InvalidateOfflinePlay()
{
	bool isInvadeWorld = false;
	if (FGsGameDataManager* dataManager = GGameData())
	{
		isInvadeWorld = dataManager->IsInvadeWorld();
	}
	_btnOffLinePlay->SetVisibility(isInvadeWorld ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

	bool isDialogOpened = GUI()->IsActiveWidget(TEXT("NpcDialog"));
	bool isOfflinePlayUnlocked = GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::OFFLINE_PLAY);

	if (isOfflinePlayUnlocked && !isDialogOpened && _animationState != AnimationType::Dead)
	{
		_btnOffLinePlay->SetSwitcherIndex(0);
	}
	else
	{
		_btnOffLinePlay->SetSwitcherIndex(1);
	}
}

void UGsUITraySaveBattery::InvalidateDetectionTarget()
{
	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
		return;

	const int32 OPENED_INFO = 0;
	const int32 CLOSED_INFO = 1;
	bool isOpened = (_switcherDetectionTargetImage->GetActiveWidgetIndex() == OPENED_INFO);

	_panelDetectionTarget->SetVisibility(isOpened ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	if (isOpened)
	{
		int32 countAll = PKBookManager->GetDetectionTargetCountAll();
		const int32 EMPTY_COUNT = 0;
		if (EMPTY_COUNT >= countAll)
		{
			const int32 SWITCHER_EMPTY_INDEX = 0;
			_switcherDetectionTarget->SetActiveWidgetIndex(SWITCHER_EMPTY_INDEX);
			_slotHelperDetectionTargetData->RefreshAll(EMPTY_COUNT);
		}
		else
		{
			const int32 SWITCHER_ENOUGHT_INDEX = 1;
			_switcherDetectionTarget->SetActiveWidgetIndex(SWITCHER_ENOUGHT_INDEX);
			_slotHelperDetectionTargetData->RefreshAll(countAll);
		}
	}
}

void UGsUITraySaveBattery::OnIncreasePlayerExp(Exp InExp)
{
	FGsGameDataManager* dataManager = GGameData();
	const FGsNetUserData* userData = dataManager->GetUserData();
	if (nullptr == userData)
	{
		return;
	}

	Exp increasedExp = GSSaveBattery()->IncreaseExp(InExp);
	float expRate = FGsRewardHelper::GetExpRate(userData->mLevel, increasedExp, false);

	FString strExp = FString::Printf(TEXT("%.5f%%"), expRate * 100.0f);
	_textBlockExp->SetText(FText::FromString(strExp));
}

void UGsUITraySaveBattery::OnIncreaseGold(Currency InAmount)
{
	Currency increasedGold = GSSaveBattery()->IncreaseGold(InAmount);

	_textBlockGold->SetText(FText::AsNumber(increasedGold));
}

void UGsUITraySaveBattery::OnUpdateItemWeight(float InWeightPercent)
{
	WeightPenaltyGrade penaltyGradeType = GItem()->GetWeightPenaltyGradeType();
	_switcherWeightPenalty->SetActiveWidgetIndex((uint8)penaltyGradeType);

	FText weightPercentText;
	FGsItemHelper::GetWeightPercentText(InWeightPercent, weightPercentText);

	switch (penaltyGradeType)
	{
	case WeightPenaltyGrade::NONE:
		_weightLowText->SetText(weightPercentText);
		break;
	case WeightPenaltyGrade::FIRST:
		_weightMidText->SetText(weightPercentText);
		break;
	case WeightPenaltyGrade::SECOND:
		_weightHighText->SetText(weightPercentText);
		break;
	}
}

void UGsUITraySaveBattery::OnUpdateWeightPenaltyType(WeightPenaltyGrade InPenaltyGradeType)
{
	_switcherWeightPenalty->SetActiveWidgetIndex((uint8)InPenaltyGradeType);
}

void UGsUITraySaveBattery::OnClickReturn(UGsUIIconItem& InIcon)
{
	if (AnimationType::Dead == _animationState)
		return;

	if (nullptr == _saveBatteryHandler)
		return;

	if (GUI()->IsActiveWidget(TEXT("NpcDialog")))
		return;

	TArray<TWeakPtr<FGsItem>> returnScrolls = GItem()->FindByTID(_returnScrollId, ItemType::CONSUME);
	if (0 < returnScrolls.Num())
	{	
		_saveBatteryHandler->RequestReturn();
	}
}

void UGsUITraySaveBattery::OnClickOfflinePlay()
{
	if (AnimationType::Dead == _animationState)
		return;

	if (nullptr == _saveBatteryHandler)
		return;

	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	if (unlockManager->IsContentsUnlock(EGsUnlockContentsType::OFFLINE_PLAY))
	{
		_saveBatteryHandler->RequestOfflinePlay();
	}
	else
	{
		FText tickerText;
		unlockManager->GetUnLockMessage(EGsUnlockContentsType::OFFLINE_PLAY, tickerText);
		if (UGsUIManager* uiMgr = GUI())
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerMessage"));
			if (widget.IsValid())
			{
				UGsUITrayTickerMessage* ticker = Cast<UGsUITrayTickerMessage>(widget.Get());
				if (ticker)
				{
					ticker->SetTrayCondition(TrayCondition::TC_AlwaysShow);
				}
			}

			uiMgr->CloseByKeyName(TEXT("TrayTickerMessage"));

			widget = uiMgr->OpenAndGetWidget(TEXT("TrayTickerMessage"));
			if (widget.IsValid())
			{
				UGsUITrayTickerMessage* ticker = Cast<UGsUITrayTickerMessage>(widget.Get());
				if (ticker)
				{
					ticker->SetTrayCondition(TrayCondition::TC_Priority);
					ticker->SetData(tickerText, true, 1.0f, EGsTickerAnimationType::FADE_IN_OUT);
				}
			}
		}
	}
}

void UGsUITraySaveBattery::OnClickDetectionTarget()
{
	const int32 OPENED_INFO = 0;
	const int32 CLOSED_INFO = 1;

	bool isOpened = (_switcherDetectionTargetImage->GetActiveWidgetIndex() == OPENED_INFO);
	_switcherDetectionTargetImage->SetActiveWidgetIndex(isOpened ? CLOSED_INFO : OPENED_INFO);
	InvalidateDetectionTarget();
}

void UGsUITraySaveBattery::OnRefreshEntryDetectionTarget(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIPKTargetDetectionDataEntry> entry = Cast<UGsUIPKTargetDetectionDataEntry>(InEntry);
	if (false == entry.IsValid())
	{
		GSLOG(Warning, TEXT("false == entry.IsValid()"));
		return;
	}

	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	const FGsPKDetectionTargetData* detectionTargetData = PKBookManager->GetDetectionTargetAt(InIndex);
	if (nullptr == detectionTargetData)
	{
		return;
	}

	entry->SetData(detectionTargetData);
}

void UGsUITraySaveBattery::UpdateTime()
{
	if (nullptr == _startTime)
		return;

	// 경과 시간
	const FTimespan elapsedTime = FGsTimeSyncUtil::GetServerNowDateTime() - *_startTime;

	FString strTime;
	FGsTimeStringHelper::GetTimeStringCeil(elapsedTime, strTime);

	_textBlockTime->SetText(FText::FromString(strTime));
}

void UGsUITraySaveBattery::UpdateRemainTime()
{
	bool isShow = false;
	do 
	{
		if (nullptr == _saveBatteryHandler)
			break;

		if (false == _saveBatteryHandler->IsDungeonTimeLimit())
			break;

		if (false == _saveBatteryHandler->IsDungeonTimeShow())
			break;

		isShow = true;
		if (_saveBatteryHandler->IsDungeonTimeLimit())
		{
			FTimespan timeSpan = _saveBatteryHandler->GetDungeonEndTime() - FGsTimeSyncUtil::GetServerNowDateTime();
			FString strRemainTime;
			FGsTimeStringHelper::GetTimeStringDungeon(timeSpan, _saveBatteryHandler->IsDungeonTimeShowSecond(), strRemainTime);
			FText remainTimeText;
			if (strRemainTime.IsEmpty())
			{
				FText::FindText(TEXT("FriendText"), TEXT("FriendOffline_Sec"), remainTimeText);
			}
			else
			{
				remainTimeText = FText::Format(_remainTimeFormatText, FText::FromString(strRemainTime));
			}
			_textBlockRemainTime->SetText(remainTimeText);
		}

	} while (0);
	
	if (isShow)
	{
		if (_textBlockRemainTime->GetVisibility() != ESlateVisibility::SelfHitTestInvisible)
		{
			_textBlockRemainTime->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (_textBlockRemainTime->GetVisibility() != ESlateVisibility::Hidden)
		{
			_textBlockRemainTime->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UGsUITraySaveBattery::PlayRemainTime()
{
	GetWorld()->GetTimerManager().UnPauseTimer(_remainTimerHandler);
}

void UGsUITraySaveBattery::PauseRemainTime()
{
	GetWorld()->GetTimerManager().PauseTimer(_remainTimerHandler);
}

void UGsUITraySaveBattery::StopRemainTime()
{
	GetWorld()->GetTimerManager().PauseTimer(_remainTimerHandler);
}
