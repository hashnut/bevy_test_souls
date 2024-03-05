// Fill out your copyright notice in the Description page of Project Settings.

#include "GsInputBindingLocalPlayer.h"
#include "GsInputEventMsgBase.h"

#include "Framework/Application/SlateApplication.h"
#include "Engine/World.h"
#include "GameFramework/InputSettings.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorComponentEx/GsEffectComponent.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGame/GsSaveBatteryManager.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGameClientVersion.h"

#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"

#include "DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/GsAutoMoveInfo.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"

#include "ControllerEx/GsPlayerController.h"

#include "Sequence/GsSequencePlayer.h"
#include "Sequence/GsSequenceManager.h"

#include "Option/GsGameUserSettings.h"
#include "Skill/GsSkill.h"
#include "AI/NavigationSystemBase.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "UTIL/GsDrawDebugHelpers.h"
#include "UTIL/GsMove.h"

#if UE_EDITOR	
#include "GameObject/Movement/GsMovementHandlerBase.h"
#endif
#if !UE_BUILD_SHIPPING
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#endif
#include "T1Project.h"
#include "DataSchema/KeyMapping/GsSchemaKeyMapping.h"
#include "Option/GsOptionEnumType.h"
#include "Message/GsMessageInput.h"
#include "Net/GsNetSendServiceWorld.h"

#include "Input/GsJoystickInpuProcessor.h"
#include "Input/GsHerculesInputProcess.h"
#include "Input/GsInputEventValidator.h"

#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "UI/UIContent/Popup/GsUIPopupChat.h"

#include "Engine/Classes/Camera/PlayerCameraManager.h"


UGsInputBindingLocalPlayer::UGsInputBindingLocalPlayer() : UGsInputBindingBase(), _inputKeyEventValidator(MakeShared<FGsInputKeyEventValidator>())
{
}

//[Todo]타겟 클래스 설정 방식에 좀더 좋은 구조를 생각해볼것
void UGsInputBindingLocalPlayer::Initialize()
{
	//error
}

void UGsInputBindingLocalPlayer::Initialize(UGsGameObjectLocalPlayer* target)
{
	_owner = target;

	BindMessage();

	// 카메라 줌 강도 값 저장하기
	_touchZoomFactor = GData()->GetGlobalData()->CamZoomPower;
	_pitchRotMul = GData()->GetGlobalData()->InputPitchRotMul;
	_touchMoveFactorMobile = GData()->GetGlobalData()->_touchMoveFactor;
	_touchMoveFactorPC = GData()->GetGlobalData()->_touchMoveFactorPC;
	_touchMovePosClass = LoadObject<UClass>(this, *(GData()->GetGlobalData()->_touchMovePosObjectPath.ToString() += TEXT("_C")));

	if (GEngine && GEngine->GameViewport)
	{
		FVector2D viewPortVec;
		GEngine->GameViewport->GetViewportSize(viewPortVec);
		if (viewPortVec != FVector2D::ZeroVector)
		{
			_rotPitchAspectRate = viewPortVec.Y / viewPortVec.X;
		}

		// 아트팀 독립형에서 해당값이 0이어서 pitch 조절안되는이슈
		// 강제로 세팅
		if (_rotPitchAspectRate == 0.0f)
		{
			_rotPitchAspectRate = 0.5f;
		}
	}


	// 인터랙션 충돌체 채널
	// 몬스터
	_arrayChannelInteraction.Add(ECollisionChannel::ECC_Pawn);
	// ObjectChannel 이름 : 보스(PawnBoss)
	_arrayChannelInteraction.Add(ECollisionChannel::ECC_GameTraceChannel1);
	// ObjectChannel 이름 : 타유저(OtherCollision)
	_arrayChannelInteraction.Add(ECollisionChannel::ECC_GameTraceChannel9);
	// ObjectChannel 이름 : PawnNpc
	_arrayChannelInteraction.Add(ECollisionChannel::ECC_GameTraceChannel11);	

	// ObjectChannel 이름 : 타유저(OtherBlockLocal) 충돌상태
	_arrayChannelInteraction.Add(ECollisionChannel::ECC_GameTraceChannel13);
	// ObjectChannel 이름 : 보스(PawnBossBlock) 충돌상태
	_arrayChannelInteraction.Add(ECollisionChannel::ECC_GameTraceChannel14);
	// ObjectChannel 이름 : CampObject
	_arrayChannelInteraction.Add(ECollisionChannel::ECC_GameTraceChannel15);

	// ObjectChannel 이름 : TouchBlock 
	// 에키나드 같이 충돌체랑 실제 보여지는 모델링이랑 터치할 위치가 다를때 별도 충돌체 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-5004
	_arrayChannelInteraction.Add(ECollisionChannel::ECC_GameTraceChannel16);

	// 터치 이동 못하게 위에서 막는 충돌체
	// ObjectChannel 이름 : 보스(PawnBossBlock) 충돌상태(ex: 시공의 틈새 오염체 )
	_arrayChannelNonTouchMoveTarget.Add(ECollisionChannel::ECC_GameTraceChannel14);

	_joystickInputProcessor = MakeShared<FGsJoystickInputProcessor>();
	_joystickInputProcessor->OnJoystickPressed.BindUObject(this, &UGsInputBindingLocalPlayer::OnTouchJoystickPress);
	_joystickInputProcessor->OnJoystickReleased.BindUObject(this, &UGsInputBindingLocalPlayer::OnTouchJoystickRelease);

	FSlateApplication::Get().RegisterInputPreProcessor(_joystickInputProcessor);

	_heculesInputProcessor = MakeShared<FGsHerculesInputProcess>();
	FSlateApplication::Get().RegisterInputPreProcessor(_heculesInputProcessor, 0);

	_blockInputFlagSet.Empty();
}

void UGsInputBindingLocalPlayer::BindMessage()
{
	FGsMessageHolder* msg = GMessage();

	InitializeMessageDelegateList(&msg->GetInput());

	AddMessageDelegate(msg->GetInput().AddUObject(MessageInput::PressSkill,
		this, &UGsInputBindingLocalPlayer::OnPressSkill));
	AddMessageDelegate(msg->GetInput().AddUObject(MessageInput::ReleaseSkill,
		this, &UGsInputBindingLocalPlayer::OnReleaseSkill));
	AddMessageDelegate(msg->GetInput().AddUObject(MessageInput::ClickSkill,
		this, &UGsInputBindingLocalPlayer::OnClickSkill));
	AddMessageDelegate(msg->GetInput().AddUObject(MessageInput::UnmountVehicle,
		this, &UGsInputBindingLocalPlayer::OnUnmountVehicle));	
	AddMessageDelegate(msg->GetInput().AddUObject(MessageInput::ViewChange,
		this, &UGsInputBindingLocalPlayer::OnViewChange));
	AddMessageDelegate(msg->GetInput().AddUObject(MessageInput::BlockInput,
		this, &UGsInputBindingLocalPlayer::OnBlockInputFlag));
	AddMessageDelegate(msg->GetInput().AddUObject(MessageInput::TestPrintTouch,
		this, &UGsInputBindingLocalPlayer::OnTestPrintTouch));

	_gameObjectMsgDelegates.Emplace(
		msg->GetGameObject().AddUObject(MessageGameObject::SpectatorTerrainTouchEffect, this, &UGsInputBindingLocalPlayer::OnSpectatorTerrainTouchEffect)
	);
	_gameObjectMsgDelegates.Emplace(
		msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_DIE, this, &UGsInputBindingLocalPlayer::OnLocalPlayerDie)
	);
	_gameObjectMsgDelegates.Emplace(
		msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &UGsInputBindingLocalPlayer::OnLocalPlayerSpawnComplete)
	);
	_gameObjectMsgDelegates.Emplace(
		msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_REVIVE, this, &UGsInputBindingLocalPlayer::OnLocalPlayerSpawnComplete)
	);

	// 이동중 멈추는 현상 수정중
	// 스킬에서 처리해서 일단 막아놓음
	// https://jira.com2us.com/jira/browse/C2URWQ-6334

	//_gameObjectMsgDelegates.Emplace(
	//	msg->GetGameObject().AddUObject(MessageGameObject::LOCAL_TARGET_CHANGED, this, &UGsInputBindingLocalPlayer::OnLocalTargetChanged)
	//);

	_systemMsgDelegates.Emplace(
		msg->GetSystemParam().AddUObject(MessageSystem::LOAD_TOPO_START, this, &UGsInputBindingLocalPlayer::OnLoadTopoStart)
	);

}

void UGsInputBindingLocalPlayer::Finalize()
{
	FinalizeMessageDelegateList();
	DeleteTouchMoveActor();

	ReleaseMessage();

	_arrayChannelInteraction.Empty();

	FSlateApplication::Get().UnregisterInputPreProcessor(_joystickInputProcessor);
	_joystickInputProcessor.Reset();

	FSlateApplication::Get().UnregisterInputPreProcessor(_heculesInputProcessor);
	_heculesInputProcessor.Reset();

	_blockInputFlagSet.Empty();
	
	Super::Finalize();
}

