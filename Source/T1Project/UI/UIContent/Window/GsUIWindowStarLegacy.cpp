#include "GsUIWindowStarLegacy.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsStarLegacyManager.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacySlotArrangementInfo.h"
#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacyDefine.h"
#include "GameFlow/GameContents/ContentsGame/StarLegacy/GsStarLegacyCategoryInfo.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateStarLegacy.h"

#include "UI/UIContent/Window/StarLegacy/GsUIStarLegacySlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"

#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacyCategory.h"

#include "Runtime/UMG/Public/Components/PanelWidget.h"

void UGsUIWindowStarLegacy::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowStarLegacy::OnClickCloseAllStack);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowStarLegacy::OnClickBack);
	
	_btnSummonStart->OnClickCurrencyButton.BindUObject(this, &UGsUIWindowStarLegacy::OnClickSummonStart);
	_btnReroll->OnClickCurrencyButton.BindUObject(this, &UGsUIWindowStarLegacy::OnClickReroll);

	_numberOfRouletteSpins = GSStarLegacy()->GetNumberOfRouletteSpins();

	// 재화 버튼 텍스트 세팅
	FText textSummonBtnName;	
	// TEXT: 소환
	FText::FindText(TEXT("StarLegacyUIText"), TEXT("StarLegacy_Summon_Button"), textSummonBtnName);
	_btnSummonStart->SetButtonName(textSummonBtnName);

	FText textRerollBtnName;
	// TEXT: 새로 고침
	FText::FindText(TEXT("StarLegacyUIText"), TEXT("StarLegacy_Reroll_Button"), textRerollBtnName);
	_btnReroll->SetButtonName(textRerollBtnName);

	_starLegacyTabToggleGroup.OnSelectChanged.BindUObject(this, &UGsUIWindowStarLegacy::OnClickTab);

	_starLegacyTabToggleGroup.AddToggleWidget(_tabCategory1->GetToggleGroupEntry());
	_starLegacyTabToggleGroup.AddToggleWidget(_tabCategory2->GetToggleGroupEntry());

	if (const FGsStarLegacyCategoryInfo* tab1Info = GSStarLegacy()->FindCategoryInfoByTabIndex(0))
	{
		if (tab1Info->_tblInfo != nullptr)
		{
			_tabCategory1->SetTitleText(tab1Info->_tblInfo->nameText);
		}
	}
	


	if(const FGsStarLegacyCategoryInfo* tab2Info = GSStarLegacy()->FindCategoryInfoByTabIndex(1))
	{
		if (tab2Info->_tblInfo != nullptr)
		{
			_tabCategory2->SetTitleText(tab2Info->_tblInfo->nameText);
		}
	}
}


void UGsUIWindowStarLegacy::NativeConstruct()
{
	InitializeMessage();
	Super::NativeConstruct();
}
void UGsUIWindowStarLegacy::NativeDestruct()
{
	FinalizeMessage();
	Super::NativeDestruct();
}

// 메시지 바인딩 초기화
void UGsUIWindowStarLegacy::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
	{
		return;
	}

	MUI& msgUI = msg->GetUI();
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::CHANGED_STAR_LEGACY_MAGNITUDE_OPEN_INFO, 
		this, &UGsUIWindowStarLegacy::OnChangedStarLegacyMagnitudeOpenInfo));

	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::CHANGED_STAR_LEGACY_SLOT_RELEASE_INFO,
		this, &UGsUIWindowStarLegacy::OnChangedStarLegacySlotReleaseInfo));
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::CHANGED_STAR_LEGACY_SLOT_SELECT_INFO,
		this, &UGsUIWindowStarLegacy::OnChangedStarLegacySlotSelectInfo));
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::START_STAR_LEGACY_ROULETTE,
		this, &UGsUIWindowStarLegacy::OnStartStarLegacyRoulette));
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::REROLL_STAR_LEGACY,
		this, &UGsUIWindowStarLegacy::OnRerollStarLegacy));
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::CHANGED_STAR_LEGACY_CATEGORY_TAB,
		this, &UGsUIWindowStarLegacy::OnChangedStarLegacyCategoryTab));
}
// 메시지 바인딩 해제
void UGsUIWindowStarLegacy::FinalizeMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
	{
		return;
	}

	if (0 != _uiMsgArray.Num())
	{
		for (auto iter : _uiMsgArray)
		{
			msg->GetUI().Remove(iter);
		}
		_uiMsgArray.Empty();
	}
}

