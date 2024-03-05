// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMultiLevelRankSlot.h"

#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"

#include "PaperSprite.h"

#include "DataSchema/MultiLevelRank/GsSchemaMultiLevelRankSlot.h"

#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

#include "Management/ScopeGame/GsMultiLevelRankManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "MultiLevelRank/GsMultiLevelRankData.h"

#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiLevelRankIcon.h"
#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiLevelRankGradeLoopEffect.h"
#include "UI/UIContent/Popup/GsUIPopupMultiLevelRankQuest.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Item/GsItemManager.h"

#include "Reward/GsRewardHelper.h"

#include "T1Project.h"


void UGsUIMultiLevelRankSlot::BeginDestroy()
{
	if (nullptr != _btnAccept)
	{
		_btnAccept->OnClicked.RemoveDynamic(this, &UGsUIMultiLevelRankSlot::OnClickFunction);
	}
	_btnAccept = nullptr;
	if (nullptr != _btnPledge)
	{
		_btnPledge->OnClicked.RemoveDynamic(this, &UGsUIMultiLevelRankSlot::OnClickFunction);
	}
	_btnPledge = nullptr;
	if (nullptr != _btnLevelLimit)
	{
		_btnLevelLimit->OnClicked.RemoveDynamic(this, &UGsUIMultiLevelRankSlot::OnClickFunction);
	}
	_btnLevelLimit = nullptr;
	if (nullptr != _btnStepLimit)
	{
		_btnStepLimit->OnClicked.RemoveDynamic(this, &UGsUIMultiLevelRankSlot::OnClickFunction);
	}
	_btnStepLimit = nullptr;

	_onClickRankSlot.Unbind();

	Super::BeginDestroy();
}

void UGsUIMultiLevelRankSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAccept->OnClicked.AddDynamic(this, &UGsUIMultiLevelRankSlot::OnClickFunction);
	_btnPledge->OnClicked.AddDynamic(this, &UGsUIMultiLevelRankSlot::OnClickFunction);
	_btnLevelLimit->OnClicked.AddDynamic(this, &UGsUIMultiLevelRankSlot::OnClickFunction);
	_btnStepLimit->OnClicked.AddDynamic(this, &UGsUIMultiLevelRankSlot::OnClickFunction);

	_onClickRankSlot.Unbind();
}

void UGsUIMultiLevelRankSlot::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsSelected(false);
}

void UGsUIMultiLevelRankSlot::NativeDestruct()
{
	_bIsTutorialInteraction = false;
	OnTutorialInteraction.Unbind();

	Super::NativeDestruct();
}

FReply UGsUIMultiLevelRankSlot::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	OnClickSlot();

	// 클릭 사운드 출력
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->PlayPressSound();
	}

	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}

void UGsUIMultiLevelRankSlot::OnClickSlot()
{
	if (IsNormalSlot())
	{
		if (_slotData)
		{
			TWeakObjectPtr<UGsUIPopupMultiLevelRankQuest> popup = Cast<UGsUIPopupMultiLevelRankQuest>(GUI()->OpenAndGetWidget(TEXT("PopupMultiLevelRankQuest")));
			if (popup.IsValid())
			{
				int32 pledgeGroupId = GSMultiLevelRank()->IsPledgeSelected() ? 1 : 0; // 다중 서약 구현이 필요한 경우 보강 필요
				int32 rankGroupId = GSMultiLevelRank()->IsPledgeSelected() ? GSMultiLevelRank()->GetSelectedPledgeId() : 0;
				popup->InvalidateQuest(_slotData, pledgeGroupId, rankGroupId, true);
			}
		}
	}

	_onClickRankSlot.ExecuteIfBound(this);

	if (IsTutorialInteraction())
	{
		OnTutorialInteraction.ExecuteIfBound();
	}
}

