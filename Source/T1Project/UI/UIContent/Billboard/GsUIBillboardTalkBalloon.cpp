// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIBillboardTalkBalloon.h"

#include "CanvasPanel.h"
#include "Components/TextBlock.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "FontMeasure.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/Actor.h"

#include "Achievement/GsAchievementData.h"

#include "DataSchema/TalkBalloon/GsSchemaTalkBalloonData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"

#include "Item/GsItemManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAchievementManager.h"
#include "Management/ScopeGame/GsCommunityEventManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Net/GsNetSendService.h"

#include "Option/GsGameUserSettings.h"

#include "Rendering/SlateRenderer.h"

#include "UTIL/GsText.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsTimeSyncUtil.h"



void UGsUIBillboardTalkBalloon::NativeOnInitialized()
{
	_updateDelegateMap.Emplace(TalkBalloonState::Opened, FOnUpdateTalkBalloonDelegate::CreateUObject(
		this, &UGsUIBillboardTalkBalloon::UpdateOpened));
	_updateDelegateMap.Emplace(TalkBalloonState::Closing, FOnUpdateTalkBalloonDelegate::CreateUObject(
		this, &UGsUIBillboardTalkBalloon::UpdateClosing));
	_updateDelegateMap.Emplace(TalkBalloonState::Closed, FOnUpdateTalkBalloonDelegate::CreateUObject(
		this, &UGsUIBillboardTalkBalloon::UpdateClosed));
}

void UGsUIBillboardTalkBalloon::NativeConstruct()
{
	Super::NativeConstruct();

	EndTalkBalloon();
}

void UGsUIBillboardTalkBalloon::NativeDestruct()
{
	_localPlayer = nullptr;

	Super::NativeDestruct();
}

void UGsUIBillboardTalkBalloon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (nullptr == _talkBalloonData)
		return;

	if (GGameData()->GetInvasionDefenseState())
	{
		// 침공전 진행 중
		if (false == IsClosed())
		{
			SetClose();
		}
	}
	else
	{
		if (IsOptionEnabled())
		{
			_isOptionEnd = false;
			if (_wasLocalPlayerInRange)
			{
				if (IsLocalPlayerInRange(_talkBalloonData->talkRange * METER_TO_CM + _additionalDist))
				{ // 범위 안 -> 범위 안 : 갱신
					UpdateTalkBalloon(InDeltaTime);
				}
				else
				{ // 범위 안 -> 범위 밖 : 출력 중지
					EndTalkBalloon();
				}
			}
			else
			{
				if (IsLocalPlayerInRange(_talkBalloonData->talkRange * METER_TO_CM))
				{ // 범위 밖 -> 범위 안 : 출력 시작
					StartTalkBalloon();
				}
			}
		}
		else
		{
			if (!_isOptionEnd)
			{
				_isOptionEnd = true;
				EndTalkBalloon();
			}
		}

		// 텍스트가 변하는 모습이 보이지 않는 트릭
		if (_tick2)
		{
			_panelTalkBalloon->SetRenderOpacity(1);
			_tick1 = false;
			_tick2 = false;
		}

		if (_tick1)
		{
			_tick1 = false;
			_tick2 = true;
		}
	}

	// 말풍선 출력 거리(디버그용)
	if (_isDebuging)
	{
		if (nullptr != _talkBalloonData)
		{
			DrawDebugTalkBalloonRange();
		}
	}
}

void UGsUIBillboardTalkBalloon::OnChangeRenderState(bool bIsRendered)
{
	SetRenderOpacity(bIsRendered ? 1.f : 0.f);
}

void UGsUIBillboardTalkBalloon::UpdateTalkBalloon(float InDeltaTime)
{
	// 각 상태에 대한 업데이트 함수 실행
	if (_updateDelegateMap.Contains(_talkBalloonState))
	{
		_updateDelegateMap[_talkBalloonState].ExecuteIfBound(InDeltaTime);
	}
}