// 별의 유산 등급 오픈 정보 변경
void UGsUIWindowStarLegacy::OnChangedStarLegacyMagnitudeOpenInfo(const IGsMessageParam* inParam)
{
	const FGsUIMsgParamInt* param = inParam->Cast<const FGsUIMsgParamInt>();
	if (nullptr == param)
	{
		return;
	}

	int slotId = param->_data;
	UGsUIStarLegacySlot* slot = FindStarLegacySlot(slotId);
	if (slot == nullptr)
	{
		return;
	}
	slot->SetMagnitudeInfo(true);

	// 남은 횟수 갱신
	SetRemainCount();
}
// 별의 유산 해제 정보 갱신
void UGsUIWindowStarLegacy::OnChangedStarLegacySlotReleaseInfo(const IGsMessageParam* inParam)
{
	const FGsUIMsgParamInt* param = inParam->Cast<const FGsUIMsgParamInt>();
	if (nullptr == param)
	{
		return;
	}

	int slotId = param->_data;
	UGsUIStarLegacySlot* slot = FindStarLegacySlot(slotId);
	if (slot == nullptr)
	{
		return;
	}
	slot->SetSelectVisible(false);
}
// 별의 유산 선택 정보 갱신
void UGsUIWindowStarLegacy::OnChangedStarLegacySlotSelectInfo(const IGsMessageParam* inParam)
{
	const FGsUIMsgParamInt* param = inParam->Cast<const FGsUIMsgParamInt>();
	if (nullptr == param)
	{
		return;
	}

	int slotId = param->_data;

	UGsUIStarLegacySlot* slot = FindStarLegacySlot(slotId);
	if (slot == nullptr)
	{
		return;
	}
	slot->SetSelectVisible(true);
}
// 슬롯 구하기
UGsUIStarLegacySlot* UGsUIWindowStarLegacy::FindStarLegacySlot(int In_slotId)
{
	if (_arraySlots.Num() <= In_slotId)
	{
		return nullptr;
	}
	return _arraySlots[In_slotId];
}
// 현재 슬롯 정보 세팅
void UGsUIWindowStarLegacy::SetCurrentSlotInfo()
{
	int categoryId = GSStarLegacy()->GetCurrentCategoryId();
	// 현재 정보 구하기
	TArray<FGsStarLegacySlotArrangementInfo*> slotInfos = GSStarLegacy()->GetSlotInfo(categoryId);
	if (slotInfos.Num() == 0)
	{
		return;
	}

	for (int i = 0; i < _arraySlots.Num(); ++i)
	{
		UGsUIStarLegacySlot* slot = _arraySlots[i];
		if (slot == nullptr)
		{
			continue;
		}

		// 슬롯 id가 정보보다 많으면 다음
		if (slotInfos.Num() <= i)
		{
			continue;
		}

		FGsStarLegacySlotArrangementInfo* info = slotInfos[i];
		if (info == nullptr)
		{
			continue;
		}

		slot->SetSlotInfo(info);
	}
}

// 정보 세팅
void UGsUIWindowStarLegacy::SetStarLegacyInfo()
{
	// 카테고리 탭 1성계 세팅
	_starLegacyTabToggleGroup.SetSelectedIndex(0);

	// 슬롯 갱신
	SetCurrentSlotInfo();
	// 남은 open 갯수 갱신
	SetRemainCount();

	// 룰렛 연출 초기화
	InitializeRouletteData();

	// 룰렛 연출 패널 off, bottom on
	SetRoulettePanel(false);

	// bm 아이템 갯수 갱신
	SetBMItemCount();

	const FGsSchemaStarLegacyCategory* categoryTbl = GSStarLegacy()->GetCurrentCategoryTbl();
	if (categoryTbl == nullptr)
	{
		return;
	}

	// 소환 비용 세팅
	SetSummonCurrency(categoryTbl);
	// 변경 재화 세팅
	SetRerollCurrency(categoryTbl);
}

bool UGsUIWindowStarLegacy::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsStarLegacy))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_STAR_LEGACY);
		return true;
	}

	return false;
}

// 백버튼 클릭
bool UGsUIWindowStarLegacy::OnBack()
{
	// 대륙맵도 닫는다
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->CloseByKeyName(TEXT("WindowStarLegacy"));
	}


	return Super::OnBack();
}

