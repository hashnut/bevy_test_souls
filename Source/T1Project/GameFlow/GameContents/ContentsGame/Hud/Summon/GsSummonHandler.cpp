#include "GsSummonHandler.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"

#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageInput.h"

#include "ActorEx/GsActorSummonComposeSceneObject.h"
#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "DataSchema/GsSchemaEnums.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsGameClientVersion.h"

#include "UI/UILib/Define/GsUIDefine.h"

#include "UI/UIContent/Tray/GsUITraySummon.h"
#include "UI/UIContent/Tray/GsUITrayNavigationBar.h"

#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDMainMenu.h"

#include "UI/UIContent/Window/BMShop/GsUIWindowBMShop.h"

#include "ActorEx/GsActorSummonSceneObject.h"

#include "Fairy/GsFairyData.h"
#include "Costume/GsCostumeData.h"

#include "Summon/GsSummonDefine.h"
#include "Summon/GsSummonCardData.h"
#include "Summon/GsSummonFunc.h"

#include "Sound/GsSoundPlayer.h"
#include "Sound/GsSoundMixerController.h"

#include "Currency/GsCurrencyHelper.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "Net/GsNetSendServiceBMShop.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateFairy.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"

#include "Engine/Engine.h"

#include "GameFrameWork/PlayerController.h"


void FGsSummonHandler::Init()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		_msgSystemParamDelegates.Emplace(
			msg->GetSystemParam().AddRaw(MessageSystem::SEQUENCE_PLAYER_END,
				this, &FGsSummonHandler::OnStopMovie));

		_msgSystemParamDelegates.Emplace(
			msg->GetSystemParam().AddRaw(MessageSystem::SUMMON_COMPOSE_SKIP,
				this, &FGsSummonHandler::OnSummonComposeSkip));

		//https://jira.com2us.com/jira/browse/C2URWQ-4326
		_msgSystemNoParamDelegates.Emplace(
			msg->GetSystem().AddRaw(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS,
				this, &FGsSummonHandler::OnGameServerReconnectionSuccess));

		_msgGameObjectDelegates.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_DIE,
				this, &FGsSummonHandler::OnLocalPlayerDie));		

		_msgGameObjectDelegates.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_WARP,
				this, &FGsSummonHandler::OnLocalWarp));
	}
}

void FGsSummonHandler::Close()
{
	if (FGsMessageHolder* scopeHolder = GMessage())
	{
		if (_msgSystemParamDelegates.Num() != 0)
		{
			for (auto iter : _msgSystemParamDelegates)
			{
				scopeHolder->GetSystemParam().Remove(iter);
			}
			_msgSystemParamDelegates.Empty();
		}

		if (_msgSystemNoParamDelegates.Num() != 0)
		{
			for (auto iter : _msgSystemNoParamDelegates)
			{
				scopeHolder->GetSystem().Remove(iter);
			}
			_msgSystemNoParamDelegates.Empty();
		}

		if (_msgGameObjectDelegates.Num() != 0)
		{
			for (auto iter : _msgGameObjectDelegates)
			{
				scopeHolder->GetGameObject().Remove(iter);
			}
			_msgGameObjectDelegates.Empty();
		}

	}
}

// 소환 시작 - 단독으로 summon 가능하도록 주변 환경도 만들어준다.
void FGsSummonHandler::StartSummon(EGsSummonType In_summonType)
{
	_summonType = In_summonType;


	if (_isRetry == true)
	{
		_isRetry = false;
		RetrySummon();
	}		
	else
	{
		GSEventProgress()->OnReqStartEvent(EGsEventProgressType::SUMMON_ROOM, this);
	}
}