void UGsInputBindingLocalPlayer::ReleaseMessage()
{
	if (FGsMessageHolder* msg = GMessage())
	{
		if (_gameObjectMsgDelegates.Num() != 0)
		{
			for (auto iter : _gameObjectMsgDelegates)
			{
				msg->GetGameObject().Remove(iter);
			}
			_gameObjectMsgDelegates.Empty();
		}

		if (_systemMsgDelegates.Num() != 0)
		{
			for (auto iter : _systemMsgDelegates)
			{
				msg->GetSystemParam().Remove(iter);
			}
			_systemMsgDelegates.Empty();
		}
	}
}

void UGsInputBindingLocalPlayer::SetBinding(UInputComponent* input)
{
	Super::SetBinding(input);

	if (input)
	{
		// input->BindAction(TEXT("Skill5"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnInputPressAxisSkill);
		// input->BindAction(TEXT("Skill5"), EInputEvent::IE_Released, this, &UGsInputBindingLocalPlayer::OnInputReleaseAxisSkill);

		// input->BindAction(TEXT("Action"), IE_Released, this, &UGsInputBindingLocalPlayer::OnAction);

		// Camera
		input->BindAction(TEXT("ZoomIn"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnZoomIn);
		input->BindAction(TEXT("ZoomOut"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnZoomOut);

		// https://jira.com2us.com/jira/browse/C2URWQ-5354
		// 플레이어블 이벤트 평타 스페이스바 인풋 받기
		input->BindAction(TEXT("KeyboardMappingName_NormalAttack"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnPlayableEventNoramlAttack);


		// 카메라 변경모드 
		input->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnViewChange);
		// 채팅 테스트 시 눌려져서
		// 필요할때만 풀어서 사용
#ifdef TEST_BINDING	
		input->BindAction(TEXT("TestAutoChange"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnTestAutoChange);
		input->BindAction(TEXT("TestAutoTime"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnToggleTestAutoChangeTimer);
		input->BindAction(TEXT("TestPostionEffect"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnTestPostionEffect);
		input->BindAction(TEXT("TestDebuffActive"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnTestDebuffActive);
		input->BindAction(TEXT("TestDebuffDeactive"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnTestDebuffDeactive);
#endif

		// Android Back Key
		input->BindAction(TEXT("Back"), EInputEvent::IE_Released, this, &UGsInputBindingLocalPlayer::OnBackKey);

		// Movement
		input->BindAxis(TEXT("MoveForward"), this, &UGsInputBindingLocalPlayer::OnMoveForward);
		input->BindAxis(TEXT("MoveBackward"), this, &UGsInputBindingLocalPlayer::OnMoveBackward);
		input->BindAction<FOnAttack1>(TEXT("TestMoveForward"), EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnTestMoveForward, 0);
		input->BindAction<FOnAttack1>(TEXT("TestMoveForward"), EInputEvent::IE_Released, this, &UGsInputBindingLocalPlayer::OnTestMoveForward, 1);
		input->BindAxis(TEXT("MoveRight"), this, &UGsInputBindingLocalPlayer::OnMoveRight);
		input->BindAxis(TEXT("MoveLeft"), this, &UGsInputBindingLocalPlayer::OnMoveLeft);

		input->BindAxis(TEXT("MoveRotate"), this, &UGsInputBindingLocalPlayer::OnMoveRotateYaw);
		input->BindAxis(TEXT("Turn"), this, &UGsInputBindingLocalPlayer::OnMoveRotateYaw);
		input->BindAxis(TEXT("LookUp"), this, &UGsInputBindingLocalPlayer::OnMoveRotatePitch);

		// Touch Common
		input->BindTouch(EInputEvent::IE_Pressed, this, &UGsInputBindingLocalPlayer::OnTouchPress);
		input->BindTouch(EInputEvent::IE_Released, this, &UGsInputBindingLocalPlayer::OnTouchRelease);
		input->BindTouch(EInputEvent::IE_Repeat, this, &UGsInputBindingLocalPlayer::OnTouchMove);


		//Bak1210 : 디버그용 터치코드 추가	
		input->BindTouch(EInputEvent::IE_Pressed, GScope(), &UGsScopeHolder::OnTouchPress);
		input->BindTouch(EInputEvent::IE_Released, GScope(), &UGsScopeHolder::OnTouchRelease);
		input->BindTouch(EInputEvent::IE_Repeat, GScope(), &UGsScopeHolder::OnTouchMove);

		input->BindAction(TEXT("QuickExecute"), IE_Released, this, &UGsInputBindingLocalPlayer::OnQuickExecute);

		OnOptionKeyMapping(input);
	}
	else
	{
		GSLOG(Error, TEXT("Log - UInputComponent 정보가 없습니다. 정상 작동이 되지 않는 상황으로 문의가 필요.."));
	}
}

void UGsInputBindingLocalPlayer::OnOptionKeyMapping(UInputComponent* input)
{
	TArray<const FGsSchemaKeyMapping*> schemaKeyMappingList;
	if (const UGsTable* table = FGsSchemaKeyMapping::GetStaticTable())
	{
		table->GetAllRows<FGsSchemaKeyMapping>(schemaKeyMappingList);
	}

	for (const FGsSchemaKeyMapping* schemaKeyMapping : schemaKeyMappingList)
	{
		if (nullptr == schemaKeyMapping)
			continue;

		for (TEnumAsByte<EInputEvent> iter : schemaKeyMapping->keyInputEvents)
		{
/*			if (EGsOptionKeyBindType::Action == schemaKeyMapping->keyBindType)
			{
				input->BindAction<FOnOptionKeyMapping>(schemaKeyMapping->keyName, iter, this,
					&UGsInputBindingLocalPlayer::OnKeyboardActionOptionKey, int32(schemaKeyMapping->keyMappingType), iter);
			}
			else */
			if (EGsOptionKeyBindType::Axis == schemaKeyMapping->keyBindType)
			{
				switch (schemaKeyMapping->keyMappingType)
				{
				case EGsKeyMappingType::KEY_MAPPING_MOVE_UP:
					input->BindAxis(schemaKeyMapping->keyName, this, &UGsInputBindingLocalPlayer::OnMoveForward);
					break;
				case EGsKeyMappingType::KEY_MAPPING_MOVE_DOWN:
					input->BindAxis(schemaKeyMapping->keyName, this, &UGsInputBindingLocalPlayer::OnMoveBackward);
					break;
				case EGsKeyMappingType::KEY_MAPPING_MOVE_RIGHT:
					input->BindAxis(schemaKeyMapping->keyName, this, &UGsInputBindingLocalPlayer::OnMoveRight);
					break;
				case EGsKeyMappingType::KEY_MAPPING_MOVE_LEFT:
					input->BindAxis(schemaKeyMapping->keyName, this, &UGsInputBindingLocalPlayer::OnMoveLeft);
					break;
				default:
					break;
				}				
			}
			else if(EGsOptionKeyBindType::Touch == schemaKeyMapping->keyBindType)
			{
				// 없음
			}
		}
	}
}

void UGsInputBindingLocalPlayer::OnKeyboardActionOptionKey(int32 InKeyMappingType, TEnumAsByte<EInputEvent> InInputEventType)
{
	if (IsInputBlock())
	{
		return;
	}

// 	switch (static_cast<EGsKeyMappingType>(InKeyMappingType))
// 	{
// 	case EGsKeyMappingType::KEY_MAPPING_NORMALATTACK:
// 		break;
// 	default:
// 		break;
// 	}

	FGsInputEventMsgWindowKey InputMsg = FGsInputEventMsgWindowKey(InKeyMappingType, InInputEventType);
	GMessage()->GetWindowInput().SendMessage(MessageInput::Window_keyboard_Action, &InputMsg);
}

void UGsInputBindingLocalPlayer::OnQuickExecute()
{
	FGsInputEventMsgBase InputMsg;
	GMessage()->GetInput().SendMessage(MessageInput::QuickExecute, InputMsg);
}

void UGsInputBindingLocalPlayer::OnMoveForward(float Value)
{
	if (EnumHasAnyFlags(_blockInputFlag, EGsKeyMappingInputFlags::INPUT_KEY_MOVE))
	{
		return;
	}

	_axisYForwardValue = Value;
}

void UGsInputBindingLocalPlayer::OnMoveBackward(float Value)
{
	if (EnumHasAnyFlags(_blockInputFlag, EGsKeyMappingInputFlags::INPUT_KEY_MOVE))
	{
		return;
	}

	_axisYBackwardValue = Value;
}

void UGsInputBindingLocalPlayer::OnMoveLeft(float Value)
{
	if (EnumHasAnyFlags(_blockInputFlag, EGsKeyMappingInputFlags::INPUT_KEY_MOVE))
	{
		return;
	}

	_axisXLeftValue = Value;
}

void UGsInputBindingLocalPlayer::OnMoveRight(float Value)
{
	if (EnumHasAnyFlags(_blockInputFlag, EGsKeyMappingInputFlags::INPUT_KEY_MOVE))
	{
		return;
	}

	_axisXRightValue = Value;
}

void UGsInputBindingLocalPlayer::Update(float Delta)
{
	if (_owner)
	{
		_axisValue = FVector(_axisXLeftValue + _axisXRightValue, _axisYForwardValue + _axisYBackwardValue, .0f);

		if (FMath::Abs(_axisYForwardValue) > 0.f
			|| FMath::Abs(_axisYBackwardValue) > 0.f
			|| FMath::Abs(_axisXLeftValue) > 0.f
			|| FMath::Abs(_axisXRightValue) > 0.f
			|| FMath::Abs(_prevAxisValue.Size() - _axisValue.Size()) > FLT_EPSILON
			//|| (FMath::Abs(_prevAxisValue.Size()) - FMath::Abs(_axisValue.Size())) > FLT_EPSILON
			)
		{
			if (FGsMovementHandlerLocalPlayer* movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>())
			{
				// 캐릭터는 X축이 전방
				FVector2D moveAxis = FVector2D(_axisValue.Y, _axisValue.X);
				if (FGsMovementStateInput* movementInput = movement->ChangeInputMoveState(moveAxis))
				{
					movementInput->Start(_owner);
				}
			}

			if (FGsCameraModeHandler* cameraModehandler = _owner->GetCameraModeHandler())
			{
				cameraModehandler->GetState()->SetAxisValue(_axisValue);
				cameraModehandler->GetState()->OnJoysticChanged();
			}

			// 이전에 값이 있고
			// 새로 들어온값이 없으면
			// 조이스틱 땐 순간임
			if (_prevAxisValue.Size() > 0.f &&
				_axisValue.Size() == 0.0f)
			{
				OnReleaseJoystick();
				_isJoystickClicked = false;
			}

			// 이전에 값이 없었고
			// 새로 들어온 값이 있으면
			// 조이스틱 누르는 순간임
			if (_prevAxisValue.Size() == 0.0f &&
				_axisValue.Size() > 0.f)
			{
				OnPressJoystick();
			}

			_prevAxisValue = _axisValue;

			if (FGsSaveBatteryManager* saveBatteryManager = GSSaveBattery())
			{
				saveBatteryManager->SetIsKeyboardPressed(true);
			}
		}
		
		if (_isJoystickClicked)
		{
			GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_INPUT_TARGET_CHANGE, nullptr);
			_isJoystickClicked = false;
		}
	}
}

//void UGsInputBindingLocalPlayer::OnInputPressAxisSkill()
//{
//	FGsInputEventMsgBase InputMsg;
//	InputMsg.Id = 5;
//	GMessage()->GetInput().SendMessage(MessageInput::PressAxisSkill, InputMsg);
//}
//void UGsInputBindingLocalPlayer::OnInputReleaseAxisSkill()
//{
//	FGsInputEventMsgBase InputMsg;
//	InputMsg.Id = 5;
//	GMessage()->GetInput().SendMessage(MessageInput::ReleaseAxisSkill, InputMsg);
//}
// 테스트 탈것 처리
// 근처에 탈것이 있는지 찾는다
//void UGsInputBindingLocalPlayer::OnAction()
//{
//	if (UGsGameObjectBase* findVehicle = GSGameObject()->FindObject(EGsGameObjectType::Vehicle))
//	{
//		FGsGameObjectMessageParamTarget param;
//		param._paramOwner = findVehicle;
//		param._paramTarget = _owner;
//		GMessage()->GetGameObject().SendMessage(MessageGameObject::COMMON_VEHICLE_RIDE, &param);
//	}
//}

static bool staticGlobalTestAnimPose = false;

void UGsInputBindingLocalPlayer::OnTestMoveForward(int32 Value)
{
#if UE_EDITOR	
	// 대시 테스트 처리
	const FGsSchemaCreatureCommonInfo* commonInfo = _owner->GetCommonInfoTable();

	FGsMovementHandlerBase* movement = _owner->GetMovementHandler();
	int movespeed = movement->GetMoveSpeed();
	FGsGameObjectDataCreature* objdata = _owner->GetCastData<FGsGameObjectDataCreature>();
	check(objdata);

	// 이동 속도 설정
	TSharedPtr<FGsStatBase> statData = objdata->GetStatBase();
	check(statData.IsValid());

	const FGsStatInfo* moveStat = statData->GetStatInfo(StatType::MOVE_SPEED_RATE);
	if (movespeed < commonInfo->dashSpeed)
	{
		_testTempValue = moveStat->GetStatValue();
	}

	TArray<TPair<StatType, int32>> statMap;
	statMap.Add(TPair<StatType, int32>(StatType::MOVE_SPEED_RATE, (Value == 0) ? 30000 : _testTempValue));
	statData->UpdateStatInfo(statMap);
#endif
}

void UGsInputBindingLocalPlayer::OnMoveRotateYaw(float Value)
{
	if (nullptr == _owner ||
		Value == 0.0f)
	{
		return;
	}


	FGsCameraModeHandler* cameraModehandler = _owner->GetCameraModeHandler();

	if (cameraModehandler == nullptr ||
		cameraModehandler->GetState()->IsUseInputRotationYaw() == false)
	{
		return;
	}

	// 모드 회전 변환중이 아니어야 함
	if (cameraModehandler->GetState()->GetIsModeRotChanging() == false)
	{
		_owner->GetLocalCharacter()->AddControllerYawInput(Value);

		cameraModehandler->GetState()->CameraRotationChanged();
	}

}

void UGsInputBindingLocalPlayer::OnMoveRotatePitch(float Value)
{
	if (nullptr == _owner ||
		Value == 0.0f)
	{
		return;
	}

	FGsCameraModeHandler* cameraModehandler = _owner->GetCameraModeHandler();

	if (cameraModehandler == nullptr ||
		cameraModehandler->GetState()->IsUseInputRotationPitch() == false)
	{
		return;
	}

	// 모드 회전 변환중이 아니어야 함
	if (cameraModehandler->GetState()->GetIsModeRotChanging() == false)
	{
		_owner->GetLocalCharacter()->AddControllerPitchInput(Value);

		cameraModehandler->GetState()->CameraRotationChanged();
	}

	// 로비에서는 저장할 필요 없다
	if (UWorld* world = GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::Lobby)
			{
				return;
			}
		}
	}


	// 값이 안들어오면 처리 안함
	if (FMath::IsNearlyZero(Value) == false)
	{
		// 변환 가능 모드일때만 저장한다
		if (GSCameraMode()->GetIsSavePossible() == true)
		{
			GSCameraMode()->SetBackupCamPitch();
		}
	}
}

// 줌모드 갱신
void UGsInputBindingLocalPlayer::UpdateZoomMode()
{
	// 두개 동시에 입력이 있어야 줌모드임...
	if (_bTouchPressed1 == true
		&& _bTouchPressed2 == true)
	{
		_isZoomMode = true;
	}
	else
	{
		_isZoomMode = false;
	}
}

bool UGsInputBindingLocalPlayer::IsInputBlock(EGsKeyMappingType In_Key)
{
	// 트레이 로딩 켜져있으면 막기
	if (GUI() && GUI()->IsLocked())
	{
		//GSLOG(Error, TEXT("Block touch by trayLoading"));
		return true;
	}

	// 특정 키 입력이 현재 상태에 막혀 있는지 체크
	if (_inputKeyEventValidator->IsValidEventToState(In_Key, _blockInputFlag))
	{
		return true;
	}

	// 키보드 인풋이 전부 막혀 있다면 블락 -> Validator 에서 처리하고 있음
	// 만약 터치 인풋 (EGsKeyMappingType::NONE) 에서 이상 동작 시 여기가 문제가 아닐 것임
	//if (EGsKeyMappingInputFlags::INPUT_LOCK_ALL == _blockInputFlag)
	//{
	//	return true;
	//}

	return false;
}

bool UGsInputBindingLocalPlayer::IsInputFlagBlock(EGsKeyMappingInputFlags In_Key)
{
	// 트레이 로딩 켜져있으면 막기
	if (GUI() && GUI()->IsLocked())
	{
		//GSLOG(Error, TEXT("Block touch by trayLoading"));
		return true;
	}

	// 특정 인풋 플래그가 켜져 있는지를 통해, 입력이 현재 상태에 막혀 있는지 체크
	return _inputKeyEventValidator->IsValidStateToState(In_Key, _blockInputFlag);
}

void UGsInputBindingLocalPlayer::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
{

	if (IsLocalTouchUseContentsGameState() == false)
	{
		return;
	}

	// 1, 2 번째 아니면 제낀다
	if (FingerIndex != ETouchIndex::Touch1 &&
		FingerIndex != ETouchIndex::Touch2)
	{
		return;
	}

	_touchPressTime = FDateTime::UtcNow();

	// 들어온걸로 저장
	if (ETouchIndex::Touch1 == FingerIndex)
	{
		_prevTouchLoc1 = Location;
		_bTouchPressed1 = true;
	}
	else if (ETouchIndex::Touch2 == FingerIndex)
	{
		_prevTouchLoc2 = Location;
		_bTouchPressed2 = true;
	}

	_pressLoc2D = (FVector2D)Location;


	if (false == IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
	{	
		FGsInputEventMsgBase InputMsg;
		GMessage()->GetInput().SendMessage(MessageInput::TouchPress, InputMsg);
	}

	UpdateZoomMode();

	if (_isTestPrintTouch == true)
	{
		const FString log =
			FString::Printf(TEXT("[TOUCH_TEST] OnTouchPress fingerIndex: %d, _isZoomMode: %d"),
				FingerIndex, _isZoomMode);

		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Yellow, *log);
	}

	if (_owner && UGsGameObjectLocalPlayer::EMovementType::Spectator == _owner->GetMovementType())
	{	// 2021/10/25 PKT - 관중 모드 이며 아래는 처리 하지 않겠다.
		return;
	}

	// 시퀀스 플레이일때는 처리 안함
	if (UGsSequencePlayer* sequencePlayer = GSequencePlayer())
	{
		if (sequencePlayer->IsPlaying() == true)
		{
			return;
		}
	}

	// 터치 시작이다
	_isTouchStart = true;
}


// 인터랙션 액터 클릭
bool UGsInputBindingLocalPlayer::TouchInteractionActor(const FVector2D& In_touchPos, IGsActorInteractionInterface** Out_interactionInterface)
{
	// 터치 처리
	AGsCharacterLocalPlayer* localCharacter = _owner->GetLocalCharacter();
	if (localCharacter == nullptr ||
		localCharacter->Controller == nullptr)
	{
		return false;
	}

	AGsPlayerController* const PC =
		CastChecked<AGsPlayerController>(localCharacter->Controller);

	if (nullptr == PC)
	{
		return false;
	}

	if (_isDrawDebugArrow == true)
	{
		FGsDrawDebugHelpers::DrawDebugArrow(In_touchPos);
	}

#if INTERACTION_DEBUG
	TArray<TSharedPtr<FName>> profileNameArray;
	UCollisionProfile::Get()->GetProfileNames(profileNameArray);
#endif

	// 채널종류별로 돌려본다


	// 그림자 전장 비석 뒤에 몬스터 터치 되는 이슈때문에
	// 단일 hit result이 아니라 multi로 뽑아서 거리순 정렬하여 제일 가까운걸로 처리
	// 채널들 모든체크 다뽑아서 한번에 정렬하게 해야함(pawn 이 pawn boss 뒤에 있을수 있음 ex)비석뒤 몬스터)
	// https://jira.com2us.com/jira/browse/C2URWQ-7877


	TArray<FGsHitResultSortInfo> arrayHitResultSortInfo;
	
	// 로컬 위치로 하면 로컬과 카메라 사이에 있는 녀석들은 계산이 이상하게 되어서 카메라 위치기준으로 해야함

	FVector currCamLocation = FVector::ZeroVector;
	APlayerCameraManager* camManager = PC->PlayerCameraManager;
	if (camManager != nullptr)
	{
		currCamLocation = camManager->GetCameraLocation();
	}
	

	FHitResult result;
	for (ECollisionChannel iter : _arrayChannelInteraction)
	{
		TArray<TEnumAsByte<EObjectTypeQuery> > arrayType;
		// "InteractionButton"
		EObjectTypeQuery objectTypeQuery = UEngineTypes::ConvertToObjectType(iter);
		arrayType.Add(objectTypeQuery);

#if INTERACTION_DEBUG
		if (profileNameArray.IsValidIndex((int32)objectTypeQuery))
		{
			GSLOG(Log, TEXT("TouchInteractionActor - array : %s"), *profileNameArray[(int32)objectTypeQuery].Get()->ToString());
		}
#endif

		// GetHitResultUnderCursorForObjects 함수 사용시 함수 내부의 마우스 위치 가져올때
		// 프레스에선 잘 동작했는데
		// 릴리즈 시점엔 마우스값이 -1로나와서 직접 밀어 넣어야 함

		
	
		TArray<FHitResult> HitResults;

		if (PC->GetHitResultsAtScreenPosition(In_touchPos, arrayType, false, HitResults))
		{

			for(auto iterHitResult: HitResults)
			{
				if(iterHitResult.GetActor() == nullptr)
				{
					continue;
				}
				FVector hitActorPos = iterHitResult.GetActor()->GetActorLocation();
				float distSquared = FVector::DistSquared2D(currCamLocation, hitActorPos);

				arrayHitResultSortInfo.Add(FGsHitResultSortInfo{
					distSquared, iterHitResult.GetActor(), iter,  hitActorPos });
			}
			

		}
	}

	if (arrayHitResultSortInfo.Num() == 0)
	{
		return false;
	}

	arrayHitResultSortInfo.Sort([](const FGsHitResultSortInfo& lhs, const FGsHitResultSortInfo& rhs)
		{
			return lhs._distSquared < rhs._distSquared;
		});


	AActor* targetActor = arrayHitResultSortInfo[0]._actor;
	ECollisionChannel channel = arrayHitResultSortInfo[0]._collisionChannel;
	if (targetActor)
	{
#if !UE_BUILD_SHIPPING
		if (-1 != UGsGameObjectNonPlayer::catchGhost)
		{
			if (AGsCharacterBase* logActor = Cast<AGsCharacterBase>(targetActor))
			{
				GSLOG(Error, TEXT("Dump Damage Log Start ================================================================================="));

				for (int i = UGsGameObjectNonPlayer::catchGhost; i < logActor->_eventLog.Num(); ++i)
				{
					SCREEN_MESSAGE(10004 + i, 200.0f, FColor::Green, "ID : %s => [%d] : [%s]", *logActor->_ServerID, i, *logActor->_eventLog[i]);
					GSLOG(Error, TEXT("ID : %s = > [%d] : [%s] "), *logActor->_ServerID, i, *logActor->_eventLog[i]);
				}
				GSLOG(Error, TEXT("Dump Damage Log End ==================================================================================="));
#if 1
				SCREEN_MESSAGE(20005, 200.0f, FColor::Red, "IsActorBeingDestroyed[%d]", logActor->IsActorBeingDestroyed());
				SCREEN_MESSAGE(20006, 200.0f, FColor::Red, "IsPendingKillOrUnreachable[%d]", logActor->IsPendingKillOrUnreachable());
				SCREEN_MESSAGE(20007, 200.0f, FColor::Red, "IsUnreachable[%d]", logActor->IsUnreachable());
				SCREEN_MESSAGE(20008, 200.0f, FColor::Red, "IsPendingKill[%d]", logActor->IsPendingKill());
				SCREEN_MESSAGE(20009, 200.0f, FColor::Red, "IsPendingKillPending[%d]", logActor->IsPendingKillPending());

				UGsGameObjectBase* FindObjActor = GSGameObject()->FindObject(targetActor);
				bool FindActor = (nullptr != FindObjActor);
				SCREEN_MESSAGE(20011, 200.0f, FColor::Red, "FindObjActor[%d]", FindActor);
#endif
			}
		}
#endif
		if (IGsActorInteractionInterface* interaction = Cast<IGsActorInteractionInterface>(targetActor))
		{
			*Out_interactionInterface = interaction;
			// 퀘스트 액티브 해제후 인터랙션 시작
			//GMessage()->GetGameObject().SendMessage(MessageGameObject::QUEST_ACTIVE_OFF, nullptr);				
			// 몬스터 타겟팅에서도 써서 클릭된 각자 대상에서 처리
			return true;
		}
		// 인터페이스가 없어도 터치 제외 충돌체면
		// 걍 리턴한다(오염체(그냥 터치 막는 충돌체) 뒤의 충돌체가 터치  타겟 되는 현상 막기 위해)
		else if (true == _arrayChannelNonTouchMoveTarget.Contains(channel))
		{
			return true;
		}

	}

	return false;
}

// 터치 이동 못하게 하는 충돌체(터치 이벤트를 그냥 소모하게 한다)
bool UGsInputBindingLocalPlayer::TouchNonTouchMoveActor(const FVector2D& In_touchPos)
{
	// 터치 처리
	AGsCharacterLocalPlayer* localCharacter = _owner->GetLocalCharacter();
	if (localCharacter == nullptr ||
		localCharacter->Controller == nullptr)
	{
		return false;
	}

	APlayerController* const PC =
		CastChecked<APlayerController>(localCharacter->Controller);

	if (nullptr == PC)
	{
		return false;
	}

	// 채널종류별로 돌려본다
	FHitResult result;
	for (ECollisionChannel iter : _arrayChannelNonTouchMoveTarget)
	{
		TArray<TEnumAsByte<EObjectTypeQuery> > arrayType;
		// "InteractionButton"
		EObjectTypeQuery objectTypeQuery = UEngineTypes::ConvertToObjectType(iter);
		arrayType.Add(objectTypeQuery);


		// GetHitResultUnderCursorForObjects 함수 사용시 함수 내부의 마우스 위치 가져올때
		// 프레스에선 잘 동작했는데
		// 릴리즈 시점엔 마우스값이 -1로나와서 직접 밀어 넣어야 함
		if (PC->GetHitResultAtScreenPosition(In_touchPos, arrayType, false, result))
		{
			if (result.GetActor())
			{
				return true;
			}
		}
	}

	return false;
}
void UGsInputBindingLocalPlayer::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
{

	if (IsLocalTouchUseContentsGameState() == false)
	{
		return;
	}

	// 1, 2 번째 아니면 제낀다
	if (FingerIndex != ETouchIndex::Touch1 &&
		FingerIndex != ETouchIndex::Touch2)
	{
		return;
	}
	// 스크린 모드 해제는 터치 이동과 상관없으므로 lock 처리 안함
	// 기사단 던전 관전 모드에서 스크린 모드 해제 안되는 이슈 수정
	// https://jira.com2us.com/jira/browse/C2URWQ-4957
	
	// 짧게 터치했으면 클릭으로 인식
	FTimespan touchTime = FDateTime::UtcNow() - _touchPressTime;
	if (0.1f >= touchTime.GetTotalSeconds())
	{
		GUI()->OnInputTouchClick();
	}
	

	// 땐거만 초기화
	if (ETouchIndex::Touch1 == FingerIndex)
	{
		_prevTouchLoc1 = FVector::ZeroVector;
		_bTouchPressed1 = false;
	}
	else if (ETouchIndex::Touch2 == FingerIndex)
	{
		_prevTouchLoc2 = FVector::ZeroVector;
		_bTouchPressed2 = false;
	}



	UpdateZoomMode();

	if (_isTestPrintTouch == true)
	{
		const FString log =
			FString::Printf(TEXT("[TOUCH_TEST] OnTouchRelease fingerIndex: %d, _isZoomMode: %d"),
				FingerIndex, _isZoomMode);

		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Yellow, *log);
	}

	// 이전 거리는 무조건 초기화
	_prevDist = 0.0f;

	bool isSimpleTouch = false;

	if (_isTouchStart == true)
	{
		// 눌렀을때랑 차이 많이 안날때만 처리
		FVector2D releasePos2D = (FVector2D)Location;
		float diffVal =
			GData()->GetGlobalData()->_touchMoveReleaseDiff;
#ifdef TEST_DIFF_CHECK
		float xDiff = FMath::Abs(releasePos2D.X - _pressLoc2D.X);
		float yDiff = FMath::Abs(releasePos2D.Y - _pressLoc2D.Y);
#endif

		if (UWorld* world = GetWorld())
		{
			if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
			{
				// 로비가 아닐때만 처리
				if (gameMode->GetGameModeType() != EGsGameModeType::Lobby)
				{
					if (_pressLoc2D.Equals(releasePos2D, diffVal) == true)
					{
						OnSimpleTouch(Location);
						isSimpleTouch = true;
					}
				}
			}
		}

		// 초기화
		_pressLoc2D = FVector2D::ZeroVector;
		// 터치 시작 초기화
		_isTouchStart = false;
	}
	if (false == IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
	{
		FGsInputEventMsgTouchRelease InputMsg(isSimpleTouch);
		GMessage()->GetInput().SendMessage(MessageInput::TouchRelease, InputMsg);
	}
}
// 지형 터치 처리
void UGsInputBindingLocalPlayer::ProcTouchTerrain(const FVector& In_touchLocation)
{

	FVector terrainPos;
	if (true == GetTerrainPosByScreenPos((FVector2D)In_touchLocation, terrainPos))
	{
		MakeTouchMoveActor(terrainPos);
		StartTerrainTouchMove(terrainPos);
	}
}
// 터치 이동 액터 만들기
void UGsInputBindingLocalPlayer::MakeTouchMoveActor(const FVector& In_pos)
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	// 기존꺼 삭제
	DeleteTouchMoveActor();

	//FString objectPath =
	//	GData()->GetGlobalData()->_touchMovePosObjectPath.ToString();
	//objectPath += TEXT("_C");
	//if (UClass* loadObjectC = LoadObject<UClass>(nullptr, *objectPath))
	//{
	//	FTransform tr = FTransform::Identity;
	//	tr.SetLocation(In_pos);

	//	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	//	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//	if (AActor* actor = Cast<AActor>(world->SpawnActor(loadObjectC, &tr, spawnParam)))
	//	{
	//		_touchMovePosActor = actor;
	//	}
	//}

	// 마커 클래스를 Initialize 에서 미리 로드 -> 약간의 최적화?
	if (_touchMovePosClass)
	{
		FTransform tr = FTransform::Identity;
		tr.SetLocation(In_pos);

		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* actor = Cast<AActor>(world->SpawnActor(_touchMovePosClass, &tr, spawnParam)))
		{
			_touchMovePosActor = actor;
		}
	}
}
// 터치 이동 액터 삭제
void UGsInputBindingLocalPlayer::DeleteTouchMoveActor()
{
	if (_touchMovePosActor == nullptr)
	{
		return;
	}

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	world->DestroyActor(_touchMovePosActor);
	_touchMovePosActor = nullptr;
}
// 로컬 죽을때
void UGsInputBindingLocalPlayer::OnLocalDie()
{
	DeleteTouchMoveActor();

	// 사망 시, 모든 단축키를 막을 것이다
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::INPUT_LOCK_ALL, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);
}
void UGsInputBindingLocalPlayer::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{

	if (IsLocalTouchUseContentsGameState() == false)
	{
		return;
	}

	if (false == _bTouchPressed1 &&
		false == _bTouchPressed2 ||
		// 1, 2 번째 아니면 제낀다
		(FingerIndex != ETouchIndex::Touch1 &&
			FingerIndex != ETouchIndex::Touch2))
	{
		return;
	}


#ifdef SHOW_LOG
	//GSLOG(Error, TEXT("OnTouchMove yaw: %f, pitch: %f"), yaw, pitch);
#endif

	//GSLOG(Warning, TEXT("OnTouchMove"));

	FVector prevTouchLocation = (_bTouchPressed1 == true) ?
		_prevTouchLoc1 : _prevTouchLoc2;

	// 터치 줌인, 아웃 처리
	if (ETouchIndex::Touch1 == FingerIndex)
	{
		_prevTouchLoc1 = Location;
	}
	else if (ETouchIndex::Touch2 == FingerIndex)
	{
		_prevTouchLoc2 = Location;
	}

	UpdateZoomMode();

	// 줌모드 아닐때만 회전 처리
	if (_isZoomMode == false)
	{
		if (false == IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
		{
			FVector dir = Location - prevTouchLocation;

			// 바뀐게 없다면 리턴
			if (dir.Equals(FVector::ZeroVector))
			{
				return;
			}

			float moveFactor = GetTouchMoveFactor();
			//float yaw = FMath::Clamp(dir.X * moveFactor, GetTouchMoveLimitMin(), GetTouchMoveLimitMax());
			//float pitch = FMath::Clamp(dir.Y * moveFactor, GetTouchMoveLimitMin(), GetTouchMoveLimitMax());
			float yaw = dir.X * moveFactor;
			float pitch = dir.Y * moveFactor;

			pitch *= (_rotPitchAspectRate * _pitchRotMul);
			// pitch 값은 apect rate 에서 
			// y/x 값이 되어야 함(즉 비율만큼 크기)
			OnMoveRotateYaw(yaw);
			OnMoveRotatePitch(pitch);
		}

		return;
	}


	// 1,2 거리 값>
	FVector distLoc = _prevTouchLoc1 - _prevTouchLoc2;

	// 새로운 dist 거리
	// SizeSquare2D 쓰면 값이 너무 커짐...
	float newDist = distLoc.Size2D();
	// 이전 터치가 없었으면(이번이 두손가락 동시 첫터치면)
	// 나감
	if (_prevDist == 0.0f)
	{
		// 백업
		_prevDist = newDist;
		return;
	}

	// 이전과 차이(손가락 사이 거리 변화값)
	// 양수면 줌인
	// 음수면 줌아웃
	float diffDist = newDist - _prevDist;

	// 백업
	_prevDist = newDist;

	// 바뀐게 없음
	// 나감
	if (diffDist == 0.0f)
	{
		return;
	}

	// 몬가 움직이면 터치 아니다
	if (_isTouchStart == true)
	{
		_isTouchStart = false;
	}

	FGsCameraModeHandler* cameraModehandler = _owner->GetCameraModeHandler();
	// null 이거나
	// 줌 사용안하면 return
	if (nullptr == cameraModehandler ||
		cameraModehandler->GetState()->IsUseInputZoom() == false)
	{
		return;
	}

	if (false == IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
	{
		// 줌 강도
		float zoomPower = FMath::Abs(diffDist) * _touchZoomFactor;

		// 줌인
		if (diffDist > 0.0f)
		{
			cameraModehandler->GetState()->ZoomIn(zoomPower);
		}
		// 줌아웃
		else
		{
			cameraModehandler->GetState()->ZoomOut(zoomPower);
		}
	}

}

void UGsInputBindingLocalPlayer::OnTouchJoystickPress()
{
	_joystickPressTime = FDateTime::UtcNow();
	_isJoystickClicked = false;
}

void UGsInputBindingLocalPlayer::OnTouchJoystickRelease()
{
	FTimespan touchTime = FDateTime::UtcNow() - _joystickPressTime;
	if (0.2f >= touchTime.GetTotalSeconds())
	{
		_isJoystickClicked = true;
	}
}

// 줌인
void UGsInputBindingLocalPlayer::OnZoomIn()
{
	if (IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
	{
		return;
	}

	if (IsLocalTouchUseContentsGameState() == false)
	{
		return;
	}

	if (FGsCameraModeHandler* cameraModehandler = _owner->GetCameraModeHandler())
	{
		// 줌 사용안하면 return
		if (cameraModehandler->GetState()->IsUseInputZoom() == false)
		{
			return;
		}
		cameraModehandler->GetState()->ZoomIn(GData()->GetGlobalData()->CamZoomPowerPC);

	}
}
// 줌아웃
void UGsInputBindingLocalPlayer::OnZoomOut()
{
	if (IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
	{
		return;
	}

	if (IsLocalTouchUseContentsGameState() == false)
	{
		return;
	}

	if (FGsCameraModeHandler* cameraModehandler = _owner->GetCameraModeHandler())
	{
		// 줌 사용안하면 return
		if (cameraModehandler->GetState()->IsUseInputZoom() == false)
		{
			return;
		}
		cameraModehandler->GetState()->ZoomOut(GData()->GetGlobalData()->CamZoomPowerPC);


	}
}

// 카메로 모드 변경
void UGsInputBindingLocalPlayer::OnViewChange()
{
	if (FGsCameraModeHandler* cameraModehandler = _owner->GetCameraModeHandler())
	{
		cameraModehandler->NextStep();
	}
}

// 테스트 오토 체인지
void UGsInputBindingLocalPlayer::OnTestAutoChange()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_ACTIVE_TOGGLE, nullptr);
}
// 테스트 오토 체인지 타이머 
void UGsInputBindingLocalPlayer::OnToggleTestAutoChangeTimer()
{
	_isTestAutoInputTimerOn = !_isTestAutoInputTimerOn;

	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}
	if (true == _isTestAutoInputTimerOn)
	{
		world->GetTimerManager().SetTimer(_testAutoInputTimeHandle,
			FTimerDelegate::CreateUObject(this, &UGsInputBindingLocalPlayer::OnTestAutoChange), 0.1f, true);
	}
	else
	{
		world->GetTimerManager().ClearTimer(_testAutoInputTimeHandle);
		_testAutoInputTimeHandle.Invalidate();
	}
}
void UGsInputBindingLocalPlayer::OnPressSkill(const FGsInputEventMsgBase& InMsg)
{
	if (const FGsSkill* skillData = GSkill()->FindSkillSlot(InMsg.Id))
	{
		if (FGsSkillHandlerLocalPlayer* skillHandler = _owner->GetCastSkill<FGsSkillHandlerLocalPlayer>())
		{
			// 평타는 Press에 발동
			if (skillHandler->IsComboSkill(skillData->_tableData->id))
			{
				FGsSkillHandlerLocalPlayer::SetPress(FGsSkillHandlerLocalPlayer::InputPressType::ComboSkillbutton);
				GMessage()->GetSkillRserveActiveState().SendMessage(MessageContentHud::SKILL_ACTIVE, InMsg.Id);
			}
		}
	}

	// 이동중 멈추는 현상 수정중
	// 스킬에서 처리해서 일단 막아놓음
	// https://jira.com2us.com/jira/browse/C2URWQ-6334

	// 예약된 데이터 삭제
	// https://jira.com2us.com/jira/browse/C2URWQ-6334

}

void UGsInputBindingLocalPlayer::OnReleaseSkill(const FGsInputEventMsgBase& InMsg)
{
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}
	AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>();
	if (gameMode == nullptr)
	{
		return;
	}

	if (gameMode->GetGameModeType() != EGsGameModeType::Lobby)
	{
		if (const FGsSkill* skillData = GSkill()->FindSkillSlot(InMsg.Id))
		{
			if (FGsSkillHandlerLocalPlayer* skillhandler = _owner->GetCastSkill<FGsSkillHandlerLocalPlayer>())
			{
				if (skillhandler->IsComboSkill(skillData->_tableData->id))
				{
					FGsSkillHandlerLocalPlayer::SetRelease(FGsSkillHandlerLocalPlayer::InputPressType::ComboSkillbutton);
				}
			}
		}
	}
}

void UGsInputBindingLocalPlayer::OnClickSkill(const FGsInputEventMsgBase& InMsg)
{
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}
	AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>();
	if (gameMode == nullptr)
	{
		return;
	}

	if (gameMode->GetGameModeType() != EGsGameModeType::Lobby)
	{
		if (const FGsSkill* skillData = GSkill()->FindSkillSlot(InMsg.Id))
		{
			if (FGsSkillHandlerLocalPlayer* skillhandler = _owner->GetCastSkill<FGsSkillHandlerLocalPlayer>())
			{
				if (false == skillhandler->IsComboSkill(skillData->_tableData->id))
				{
					GMessage()->GetSkillRserveActiveState().SendMessage(MessageContentHud::SKILL_ACTIVE, InMsg.Id);
				}
			}
		}
	}
	else
	{
		if (IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
		{
			return;
		}

		GMessage()->GetSkillRserveActiveState().SendMessage(MessageContentHud::SKILL_ACTIVE, InMsg.Id);
	}

	// 이동중 멈추는 현상 수정중
	// 스킬에서 처리해서 일단 막아놓음
	// https://jira.com2us.com/jira/browse/C2URWQ-6334

	// 예약된 데이터 삭제
	// https://jira.com2us.com/jira/browse/C2URWQ-6334

}

void UGsInputBindingLocalPlayer::OnUnmountVehicle(const FGsInputEventMsgBase& InMsg)
{
	FGsNetSendServiceWorld::SendUnmountVehicle();
}

void UGsInputBindingLocalPlayer::OnViewChange(const FGsInputEventMsgBase& InMsg)
{
	OnViewChange();
}

void UGsInputBindingLocalPlayer::SetIsBlockInputFlag(EGsKeyMappingInputFlags InFlag, bool InIsAdd /*= true*/, bool InFlushAll /*= false*/)
{
	// 죽음, 스폰 등의 특수 케이스의 경우, 기존에 쌓인 Flag 를 전부 날려준다
	if (InFlushAll)
	{
		_blockInputFlagSet.Empty();
		_blockInputFlag = InFlag;
	}
	// 그 외의 케이스의 경우, 중첩 관련 처리를 진행
	else
	{
		// Set 에 Flag 추가/삭제
		if (InIsAdd)
		{
			_blockInputFlagSet.Add(InFlag);
		}
		else
		{
			_blockInputFlagSet.Remove(InFlag);
		}

		// _blockInputFlag 를 일단 0 으로 세팅 (초기화)
		_blockInputFlag = EGsKeyMappingInputFlags::INPUT_NONE;

		// Set 을 순회하며 _blockInputFlag 를 세팅해준다
		for (const auto& flag : _blockInputFlagSet)
		{
			EnumAddFlags(_blockInputFlag, flag);
		}
	}

	// 튜토리얼이 시작할 때, 인풋 블락 메시지를 보내주므로, 특별취급할 필요가 없을 것으로 보임
	//// 튜토리얼 모드일 경우, 플래그 추가
	//if (FGsTutorialManager* tutorialMgr = GSTutorial())
	//{
	//	if (tutorialMgr->IsActiveTutorial())
	//	{
	//		//EnumAddFlags(_blockInputFlag, EGsKeyMappingInputFlags::SETTING_TUTORIAL);
	//		_blockInputFlag = EGsKeyMappingInputFlags::SETTING_TUTORIAL;
	//		_blockInputFlagStack.Emplace(EGsKeyMappingInputFlags::SETTING_TUTORIAL);
	//	}
	//}

	// 움직임을 막을 경우 기존값 초기화
	if (EnumHasAnyFlags(_blockInputFlag, EGsKeyMappingInputFlags::INPUT_KEY_MOVE))
	{
		_axisValue = FVector::ZeroVector;
		_axisYForwardValue = .0f;
		_axisYBackwardValue = .0f;
		_axisXRightValue = .0f;
		_axisXLeftValue = .0f;
	}
}

// 플래그 기반 입력 막기 이벤트
void UGsInputBindingLocalPlayer::OnBlockInputFlag(const FGsInputEventMsgBase& InMsg)
{
	const FGsInputEventMsgFlag* msgFlag = StaticCast<const FGsInputEventMsgFlag*>(&InMsg);
	if (nullptr == msgFlag)
	{
		GSLOG(Error, TEXT("nullptr == msgFlag"));
		return;
	}

	// 플래그 타입으로 조절
	SetIsBlockInputFlag(StaticCast<EGsKeyMappingInputFlags>(msgFlag->Id), msgFlag->IsAdd);
}


// 조이스틱 놓는 순간 호출
void UGsInputBindingLocalPlayer::OnReleaseJoystick()
{
	FGsSkillHandlerLocalPlayer::SetRelease(FGsSkillHandlerLocalPlayer::InputPressType::Joystick);

	if (false == EnumHasAnyFlags(_blockInputFlag, EGsKeyMappingInputFlags::INPUT_KEY_MOVE))
	{			
		FGsInputEventMsgBase InputMsg;
		GMessage()->GetInput().SendMessage(MessageInput::ReleaseJoystick, InputMsg);
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("Log - block EGsKeyMappingInputFlags::INPUT_KEY_MOVE"));
#endif
	}

	_isJoystickPressed = false;
}