void UGsUIMultiLevelRankSlot::OnClickFunction()
{
	// 슬롯 클릭 인터렉션 튜토리얼 중엔 다른 버튼 동작을 막음
	if (IsTutorialInteraction())
	{
		return;
	}

	switch (_functionButtonState)
	{
	case FunctionButtonState::QuestAccept:
		OnClickedQuestAccept();
		break;
	case FunctionButtonState::RankUp:
		OnClickedPromotion();
		break;
	case FunctionButtonState::Pledge:
		OnClickedPledgeSelect();
		break;
	case FunctionButtonState::LevelLimit:
		OnClickedLevelLimit();
		break;
	case FunctionButtonState::StepLimit:
		OnClickedStepLimit();
		break;
	}
}

void UGsUIMultiLevelRankSlot::OnClickedQuestAccept()
{
	if (FGsMultiLevelRankManager* rankManager = GSMultiLevelRank())
	{
		FText textMessageTicker;
		do
		{
			// 경험치 조건 확인
			if (false == rankManager->CheckExpConditionById(_slotData->_ownerRank->_rankSchema->rank))
			{	// Ticker Text : 경험치 조건을 만족하지 못하였습니다.			
				FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Ticker_NotEnoughExp"), textMessageTicker);
				break;
			}

			// 이전 등급이 퀘스트 완료 상태 확인
			if (false == rankManager->CheckPrevRankCondition(_slotData))
			{	// 이전 등급을 아직 완료 한적이 없다.
				FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Ticker_NotPrevRankCompleted"), textMessageTicker);
				break;
			}

			// send...
			const FGsSchemaQuestMultiLevelRankStory* story = _slotData->_slotSchema->storyId.GetRow();
			if (nullptr == story)
			{
				return;
			}

			rankManager->SendQuestAccept(story->id);

		} while (0);

		// message ticker 있으면 출력...
		if (false == textMessageTicker.IsEmpty())
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);
		}
	}
}

void UGsUIMultiLevelRankSlot::OnClickedPromotion()
{
	FText textMessageTicker;
	if (_slotData->_storyState != RankStoryState::Completed)
	{
		// 해당 랭크의 승급 임무를 완료하지 못하였습니다.
		FText::FindText(TEXT("MultiLevelRank"), TEXT("Error_Text_Promotion_Rank"), textMessageTicker);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);
	}
	else
	{
		if (_rankData->_rankSchema->rewardType != EGsMultiLevelRewardType::STAT)
		{
			if (GItem()->IsInvenSlotFull())
			{
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Inventory_Full"), textMessageTicker);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);
				return;
			}
			else if (GItem()->IsInvenWeightFull())
			{
				FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Weight_Full"), textMessageTicker);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);
				return;
			}
		}
		GSMultiLevelRank()->SendRankPromotion(_slotData->_slotSchema->rankId);
	}
}

void UGsUIMultiLevelRankSlot::OnClickedPledgeSelect()
{
	GUI()->Open(TEXT("PopupMultiLevelRankPledgeSelect"));
}

void UGsUIMultiLevelRankSlot::OnClickedLevelLimit()
{
	FGsMultiLevelRankManager* rankManager = GSMultiLevelRank();
	if (nullptr == rankManager)
		return;

	if (false == rankManager->CheckExpConditionById(_slotData->_ownerRank->_rankSchema->rank))
	{	// Ticker Text : 경험치 조건을 만족하지 못하였습니다.			
		FText textMessageTicker;
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Ticker_NotEnoughExp"), textMessageTicker);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);
	}
	else
	{
		InvalidateButton();
	}
}

void UGsUIMultiLevelRankSlot::OnClickedStepLimit()
{
	FGsMultiLevelRankManager* rankManager = GSMultiLevelRank();
	if (nullptr == rankManager)
		return;

	if (false == rankManager->CheckPrevRankCondition(_slotData))
	{	// 이전 등급을 아직 완료 한적이 없다.
		FText textMessageTicker;
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Ticker_NotPrevRankCompleted"), textMessageTicker);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);
	}
	else
	{
		InvalidateButton();
	}
}

void UGsUIMultiLevelRankSlot::OnInitialized(const FGsMultiLevelRankData* InRank)
{
	if (nullptr == InRank || InRank->_slotList.Num() == 0)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("UGsUIMultiLevelRankSlot::OnInitialized : Invaild InRank Ptr"));