// 소환 종료
void FGsSummonHandler::EndSummon(bool Is_sendEventProgress)
{
	_isSummonPlay = false;

	if (GEngine == nullptr || GEngine->GameViewport == nullptr)
	{
		return;
	}


	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	FGsMessageHolder* msgHolder = GMessage();
	if(msgHolder == nullptr)
	{
		return;
	}

	UGsSummonManager* summonManager = GSSummon();
	if(summonManager == nullptr)
	{
		return;
	}
	FGsEventProgressManager* eventProgressManager = GSEventProgress();
	if (eventProgressManager == nullptr)
	{
		return;
	}

	// 시퀀스 플레이중이면 stop 요청
	if(summonManager->GetIsPlaySequence() == true)
	{
		summonManager->SetIsPlaySequence(false);
		msgHolder->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_STOP, nullptr);
	}
	

	// 아이템 일때는 나머지들 복구
	EGsSummonOriginContentType contentType =
		summonManager->GetSummonOriginContent();
	if (contentType  == EGsSummonOriginContentType::SUMMON_ITEM ||
		contentType == EGsSummonOriginContentType::SUMMON_RE_COMPOSE ||
		contentType == EGsSummonOriginContentType::SUMMON_EXCHANGE)
	{
		// 지형 처리 복구
		if (UGsLevelManager* levelManager = GLevel())
		{
			levelManager->UnHoldStreamingLevel();
		}

		RestoreJoystick();

		// ui 플래그 복구
		uiMgr->ClearHideFlags(EGsUIHideFlags::STATE_SUMMON);
		uiMgr->SetHUDMode(EGsUIHUDMode::NORMAL);

		// 기존에 켜져있으면 복구
		if (_isBeforeShowNavigationBarUI == true)
		{
			FName nameNav(TEXT("TrayNavigationBar"));			
			uiMgr->Open(nameNav);

			//FGsUIMsgParamInt paramCurrency(static_cast<int32>(_currentNavigationCurrency));
			//msgHolder->GetUI().SendMessage(MessageUI::SET_NAVIGATION_BAR_CURRENCY, &paramCurrency);
			// 재화들이 안나올때가 있다고 함
			// 재화 갱신할때 세진님이 제공한 함수 직접 호출하게 수정
			// https://jira.com2us.com/jira/browse/C2URWQ-5457

			if (true == uiMgr->IsActiveWidget(TEXT("WindowBMShop")))
			{
				TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(TEXT("WindowBMShop"));
				if (widget.IsValid())
				{
					TWeakObjectPtr<UGsUIWindowBMShop> bmShopWidget = Cast<UGsUIWindowBMShop>(widget);
					if (bmShopWidget.IsValid())
					{
						bmShopWidget->CheckExtraCurrency();
					}
				}
			}
		}
		
	}
	// 페어리 state일때
	else if (contentType == EGsSummonOriginContentType::SUMMON_COMPOSE ||
			 contentType == EGsSummonOriginContentType::SUMMON_WAIT_CHANGE ||
			 contentType == EGsSummonOriginContentType::SUMMON_CEILING )
	{
		// 현재 이벤트 상태를 복구한다(예외 처리)
		// 더 생기면 시스템화 필요		
		UGsUIHUDMainMenu* mainMenu = nullptr;
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(TEXT("HUDMain"), true);
		if (widget.IsValid())
		{
			if (UGsUIHUDFrameMain* mainWidget = Cast<UGsUIHUDFrameMain>(widget.Get()))
			{
				mainMenu = mainWidget->GetMainMenu();
			}
		}

		if (_summonType == EGsSummonType::SUMMON_FAIRY)
		{
			eventProgressManager->AddNextInfoFirstIndex(mainMenu, EGsEventProgressType::FAIRY_ROOM);
		}
		else if (_summonType == EGsSummonType::SUMMON_COSTUME)
		{
			eventProgressManager->AddNextInfoFirstIndex(mainMenu, EGsEventProgressType::COSTUME_ROOM);
		}
		
	}

	// 환경효과 이벤트 전달	
	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::GACHA_ROOM));
	msgHolder->GetGameObject().SendMessage(MessageGameObject::ENV_END_EVENT, &param);
	

	// 소환룸 삭제
	summonManager->DespawnSceneObject();

	summonManager->DespawnUnveilObjects();

	if (_sceneObject != nullptr)
	{
		_sceneObject = nullptr;
	}


	if (_traySummon != nullptr)
	{
		_traySummon = nullptr;
	}

	// 정렬 데이터 삭제	
	summonManager->ClearCardOrderData();
	

	_localData = nullptr;


	if (Is_sendEventProgress == true)
	{		
		eventProgressManager->OnReqFinishEvent(EGsEventProgressType::SUMMON_ROOM);		
	}

	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		msgHolder->GetFairy().SendMessage(MessageFairy::SUMMON_COMPLETE, nullptr);
	}
	else if (_summonType == EGsSummonType::SUMMON_COSTUME)
	{
		msgHolder->GetCostume().SendMessage(MessageCostume::SUMMON_COMPLETE, nullptr);
	}

	// B1 issue: 
	// 1. fairy(or costume) room open
	// 2. compose -> summon room open
	// 3. input close key(mapping key)
	// 4. remain tray summon~ ui(bug)	
	// block input
	//bool isLockInput = false;
	//FGsInputEventMsgBase msg;
	//msg.Id = (isLockInput == true) ? 1 : 0;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_SUMMON, false);
	msgHolder->GetInput().SendMessage(MessageInput::BlockInput, msg);

	// 사운드 클리어
	ChangeSoundSummonMixer(false);

	CloseAllSummonUI();

	// https://jira.com2us.com/jira/browse/C2URWQ-4326
	// 뽑기에서 망단절후 
	// 재연결시 _isRetry 값이 초기화 안되면 
	// 다시 뽑기시 처음에 세팅 되어야 할 ui flag를
	// 세팅하지 않아서 인게임 hud가 뽑기 시퀀스에서 출력됨
	// retry flag clear하기
	_isRetry = false;
}


