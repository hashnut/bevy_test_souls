// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIPopupRevive.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Net/GsNetSendServiceWorld.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "UTIL/GsTableUtil.h"

#include "GameObject/Npc/GsSchemaNpcData.h"

#include "Guild/GsGuildHelper.h"

#include "TextBlock.h"
#include "Widget.h"
#include "AssertionMacros.h"
#include "Image.h"
#include "PaperSprite.h"
#include "Classes/Engine/Texture2D.h"
#include "Core/Public/Misc/Timespan.h"

#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Common/GsUICurrencySlot.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"


#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Resurrect/GsSchemaResurrectData.h"
#include "DataSchema/Resurrect/GsSchemaResurrectionCost.h"
#include "DataSchema/Resurrect/GsSchemaResurrectConfig.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "PKBook/GsPKBookHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Net/GsNetSendServiceGuild.h"


const FTextKey UI_COMMON_TEXT = TEXT("UICommonText");
const FTextKey RESURRECT_TEXT_NAME = TEXT("ResurrectionText");
const FTextKey RESURRECT_NOW_TEXT_KEY = TEXT("Resurrection_Now");
const FTextKey ATTACKER = TEXT("Attacker");
const FTextKey REMAIN_RECOVERY_TIME = TEXT("Resurrection_Remain_Time");
const FTextKey POPUP_REVIVE_CONTENT = TEXT("PopupReviveContent");
const FTextKey INVADE_ALLOW_TIME_HOUR_TEXT_KEY = TEXT("InvadeAllowTime_Hour");
const FTextKey INVADE_ALLOW_TIME_MINUT_TEXT_KEY = TEXT("InvadeAllowTime");
const FTextKey INVADE_ALLOW_TIME_SECOND_TEXT_KEY = TEXT("InvadeAllowTime_Second");

void UGsUIPopupRevive::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnReviveTown->OnClicked.AddDynamic(this, &UGsUIPopupRevive::OnClickReviveTown);
	_btnReviveField->OnClicked.AddDynamic(this, &UGsUIPopupRevive::OnClickReviveField);
	_btnReviveInDungeon->OnClicked.AddDynamic(this, &UGsUIPopupRevive::OnClickInDungeon);
	_btnReviveImmediatly->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupRevive::OnClickReviveImmediatly);	
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupRevive::OnClickOk);
	_btnReviveLastLocation->OnClicked.AddDynamic(this, &UGsUIPopupRevive::OnClickReviveLastPosition);
	_btnObserving->OnClicked.AddDynamic(this, &UGsUIPopupRevive::OnClickObserving);
	_btnInvasionOut->OnClicked.AddDynamic(this, &UGsUIPopupRevive::OnClickInvasionOut);

	_buttonArray.Empty();
	_buttonArray.Add(_btnReviveTown);
	_buttonArray.Add(_btnReviveField);
	_buttonArray.Add(_btnReviveInDungeon);
	_buttonArray.Add(_immediateResurrectionWidget);
	_buttonArray.Add(_btnOk);
	_buttonArray.Add(_btnReviveLastLocation);
	_buttonArray.Add(_btnObserving);
	_buttonArray.Add(_btnInvasionOut);
}