#endif
		return;
	}

	// 랭크 데이터 설정
	_rankData = InRank;
	_isNormalSlot = true;

	// 랭크 노드 영역 초기화
	const FGsMultiLevelRankSlotData* slotData = nullptr;
	if (FGsMultiLevelRankManager* rankManager = GSMultiLevelRank())
	{
		slotData = &(_rankData->_slotList[0]);
		if (rankManager->IsPledgeSelected())
		{
			for (uint8 index = 0;index < _rankData->_slotList.Num();++index)
			{
				if (_rankData->_slotList[index]._slotSchema->groupId == rankManager->GetSelectedPledgeId())
				{
					slotData = &(_rankData->_slotList[index]);
					break;
				}
			}
		}
	}

	InitializeSlot(slotData);
}

void UGsUIMultiLevelRankSlot::InitializePledgeSlot(const FGsMultiLevelRankData* InRank)
{
	if (nullptr == InRank || InRank->_slotList.Num() == 0)
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("UGsUIMultiLevelRankSlot::OnInitialized : Invaild InRank Ptr"));
#endif
		return;
	}

	// 랭크 데이터 설정 : 서약 슬롯은 경험치바 설정을 위해 데이터가 필요
	_rankData = InRank;
	_isNormalSlot = false;

	// 슬롯 UI 기본 설정
	const FGsMultiLevelRankSlotData* slotData = nullptr;
	if (nullptr != _rankData && _rankData->_slotList.IsValidIndex(0))
	{
		slotData = &(_rankData->_slotList[0]);
	}

	InitializeSlot(slotData);
}

void UGsUIMultiLevelRankSlot::InitializeSlot(const FGsMultiLevelRankSlotData* InSlot)
{
	_slotData = InSlot;

	if (IsNormalSlot())
	{
		if (_slotData && _slotData->_ownerRank && _slotData->_ownerRank->_rankSchema)
		{
			AsyncLoadSprite(_imageField, _slotData->_slotSchema->fieldImage);
			_textBlockName->SetText(_slotData->_slotSchema->NameText);
			_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::MULTILEVELRANK_NORMAL));

			_switcherIcon->SetActiveWidgetIndex(0);
			_iconRank->SetMainImage(_slotData->_slotSchema->iconPath);
			_iconRank->SetSubImage(_slotData->_slotSchema->subIconPath);
			_iconRank->SetSubImageCount(_slotData->_slotSchema->subIconCount);

			_switcherPoint->SetActiveWidgetIndex(0);
		}
	}
	else
	{
		AsyncLoadSprite(_imageField, _pledgeImagePath);

		FText findText;
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_InfoText_Pledge"), findText);
		_textBlockName->SetText(findText);
		_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::MULTILEVELRANK_PLEDGE));

		_switcherIcon->SetActiveWidgetIndex(1);
		_switcherPoint->SetActiveWidgetIndex(1);
	}
	
	InvalidateReward();
	InvalidateButton();
	InvalidateQuest();
	SetIsSelected(false);

	// 경험치바 영역 초기화
	InitializeProgressBar();
	InvalidateProgressBar();
}