// 소환 정보 세팅
void FGsSummonHandler::SetSummonFairyInfo(TArray<FairyId> In_arrId)
{

	TArray<TSharedPtr<FGsFairyData>> arrFairyData;
	for (FairyId id: In_arrId)
	{
		const TWeakPtr<FGsFairyData> findData =
			GSFairy()->GetFairyData(id);

		if (TSharedPtr<FGsFairyData> data = findData.Pin())
		{
			arrFairyData.Add(data);
		}
	}

	GSSummon()->SetSummonFairyData(arrFairyData);

	// check compose success
	EGsSummonOriginContentType contentType = GSSummon()->GetSummonOriginContent();
	if(contentType == EGsSummonOriginContentType::SUMMON_COMPOSE ||
		contentType == EGsSummonOriginContentType::SUMMON_RE_COMPOSE ||
		contentType == EGsSummonOriginContentType::SUMMON_CEILING ||
		contentType == EGsSummonOriginContentType::SUMMON_EXCHANGE)
	{
		bool isComposeSuccess = false;
		int tryGradeIntVal =(int)GSSummon()->GetFairySummonComposeTryGrade();
		for(int i=0; i< arrFairyData.Num(); ++i)
		{
			TSharedPtr<FGsFairyData> data = arrFairyData[i];
			if(true == data.IsValid())
			{
				if((int)data->GetFairyGrade() > tryGradeIntVal)
				{
					isComposeSuccess = true;
					break;
				}
			}
		}

		GSSummon()->SetIsComposeSuccess(isComposeSuccess);
	}

}
// 소환 정보 세팅(코스튬)
void FGsSummonHandler::SetSummonCostumeInfo(TArray<CostumeId> In_arrId)
{
	TArray<TSharedPtr<FGsCostumeData>> arrCostumeData;
	for (CostumeId id : In_arrId)
	{
		const TWeakPtr<FGsCostumeData> findData =
			GSCostume()->GetCostumeData(id);

		if (TSharedPtr<FGsCostumeData> data = findData.Pin())
		{
			arrCostumeData.Add(data);
		}
	}


	GSSummon()->SetSummonCostumeData(arrCostumeData);


	// check compose success
	EGsSummonOriginContentType contentType = GSSummon()->GetSummonOriginContent();
	if (contentType == EGsSummonOriginContentType::SUMMON_COMPOSE ||
		contentType == EGsSummonOriginContentType::SUMMON_RE_COMPOSE ||
		contentType == EGsSummonOriginContentType::SUMMON_CEILING ||
		contentType == EGsSummonOriginContentType::SUMMON_EXCHANGE)
	{
		bool isComposeSuccess = false;
		int tryGradeIntVal = (int)GSSummon()->GetCostumeSummonComposeTryGrade();
		for (int i = 0; i < arrCostumeData.Num(); ++i)
		{
			TSharedPtr<FGsCostumeData> data = arrCostumeData[i];
			if (true == data.IsValid())
			{
				if ((int)data->GetCostumeGrade() > tryGradeIntVal)
				{
					isComposeSuccess = true;
					break;
				}
			}
		}

		GSSummon()->SetIsComposeSuccess(isComposeSuccess);
	}
}