// 조이스틱 누르는 순간 호출
void UGsInputBindingLocalPlayer::OnPressJoystick()
{
	FGsSkillHandlerLocalPlayer::SetPress(FGsSkillHandlerLocalPlayer::InputPressType::Joystick);

	if (false == EnumHasAnyFlags(_blockInputFlag, EGsKeyMappingInputFlags::INPUT_KEY_MOVE))
	{
		FGsInputEventMsgBase InputMsg;
		GMessage()->GetInput().SendMessage(MessageInput::PressJoystick, InputMsg);
	}
	else
	{
#if WITH_EDITOR
		GSLOG(Error, TEXT("Log - block EGsKeyMappingInputFlags::INPUT_KEY_MOVE"));
#endif
	}

	_isJoystickPressed = true;

	// 예약된 데이터 삭제
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	
	// 신규 코드
	ClearReserveDataAndAutoMoveStop();
}

void UGsInputBindingLocalPlayer::OnBackKey()
{
	// 만약 INPUT_KEY_NO(ESC, 안드로이드 Back 키) 키가 막혀있는 상황이라면, UI 닫음 처리하지 않는다
	if (IsInputFlagBlock(EGsKeyMappingInputFlags::INPUT_KEY_NO))
		return;

	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->Back();
	}
}

// 테스트 포션 이펙트 출력
void UGsInputBindingLocalPlayer::OnTestPostionEffect()
{
	if (_owner != nullptr)
	{
		UGsEffectComponent* effectComp =
			_owner->GetLocalCharacter()->GetEffectComponent();

		if (effectComp != nullptr)
		{
			effectComp->OnEffect(GData()->GetGlobalData()->_potionParticle);
		}
	}
}