void UGsUIMultiLevelRankSlot::InitializeProgressBar()
{
	// 각 구간 마다의 exp 시작과 끝을 설정해야 한다.
	auto CalcPoint = [=](const FGsMultiLevelRankData* InBeineRank
		, const FGsMultiLevelRankData* InEndRank
		, const FGsMultiLevelRankData* InDeltaRank
		, SectionPointIndex InIndex
		, float InMaxRate)
	{
		uint64 begin = (InBeineRank) ? InBeineRank->_rankSchema->exp : 0;
		uint64 end = (InEndRank) ? InEndRank->_rankSchema->exp : 0;

		SectionPoint newPoint;
		if (SectionPointIndex::Front == InIndex)
		{
			newPoint._beginePoint = (begin > 0) ? begin + ((end - begin) * 0.5f) : 0;
			newPoint._endPoint = end;
			newPoint._maxRateValue = (nullptr != InDeltaRank) ? 1.0f : 2.0f;
		}
		else if (SectionPointIndex::Back == InIndex)
		{
			newPoint._beginePoint = begin;
			newPoint._endPoint = begin + ((end - begin) * 0.5f);
			// 2021/06/22 PKT - 가장 마지막 라인은 Percent 값이 0이여야 한다.
			newPoint._maxRateValue = (nullptr != InEndRank) ? 1.0f : 0.f;
		}

		return newPoint;
	};

	FGsMultiLevelRankManager* rankManager = GSMultiLevelRank();
	if (nullptr == rankManager)
		return;

	int32 CurrentIndex = rankManager->GetIndexByRankData(_rankData);
	const FGsMultiLevelRankData* PrevRank = rankManager->GetRankByIndex(CurrentIndex - 1);
	const FGsMultiLevelRankData* NextRank = rankManager->GetRankByIndex(CurrentIndex + 1);
	float maxRate = 1.0f;

	_point[SectionPointIndex::Front] = CalcPoint(PrevRank, _rankData, NextRank, SectionPointIndex::Front, maxRate);
	_point[SectionPointIndex::Back] = CalcPoint(_rankData, NextRank, PrevRank, SectionPointIndex::Back, maxRate);
}

void UGsUIMultiLevelRankSlot::InvalidateSlotAll()
{
	InvalidateReward();
	InvalidateButton();
	InvalidateQuest();
	InvalidateProgressBar();
}