// 페어리 모두 보여주기
void FGsSummonHandler::ShowAllUnveilObjects()
{
	if (_sceneObject == nullptr)
	{
		return;
	}

	_sceneObject->ShowAllNormalGradeUnveilObjectResult();
}
// 고등급 페어리 소환석 바로 열기
void FGsSummonHandler::OpenHighGradeStoneNow()
{
	if (_sceneObject == nullptr)
	{
		return;
	}
	_sceneObject->ShowAllHighGradeUnveilObjectResult();
}
// 소환 반복 시도
void FGsSummonHandler::RetrySummon()
{
	if (_traySummon.IsValid())
	{
		_traySummon->SetButtonType(EGsSummonBtnType::HIDE);
		_traySummon->SetMileageWidgetHide();
		_traySummon->ClearList();
	}


	ChangeSoundSummonMixer(false);

	// 소환룸 삭제
	GSSummon()->DespawnSceneObject();

	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (player == nullptr)
	{
		return;
	}

	FVector localPos = player->GetLocation();
	StartSummonContents(localPos);
}
// 재시도 시작
void FGsSummonHandler::StartRetry()
{
	// 테스트 모드 아니고 인게임이면 서버 전송
	if (GSSummon()->GetIsTestClientMode() == false)
	{
		// 소환 리스트 다찼는지 체크
		if(false == FGsSummonFunc::CheckSummonWaitCount(_summonType))
		{
			return;
		}
		//https://jira.com2us.com/jira/browse/C2URWQ-5176
		// stack 갯수 다찼는지 체크
		if (false == FGsSummonFunc::CheckSummonStackCount(_summonType))
		{
			return;
		}

		// https://jira.com2us.com/jira/browse/C2URWQ-4530
		// 원래는 밑으로 내렸는데
		// 클릭후 아래처리가 오래 걸리면 틈이 생길수 있어서
		// 일단 처리하고 쿨타임에 걸리면 다시 false로 감

		_isRetry = true;
		// 각 컨텐츠별 서버 요청 시도
		// 1. 상점구매
		// 2. 퀵슬롯, 아이템 툴팁 사용
		
		if (GSSummon()->GetSummonBuyType() == EGsSummonBuyType::BM_SHOP)
		{		
			uint16 amount = GSSummon()->GetRetryBMamount();
			BMProductId productId = GSSummon()->GetRetryBMProductId();

			// https://jira.com2us.com/jira/browse/C2URWQ-4437		
			// 아이템 쿨타임 이슈로 쿨타임 체크후 사용으로 변경
			
			// 상점 product id 로 쿨타임 체크
			uint64 groupId = GSSummon()->GetGroupIdByProductId(productId);
			// 쿨타임 중이면 return
			if (true == GCoolTime()->IsCoolTime(EGsCoolTime::Item, groupId, 0))
			{
				_isRetry = false;
				return;
			}

			TArray<BMProductIdAmountPair> pairList;
			pairList.Emplace(BMProductIdAmountPair(productId, amount));
			SetRetryDataBMShopByProductId(productId, amount);
			FGsNetSendServiceBMShop::SendBMShopProductValidCheck(pairList,false);
		}
		else
		{
			ItemDBId summonItemDbId = GSSummon()->GetRetryItemDBId();

			// https://jira.com2us.com/jira/browse/C2URWQ-4437		
			// 아이템 쿨타임 이슈로 쿨타임 체크후 사용으로 변경

			// 아이템 db id로 쿨타임 체크

			TWeakPtr<FGsItem> itemData = GItem()->FindItem(summonItemDbId);
			if (itemData.IsValid())
			{
				uint64 groupId = itemData.Pin()->GetCooldownGroupID();

				// 쿨타임 중이면 return
				if (true == GCoolTime()->IsCoolTime(EGsCoolTime::Item, groupId, 0))
				{
					_isRetry = false;
					return;
				}
				// 퀵슬롯 1, 2번에 등록한뒤(1: 페어리, 2: 코스튬)
				// 1, 2 연속해서 누르면
				// 1번(페어리) 성공, 2번(코스튬) 실패 
				// 하지만 다시뽑기 데이터는 2번이 저장되어서
				// 두번째는 코스튬뽑게됨
				// 유효하지 않으면 저장 안하게 처리
				//	https://jira.com2us.com/jira/browse/C2URWQ-5204	
				SetRetryDataByItemInfo(itemData.Pin().Get());
				GItem()->TryUseItem(itemData.Pin().Get());
			}
		}
		
	}
	// 테스트 모드면 바로
	else
	{
		RetrySummon();
	}
	

}
void FGsSummonHandler::OnStopMovie(const IGsMessageParam* In_param)
{
	// 소환룸 사용 EndSummon 호출뒤 들어온건 무시
	if (_isSummonPlay == false)
	{
		return;
	}

	if (nullptr == In_param)
	{
		return;
	}

	const FGsSequenceMessageParam* Param = In_param->Cast<const FGsSequenceMessageParam>();
	


	GSSummon()->SetIsPlaySequence(false);

	TArray<int> arrCostumeAppearCinematic = GSSummon()->GetArrCostumeAppearCinematic();

	// summon fairy
	if (Param->_sequenceId == GData()->GetGlobalData()->_summonFairySequenceId ||
		Param->_sequenceId == GData()->GetGlobalData()->_summonFairySequenceId_Single ||
		Param->_sequenceId == GData()->GetGlobalData()->_summonCostumeSequenceId ||
		Param->_sequenceId == GData()->GetGlobalData()->_summonCostumeSequenceId_Single)
	{
		StopSummonSequence();
	}

	// compose check
	// compose first start sequence id
	else if (Param->_sequenceId == GData()->GetGlobalData()->_summonComposeStartSequenceId)
	{
		// skip check
		// skip == true -> card
		if(Param->_isSkip == true)
		{
			StopSummonSequence();		
		}
		// compose interaction
		else
		{
			StartComposeInteraction();
		}
	}	
	// compose check
	// result id check
	else if( (true == GSSummon()->GetIsComposeSuccess() && true == GData()->GetGlobalData()->_summonComposeResultSuccessSequenceId.Contains(Param->_sequenceId)) ||
		(false == GSSummon()->GetIsComposeSuccess() && true == GData()->GetGlobalData()->_summonComposeResultFailSequenceId.Contains(Param->_sequenceId)))
	{
		StopSummonSequence();
	}
	// 고등급 등장 시네마틱이냐
	else if (true == GData()->GetGlobalData()->_summonFairyHighGetSequenceId.Contains(Param->_sequenceId)
	||true == GData()->GetGlobalData()->_summonCostumeHighGetSequenceId.Contains(Param->_sequenceId) ||
	true == arrCostumeAppearCinematic.Contains(Param->_sequenceId))
	{
		StopHighGetSequence();
		PlayHighGetCardAnimation();
	}
	// 고등급 등장 전(에픽 이상)(테미르 꽝)
	else if (Param->_sequenceId == GData()->GetGlobalData()->_summonHighPreSequenceId_OverEpic)
	{
		// 고등급 등장 시네마틱 틀기
		PlayHighGetSequence();

	}
	// 고등급 등장 전(레어)(테미르 카드 돌리기)
	else if (Param->_sequenceId == GData()->GetGlobalData()->_summonHighPreSequenceId_Rare)
	{
		// 레어 카드 오픈 애니메이션
		PlayHighGetCardAnimation();
	}

	// 페어리 이펙트 사운드 끄기
	// 시퀀스 끝이면 무조건 끄기
	PlayFairyEffectSound(false);

	// 고등급 등장 사운드 끄기
	PlaySummonHighShowSound(false);

}
// stop sequence: fairy summon
void FGsSummonHandler::StopSummonSequence()
{
	if (_traySummon.IsValid())
	{
		_traySummon->InitList();
		SetFirstButtonState();
	}
}