// 테스트 디버프 액티브
void UGsInputBindingLocalPlayer::OnTestDebuffActive()
{
	if (_owner != nullptr)
	{
		UGsEffectComponent* effectComp =
			_owner->GetLocalCharacter()->GetEffectComponent();

		if (effectComp != nullptr)
		{
			effectComp->OnDebuffActive();
		}
	}
}
// 테스트 디버프 디액티브
void UGsInputBindingLocalPlayer::OnTestDebuffDeactive()
{
	if (_owner != nullptr)
	{
		UGsEffectComponent* effectComp =
			_owner->GetLocalCharacter()->GetEffectComponent();

		if (effectComp != nullptr)
		{
			effectComp->OnDebuffDeactive();
		}
	}
}

void UGsInputBindingLocalPlayer::OnTestPrintTouch(const FGsInputEventMsgBase& InMsg)
{
	_isTestPrintTouch = !_isTestPrintTouch;

	if (_isTestPrintTouch == false)
	{
		// 메시지 지우기
		GEngine->ClearOnScreenDebugMessages();
	}
}

void UGsInputBindingLocalPlayer::OnSpectatorTerrainTouchEffect(const IGsMessageParam* InParam)
{
	const FGsInputEventMsgLocation* param = StaticCast<const FGsInputEventMsgLocation*>(InParam);

	FVector startPos = param->Pos;
	FVector endPos = startPos + param->Dir * 10000.0f;

	UWorld* world = GetWorld();
	if (nullptr == world)
	{
		return;
	}

	TArray<FHitResult> arrayHitRes;
	world->LineTraceMultiByObjectType(
		OUT arrayHitRes,
		startPos, endPos,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic));

	if (arrayHitRes.Num() == 0)
	{
		return;
	}

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
	if (NavSys)
	{
		FNavLocation navLoc;
		FVector extendPos(0.0f, 0.0f, 5000.0f);

		for (const FHitResult& result : arrayHitRes)
		{
			if (NavSys->ProjectPointToNavigation(result.Location, navLoc, extendPos) == true)
			{
				MakeTouchMoveActor(navLoc.Location);
				break;
			}
		}
	}
}