void UGsUIPopupRevive::NativeConstruct()
{
	Super::NativeConstruct();

	// 주의: 텍스트 찾아놓는 것을 NativeOnInitialized 에서 하면 언어변경 시 로컬라이즈 대응 안됨

	// TEXT: 플레이어가 사망하였습니다.
	FText::FindText(UI_COMMON_TEXT, POPUP_REVIVE_CONTENT, _contentText);

	//사망 부활 시간 
	FText::FindText(RESURRECT_TEXT_NAME, REMAIN_RECOVERY_TIME, _timeFormat);
		//공격자 이름
	FText::FindText(RESURRECT_TEXT_NAME, ATTACKER, _attackNameText);

	//경험치 패널티
	FText::FindText(RESURRECT_TEXT_KEY, LOSS_EXP_AMOUNT_TEXT_KEY, _lossExpText);	
	FText::FindText(RESURRECT_TEXT_KEY, RESURRECT_NOW_TEXT_KEY, _resurrectNowText);

	FText::FindText(INVADE_TEXT_KEY, INVADE_ALLOW_TIME_HOUR_TEXT_KEY, _hourMinutSecondText);
	FText::FindText(INVADE_TEXT_KEY, INVADE_ALLOW_TIME_MINUT_TEXT_KEY, _minutSecondText);
	FText::FindText(INVADE_TEXT_KEY, INVADE_ALLOW_TIME_SECOND_TEXT_KEY, _secondText);

	FText::FindText(TEXT("GuildText"), TEXT("Dungeon_Observating_NoUser"), _dungeonObservatingNoUserText);

	FText::FindText(TEXT("GuildText"), TEXT("GuildDungeon_Death_CountDown"), _guildDungeonDeathKickWaitTextFormat);

	//길드 던전이라면 길드원이 죽을 때 갱신해야한다. 
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (gameFlow)
	{
		FGsStageManagerGame* stage = gameFlow->GetStageManagerGame();
		if (stage)
		{
			if (stage->GetCurrentStageMode() == FGsStageMode::Game::GUILD_DUNGEON)
			{
				_invalidObservingEvent = GMessage()->GetGameObject().AddUObject(MessageGameObject::REMOTEPLAYER_DIE, this, &UGsUIPopupRevive::OnInvalidRevive);

			}
		}
	}

	//즉시 부활 버튼이 원래 강화하기 버튼에 사용되는거라 텍스트 수정이 필요
	_btnReviveImmediatly->SetButtonName(_resurrectNowText);
	
	//icon
	InitIconList();
	HideAllPenalty();
	HideAllButtons();

#if RESURRECT_DEBUG
	GSLOG(Log, TEXT("%s"), TEXT("PopupRevive - NativeConstruct"));
#endif
}

void UGsUIPopupRevive::NativeDestruct()
{

#if RESURRECT_DEBUG
	GSLOG(Log, TEXT("%s"), TEXT("PopupRevive - NativeDestruct"));
#endif

	// 부활 요청 막았던 코드 풀기
	ClearLockAll();
	ClearIconList();

	if (_invalidObservingEvent.Value.IsValid())
	{
		GMessage()->GetGameObject().Remove(_invalidObservingEvent);
	}		

	
	_immediateResurrectionTimer.Stop(GetWorld());
	_immediateResurrectionTimer.Clear();

	_waitTimer.Stop(GetWorld());
	_waitTimer.Clear();

	Super::NativeDestruct();
}

void UGsUIPopupRevive::NativeTick(const FGeometry& inMyGeometry, float inDeltaTime)
{
	Super::NativeTick(inMyGeometry, inDeltaTime);

	if (_isWaitDeathKick == true)
	{
		int64 nowTick = FDateTime::UtcNow().GetTicks();
		int64 diffTick = nowTick - _waitDeathKickStartTime;

		int diffSecond = diffTick / ETimespan::TicksPerSecond;

		int remainTime = _waitDeathKickTimeSec - diffSecond;

		// 초가 바뀌었으면 ui에게 알림
		if (remainTime != _currentRemainDeathKickTimeSec)
		{
			// 저장
			_currentRemainDeathKickTimeSec = remainTime;
			

			SetDeathKickWaitRemainTime(remainTime);
		}


		if (diffTick >= _waitDeathKickTimeTick)
		{
			_isWaitDeathKick = false;
			// 클라에서 나가게 처리
			FGsNetSendServiceGuild::SendReqExitGuildDungeon();
		}
	}
}

bool UGsUIPopupRevive::OnBack()
{
	// 뒤로가기로 못닫게 처리. 뒤로가기 누르면 게임 닫기 출력
	return false;
}

void UGsUIPopupRevive::OnClickReviveTown()
{
	//길드 던전으로 워프 중 죽으면 사망으로 나오고 기본 UI가 나오는데 마을이 등록되어 있지 않기 때문에 에러가 난다. 
	//길드 던전맵이면 마지막 위치로 보내도록 수정
	if (GLevel()->IsGuildDungeonMap())
	{
		FGsNetSendServiceGuild::SendReqExitGuildDungeon();
		return;
	}

	FGsNetSendServiceWorld::SendResurrect(ResurrectType::TOWN);
}

void UGsUIPopupRevive::OnClickReviveField()
{
	// 주의: ResurrectType::GRAVE 타입은 사용하는 곳이 없다고 함
	FGsNetSendServiceWorld::SendResurrect(ResurrectType::GRAVE);
}

void UGsUIPopupRevive::OnClickInDungeon()
{
	// 던전 안 지정 스팟에서 부활. 길드 영지 던전에서 활용
	FGsNetSendServiceWorld::SendResurrect(ResurrectType::IN_DUNGEON);
}