// 조이스틱 복구
void FGsSummonHandler::RestoreJoystick()
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr)
	{
		return;
	}

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	if (_isClearJoystick == false)
	{
		return;
	}

	_isClearJoystick = false;

	// 조이스틱 복구
	if (APlayerController* playerController = world->GetFirstPlayerController())
	{
		playerController->CreateTouchInterface();
	}
}

void FGsSummonHandler::OnLocalPlayerDie(const IGsMessageParam*)
{
	
	FinishSummonForced();
}

// 로컬 성별(코스튬)
CreatureGenderType FGsSummonHandler::GetLocalGender()
{
	if (_localData == nullptr)
	{
		return CreatureGenderType::ALL;
	}

	return _localData->GetCreatureGenderType();
}

// 이벤트 시작
void FGsSummonHandler::StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType)
{

	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (player == nullptr)
	{
		return;
	}

	FVector localPos = player->GetLocation();

	StartCommonAction(localPos);

	StartSummonContents(localPos);

	// B1 issue: 
	// 1. fairy(or costume) room open
	// 2. compose -> summon room open
	// 3. input close key(mapping key)
	// 4. remain tray summon~ ui(bug)	
	// block input
	//bool isLockInput = true;
	//FGsInputEventMsgBase msg;
	//msg.Id = (isLockInput == true) ? 1 : 0;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_SUMMON, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
}// 이벤트 종료
void FGsSummonHandler::FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType)
{
	EndSummon(false);
	GUI()->CloseByKeyName("TraySummon", true);
}


// retry 아이템 정보 세팅
void FGsSummonHandler::SetRetryDataItemByDBId(ItemDBId In_itemDBId)
{
	TWeakPtr<FGsItem> itemData = GItem()->FindItem(In_itemDBId);
	if (itemData.IsValid())
	{
		// 아이템 갯수가 2개 이상인가(현재 소모, retry 까지)
		if (itemData.Pin()->GetAmount() >= 2)
		{
			GSSummon()->SetRetryDataItem(itemData.Pin()->GetTID(), In_itemDBId);
		}
		else if (itemData.Pin()->GetAmount() == 1)
		{
			GSSummon()->SetNotUseRetry();
		}
	}
}

// retry bm shop 정보 세팅
void FGsSummonHandler::SetRetryDataBMShopByProductId(BMProductId In_productId, uint16 In_amount)
{
	FGsBMShopManager* bmShopManager = GBMShop();
	if (bmShopManager == nullptr)
	{
		return;
	}

	FGsBMShopProductData* productData = bmShopManager->FindProduct(In_productId);

	if (productData == nullptr)
	{
		return;
	}
	FGsGameDataManager* gameDataManager = GGameData();

	if (gameDataManager == nullptr)
	{
		return;
	}
	UGsSummonManager* summonManager =  GSSummon();

	if (summonManager == nullptr)
	{
		return;
	}

	// 1. 재화가 충분한가(이번꺼와 다음꺼 합 즉 2배의 금액이 있는가)
	bool isEnoughCurrency = false;
	if (CurrencyType::CASH != productData->GetPurchaseCurrencyType())
	{
		Currency amount = FGsCurrencyHelper::GetCurrencyAmount(productData->GetPurchaseCurrencyType());
		// 2배, 즉 한번더 구매할수 있을것인가
		uint64 price = productData->GetPurchasePrice() * In_amount * 2;
		isEnoughCurrency = (price > amount) ? false : true;
	}
	// 2. 횟수가 아직 남아있나
	bool isEnoughCount = (productData->IsRetryBuyPossible() == true) ? true : false;

	if (true == isEnoughCurrency && true == isEnoughCount)
	{
		summonManager->SetRetryDataBMShop(productData->GetPurchaseCurrencyType(),
			productData->GetPurchasePrice() * In_amount,
			productData->GetProductID(),
			In_amount);
	}
	else
	{
		summonManager->SetNotUseRetry();
	}
}