// 컨텐츠 상태가 local 터치 사용하나
// 뽑기 제외
bool UGsInputBindingLocalPlayer::IsLocalTouchUseContentsGameState()
{
	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return true;
	}

	if (true == summonHandler->GetIsSummonPlay())
	{
		return false;
	}

	if (auto widget = Cast<UGsUIPopupChat>(GUI()->GetWidgetByKey(TEXT("PopupChat"))))
	{
		return widget->IsInputFocused() ? false : true;
	}


	return true;
}

void UGsInputBindingLocalPlayer::OnLocalPlayerDie(const IGsMessageParam*)
{
	SetIsBlockInputFlag(EGsKeyMappingInputFlags::INPUT_LOCK_ALL, true, true);
}

void UGsInputBindingLocalPlayer::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	if (UWorld* MyWorld = GetWorld())
	{
		FTimerManager& TimerManager = MyWorld->GetTimerManager();
		TimerManager.SetTimerForNextTick(FSimpleDelegate::CreateWeakLambda(this, [this]() { 
			SetIsBlockInputFlag(EGsKeyMappingInputFlags::INPUT_NONE, true, true); 
			}));
	}

	// 윈도우 
#if WITH_EDITOR || PLATFORM_WINDOWS	
	GInputSettings()->RequestKeyboardMapping();