void UGsUIMultiLevelRankSlot::InvalidateProgressBar()
{
	if (nullptr == _rankData)
		return;

	auto CalcRate = [=](const uint64 InCurrnetExp, const SectionPoint& InNewPoint)
	{
		if (InCurrnetExp < InNewPoint._beginePoint)
			return 0.0f;
		else if (InCurrnetExp > InNewPoint._endPoint)
			return InNewPoint._maxRateValue;

		int64 begin = InNewPoint._beginePoint;
		int64 end = InNewPoint._endPoint;
		int64 current = InCurrnetExp;

		float rate = static_cast<float>(current - begin) / (end - begin);
		rate *= InNewPoint._maxRateValue;
		return FMath::Clamp(rate, 0.f, 2.0f);
	};

	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	const FGsGameObjectDataLocalPlayer* LocalPlayer = (local) ? local->GetCastData<FGsGameObjectDataLocalPlayer>() : nullptr;
	if (nullptr == LocalPlayer)
	{
		return;
	}

	uint64 maxAccumulateExp = GSMultiLevelRank()->GetMaxAccumulateExp();

	float frontPercent = CalcRate(maxAccumulateExp, _point[static_cast<int32>(SectionPointIndex::Front)]) * 0.5f;
	float backPercent = CalcRate(maxAccumulateExp, _point[static_cast<int32>(SectionPointIndex::Back)]) * 0.5f;
	float percent = frontPercent + backPercent;

	if (maxAccumulateExp >= _point[static_cast<int32>(SectionPointIndex::Front)]._endPoint)
	{
		_imageExpPointOn->SetVisibility(ESlateVisibility::HitTestInvisible);
		_imageExpPointOff->SetVisibility(ESlateVisibility::Collapsed);
		_imageExpPointOnPledge->SetVisibility(ESlateVisibility::HitTestInvisible);
		_imageExpPointOffPledge->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_imageExpPointOn->SetVisibility(ESlateVisibility::Collapsed);
		_imageExpPointOff->SetVisibility(ESlateVisibility::HitTestInvisible);
		_imageExpPointOnPledge->SetVisibility(ESlateVisibility::Collapsed);
		_imageExpPointOffPledge->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	_progressBarExp->SetPercent(percent);

	if (!_isNormalSlot && GSMultiLevelRank()->IsPledgeSelected())
	{
		_progressBarExp->SetPercent(1);
	}
}

void UGsUIMultiLevelRankSlot::InvalidateReward()
{
	if (IsNormalSlot())
	{
		_panelReward->SetVisibility(ESlateVisibility::HitTestInvisible);
		_textBlockReward->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		if (_slotData->_slotState == MultiLevelRankSlotState::ACTIVE)
		{
			_effectRewardGrade->SetVisibility(ESlateVisibility::Collapsed);
			_effectRewardGrade->EndEffect();
		}
		else
		{
			_effectRewardGrade->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_effectRewardGrade->StartEffect(_slotData->_ownerRank->_rankSchema->effectType);
		}
	}
	else
	{
		_panelReward->SetVisibility(ESlateVisibility::Collapsed);
		_textBlockReward->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	CreatureGenderType genderType = CreatureGenderType::MALE;
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (FGsGameObjectDataLocalPlayer* localData = player->GetCastData<FGsGameObjectDataLocalPlayer>())
		{
			genderType = localData->GetCreatureGenderType();
		}
	}

	if (genderType == CreatureGenderType::MALE)
	{
		_imageReward->SetVisibility(ESlateVisibility::Hidden);
		AsyncLoadSprite(_imageReward, _slotData->_ownerRank->_rankSchema->manRewardShortImage);
	}
	else
	{
		_imageReward->SetVisibility(ESlateVisibility::Hidden);
		AsyncLoadSprite(_imageReward, _slotData->_ownerRank->_rankSchema->womanRewardShortImage);
	}

	_textBlockReward->SetText(_slotData->_ownerRank->_rankSchema->rewardSlotDesc);
}

void UGsUIMultiLevelRankSlot::InvalidateButton()
{
	FGsMultiLevelRankManager* rankManager = GSMultiLevelRank();
	if (nullptr == rankManager)
		return;

	// 승급 단계
	if (IsNormalSlot())
	{
		_switcherSlotState->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		switch (_slotData->_slotState)
		{
		case MultiLevelRankSlotState::LOCK:
		case MultiLevelRankSlotState::UNLOCK:
		{
			//const FGsMultiLevelRankSlotData* activeLastSlot = rankManager->GetActiveLastSlot();
			//if (_slotData->_linkedPrevSlot == activeLastSlot)
			{
				// 경험치 조건을 달성하지 못했을 경우
				if (false == rankManager->CheckExpConditionById(_slotData->_ownerRank->_rankSchema->rank))
				{
					_switcherSlotState->SetActiveWidgetIndex(SlotStateIndex::LevelLimit);
					_functionButtonState = FunctionButtonState::LevelLimit;

					Level level = FGsRewardHelper::GetLevelByExp(_slotData->_ownerRank->_rankSchema->exp);
					float expRate = FGsRewardHelper::GetExpRate(level, _slotData->_ownerRank->_rankSchema->exp, true) * 100;
					int32 exp = int32(expRate);

					FText levelText;
					if (exp > 0)
					{
						FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Text_LevelExpInfo"), levelText);
						levelText = FText::Format(levelText, level, exp);
					}
					else
					{
						FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Text_Level"), levelText);
						levelText = FText::Format(levelText, level);
					}
					_textBlockLevelLimit->SetText(levelText);
					break;
				}
				// 이전 단계를 달성하지 못했을 경우
				if (false == rankManager->CheckPrevRankCondition(_slotData))
				{
					_switcherSlotState->SetActiveWidgetIndex(SlotStateIndex::StepLimit);
					_functionButtonState = FunctionButtonState::StepLimit;
					break;
				}
				// 조건을 모두 만족한 경우
				_switcherSlotState->SetActiveWidgetIndex(SlotStateIndex::None);
				_functionButtonState = FunctionButtonState::QuestAccept;
				SetButtonText(TEXT("UI_Button_Quest_Accept"));
			}
			//else
			{
				// 아무 표시하지 않는다.
				//_switcherSlotState->SetVisibility(ESlateVisibility::Collapsed);
			}
			break;
		}
		case MultiLevelRankSlotState::IN_PROGRESS:
		{
			// 진행중 텍스트 표시
			_switcherSlotState->SetActiveWidgetIndex(SlotStateIndex::QuestInProgress);
			_functionButtonState = FunctionButtonState::QuestInProgress;
			break;
		}
		case MultiLevelRankSlotState::FINISH:
		{
			// 승급하기 버튼 표시
			_switcherSlotState->SetActiveWidgetIndex(SlotStateIndex::None);
			_functionButtonState = FunctionButtonState::RankUp;
			SetButtonText(TEXT("UI_RankButtonText_Rankup"));
			break;
		}
		case MultiLevelRankSlotState::ACTIVE:
		{
			// 완료 체크 표시
			_switcherSlotState->SetActiveWidgetIndex(SlotStateIndex::Complete);
			_functionButtonState = FunctionButtonState::Completed;
			break;
		}
		}
	}
	// 서약 단계
	else
	{
		_switcherSlotState->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (rankManager->IsPledgeSelected())
		{
			// 완료 체크 표시
			_switcherSlotState->SetActiveWidgetIndex(SlotStateIndex::Complete);
			_functionButtonState = FunctionButtonState::PledgeCompleted;
		}
		else
		{
			do 
			{
				// 서약 선택 버튼 표시
				_switcherSlotState->SetActiveWidgetIndex(SlotStateIndex::None);
				_functionButtonState = FunctionButtonState::Pledge;

				// 경험치 조건을 달성하지 못했을 경우
				if (false == rankManager->CheckExpConditionById(_slotData->_ownerRank->_rankSchema->rank))
				{
					Level level = FGsRewardHelper::GetLevelByExp(_slotData->_ownerRank->_rankSchema->exp);
					float expRate = FGsRewardHelper::GetExpRate(level, _slotData->_ownerRank->_rankSchema->exp, true) * 100;
					int32 exp = int32(expRate);

					FText levelText;
					if (exp > 0)
					{
						FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Text_LevelExpInfo"), levelText);
						levelText = FText::Format(levelText, level, exp);
					}
					else
					{
						FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Text_Level"), levelText);
						levelText = FText::Format(levelText, level);
					}
					_textBlockFunction->SetText(levelText);
					break;
				}
				// 이전 단계를 달성하지 못했을 경우
				if (false == rankManager->CheckPrevRankCondition(_slotData))
				{
					SetButtonText(TEXT("UI_Text_RequirePrevRank"));
					break;
				}
				
				SetButtonText(TEXT("UI_Button_SelectPledge"));
			} while (0);
		}
	}
}

void UGsUIMultiLevelRankSlot::InvalidateQuest()
{
	if (IsNormalSlot())
	{
		switch (_slotData->_slotState)
		{
		case MultiLevelRankSlotState::LOCK:
		case MultiLevelRankSlotState::UNLOCK:
		case MultiLevelRankSlotState::IN_PROGRESS:
		case MultiLevelRankSlotState::FINISH:
		{
			_panelQuest->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		}
		case MultiLevelRankSlotState::ACTIVE:
		{
			_panelQuest->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
		}
	}
	else
	{
		_panelQuest->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIMultiLevelRankSlot::SetIsSelected(bool bInIsSelected)
{
	_isSelected = bInIsSelected;

	if (nullptr != _imageHighlight)
	{
		_imageHighlight->SetVisibility(_isSelected ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

bool UGsUIMultiLevelRankSlot::IsNormalSlot()
{
	return _isNormalSlot;
}

void UGsUIMultiLevelRankSlot::AsyncLoadSprite(UImage* InImage, const FSoftObjectPath& InImagePath)
{
	TWeakObjectPtr<UImage> weakThis(InImage);
	FGsUIHelper::AsyncLoadSpriteFromPath(InImagePath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UImage* strongThis = weakThis.Get())
			{
				strongThis->SetVisibility(ESlateVisibility::Visible);
				strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
			}
		}));
}

void UGsUIMultiLevelRankSlot::SetButtonText(FTextKey InTextKey)
{
	FText findText;
	FText::FindText(TEXT("MultiLevelRank"), InTextKey, findText);

	_textBlockFunction->SetText(findText);
}