void UGsUIBillboardTalkBalloon::UpdateOpened(float InDeltaTime)
{
	_remainKeepTime -= InDeltaTime;

	if (0 >= _remainKeepTime)
	{
		if (1 < _remainIntervalTime)
		{ // 대기 시간이 1초가 넘으면 투명도가 올라가는 효과가 있는 숨김 처리
			SetCloseWithEffect();
		}
		else
		{ // 대기 시간이 짧으면 바로 숨김 처리
			SetClose();
		}
	}
}

void UGsUIBillboardTalkBalloon::UpdateClosing(float InDeltaTime)
{
	_remainIntervalTime -= InDeltaTime;

	float spendingTime = _talkBalloonData->talkBalloonDataSetList[_currentIndex].intervalTime - _remainIntervalTime;
	if (_remainIntervalTime < 0 || 1 < spendingTime)
	{
		SetClose();
	}
}

void UGsUIBillboardTalkBalloon::UpdateClosed(float InDeltaTime)
{
	_remainIntervalTime -= InDeltaTime;

	if (0 >= _remainIntervalTime)
	{
		_nextState = _currentState;
		UpdateData(TalkBalloonUpdateType::NORMAL);
	}
}

void UGsUIBillboardTalkBalloon::StartTalkBalloon()
{
	_wasLocalPlayerInRange = true;

	if (_gameObjectDelegate.Num() == 0)
	{
		if (UGsGameObjectManager* gameObjectManager = GSGameObject())
		{
			if (UGsGameObjectNonPlayerBase* nonPlayer =
				Cast<UGsGameObjectNonPlayerBase>(gameObjectManager->FindObject(_attachedActor, EGsGameObjectType::NonPlayerBase)))
			{
				const FGsSchemaNpcData* npcData = nonPlayer->GetNpcData();
				// 커뮤니티 소녀용 말풍선 처리 (서버 통신)
				if (npcData && npcData->npcFunctionType == NpcFunctionType::COMMUNITY)
				{
					_gameObjectDelegate.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::TALKBALLOON_COMMUNITY_RANK,
						this, &UGsUIBillboardTalkBalloon::OnReceiveCommunityRank));
					_gameObjectDelegate.Emplace(GMessage()->GetGameObject().AddUObject(MessageGameObject::TALKBALLOON_COMMUNITY_MARKET,
						this, &UGsUIBillboardTalkBalloon::OnReceiveCommunityMarket));
				}
			}
		}
	}

	_talkBalloonState = TalkBalloonState::Closed;
	_remainIntervalTime = 0;
}

void UGsUIBillboardTalkBalloon::EndTalkBalloon()
{
	_isRequestingData = false;
	_wasLocalPlayerInRange = false;
	_remainIntervalTime = 0;

	if (FGsMessageHolder* msg = GMessage())
	{
		if (_gameObjectDelegate.Num() > 0)
		{
			for (auto iter : _gameObjectDelegate)
			{
				msg->GetGameObject().Remove(iter);
			}
			_gameObjectDelegate.Empty();
		}
	}

	SetClose();
}

void UGsUIBillboardTalkBalloon::SetOpen()
{
	_talkBalloonState = TalkBalloonState::Opened;

	if (_updateReason == TalkBalloonUpdateType::NORMAL && _talkBalloonData->talkRandomRate < FMath::RandRange(0, 10000))
	{
		return;
	}

	OnStateChanged.Broadcast();

	// 2틱 후에 말풍선이 보인다.
	_panelTalkBalloon->SetVisibility(ESlateVisibility::HitTestInvisible);
	_panelTalkBalloon->SetRenderOpacity(0);
	_tick1 = true;
}

void UGsUIBillboardTalkBalloon::SetCloseWithEffect()
{
	_talkBalloonState = TalkBalloonState::Closing;
	PlayCloseAnimation();
}