#endif
}

// 이동 성공
void UGsInputBindingLocalPlayer::CallbackTouchMoveEnd()
{
	if (_owner == nullptr)
	{
		return;
	}

	ClearReserveTouchMoveData();
	DeleteTouchMoveActor();

	// auto move 멈춤
	_owner->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	// 이동 성공시 돌아갈 위치 현재 위치로 저장
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_SAVE_AUTO_POSITION, nullptr);


	FGsMovementHandlerLocalPlayer* movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (movement == nullptr)
	{
		return;
	}
	movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);
}
// 이동 실패
void UGsInputBindingLocalPlayer::CallbackTouchMoveFailed(const FPathFollowingResult& Result)
{
	// 피격시 자동 이동 취소 되는 현상 수정하기 위해
	// 여기서 데이터 클리어 안함
	// 1. 수동 입력(이동, 스킬)시 클리어
	// 2. 다른 reserve data 입력시 클리어
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	

	DeleteTouchMoveActor();

	// auto move 멈춤
	_owner->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

	// 이동 실패시도 돌아갈 위치 현재 위치로 저장
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_SAVE_AUTO_POSITION, nullptr);
}

// 터치 이동 예약된게 있나
bool UGsInputBindingLocalPlayer::IsReserveTouchMoveData()
{
	return _reserveTouchMoveInfo._isReserved;
}
// 예약된 터치 이동 진행
void UGsInputBindingLocalPlayer::ProcessReserveTouchMove()
{
	if (_owner == nullptr)
	{
		return;
	}

	FGsSkillHandlerLocalPlayer* skillHandler = _owner->GetCastSkill<FGsSkillHandlerLocalPlayer>();
	if (skillHandler == nullptr)
	{
		return;
	}

	// 스킬 요청 보낸 상태면 일단 대기
	if (skillHandler->IsRequestData())
	{
		return;
	}

	// 도착했는지 체크
	if (IsInsideTerrainTouchMovePosRange(_reserveTouchMoveInfo._touchPos) == true)
	{
		// 도착함
		ClearReserveTouchMoveData();
		DeleteTouchMoveActor();

		// auto move 멈춤
		_owner->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
	}
	else
	{
		// ai에서 이동시 auto 해제 조건있으면 auto 해제
		GSAI()->StartTouchMove();

		FGsMovementHandlerLocalPlayer* movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>();

		if (movement == nullptr)
		{
			return;
		}
		
		skillHandler->ClearReserveSkill();

		float acceptanceRadius = FGsMove::GetMovePosRange();

		// https://jira.com2us.com/jira/browse/C2URWQ-3437
		// auto move delegate 연동은 안되었는데 interaction data만 세팅 되어있다면....
		// interaction data clear 한다(fail callback 으로 호출 못하므로)
		// 다른데서도 이러면 공통적으로 처리할 필요가 있다
		FGsInteractionHandlerLocalPlayer* localInteraction = _owner->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
		if (localInteraction != nullptr)
		{
			if(true == localInteraction->IsReserveInteraction())
			{ 
				FGsMovementStateAuto& moveStateAuto = movement->GetStateAutoMove();
				const FGsAutoMoveInfo& autoMoveInfo = moveStateAuto.GetData();

				if(autoMoveInfo._funcFailDelegate.IsBound() == false)
				{
					localInteraction->ClearBindAUtoMoveMessageByNPCFunctionSlot();
					localInteraction->ClearReserveInteraction();
				}
			}
		}

		// 터치 이동 시작때 나머지 reserve 데이터 삭제(현재는 waypoint, input handler (터치 이동) 이동만 처리)
		// https://jira.com2us.com/jira/browse/C2URWQ-6334
		_owner->ClearReserveDataByReserveWork(this);


		movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::TerrainTouchMove);
		movement->ChangeAutoMoveState().StartPos(_owner,
			_reserveTouchMoveInfo._touchPos, acceptanceRadius,
			FGsAutoMoveCompletedDelegate::CreateUObject(this,
				&UGsInputBindingLocalPlayer::CallbackTouchMoveEnd),
			FGsAutoMoveCompletedOneDelegate::CreateUObject(this,
				&UGsInputBindingLocalPlayer::CallbackTouchMoveFailed));
	}
}