void FGsSummonHandler::StartSummonContents(const FVector& In_localLocaltion)
{
	if (_summonType == EGsSummonType::SUMMON_COSTUME)
	{
		// 코스튬이면 로컬 데이터 받아놓기
		if (UGsGameObjectBase* findLocal =
			GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
		{
			if (UGsGameObjectLocalPlayer* localPlayer =
				Cast<UGsGameObjectLocalPlayer>(findLocal))
			{
				(_localData) = localPlayer->GetCastData<FGsGameObjectDataLocalPlayer>();
			}
		}

		GSSummon()->SetLocalData(_localData);
	}

	bool isPlayCinematic = false;

	bool isSkip = false;
	if (UGsSummonManager* summonManager = GSSummon())
	{
		isSkip = summonManager->IsSummonSkip();
	}

	EGsSummonOriginContentType contentType =
		GSSummon()->GetSummonOriginContent();

	// set card data
	GSSummon()->SetCardData(_summonType);

	GSSummon()->SummonAllObjects();


	int sequenceId = -1;
	AActor* sceneActor = nullptr;
	// summon type
	if (contentType == EGsSummonOriginContentType::SUMMON_ITEM)
	{
		isPlayCinematic = !isSkip;

		// not skip 
		// show cinematic
		if (isPlayCinematic == true)
		{
			TArray<FGsSummonCardData*> arrayCardOrderData = GSSummon()->GetCardOrderData();

			// only load scene bp not skip cinematic
			sceneActor = GSSummon()->SpawnSceneObjectByType(
				(_summonType == EGsSummonType::SUMMON_FAIRY) ? EGsSummonSceneType::SummonFairy : EGsSummonSceneType::SummonCostume,
				In_localLocaltion);

			if (_summonType == EGsSummonType::SUMMON_FAIRY)
			{						
				sequenceId = (arrayCardOrderData.Num() == 1) ? GData()->GetGlobalData()->_summonFairySequenceId_Single :
					GData()->GetGlobalData()->_summonFairySequenceId;
			}
			else
			{
				sequenceId = (arrayCardOrderData.Num() == 1) ? GData()->GetGlobalData()->_summonCostumeSequenceId_Single :
					GData()->GetGlobalData()->_summonCostumeSequenceId;
			}
		}
	}
	// compose type
	else if (contentType == EGsSummonOriginContentType::SUMMON_COMPOSE ||
		contentType == EGsSummonOriginContentType::SUMMON_RE_COMPOSE||
		contentType == EGsSummonOriginContentType::SUMMON_CEILING ||
		contentType == EGsSummonOriginContentType::SUMMON_EXCHANGE)
	{
		// u1 합성일때는 재료가 레어 이상이면 skip 옵션에 상관없이
		// 초기 시네마틱 + 쪼는 연출 출력한다
		
		if (contentType == EGsSummonOriginContentType::SUMMON_COMPOSE)
		{
			bool isOverRare = GSSummon()->IsComposeByOverRare();

			// 재료 레어 이상이면 cinematic 플레이
			if (isOverRare == true)
			{
				isPlayCinematic = true;
			}
			// 그외는 스킵 옵션으로 처리
			else
			{			
				isPlayCinematic = !isSkip;
			}
		}
		else
		{
			isPlayCinematic = !isSkip;
		}


		// not skip 
		// show cinematic
		if (isPlayCinematic == true)
		{
			sequenceId = GData()->GetGlobalData()->_summonComposeStartSequenceId;

			sceneActor = GSSummon()->SpawnSceneObjectByType(
				EGsSummonSceneType::Compose,
				In_localLocaltion);
		}
	}
	

	// not skip && not wait change
	// show cinematic
	if (isPlayCinematic == true &&
	contentType != EGsSummonOriginContentType::SUMMON_WAIT_CHANGE)
	{
		if (sequenceId == -1 ||
			sceneActor == nullptr)
		{
			EndSummon();
			return;
		}
		// 저사양 ios 메모리 부족 크래쉬 이슈
		// https://jira.com2us.com/jira/browse/C2URWQ-6794
		bool isStore = IsBranch(EGsGameClientBranchType::SUMMON_CINEMATIC_IOS_MEMORY_ISSUE) ? false : true;
		FGsSequenceMessageParam param(sequenceId, sceneActor, isStore);
		GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);

		GSSummon()->SetIsPlaySequence(true);

		_isPlaySequence = true;
	}
	// skip-> show card
	else
	{
		if (_traySummon.IsValid())
		{
			_traySummon->InitList();
			SetFirstButtonState();
		}
	}

	// 스킵 버튼 초기화
	if (_traySummon.IsValid())
	{
		_traySummon->SetSkipButtonVisible(false);
	}
}