void UGsUIBillboardTalkBalloon::SetClose()
{
	_talkBalloonState = TalkBalloonState::Closed;
	OnStateChanged.Broadcast();

	_panelTalkBalloon->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIBillboardTalkBalloon::UpdateData(TalkBalloonUpdateType InUpdateReason)
{
	// 죽은 상태일 때는 말풍선을 갱신하지 않는다.
	if (_isDead)
		return;

	// 말풍선 기본 데이터 ( 출력 범위, 랜덤성 ) 확인
	if (nullptr == _talkBalloonData)
		return;
	
	_updateReason = InUpdateReason;
	switch (InUpdateReason)
	{
	case TalkBalloonUpdateType::NORMAL:
		UpdateDataNormal(_currentOrder);
		break;
	case TalkBalloonUpdateType::STATE_CHANGED:
		UpdateDataNormal(_currentOrder);
		break;
	case TalkBalloonUpdateType::ANIM_NOTIFIED:
		UpdateDataAnimNotified();
		break;
	case TalkBalloonUpdateType::COMMUNITY:
		UpdateDataOnReceiveCommunity();
		break;
	case TalkBalloonUpdateType::HIT:
		UpdateDataOnHit();
	}
}

void UGsUIBillboardTalkBalloon::UpdateDataNormal(int32 InCurrentOrder, int32 InCount)
{
	// 서버 데이터를 요청 중인가?
	if (_isRequestingData)
		return;
	
	// 출력할 대사 목록이 존재하는가?
	if (!_displayIndexListMap.Contains(_nextState))
		return;

	// 대사 목록을 다 확인했는가?
	if (InCount >= _displayIndexListMap[_nextState].Num())
		return;

	// 애니메이션 상태가 변했는가?
	if (_currentState != _nextState)
	{
		_currentState = _nextState;
		_currentOrder = INDEX_NONE;
		_isDead = (EGsTalkBalloonState::Dead == _nextState);
		_randomIndexList.Empty();
	}

	int32 nextOrder = GetNextOrder(InCurrentOrder);
	int32 nextIndex = GetOriginalIndex(nextOrder);

	_currentOrder = nextOrder;
	_currentIndex = nextIndex;

	// 데이터 요청이 필요한가? (커뮤니티소녀)
	if (IsRequiredDataRequest(_currentIndex))
	{
		RequestServerData(_currentIndex);
		return;
	}

	// 관련 데이터가 유효한가?
	if (CanShowTalkBalloon(_currentIndex))
	{
		SetNextDataByIndex(_currentIndex);
		SetOpen();
	}
	else
	{
		// 현재 말풍선을 표시할 수 없으면 다시 인덱스 확인
		UpdateDataNormal(InCurrentOrder + 1, InCount + 1);
	}
}

void UGsUIBillboardTalkBalloon::UpdateDataStateChanged()
{

}

void UGsUIBillboardTalkBalloon::UpdateDataAnimNotified()
{
	_remainKeepTime = _animNotifiedDataSet->keepTime;
	_remainIntervalTime = _animNotifiedDataSet->intervalTime;
	SetText(_animNotifiedDataSet->text);

	SetOpen();

	_animNotifiedDataSet = nullptr;
}

void UGsUIBillboardTalkBalloon::UpdateDataOnReceiveCommunity()
{
	_isRequestingData = false;

	if (!_wasLocalPlayerInRange)
		return;

	if (CanShowTalkBalloon(_currentIndex))
	{
		SetNextDataByIndex(_currentIndex);
		SetOpen();
	}
	else
	{
		_nextState = _currentState;
		UpdateData(TalkBalloonUpdateType::NORMAL);
	}
}

void UGsUIBillboardTalkBalloon::UpdateDataOnHit()
{
	if (_displayIndexListMap.Contains(EGsTalkBalloonState::Hit))
	{
		int32 index = _displayIndexListMap[EGsTalkBalloonState::Hit][0];
		SetNextDataByIndex(index);
		SetOpen();
	}
}

void UGsUIBillboardTalkBalloon::SetTalkBalloonData(const FGsSchemaTalkBalloonData* InSchemaData)
{
	_talkBalloonData = InSchemaData;

	InitDisplayIndexList();
}

void UGsUIBillboardTalkBalloon::SetActor(AActor* InActor)
{
	_attachedActor = InActor;
}

void UGsUIBillboardTalkBalloon::SetText(const FText& InText)
{
	auto MinDesiredWidth = _textBlockTalk->GetDesiredSize();

	FString str = InText.ToString();
	float textWidth = ComputeDesiredSize(FText::FromString(str)) + 40.f;

	if (textWidth > 270.f)
	{
		str = str.Left(10);
		textWidth = ComputeDesiredSize(FText::FromString(str)) + 40.f;
	}

	_textBlockTalk->SetMinDesiredWidth(textWidth);
	_textBlockTalk->SetText(InText);
}

void UGsUIBillboardTalkBalloon::SetActorState(uint8 InState)
{
	EGsTalkBalloonState state = GetStateEnumFromString(InState);
	if (state == EGsTalkBalloonState::MAX)
		return;

	if (_currentState == state)
		return;

	if (_displayIndexListMap.Contains(state))
	{
		_nextState = state;
		UpdateData(TalkBalloonUpdateType::STATE_CHANGED);
	}
}

void UGsUIBillboardTalkBalloon::ShowNotifiedTalkBallon(const FGsSchemaTalkBalloonDataSet* InSchemaDataSet)
{
	_animNotifiedDataSet = InSchemaDataSet;
	if (_animNotifiedDataSet)
	{
		UpdateData(TalkBalloonUpdateType::ANIM_NOTIFIED);
	}
}

void UGsUIBillboardTalkBalloon::SetIsDead(bool InIsDead)
{
	_isDead = InIsDead;
}

bool UGsUIBillboardTalkBalloon::IsLocalPlayerInRange(int32 InRadius)
{
	if (nullptr == _localPlayer)
	{
		if (UGsGameObjectManager* gameObjectManager = GSGameObject())
		{
			_localPlayer = gameObjectManager->FindObject(EGsGameObjectType::LocalPlayer);
		}
	}

	if (nullptr != _localPlayer && nullptr != _attachedActor)
	{
		float distSquared = FVector2D::DistSquared(_localPlayer->GetLocation2D(), FVector2D(_attachedActor->GetActorLocation()));
		if (distSquared <= InRadius * InRadius)
		{
			return true;
		}
	}
	return false;
}

bool UGsUIBillboardTalkBalloon::IsRequiredDataRequest(int32 InIndex)
{
	if (_talkBalloonData)
	{
		switch (_talkBalloonData->talkBalloonDataSetList[InIndex].dataSetType)
		{
		case EGsTalkBalloonDataType::RANK_LOCAL_LEVEL_1ST:
		case EGsTalkBalloonDataType::RANK_WORLD_LEVEL_1ST:
		case EGsTalkBalloonDataType::MARKET_PRICE_TOP:
		case EGsTalkBalloonDataType::MARKET_WEAPON_GRADE_TOP:
			return true;
		}
	}
	return false;
}

bool UGsUIBillboardTalkBalloon::CanShowTalkBalloon(int32 InIndex)
{
	if (_talkBalloonData)
	{
		switch (_talkBalloonData->talkBalloonDataSetList[InIndex].dataSetType)
		{
		case EGsTalkBalloonDataType::RANK_LOCAL_LEVEL_1ST:
			if (_localUserName.IsEmpty())
				return false;
			break;
		case EGsTalkBalloonDataType::RANK_WORLD_LEVEL_1ST:
			if (_worldUserName.IsEmpty())
				return false;
			break;
		case EGsTalkBalloonDataType::MARKET_PRICE_TOP:
			if (_highestPriceItemId == INVALID_ITEM_ID)
				return false;
			break;
		case EGsTalkBalloonDataType::MARKET_WEAPON_GRADE_TOP:
			if (_highestGradeItemId == INVALID_ITEM_ID)
				return false;
			break;
		case EGsTalkBalloonDataType::REDDOT_REWARD:
		case EGsTalkBalloonDataType::REDDOT_NEWS:
		case EGsTalkBalloonDataType::ANNOUNCEMENT:
			if (FGsCommunityEventManager* communityManager = GSCommunityEvent())
			{
				if (!communityManager->IsRedDot())
					return false;
			}
			break;
		}

		return true;
	}
	return false;
}

bool UGsUIBillboardTalkBalloon::IsClosed()
{
	return (_talkBalloonState == TalkBalloonState::Closed) ? true : false;
}

bool UGsUIBillboardTalkBalloon::IsOptionEnabled()
{
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		int32 optionValue = gameUserSettings->GetCombatSetting(EGsOptionCombat::NPC_TALK_BALLOON);
		return (0 == optionValue) ? false : true;
	}
	
	return false;
}