void UGsUIPopupRevive::OnClickReviveImmediatly()
{
	FGsNetSendServiceWorld::SendResurrect(ResurrectType::INSTANT);
}

void UGsUIPopupRevive::OnClickReviveLastPosition()
{
	if (GLevel()->IsGuildDungeonMap())
	{
		FGsNetSendServiceGuild::SendReqExitGuildDungeon();
		return;
	}

	FGsNetSendServiceWorld::SendResurrect(ResurrectType::LASTPOSTION);
}

//관전이 가능한 방은 방에 있는 모든 유저 정보를 알 수 있다.
//즉 거리에 따라 타유저를 디스폰 하지 않는다.
//현재 상황은 길드 던전에만 해당한다.
//맵이 커져서 거리에 따라 유저를 스폰한다면 로직이 변경될 수 도 있으니 유의할 것
void UGsUIPopupRevive::OnClickObserving()
{
	TArray<UGsGameObjectBase*> remoteUserArray = GSGameObject()->FindObjectArray(EGsGameObjectType::RemotePlayer);
	if (0 == remoteUserArray.Num())
	{
		FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("ObservingExit")));
		//현재 유저가 없다고 티커 띄우고 
		FGsUIHelper::TrayMessageTicker(_dungeonObservatingNoUserText);

		//퇴장		
		FDateTime endTime = FGsTimeSyncUtil::GetClientNowDateTime() + FTimespan(0, 0, 1);
		_waitTimer.Start(GetWorld(), endTime, 0.5f
			, nullptr
			, [this]() {
				FGsNetSendServiceWorld::SendResurrect(ResurrectType::LASTPOSTION);
			});
	}
	else
	{
		FGsNetSendServiceWorld::SendResurrect(ResurrectType::OBSERVING);
		GMessage()->GetObserver().SendMessage(MessageContentObserver::START_OBSERVE, nullptr);
		// 그냥 닫기
		Close();
	}
}

void UGsUIPopupRevive::OnClickInvasionOut()
{
	FGsNetSendServiceWorld::SendResurrect(ResurrectType::INVASIONOUT);
}

void UGsUIPopupRevive::OnClickOk()
{
	FGsNetSendServiceWorld::SendUserDeathPenaltyCheck();

	//close
	GUI()->Close(this);
}

void UGsUIPopupRevive::ShowRevivePopup(bool inIsAutoRevive /*= false*/)
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	_isAutoRevive = inIsAutoRevive;

	OnInvalidRevive();
}