void FGsSummonHandler::StartCommonAction(const FVector& In_localLocaltion)
{
	if (GEngine == nullptr || GEngine->GameViewport == nullptr)
	{
		return;
	}

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	// 아이템일때 처리
	EGsSummonOriginContentType contentType = GSSummon()->GetSummonOriginContent();
	if (contentType == EGsSummonOriginContentType::SUMMON_ITEM ||
		contentType == EGsSummonOriginContentType::SUMMON_RE_COMPOSE)
	{
		// 1. 지형 꺼짐 현상 제거			
		GLevel()->HoldStreamingLevelByLocation(In_localLocaltion);

		// 3. UI flag 세팅		
		uiMgr->SetHideFlags(EGsUIHideFlags::STATE_SUMMON);

		// ui 스택 된거 다끄기-> bm 상점 때문에 뺌
		//uiMgr->CloseAllStack();

	}
	else if (contentType == EGsSummonOriginContentType::SUMMON_EXCHANGE)
	{
		uiMgr->SetHideFlags(EGsUIHideFlags::STATE_SUMMON);
	}
	// 페어리 state일때
	else if (contentType == EGsSummonOriginContentType::SUMMON_COMPOSE ||
		contentType == EGsSummonOriginContentType::SUMMON_WAIT_CHANGE ||
		contentType == EGsSummonOriginContentType::SUMMON_CEILING )
	{

	}

	// 환경효과 이벤트 전달
	FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::GACHA_ROOM));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &param);

	//  조이스틱 안보이게 처리
	if (APlayerController* playerController = world->GetFirstPlayerController())
	{
		playerController->ActivateTouchInterface(nullptr);
		_isClearJoystick = true;
	}

	//  UI 출력
	TWeakObjectPtr<UGsUIWidgetBase> summonTray = uiMgr->OpenAndGetWidget(TEXT("TraySummon"));
	if (summonTray.IsValid())
	{
		_traySummon = Cast<UGsUITraySummon>(summonTray);

		if (_traySummon.IsValid())
		{
			_traySummon->SetButtonType(EGsSummonBtnType::HIDE);
			_traySummon->SetMileageWidgetHide();
		}
	}

	if (_traySummon.IsValid())
	{
		// 재시도 ui 갱신(재화 타입 있다면)
		_traySummon->InitRetryData();
	}

	_isSummonPlay = true;

	// navigation 재화 백업후 끄기
	FName nameNav(TEXT("TrayNavigationBar"));
	if (false == uiMgr->IsActiveWidget(nameNav))
	{
		_isBeforeShowNavigationBarUI = false;
	}
	else
	{
		_isBeforeShowNavigationBarUI = true;
		TWeakObjectPtr<UGsUIWidgetBase> widgetNav = uiMgr->GetWidgetByKey(nameNav);
		if (widgetNav.IsValid())
		{
			if (UGsUITrayNavigationBar* trayNav = Cast<UGsUITrayNavigationBar>(widgetNav))
			{
				// 최근 재화 백업 처리
				_currentNavigationCurrency = trayNav->GetCurrentOptionalCurrency();
			}
		}
		// 닫기 처리
		uiMgr->CloseByKeyName(nameNav);
	}

	// 소환 시작시 인벤토리에서 퀵슬롯 등록할 인덱스 클리어하기
	//GSQuickSlot()->ResetSelectionInfo();
	GSQuickSlot()->CloseInven();
}
// start compose interaction
void FGsSummonHandler::StartComposeInteraction()
{	
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	uiMgr->OpenAndGetWidget(TEXT("TraySummonSkip"));

	AGsActorSummonComposeSceneObject* sceneCompose = GSSummon()->GetComposeSceneObject();
	
	if(sceneCompose == nullptr)
	{
		EndSummon();
		return;
	}

	sceneCompose->Initialize();
}

// compose skip
void FGsSummonHandler::OnSummonComposeSkip(const IGsMessageParam* In_param)
{
	// hide summon skip
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	uiMgr->CloseByKeyName("TraySummonSkip", true);

	// https://jira.com2us.com/jira/browse/C2URWQ-5387
	// 합성 씬에서 믹서 돌리고 사운드 종료 처리 호출
	AGsActorSummonComposeSceneObject* sceneCompose = GSSummon()->GetComposeSceneObject();
	if (sceneCompose != nullptr)
	{
		sceneCompose->ChangeSoundSummonMixer(false);
	}

	StopSummonSequence();
}

// high get sequence finish
void FGsSummonHandler::StopHighGetSequence()
{
	// 시퀀스에서 획득 ui 닫기
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	// 획득 ui 닫기
	uiMgr->CloseByKeyName(TEXT("TraySummonObtain"));
	// 대사 ui 닫기(스킵일 경우)
	uiMgr->CloseByKeyName(TEXT("TraySummonDialogue"));
	// info ui 닫기(스킵일 경우)
	uiMgr->CloseByKeyName(TEXT("TraySummonInfo"));
}
// https://jira.com2us.com/jira/browse/C2URWQ-4063
// 처음엔 무조건 전체 카드 애니메이션 스킵 버튼(OPEN_ALL)
void FGsSummonHandler::SetFirstButtonState()
{
	if (false == _traySummon.IsValid())
	{
		return;
	}

	_traySummon->SetFirstButtonState();


	// 카드 배열 될때 트는 사운드 
	ChangeSoundSummonMixer(true);
}
void FGsSummonHandler::PlayHighGetCardAnimation()
{
	if (_traySummon.IsValid())
	{
		_traySummon->PlayHighGetCardAnimation();
	}
}
// 재연결시에는 여기 안들어옴
void FGsSummonHandler::OnLocalWarp(const IGsMessageParam* In_param)
{
	FinishSummonForced();
}

void FGsSummonHandler::FinishSummonForced()
{
	if (_isSummonPlay == false)
	{
		return;
	}

	_isBeforeShowNavigationBarUI = false;

	EndSummon();
}

// 소환 사운드 믹서 교체후 기본 음악(bgm) 출력
void FGsSummonHandler::ChangeSoundSummonMixer(bool In_isSummonMode)
{
	UGsSoundManager* soundManager = GSound();

	if (soundManager == nullptr)
	{
		return;
	}

	UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController();

	if (soundMixerController == nullptr)
	{
		return;
	}

	if (In_isSummonMode == true)
	{
		soundMixerController->OnMixChanged(EGsSoundMixType::Summon);
	}
	else
	{
		soundMixerController->OffMixMode(EGsSoundMixType::Summon);
	}

	UGsSoundPlayer* SoundPlayer = soundManager->GetOrCreateSoundPlayer();

	if (SoundPlayer == nullptr)
	{
		return;
	}

	if (In_isSummonMode == true)
	{
		if (_summonBaseSoundId == 0)
		{
			_summonBaseSoundId = SoundPlayer->PlaySound2D("Snd_Cue_BGM_Gacha_Fairy");
		}
	}
	else
	{
		SoundPlayer->StopSound(_summonBaseSoundId);
		// 초기화 추가
		_summonBaseSoundId = 0;
	}
}