int32 UGsUIBillboardTalkBalloon::GetNextOrder(int32 InOrder)
{
	int32 dataSize = _displayIndexListMap[_currentState].Num();
	if (0 == dataSize)
		return 0;

	int32 nextOrder = InOrder + 1;
	if (nextOrder >= dataSize)
	{
		nextOrder %= dataSize;
	}
	return nextOrder;
}

int32 UGsUIBillboardTalkBalloon::GetOriginalIndex(int32 InIndex)
{
	switch (_talkBalloonData->talkType)
	{
	case EGsTalkType::ORDER:
		return _displayIndexListMap[_currentState][InIndex];
	case EGsTalkType::RANDOM:
	{
		if (InIndex == 0 || _randomIndexList.Num() == 0)
		{
			ResetRandomIndexList();
		}
		return _randomIndexList[InIndex];
	}
	}
	return 0;
}

FText UGsUIBillboardTalkBalloon::GetNextText(int32 InIndex)
{
	switch (_talkBalloonData->talkBalloonDataSetList[InIndex].dataSetType)
	{
	case EGsTalkBalloonDataType::RANK_LOCAL_LEVEL_1ST:
	{
		FText findText;
		if (_localGuildName.IsEmpty())
		{
			FText::FindText(TEXT("TalkBalloonText"), TEXT("TalkBallon_ServerRank_Local_02"), findText);
			return FText::Format(findText, FText::FromString(_localUserName));
		}
		else
		{
			FText::FindText(TEXT("TalkBalloonText"), TEXT("TalkBallon_ServerRank_Local_01"), findText);
			return FText::Format(findText, FText::FromString(_localGuildName), FText::FromString(_localUserName));
		}
		break;
	}
	case EGsTalkBalloonDataType::RANK_WORLD_LEVEL_1ST:
	{
		FText findText;
		if (_worldGuildName.IsEmpty())
		{
			FText::FindText(TEXT("TalkBalloonText"), TEXT("TalkBallon_ServerRank_World_02"), findText);
			return FText::Format(findText, FText::FromString(_worldUserName));
		}
		else
		{
			FText::FindText(TEXT("TalkBalloonText"), TEXT("TalkBallon_ServerRank_World_01"), findText);
			return FText::Format(findText, FText::FromString(_worldGuildName), FText::FromString(_worldUserName));
		}
		break;
	}
	case EGsTalkBalloonDataType::MARKET_PRICE_TOP:
	{
		const FGsSchemaItemCommon* itemData = UGsItemManager::GetItemTableDataByTID(_highestPriceItemId);
		if (itemData)
		{
			return FText::Format(_talkBalloonData->talkBalloonDataSetList[InIndex].text, itemData->name);
		}
		break;
	}
	case EGsTalkBalloonDataType::MARKET_WEAPON_GRADE_TOP:
	{
		const FGsSchemaItemCommon* itemData = UGsItemManager::GetItemTableDataByTID(_highestGradeItemId);
		if (itemData)
		{
			return FText::Format(_talkBalloonData->talkBalloonDataSetList[InIndex].text, itemData->name);
		}
		break;
	}
	case EGsTalkBalloonDataType::ACHIEVEMENT_LOGIN_ACCUMULATE:
	{	 
		if(auto achievementData = GSAchivement()->FindAchievementDataByConditionType(AchievementType::LOGIN_ACCUMULATE).Pin())
		{
			uint64 accessDate = achievementData->CurrentPoint();
			return FText::Format(_talkBalloonData->talkBalloonDataSetList[InIndex].text, FText::AsNumber(accessDate));
		}
		break;
	}
	case EGsTalkBalloonDataType::ACHIEVEMENT_CREATE_DATE:
	{
		const FGsNetUserData* userData = GGameData()->GetUserData();

		FTimespan createdTime = FGsTimeSyncUtil::ConvertToDateTime(userData->_createdTime) - FDateTime(0);
		FTimespan currentTime = FGsTimeSyncUtil::GetServerNowDateTime() - FDateTime(0);

		int32 diffDay = currentTime.GetDays() - createdTime.GetDays() + 1;

		return FText::Format(_talkBalloonData->talkBalloonDataSetList[InIndex].text, FText::FromString(userData->mUserName), FText::AsNumber(diffDay));
	}
	}
	return _talkBalloonData->talkBalloonDataSetList[InIndex].text;
}