// 남은 횟수 정보 세팅
void UGsUIWindowStarLegacy::SetRemainCount()
{
	FGsGameStateStarLegacy* gameStateStarLegacy = GSStarLegacy()->GetGameStateStarLegacy();
	if (gameStateStarLegacy == nullptr)
	{
		return;
	}

	int remainCount = gameStateStarLegacy->GetMagnitudeOpenRemainCount();

	FString remainCountString = FString::Format(TEXT("x{0}"), { remainCount });

	FText remainCountText = FText::FromString(remainCountString);

	_textOpenRemainCount = remainCountText;
}
// 소환 연출 시작
void UGsUIWindowStarLegacy::OnClickSummonStart()
{
	FGsGameStateStarLegacy* gameStateStarLegacy = GSStarLegacy()->GetGameStateStarLegacy();
	if (gameStateStarLegacy == nullptr)
	{
		return;
	}
	gameStateStarLegacy->StartStarLegacySummon();

}

// 별의 유산 룰렛 연출 시작
void UGsUIWindowStarLegacy::OnStartStarLegacyRoulette(const IGsMessageParam* inParam)
{
	
	// 첫번째 슬롯 연출 틀기
	PlaySlotAnimation();

	// 룰렛 연출 패널 on, bottom off
	SetRoulettePanel(true);
}

// 별의 유산 슬롯 정보 재배치
void UGsUIWindowStarLegacy::OnRerollStarLegacy(const IGsMessageParam* inParam)
{
	// 슬롯 갱신
	SetCurrentSlotInfo();
	// 남은 open 갯수 갱신
	SetRemainCount();

	// 룰렛 연출 초기화
	InitializeRouletteData();

	// 룰렛 연출 패널 off, bottom on
	SetRoulettePanel(false);

	// 리롤 버튼 생상 갱신
	_btnReroll->UpdateAmountTextColorLocalPlayer();
}

void UGsUIWindowStarLegacy::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isWaitEnd == true)
	{
		return;
	}

	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTime;

	if (diffTick >= _currentTimeTickWaitingTimeToPass)
	{
		// 다음 처리
		ProcNextSlot();
	}

}
// 타이머 시작(한칸 진행)
void UGsUIWindowStarLegacy::StartWaitTimmer()
{
	_isWaitEnd = false;
	_startTime = FDateTime::UtcNow().GetTicks();
}

// 슬롯 애니메이션 플레이
void UGsUIWindowStarLegacy::PlaySlotAnimation()
{
	// 슬롯 애니 플레이
	UGsUIStarLegacySlot* slot = FindStarLegacySlot(_currentPlayAniSlotId);
	if (slot == nullptr)
	{
		return;
	}
	slot->StartRouletteAnimation();

	// 타이머 초기화
	StartWaitTimmer();
}

// 룰렛 연출 데이터 초기화
void UGsUIWindowStarLegacy::InitializeRouletteData()
{
	// 대기 초기화
	_isWaitEnd = true;
	// 슬롯 id 초기화
	_currentPlayAniSlotId = 0;
	// 바퀴수 초기화
	_currentRoundCount = 0;

	// 현재 대기시간 갱신
	_currentTimeTickWaitingTimeToPass = GSStarLegacy()->GetTimeTickWaitingTimeToPass(_currentRoundCount);
}
// 다음 슬롯 처리
void UGsUIWindowStarLegacy::ProcNextSlot()
{
	// 두개 만족시 끝 도달
	// 1. 현재 round가 마지막
	// 2. 현재 슬롯이 마지막

	int serverPickSlotId = GSStarLegacy()->GetServerPickSlotId();
	
	if (_currentRoundCount == _numberOfRouletteSpins && 
		_currentPlayAniSlotId == serverPickSlotId)
	{
		// 타이머 종료
		_isWaitEnd = true;

		// 룰렛 종료
		FinishRoulette();		
		return;
	}
	
	// 다음 슬롯 id 찾기
	// 마지막 슬롯 id면 다음 round로 넘기기
	if (_currentPlayAniSlotId == _arraySlots.Num() - 1)
	{
		// 다음 바퀴로 넘김
		++_currentRoundCount;
		// slot id 초기화
		_currentPlayAniSlotId = 0;
	}
	else
	{
		// 다음 슬롯 id로 변경
		++_currentPlayAniSlotId;
	}

	// 현재 대기시간 갱신
	_currentTimeTickWaitingTimeToPass = GSStarLegacy()->GetTimeTickWaitingTimeToPass(_currentRoundCount);

	// 슬롯 플레이
	PlaySlotAnimation();
}

// 룰렛 관련 패널 세팅
void UGsUIWindowStarLegacy::SetRoulettePanel(bool In_isRouletteStart)
{
	if (In_isRouletteStart == true)
	{
		_panelBottom->SetVisibility(ESlateVisibility::Hidden);
		_panelRoulette->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_panelBottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_panelRoulette->SetVisibility(ESlateVisibility::Hidden);
	}
}