// https://jira.com2us.com/jira/browse/C2URWQ-4326
// 재연결 성공시
void FGsSummonHandler::OnGameServerReconnectionSuccess()
{
	FinishSummonForced();
}

// 페어리 이펙트 사운드 재생
void FGsSummonHandler::PlayFairyEffectSound(bool In_isOn,  bool In_isHighCardExist)
{
	UGsSoundManager* soundManager = GSound();

	if (soundManager == nullptr)
	{
		return;
	}


	UGsSoundPlayer* SoundPlayer = soundManager->GetOrCreateSoundPlayer();

	if (SoundPlayer == nullptr)
	{
		return;
	}

	if (In_isOn == true)
	{
		if (_summonFairySequenceEffectSoundId == 0)
		{
			FString playSoundKey = (In_isHighCardExist == true)?
				GData()->GetGlobalData()->_summonFairySequenceEffectSoundHigh: GData()->GetGlobalData()->_summonFairySequenceEffectSoundNormal;
			_summonFairySequenceEffectSoundId = SoundPlayer->PlaySound2D(playSoundKey);
		}
	}
	else
	{
		SoundPlayer->StopSound(_summonFairySequenceEffectSoundId);
		// 초기화 추가
		_summonFairySequenceEffectSoundId = 0;
	}
}
// retry 아이템 정보 세팅(아이템 정보로)
// 유효 체크 함수도 내부 있음	
// 퀵슬롯 1, 2번에 등록한뒤(1: 페어리, 2: 코스튬)
// 1, 2 연속해서 누르면
// 1번(페어리) 성공, 2번(코스튬) 실패 
// 하지만 다시뽑기 데이터는 2번이 저장되어서
// 두번째는 코스튬뽑게됨
// 유효하지 않으면 저장 안하게 처리
//	https://jira.com2us.com/jira/browse/C2URWQ-5204
void FGsSummonHandler::SetRetryDataByItemInfo(const FGsItem* InItem)
{
	UGsItemManager* itemMgr = GItem();
	if (itemMgr == nullptr || 
		InItem == nullptr)
	{
		return;
	}

	if (itemMgr->IsValidSummonItem(InItem) == true)
	{		
		SetRetryDataItemByDBId(InItem->GetDBID());
	}
}

// https://jira.com2us.com/jira/browse/C2URWQ-4326
// 모든 뽑기 ui 닫기
void FGsSummonHandler::CloseAllSummonUI()
{
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	uiMgr->CloseByKeyName("TraySummon", true);

	// 획득 ui 닫기
	uiMgr->CloseByKeyName(TEXT("TraySummonObtain"));
	// 대사 ui 닫기(스킵일 경우)
	uiMgr->CloseByKeyName(TEXT("TraySummonDialogue"));
	// info ui 닫기(스킵일 경우)
	uiMgr->CloseByKeyName(TEXT("TraySummonInfo"));

	// https://jira.com2us.com/jira/browse/C2URWQ-4326
	// 재연결시 합성 ui 남는거 수정
	uiMgr->CloseByKeyName(TEXT("TraySummonSkip"));
}

// 고등급 등장 사운드 출력(anim notify에서)
// 시네마틱에서 스킵할때 사운드도 스킵하게 처리
// https://jira.com2us.com/jira/browse/C2URWQ-5524
void FGsSummonHandler::PlaySummonHighShowSound(bool In_isOn, FString In_soundTblKey)
{
	UGsSoundManager* soundManager = GSound();

	if (soundManager == nullptr)
	{
		return;
	}


	UGsSoundPlayer* SoundPlayer = soundManager->GetOrCreateSoundPlayer();

	if (SoundPlayer == nullptr)
	{
		return;
	}

	if (In_isOn == true)
	{				
		uint64 summonHighShowSoundId = SoundPlayer->PlaySound2D(In_soundTblKey);
		_arraySummonHighShowSoundId.Add(summonHighShowSoundId);
		
	}
	else
	{
		if (_arraySummonHighShowSoundId.Num() == 0)
		{
			return;
		}

		for (uint64 id: _arraySummonHighShowSoundId)
		{
			SoundPlayer->StopSound(id);
		}
		
		// 초기화 추가
		_arraySummonHighShowSoundId.Empty();
	}
}

// 고등급 등장 시네마틱 재생
void FGsSummonHandler::PlayHighGetSequence()
{
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	if (player == nullptr)
	{
		return;
	}

	FVector localPos = player->GetLocation();

	// make or get scene object
	AActor* sceneActor = GSSummon()->SpawnSceneObjectByType(EGsSummonSceneType::HighGet, localPos);
	// show sequence
	// 저사양 ios 메모리 부족 크래쉬 이슈
	// https://jira.com2us.com/jira/browse/C2URWQ-6794
	bool isStore = IsBranch(EGsGameClientBranchType::SUMMON_CINEMATIC_IOS_MEMORY_ISSUE) ? false : true;
	FGsSequenceMessageParam param(_highSequenceId, sceneActor, isStore, true);
	GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);

	GSSummon()->SetIsPlaySequence(true);
}