// 예약된 터치 이동 정보 클리어
void UGsInputBindingLocalPlayer::ClearReserveTouchMoveData()
{
	_reserveTouchMoveInfo._isReserved = false;
}

// 지형 터치 이동 시작
void UGsInputBindingLocalPlayer::StartTerrainTouchMove(const FVector& In_pos)
{
	// start auto loot cool time
	GSAI()->RequestAutoLootCooltime();

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);


	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (gameUserSettings == nullptr)
	{
		return;
	}
	bool touchTerrainKeepTarget =
		(bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::KEEP_TARGET_GROUND_TOUCH);

	if (false == touchTerrainKeepTarget)
	{
		UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		if (localBase == nullptr)
		{
			return;
		}
		FGsTargetHandlerLocalPlayer* targetHandler = localBase->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		if (targetHandler == nullptr)
		{
			return;
		}

		targetHandler->ClearTarget(true);
	}

	// quest에게 message 전송
	FGsInputEventMsgBase InputMsg;
	GMessage()->GetInput().SendMessage(MessageInput::TerrainTouchMove, InputMsg);

	ClearReserveTouchMoveData();

	_reserveTouchMoveInfo._isReserved = true;
	_reserveTouchMoveInfo._touchPos = In_pos;

	ProcessReserveTouchMove();	
}