EGsTalkBalloonState UGsUIBillboardTalkBalloon::GetStateEnumFromString(uint8 InState)
{
	FString InStrState = GetEnumToString(EGsStateBase, EGsStateBase(InState));
	int32 NewDoubleColonIndex = InStrState.Find(TEXT("::"), ESearchCase::CaseSensitive);
	if (NewDoubleColonIndex != INDEX_NONE)
	{
		InStrState = InStrState.RightChop(NewDoubleColonIndex + 2);
	}

	for (uint8 i = 0; i < (uint8)EGsTalkBalloonState::MAX; ++i)
	{
		FString strState = GetEnumToString(EGsTalkBalloonState, EGsTalkBalloonState(i));
		NewDoubleColonIndex = strState.Find(TEXT("::"), ESearchCase::CaseSensitive);
		if (NewDoubleColonIndex != INDEX_NONE)
		{
			strState = strState.RightChop(NewDoubleColonIndex + 2);
		}

		if (strState.Compare(InStrState) == 0)
		{
			return EGsTalkBalloonState(i);
		}
	}
	
	return EGsTalkBalloonState::MAX;
}

void UGsUIBillboardTalkBalloon::RequestServerData(int32 InIndex)
{
	if (_talkBalloonData)
	{
		switch (_talkBalloonData->talkBalloonDataSetList[_currentIndex].dataSetType)
		{
		case EGsTalkBalloonDataType::RANK_LOCAL_LEVEL_1ST:
		case EGsTalkBalloonDataType::RANK_WORLD_LEVEL_1ST:
			_isRequestingData = true;
			FGsNetSendService::SendReqCommunityTalkRank();
			return;
		case EGsTalkBalloonDataType::MARKET_PRICE_TOP:
		case EGsTalkBalloonDataType::MARKET_WEAPON_GRADE_TOP:
			_isRequestingData = true;
			FGsNetSendService::SendReqCommunityTalkMarket();
			return;
		}
	}
}