void UGsUIPopupRevive::InvalidAttackerName()
{
	if (nullptr == _userData)
	{
		return;
	}

	int32 tid = _userData->attackerNpcId;
	FText attackerName;
	FString attackerGuildName;
	GuildEmblemId guildEmblemId = INVALID_GUILD_EMBLEM_ID;
	
	if (tid > 0)
	{
#ifdef RESURRECT_DEBUG
		GSLOG(Log, TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::SetAttackerName - Attack npc id : %d"), tid);
#endif
		//npc		
		const FGsSchemaNpcData* npc = UGsTableUtil::FindRow<FGsSchemaNpcData>(tid);
		if (npc)
		{
			attackerName = npc->nameText;

#ifdef RESURRECT_DEBUG
			GSLOG(Log, TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::SetAttackerName - Attack npc name : %s"), *attackerName.ToString());
#endif
		}
	}
	else
	{
		//remote player
		attackerName = FText::FromString(FGsTextUtil::CombineUserAndPrefix(FText::FromString(_userData->attackerName), _userData->attackerWorldId));
		attackerGuildName = _userData->attackerGuildName;
		guildEmblemId = _userData->attackerGuildEmblemId;
#ifdef RESURRECT_DEBUG
		GSLOG(Log, TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::SetAttackerName - Attack player name : %s\tguild name : %s"), *attackerName.ToString(), *attackerGuildName);
#endif
	}
	
	FText resultText = FText::Format(_attackNameText, attackerName);
	_attackerNameText->SetText(resultText);

	if (false == attackerGuildName.IsEmpty())
	{
		_guildSection->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_guildNameText->SetText(FText::FromString(attackerGuildName));

#ifdef RESURRECT_DEBUG
		GSLOG(Log, TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::SetAttackerName - guild name : %s"), *attackerGuildName);
#endif
	}
	else
	{
		_guildSection->SetVisibility(ESlateVisibility::Collapsed);
		_guildNameText->SetText(FText::FromString(""));

#ifdef RESURRECT_DEBUG
		GSLOG(Log, TEXT("%s"), TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::SetAttackerName - Hide guild in revive"));
#endif
	}

	FSoftObjectPath path;
	if (true == FGsPKBookHelper::FindPathGuildIcon(guildEmblemId, true, path))
	{
		_imgGuildEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_imgGuildEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(path));
	}
	else
	{
		_imgGuildEmblem->SetVisibility(ESlateVisibility::Collapsed);
	}	
}

void UGsUIPopupRevive::InvalidInternal()
{
	_isActiveImmediateResurrectButton = false;
	if (false == _isAutoRevive)
	{
		_isActiveImmediateResurrectButton = _resurrectTypeList.Contains(ResurrectType::INSTANT);
	}	

	HideAllPenalty();
	HideAllButtons();

	//info
	InvalidAttackerName();
	InvalidPenalty();
	InvalidButtons();	
}

void UGsUIPopupRevive::InvalidPenalty()
{
	//자동 부활시
	if (_isAutoRevive)
	{
		ShowPenaltyPanel();
		InvalidExpPenalty();
		InvalidItemPenalty();
	}
	else
	{
		for (ResurrectType resurrectType : _userData->resurrectTypeList)
		{
			InvalidPenaltyInternal(resurrectType);
		}
	}
}

void UGsUIPopupRevive::InvalidExpPenalty()
{
	const DeathPenaltyExp& expPenalty = _userData->recentPenaltyExp;
	
	float penaltyPercent = 0;
	FText::FindText(RESURRECT_TEXT_KEY, LOSS_EXP_AMOUNT_TEXT_KEY, _lossExpText);
	if (expPenalty.mExp > 0)
	{
		penaltyPercent = expPenalty.mCurrentExp != 0 ? (((float)expPenalty.mExp / (float)expPenalty.mCurrentExp) * 100.0f) : 0;
	}

	FText penaltyText = FText::Format(_lossExpText, expPenalty.mExp, penaltyPercent);
	_paneltyText->SetText(penaltyText);
}

void UGsUIPopupRevive::InvalidItemPenalty()
{
	TArray<DeathPenaltyItem> penaltyList = _userData->recentPenaltyList;
	for (TWeakObjectPtr<UGsHorizontalBoxIconSelector>& selector : _itemIconSelectorList)
	{
		selector->SetVisibility(ESlateVisibility::Collapsed);
	}

	for (int32 i = 0; i < penaltyList.Num(); ++i)
	{
		ItemId tid = penaltyList[i].mDeathPenaltyItem.mTemplateId;
		TWeakObjectPtr<UGsHorizontalBoxIconSelector>& iconSelector = _itemIconSelectorList[i];
		if (iconSelector.IsValid())
		{
			iconSelector->SetVisibility(ESlateVisibility::Visible);
		}

		UGsUIIconItemInventory* icon = _itemIconList[i];
		if (icon)
		{
			icon->SetNotOwnItem(tid, 1);
			icon->SetItemEnchantLevel(penaltyList[i].mDeathPenaltyItem.mLevel);
			icon->SetSlotEquipedImage(false);
			icon->SetPresetImage(false);
			icon->SetItemNameVisibility(false);
			icon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
		}
	}
}

void UGsUIPopupRevive::InvalidButtons()
{
	//자동 부활시
	if (_isAutoRevive)
	{
		ShowOkButton(true);
	}
	else
	{
		for (ResurrectType type : _resurrectTypeList)
		{
			InvalidButtonInternal(type);
		}		
	}	
}

void UGsUIPopupRevive::ShowOkButton(bool inShow)
{
	if (inShow)
	{
		_btnOk->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}	
	else
	{
		_btnOk->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupRevive::ShowImmediateReviveButton(bool inShow)
{
	if (inShow)
	{
		_immediateResurrectionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);	
		_immediateResurrectionLeftTimeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		//hide immediate resurreciton button
		_immediateResurrectionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupRevive::ShowLastPositionReviveButton(bool inShow)
{
	if (inShow)
	{
		_btnReviveLastLocation->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_btnReviveLastLocation->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupRevive::ShowObservingButton(bool inShow)
{
	if (inShow)
	{
		_btnObserving->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_btnObserving->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupRevive::ShowInvasionButton(bool inShow)
{
	if (inShow)
	{		
		_btnInvasionOut->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{		
		_btnInvasionOut->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupRevive::HideAllPenalty()
{
	ShowPenaltyPanel(false);	
}

void UGsUIPopupRevive::InvalidImmediateReviveButton()
{	
	if (nullptr == _userData)
	{
		GSLOG(Error, TEXT("%s"), TEXT("Revive user data is missing"));
		return;
	}

	_btnReviveImmediatly->SetData(_userData->immediateResurrectionCurrencyType, _userData->immediateResurrectionCost);	

	uint64 curValue = GGameData()->GetCurrency(_userData->immediateResurrectionCurrencyType);
	bool isEnoughDia = curValue >= _userData->immediateResurrectionCost;
	EGsUIColorType color = isEnoughDia ? EGsUIColorType::ENCHANT_ENOUGH_COST : EGsUIColorType::ENCHANT_NEED_COST;
	_btnReviveImmediatly->UpdateAmountTextColorLocalPlayer();
	_btnReviveImmediatly->SetAmountTextColor(color);

	_btnReviveImmediatly->SetIsEnabled(true);
	_immediateResurrectionLeftTimeWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	_isShowImmediateResurrectionTime = true;

	//if dia enough 
	if (isEnoughDia)
	{
		//active button, show remain time
		_btnReviveImmediatly->SetIsEnabled(true);		
		_isShowImmediateResurrectionTime = true;
		_immediateResurrectionLeftTimeWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		//diactive instance resurreciton button, hide remain time
		_btnReviveImmediatly->SetIsEnabled(false);
		_immediateResurrectionLeftTimeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

//즉시 부활 초기화
void UGsUIPopupRevive::InitImmediateResurrectionButton()
{	
	if (_isActiveImmediateResurrectButton)
	{
		//show immediate resurreciton button
		ShowImmediateReviveButton(true);		

		const FGsNetUserData* userData = GGameData()->GetUserData();
		if (nullptr == userData)
		{
			return;
		}
		
		FDateTime endTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(userData->InstantResurrectionSecond);
		FTimespan remainTime = endTime - FGsTimeSyncUtil::GetClientNowDateTime();
		_btnReviveImmediatly->SetData(_userData->immediateResurrectionCurrencyType, 0);

#ifdef RESURRECT_DEBUG
		GSLOG(Log, TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::InitImmediateResurrectionButton - immediate - remain time hour : %d\tmin : %d\tsecond : %d")
		, remainTime.GetHours()
		, remainTime.GetMinutes()
		, remainTime.GetSeconds());
		GSLOG(Log, TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::InitImmediateResurrectionButton - immediate - InstantResurrectionSecond date : %s")
			, *endTime.ToString());
		GSLOG(Log, TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::InitImmediateResurrectionButton - immediate - current server date : %s")
			, *FGsTimeSyncUtil::GetServerNowDateTime().ToString());
#endif
										
		if (remainTime.GetTotalSeconds() > 0)
		{
			//timer		
			_immediateResurrectionTimer.Start(GetWorld(), endTime, 0.5f
				, [this]() {InvalidImmediateResurrectionTime();}
				, [this]() {DisableImmediateResurrectionButton();});

			_isShowImmediateResurrectionTime = true;

			InvalidImmediateReviveButton();
		}
		else
		{			
			DisableImmediateResurrectionButton();
		}		
	}
	else
	{
		_isShowImmediateResurrectionTime = false;

		//hide immediate resurreciton button
		ShowImmediateReviveButton(false);
	}
}

void UGsUIPopupRevive::InvalidButtonInternal(ResurrectType inType, bool inShow)
{
	switch (inType)
	{
	case ResurrectType::GRAVE:
	{
		_btnReviveField->SetVisibility(inShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	break;
	case ResurrectType::TOWN:
	{
		_btnReviveTown->SetVisibility(inShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	break;
	case ResurrectType::INSTANT:
	{
		InitImmediateResurrectionButton();

		if (_immediateResurrectionWidget)
		{
			ShowImmediateReviveButton(inShow);
		}
	}
	break;
	case ResurrectType::LASTPOSTION:
	{
		if (_btnReviveLastLocation)
		{
			ShowLastPositionReviveButton(inShow);
		}
	}
	break;	
	case ResurrectType::OBSERVING:
	{		
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		if (nullptr == gameFlow)
		{		
			return;
		}

		FGsStageManagerGame* stage = gameFlow->GetStageManagerGame();
		if (nullptr == stage)
		{			
			return;
		}

		//만약 플레이어가 죽었고 타유저도 다 죽었다면 비활성화
		bool isPlayerDead = GSGameObject()->IsLocalPlayerDead();
		bool isOtherPlayerDead = GSGameObject()->IsAllRemotePlayerDead();
		bool isGuildDungeon = (stage->GetCurrentStageMode() == FGsStageMode::Game::GUILD_DUNGEON);

		if (isGuildDungeon)
		{
			if (isPlayerDead)
			{
				if (isOtherPlayerDead)
				{
					ShowObservingButton(false);
				}
				else
				{
					ShowObservingButton(inShow);					
				}
			}
		}	
		else
		{
			ShowObservingButton(inShow);
		}
	}
	break;
	case ResurrectType::INVASIONOUT:
	{		
		ShowInvasionButton(inShow);
	}
	break;
	case ResurrectType::IN_DUNGEON:
	{
		_btnReviveInDungeon->SetVisibility(inShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	break;
	}

	//옵저버 버튼 추가해야함
}

void UGsUIPopupRevive::ClearLockAll()
{
	// 부활 요청 막았던 코드 풀기
	_btnReviveTown->ClearLock();
	_btnReviveField->ClearLock();
	_btnReviveInDungeon->ClearLock();
}

void UGsUIPopupRevive::HideAllButtons()
{
	for (UWidget* buttonWidget : _buttonArray)
	{
		buttonWidget->SetVisibility(ESlateVisibility::Collapsed);
	}	
}

void UGsUIPopupRevive::InitIconList()
{
#ifdef RESURRECT_DEBUG
	GSLOG(Log, TEXT("%s"), TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::InitIconList - InitIconList"));
#endif

	_itemIconSelectorList.Empty();
	_itemIconList.Empty();

	_itemIconSelectorList.Add(_itemIconSelector1);
	_itemIconSelectorList.Add(_itemIconSelector2);
	_itemIconSelectorList.Add(_itemIconSelector3);

	for (int i = 0; i < _itemIconSelectorList.Num(); ++i)
	{
		UGsHorizontalBoxIconSelector* selector = _itemIconSelectorList[i].Get();
		UGsUIIconItemInventory* icon = Cast<UGsUIIconItemInventory>(selector->CreateIcon());
		if (icon)
		{
			selector->AddChild(icon);
			icon->SetEmptyIcon();
			icon->SetEmptyStateBG(true);
			icon->SetSlotEquipedImage(false);
			icon->SetPresetImage(false);
			icon->SetItemNameVisibility(false);			
			_itemIconList.Add(icon);
		}
	}
}

void UGsUIPopupRevive::ClearIconList()
{
	_itemIconList.Empty();

	for (TWeakObjectPtr<UGsHorizontalBoxIconSelector>& selector : _itemIconSelectorList)
	{
		if (selector.IsValid())
		{
			selector.Get()->ClearChildren();
			selector.Get()->RemoveAllChildren();
		}
	}

	_itemIconSelectorList.Empty();

#ifdef RESURRECT_DEBUG
	GSLOG(Log, TEXT("%s"), TEXT("RESURRECT_DEBUG - UGsUIPopupRevive::ClearIconList - ClearIconList"));
#endif
}


void UGsUIPopupRevive::InvalidPenaltyInternal(ResurrectType inType)
{
	switch (inType)
	{
	//일반 필드
	case ResurrectType::NOW:		
	case ResurrectType::GRAVE:		
	case ResurrectType::TOWN:		
	case ResurrectType::INSTANT:
	case ResurrectType::IN_DUNGEON:
	{
		ShowPenaltyPanel(true);		
		InvalidExpPenalty();
		InvalidItemPenalty();
	}
		break;
	}
}

void UGsUIPopupRevive::OnInvalidRevive(const IGsMessageParam* inParam)
{
	_userData = GGameData()->GetUserData();
	if (nullptr == _userData)
	{
		GSLOG(Error, TEXT("Revive user data is missing"));
		return;
	}

	_resurrectTypeList = _userData->resurrectTypeList;

	// 2023/10/18 PKT - 특정 상황에 사망 정보는 침공전(타서버)에서 받고 UI는 본서버에서 띄우게 되면 해당 버튼을 바꿔 줘야 한다.
	for (auto& findType : _resurrectTypeList)
	{
		if (findType == ResurrectType::INVASIONOUT)
		{			
			if (false == GGameData()->IsInvadeWorld())
			{
				_resurrectTypeList.Remove(ResurrectType::INVASIONOUT);
			}
			break;
		}
	}
	
	//서버에서 부활 옵션 타입아 오지 않고 침공전일 경우 침공 부활 버튼으로 기본 설정 
	if (0 == _resurrectTypeList.Num())
	{
		if (GGameData()->IsInvadeWorld())
		{
			_resurrectTypeList.Add(ResurrectType::INVASIONOUT);
			GSLOG(Error, TEXT("resurrect type list is empty, set invade revive default"));
		}
		else
		{
			_resurrectTypeList.Add(ResurrectType::TOWN);
			GSLOG(Error, TEXT("resurrect type list is empty, set town revive default"));
		}
	}
	
	InvalidInternal();
}

void UGsUIPopupRevive::DisableImmediateResurrectionButton()
{	
	_immediateResurrectionTimer.Stop(GetWorld());
	_immediateResurrectionTimer.Clear();
	_isShowImmediateResurrectionTime = false;

	//show button,  but disable, hide left time

	_immediateResurrectionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_btnReviveImmediatly->SetIsEnabled(false);
	_immediateResurrectionLeftTimeWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIPopupRevive::InvalidImmediateResurrectionTime()
{
	if (false == _isShowImmediateResurrectionTime)
	{
		return;
	}
	
	const FText& leftTimeText = FText::Format(_timeFormat
		, (int32)_immediateResurrectionTimer.GetRemainTime().GetTotalSeconds());

	//leftTime.mi
	_immediateResurrectLeftTimeText->SetText(leftTimeText);
}

void UGsUIPopupRevive::ShowPenaltyPanel(bool inShow)
{
	if (_penaltyPanel)
	{
		if (inShow)
		{
			_penaltyPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			_penaltyPanel->SetVisibility(ESlateVisibility::Hidden);
		}			
	}
}
/// <summary>
/// 외부 입력 부활 타입
/// </summary>
void UGsUIPopupRevive::ShowPopupByExternalResurrectTypes(TArray<ResurrectType> In_arrResurrectType)
{
	_userData = GGameData()->GetUserData();
	if (nullptr == _userData)
	{
		GSLOG(Error, TEXT("%s"), TEXT("Revive user data is missing"));
		return;
	}

	_resurrectTypeList = In_arrResurrectType;
	
	InvalidInternal();	
}

// 마지막 위치 버튼만
void UGsUIPopupRevive::ShowPopupLastPosition()
{
	ShowPopupByExternalResurrectTypes({ ResurrectType::LASTPOSTION });
}


// 길드 던전 사망시 퇴장 시간 표시 보여줄지
// https://jira.com2us.com/jira/browse/CHR-24638
void UGsUIPopupRevive::ShowGuildDungeonKickRemainTimeText(bool In_show)
{
	ESlateVisibility newVisible = 
		(In_show == true)? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_guildDungeonKickRemainTimeText->SetVisibility(newVisible);

	if (true == In_show)
	{
		_isWaitDeathKick = true;
		_waitDeathKickStartTime = FDateTime::UtcNow().GetTicks();

		if (const FGsSchemaGuildConfig* guildConfig = FGsGuildHelper::GetGuildConfig())
		{
			// 대기 시간 틱 계산 해 놓기
			_waitDeathKickTimeTick = guildConfig->guildDungeonDeathKickSec * ETimespan::TicksPerSecond;
			// 대기 시간(초)
			_waitDeathKickTimeSec = guildConfig->guildDungeonDeathKickSec;
			_currentRemainDeathKickTimeSec = guildConfig->guildDungeonDeathKickSec;

			// 텍스트 세팅	
			SetDeathKickWaitRemainTime(_currentRemainDeathKickTimeSec);
		}
	}
	else
	{
		_isWaitDeathKick = false;
	}
}

// 남은시간 표시
void UGsUIPopupRevive::SetDeathKickWaitRemainTime(int In_remainSec)
{
	FString strTime;
	FTimespan timeData = FTimespan::FromSeconds(In_remainSec);
	FGsTimeStringHelper::GetTimeString(timeData, strTime);


	FText resultText = FText::Format(_guildDungeonDeathKickWaitTextFormat, FText::FromString(strTime));
	_guildDungeonKickRemainTimeText->SetText(resultText);
}