// 지형 터치 이동 범위안인가
bool UGsInputBindingLocalPlayer::IsInsideTerrainTouchMovePosRange(const FVector& In_targetPos)
{
	if (_owner == nullptr)
	{
		return false;
	}

	float distanceSq = FVector::DistSquared2D(In_targetPos, _owner->GetLocation());

	float interactionRange = FGsMove::GetMovePosRange(true);

	if (distanceSq <= interactionRange * interactionRange)
	{
		return true;
	}
	return false;
}

// 단순 터치 처리(drag x)
void UGsInputBindingLocalPlayer::OnSimpleTouch(FVector In_touchLocation)
{
	if (false == IsInputBlock(EGsKeyMappingType::TOUCH_INTERACTION))
	{
		// 인터랙션 버튼 클릭인가
		if (true == GSInteraction()->CheckInteractionButtonClick())
		{
			return;
		}

		// 터치 대상인가
		IGsActorInteractionInterface* interactionTargetActor = nullptr;
		if (TouchInteractionActor((FVector2D)In_touchLocation, &interactionTargetActor) == true)
		{
			if (interactionTargetActor != nullptr)
			{
				interactionTargetActor->ClickInteraction();
				return;
			}
			// 인터랙션 인터페이스 없는 충돌체가 있을수 있따
			// ex: 오염체
			else
			{
				return;
			}
		}
		// 시공의 틈새에서 오염체 안으로 클릭 이동 마크 표시되어서 해당 충돌체
		// 클릭 이벤트 소모하게 처리
		if (TouchNonTouchMoveActor((FVector2D)In_touchLocation) == true)
		{
			return;
		}
	}
	if (false == IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
	{
		// 조이스틱 안쓸때만 사용
		if (_isJoystickPressed == false)
		{
			// 옵션 체크			
			bool isUseTouchMove = false;
			if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
			{
				const int32 isUseTouchMoveInt =
					gameUserSettings->GetInputSetting(EGsOptionInput::IS_USE_TOUCH_MOVE);

				isUseTouchMove = (isUseTouchMoveInt == 0) ? false : true;
			}

			// 관전모드에서 사용 안함
			if (isUseTouchMove == true &&
				GSObserver()->GetIsObserveStart() == false)
			{
				// 지형 터치 처리
				ProcTouchTerrain(In_touchLocation);
			}
		}
	}
}

float UGsInputBindingLocalPlayer::GetTouchMoveFactor()
{
	float moveFactor = 0.0f;
	// PC라면
#if WITH_EDITOR || PLATFORM_WINDOWS	
	moveFactor = _touchMoveFactorPC;
#else
	moveFactor = _touchMoveFactorMobile;
#endif
	return moveFactor;
}

void UGsInputBindingLocalPlayer::SetTouchMoveFactor(float In_val)
{
	// PC라면
#if WITH_EDITOR || PLATFORM_WINDOWS	
	_touchMoveFactorPC = In_val;
#else
	_touchMoveFactorMobile = In_val;
#endif
}

// load to po start
void UGsInputBindingLocalPlayer::OnLoadTopoStart(const IGsMessageParam* inParam)
{
	// LoadTopoStart 는 특수하다. 기존의 Stack 을 무조건 다 비워주어야 하기 때문이다.
	// 또한 State 가 종료되는 메시지도 짝으로 보내주지 않기 때문에, 매뉴얼하게 처리해야 한다.
	_blockInputFlag = EGsKeyMappingInputFlags::INPUT_LOCK_ALL;
	_blockInputFlagSet.Empty();

	//SetIsBlockInputFlag(EGsKeyMappingInputFlags::INPUT_LOCK_ALL);
}

bool UGsInputBindingLocalPlayer::IsReserveData()
{
	return IsReserveTouchMoveData();
}

void UGsInputBindingLocalPlayer::DoRetry()
{
	// 예약 클리어가 있기 때문에 처리하지 않음 (IdleState에서 처리하게 유도)
	// ProcessReserveTouchMove();
}

bool UGsInputBindingLocalPlayer::GetTerrainPosByScreenPos(const FVector2D& In_screenPos, FVector& Out_terrainPos)
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return false;
	}
	AGsCharacterLocalPlayer* localCharacter = _owner->GetLocalCharacter();
	if (localCharacter == nullptr ||
		localCharacter->Controller == nullptr)
	{
		return false;
	}

	APlayerController* const PC =
		CastChecked<APlayerController>(localCharacter->Controller);


	FVector mousePos = FVector::ZeroVector;
	FVector mouseRot = FVector::ZeroVector;


	PC->DeprojectScreenPositionToWorld(In_screenPos.X, In_screenPos.Y,
		mousePos, mouseRot);

	if (_isShowDebugTouchMove == true)
	{
		DrawDebugCircle(world, mousePos,
			100, 100, FColor::Green, false, 10.0f,
			0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
	}

	FVector endPos = mousePos + mouseRot * 10000.0f;
	if (_isShowDebugTouchMove == true)
	{
		DrawDebugLine(world, mousePos, endPos, FColor::Green, false, 10.0f);
	}

	TArray<FHitResult> arrayHitRes;
	world->LineTraceMultiByObjectType(
		OUT arrayHitRes,
		mousePos, endPos,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic));

	if (arrayHitRes.Num() == 0)
	{
		return false;
	}

	if (_isShowDebugTouchMove == true)
	{
		for (const FHitResult& result : arrayHitRes)
		{
			DrawDebugCircle(world, result.Location,
				100, 100, FColor::Black, false, 10.0f,
				0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));

			if (nullptr != result.Actor)
			{
				DrawDebugString(
					world, result.Location, result.Actor->GetName(),
					nullptr,
					FColor::White,
					10.0f);
			}
		}
	}


	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
	if (NavSys)
	{
		FNavLocation navLoc;
		// 기존값 5000->1000으로 테스트
		// https://jira.com2us.com/jira/browse/C2URWQ-4774
		float touchMoveNavigationDepth =  GData()->GetGlobalData()->_touchMoveNavigationDepth;

		FVector extendPos(0.0f, 0.0f, touchMoveNavigationDepth);


		TArray<FVector> passLocation;
		TArray<FHitResult> passResult;
		for (const FHitResult& result : arrayHitRes)
		{
			if (NavSys->ProjectPointToNavigation(result.Location, navLoc, extendPos) == true)
			{
				if (_isShowDebugTouchMove == true)
				{
					GSLOG(Log, TEXT("pick pos: %s"), *(navLoc.Location.ToString()));
					DrawDebugCircle(world, navLoc.Location,
						100, 100, FColor::Red, false, 10.0f,
						0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
				}

				passLocation.Add(navLoc.Location);
				passResult.Add(result);
			}
		}

		if (passLocation.Num() != 0)
		{
			FVector firstPos = passLocation[0];

			if (_isShowDebugTouchMove == true)
			{
				DrawDebugCircle(world, firstPos,
					100, 100, FColor::Magenta, false, 10.0f,
					0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
			}

			Out_terrainPos = firstPos;
			return true;
		}
	}

	return false;
}

// https://jira.com2us.com/jira/browse/C2URWQ-5354
// 플레이어블 이벤트 평타 스페이스바 인풋 받기
void UGsInputBindingLocalPlayer::OnPlayableEventNoramlAttack()
{
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}
	AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>();
	if (gameMode == nullptr)
	{
		return;
	}

	if (gameMode->GetGameModeType() != EGsGameModeType::Lobby)
	{
		return;
	}

	if (IsInputBlock(EGsKeyMappingType::TOUCH_MOVE))
	{
		return;
	}

	GMessage()->GetSkillRserveActiveState().SendMessage(MessageContentHud::SKILL_ACTIVE, 0);
}
// 1차 타겟
void UGsInputBindingLocalPlayer::OnLocalTargetChanged(const IGsMessageParam* In_param)
{
	// 신규 코드
	ClearReserveDataAndAutoMoveStop();
}

// 예약된 데이터 클리어해라
void UGsInputBindingLocalPlayer::ClearReserveData()
{
	ClearReserveTouchMoveData();

	// 이동 이펙트 삭제
	DeleteTouchMoveActor();
}

// 예약 데이터 삭제와 auto move stop으로 상태 전환
// 스킬에서 특정 이동일때만 처리하는거 때문에 처리
// https://jira.com2us.com/jira/browse/C2URWQ-6334
void UGsInputBindingLocalPlayer::ClearReserveDataAndAutoMoveStop()
{
	if (true == IsReserveTouchMoveData())
	{
		// 예약된 데이터 삭제
		// https://jira.com2us.com/jira/browse/C2URWQ-6334
		ClearReserveTouchMoveData();

		// 이동 이펙트 삭제
		DeleteTouchMoveActor();

		// 이동중 멈추는 현상 수정중
		// 스킬에서 처리해서 일단 막아놓음
		// https://jira.com2us.com/jira/browse/C2URWQ-6334

		//// auto move state 이면 auto move stop 으로 상태 전환
		//if(_owner->GetBaseFSM()->IsState(EGsStateBase::AutoMove) == true)
		//{ 
		//	_owner->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

		//	if (FGsMovementHandlerLocalPlayer* movement = _owner->GetCastMovement<FGsMovementHandlerLocalPlayer>())
		//	{
		//		// 실제 이동하려던 정보 Clear
		//		FGsMovementStateAuto& moveStateAuto = movement->GetStateAutoMove();
		//		moveStateAuto.ClearData();
		//	}
		//}


	}
}