void UGsUIBillboardTalkBalloon::SetNextDataByIndex(int32 InIndex)
{
	_remainKeepTime = _talkBalloonData->talkBalloonDataSetList[InIndex].keepTime;
	_remainIntervalTime = _talkBalloonData->talkBalloonDataSetList[InIndex].intervalTime;
	SetText(GetNextText(InIndex));
}

void UGsUIBillboardTalkBalloon::ResetRandomIndexList()
{
	int32 dataSize = _displayIndexListMap[_currentState].Num();
	if (0 == dataSize)
		return;

	_randomIndexList.Empty();
	for (int32 i = 0; i < dataSize; ++i)
	{
		_randomIndexList.Emplace(_displayIndexListMap[_currentState][i]);
	}

	// Shuffle
	FRandomStream rand;
	rand.GenerateNewSeed();
	for (int32 i = 0; i < dataSize; ++i)
	{
		int32 randomPosition = rand.RandHelper(dataSize);
		int32 temp = _randomIndexList[i];
		_randomIndexList[i] = _randomIndexList[randomPosition];
		_randomIndexList[randomPosition] = temp;
	}
}

void UGsUIBillboardTalkBalloon::InitDisplayIndexList()
{
	if (_talkBalloonData)
	{
		_displayIndexListMap.Empty();
		EGsTalkBalloonState status = EGsTalkBalloonState::None;
		for (uint8 i = 0; i < _talkBalloonData->talkBalloonDataSetList.Num(); ++i)
		{
			status = _talkBalloonData->talkBalloonDataSetList[i].status;
			if (_displayIndexListMap.Contains(status))
			{
				_displayIndexListMap[status].Emplace(i);
			}
			else
			{
				_displayIndexListMap.Emplace(status, TArray<int32>());
				_displayIndexListMap[status].Emplace(i);
			}
		}
	}
}