// 룰렛 종료 호출
void UGsUIWindowStarLegacy::FinishRoulette()
{
	// 룰렛 연출 패널 off, bottom off
	SetRoulettePanel(true);

	// 룰렛 연출 다끄기
	SetAllRouletteSlotVislble(false);

	// 성공시 성공 표시, 실패시 실패 표시
	// 서버 선택 슬롯
	int serverPickSlotId = GSStarLegacy()->GetServerPickSlotId();

	// 선택 슬롯들
	TArray<int> arraySelectSlot = GSStarLegacy()->GetArraySelectSlotIds();

	// 선택한것들에 서버 선택값이 없으면 실패, 있으면 성공
	bool isSuccess = (arraySelectSlot.Contains(serverPickSlotId) == true)? true : false;

	UGsUIStarLegacySlot* slot = FindStarLegacySlot(_currentPlayAniSlotId);
	if (slot == nullptr)
	{
		return;
	}
	
	slot->SetSuccessOrFailVisible(isSuccess);
}

// 룰렛 연출 visible 세팅
void UGsUIWindowStarLegacy::SetAllRouletteSlotVislble(bool In_isVisibleOn)
{
	for (int i = 0; i < _arraySlots.Num(); ++i)
	{
		UGsUIStarLegacySlot* slot = _arraySlots[i];
		if (slot == nullptr)
		{
			continue;
		}
	
		slot->SetRouletteSlotVisible(In_isVisibleOn);
	}
}

// 남은 bm 아이템 갯수 세팅
void UGsUIWindowStarLegacy::SetBMItemCount()
{
	FGsGameStateStarLegacy* gameStateStarLegacy = GSStarLegacy()->GetGameStateStarLegacy();
	if (gameStateStarLegacy == nullptr)
	{
		return;
	}

	int bmItemCount = gameStateStarLegacy->FindBMItemCount();

	FString bmItemCountString = FString::Format(TEXT("x{0}"), { bmItemCount });

	FText bmItemCountText = FText::FromString(bmItemCountString);

	_textBMItemCount = bmItemCountText;
}
// 소환 재화 세팅
void UGsUIWindowStarLegacy::SetSummonCurrency(const FGsSchemaStarLegacyCategory* In_categoryTbl)
{
	if (In_categoryTbl == nullptr)
	{
		return;
	}

	int summonGold = In_categoryTbl->rouletteGold;

	// 소환 골드
	_btnSummonStart->SetData(CurrencyType::GOLD, summonGold);
	_btnSummonStart->UpdateAmountTextColorLocalPlayer();
	_btnSummonStart->SetIsEnabled(true);
}

// 배치 변경	
void UGsUIWindowStarLegacy::OnClickReroll()
{
	FGsGameStateStarLegacy* gameStateStarLegacy = GSStarLegacy()->GetGameStateStarLegacy();
	if (gameStateStarLegacy == nullptr)
	{
		return;
	}
	gameStateStarLegacy->ReqReroll();
}

// 변경 재화 세팅
void UGsUIWindowStarLegacy::SetRerollCurrency(const FGsSchemaStarLegacyCategory* In_categoryTbl)
{
	if (In_categoryTbl == nullptr)
	{
		return;
	}

	int rerollGold = In_categoryTbl->shuffleGold;

	// 변경 골드
	_btnReroll->SetData(CurrencyType::GOLD, rerollGold);
	_btnReroll->UpdateAmountTextColorLocalPlayer();
	_btnReroll->SetIsEnabled(true);
}

// 탭 클릭(1성계, 2성계)
void UGsUIWindowStarLegacy::OnClickTab(int32 In_index)
{
	FGsGameStateStarLegacy* gameStateStarLegacy = GSStarLegacy()->GetGameStateStarLegacy();
	if (gameStateStarLegacy == nullptr)
	{
		return;
	}
	gameStateStarLegacy->ChangedCategoryTab(In_index);
}

// 별의 유산 카테고리 탭 변경
void UGsUIWindowStarLegacy::OnChangedStarLegacyCategoryTab(const IGsMessageParam* inParam)
{
	// 슬롯 갱신
	SetCurrentSlotInfo();
	// 남은 open 갯수 갱신
	SetRemainCount();
	// 룰렛 연출 초기화
	InitializeRouletteData();
	// 룰렛 연출 패널 off, bottom on
	SetRoulettePanel(false);

	const FGsSchemaStarLegacyCategory* categoryTbl = GSStarLegacy()->GetCurrentCategoryTbl();
	if (categoryTbl == nullptr)
	{
		return;
	}

	// 소환 비용 세팅
	SetSummonCurrency(categoryTbl);
	// 변경 재화 세팅
	SetRerollCurrency(categoryTbl);
}