float UGsUIBillboardTalkBalloon::ComputeDesiredSize(const FText& InText) const
{
	const FVector2D LocalShadowOffset = _textBlockTalk->ShadowOffset;
	const float LocalOutlineSize = _textBlockTalk->Font.OutlineSettings.OutlineSize;

	const FVector2D ComputedOutlineSize(LocalOutlineSize * 2, LocalOutlineSize);
	if (FSlateRenderer* renderer = FSlateApplication::Get().GetRenderer())
	{		
		const FVector2D TextSize = renderer->GetFontMeasureService()->Measure(InText, _textBlockTalk->Font) + ComputedOutlineSize + LocalShadowOffset;
		return TextSize.X;
	}

	auto MinDesiredWidth = _textBlockTalk->GetDesiredSize();
	return MinDesiredWidth.X;
}

void UGsUIBillboardTalkBalloon::DrawDebugTalkBalloonRange()
{
	if (nullptr == _attachedActor)
		return;

	UWorld* world = GetWorld();
	if (world)
	{
		// 말풍선 출력 거리
		DrawDebugCircle(world, _attachedActor->GetActorLocation(),
			_talkBalloonData->talkRange * METER_TO_CM, 100, FColor::Blue, false, 1.f,
			0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));

		// PC가 NPC와 멀어질 때 말풍선이 보이는 거리
		DrawDebugCircle(world, _attachedActor->GetActorLocation(),
			_talkBalloonData->talkRange * METER_TO_CM + _additionalDist, 100, FColor::Green, false, 1.f,
			0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
	}
}

void UGsUIBillboardTalkBalloon::SetDrawDebugTalkBalloonRange(bool InVisible)
{
	_isDebuging = InVisible;
}

void UGsUIBillboardTalkBalloon::OnReceiveCommunityRank(const IGsMessageParam* InParam)
{
	const FGsTalkBalloonRankMessageParam* param = InParam->Cast<const FGsTalkBalloonRankMessageParam>();
	if (param)
	{
		_localGuildName = param->_localGuildName;
		_localUserName = param->_localUserName;
		_worldGuildName = param->_worldGuildName;
		_worldUserName = param->_worldUserName;
	}

	UpdateData(TalkBalloonUpdateType::COMMUNITY);
}

void UGsUIBillboardTalkBalloon::OnReceiveCommunityMarket(const IGsMessageParam* InParam)
{
	const FGsTalkBalloonMarketMessageParam* param = InParam->Cast<const FGsTalkBalloonMarketMessageParam>();
	if (param)
	{
		_highestPriceItemId = param->_highestPriceItemId;
		_highestGradeItemId = param->_highestGradeItemId;
	}

	UpdateData(TalkBalloonUpdateType::COMMUNITY);
}

void UGsUIBillboardTalkBalloon::OnHit()
{
	UpdateData(TalkBalloonUpdateType::HIT);
}
