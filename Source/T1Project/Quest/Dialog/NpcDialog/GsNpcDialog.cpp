// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNpcDialog.h"

#include "Camera/Define/EGsDialogCameraViewType.h"

#include "NpcDialog/GsSchemaNpcDialogData.h"
#include "NpcDialog/GsSchemaNpcDialogCutData.h"
#include "NpcDialog/GsSchemaNpcDialogActorData.h"

#include "Quest/Dialog/GsDialogBase.h"
#include "Quest/Management/GsQuestActionManagement.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "UI/UIContent/Dialog/GsUIDialogNpc.h"
#include "UI/UIContent/HUD/GsUIHUDEmoticon.h"
#include "UI/UIContent/Tray/GsUITrayFadeEffect.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

#include "Message/MessageParam/GsCameraMessageParam.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Skill/GsSchemaSkillCommon.h"
#include "Option/GsGameUserSettings.h"
#include "Sound/GsSoundMixerController.h"

#include "Camera/GsSchemaCameraDialogViewData.h"
#include "Camera/Define/GsCameraGameDefine.h"
#include "Data/GsDataContainManager.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"


void FGsNpcDialog::OpenDialog(const FName& InKey, int32 inDialogId)
{
	Super::OpenDialog(InKey, inDialogId);
}

void FGsNpcDialog::OpenDialog(const FName& InKey, const FGsSchemaNpcDialogData* inNpcDialogData, 
								UGsGameObjectBase* objectBase, const FGsSchemaCameraDialogViewData* inViewData)
{
	Super::OpenDialog(InKey);
	
	_playerObject = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	_npcObject = objectBase;
	_npcDialogData = inNpcDialogData;
	_cameraViewType = inViewData;

	if (FGsSkillHandlerBase* skillHandler = _playerObject != nullptr ? _playerObject->GetSkillHandler() : nullptr)
	{
		skillHandler->SetAutoIdleEmotionEnable(false);
		skillHandler->SetEmotionLock(true); //quest의 socialAction도 크게 우선 순위가 높지 않아, 직접 호출후 lock 걸어둠
	}
	if (FGsSkillHandlerBase* skillHandler = _npcObject != nullptr ? _npcObject->GetSkillHandler() : nullptr)
	{
		skillHandler->SetAutoIdleEmotionEnable(false);
		skillHandler->SetEmotionLock(true);
	}
	
	if (_playerObject != nullptr) StartMotion(_playerObject, 0); //상태전환만
	if (_npcObject != nullptr)	  StartMotion(_npcObject, 0); //상태전환만

	// 사운드 믹서 변경
	if (UGsSoundManager* soundManager = GSound())
	{
		if(UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
			soundMixerController->OnMixChanged(EGsSoundMixType::NpcDialog);
	}

	if (nullptr != _npcDialogData)
	{
		_cutGroupCount = _remainGroupCount = _npcDialogData->cutGroupId.Num();

		SetCutGroupData();
	}
}	

// 대화창 오픈
void FGsNpcDialog::OpenDialog(const FName& InKey, int32 inDialogId, 
								UGsGameObjectBase* objectBase, const FGsSchemaCameraDialogViewData* inViewData)
{
	Super::OpenDialog(InKey, inDialogId);	

	_playerObject = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	_npcObject = objectBase;
	_npcDialogData = GetNpcDialogData(inDialogId);
	_cameraViewType = inViewData;

	if (FGsSkillHandlerBase* skillHandler = _playerObject != nullptr ? _playerObject->GetSkillHandler() : nullptr)
	{
		skillHandler->SetAutoIdleEmotionEnable(false);
		skillHandler->SetEmotionLock(true); //quest의 socialAction도 크게 우선 순위가 높지 않아, 직접 호출후 lock 걸어둠
	}
	if (FGsSkillHandlerBase* skillHandler = _npcObject != nullptr ? _npcObject->GetSkillHandler() : nullptr)
	{
		skillHandler->SetAutoIdleEmotionEnable(false);
		skillHandler->SetEmotionLock(true);
	}

	if (_playerObject != nullptr) StartMotion(_playerObject, 0); //상태전환만
	if (_npcObject != nullptr)	  StartMotion(_npcObject, 0); //상태전환만

	// 사운드 믹서 변경
	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
			soundMixerController->OnMixChanged(EGsSoundMixType::NpcDialog);
	}

	if (nullptr != _npcDialogData)
	{
		_cutGroupCount = _remainGroupCount = _npcDialogData->cutGroupId.Num();
		
		SetCutGroupData();
	}
}

// 대화창 오픈
void FGsNpcDialog::OpenDialog(const FName& InKey, int32 inDialogId, float inVelocity)
{
	Super::OpenDialog(InKey, inDialogId);

	_velocity = inVelocity;
	_npcDialogData = GetNpcDialogData(inDialogId);
	if (nullptr != _npcDialogData)
	{
		_cutGroupCount = _remainGroupCount = _npcDialogData->cutGroupId.Num();

		SetCutGroupData();
	}
}

// 컷 그룹 데이터 셋팅
void FGsNpcDialog::SetCutGroupData()
{
	if (0 < _cutGroupCount)
	{
		int index = _cutGroupCount - _remainGroupCount;

		if (_npcDialogData->cutGroupId.IsValidIndex(index))
		{
			FGsSchemaNpcDialogCutGroupDataRow groupDataRow = _npcDialogData->cutGroupId[_cutGroupCount - _remainGroupCount];
			_npcDialogGroupData = groupDataRow.GetRow();
			if (_npcDialogGroupData != nullptr)
			{
				_remainCut = _cutCount = _npcDialogGroupData->cutDataId.Num();
				--_remainGroupCount;
			}
		}
	}

	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		_isAutoNextCut =
			static_cast<bool>(gameUserSettings->GetOutputSetting(EGsOptionOutput::QUEST_SKIP));
	}

	StartCutAction();
}

// 한컷 시작
void FGsNpcDialog::StartCutAction()
{
	// 무조건 그룹에 속해있어야 한다.
	if (nullptr == _npcDialogGroupData)
		return;

	// 컷수가 남은 컷보다 작을수 없다.
	if(_remainCut > _cutCount)
		return;
		
	int index = _cutCount - _remainCut;
	if (false == _npcDialogGroupData->cutDataId.IsValidIndex(index))
		return;
	
	FGsSchemaNpcDialogCutDataRow cutDataRow = _npcDialogGroupData->cutDataId[index];
	const FGsSchemaNpcDialogCutData* cutData = cutDataRow.GetRow();

	// 컷 데이타가 없다.
	if (nullptr == cutData)
		return;

	// 스폰 , 디스폰 처리(컷 전처리 시점)
	ChangeGameObjectSpawnState(cutData, true);
		
	// 카메라 view 변경
	SetCameraDialogView(cutData);

	if (true == cutData->isFadeInEffect)
	{
		if (_uiDialog.IsValid())
		{
			if (class UGsUIDialogNpc* uiNpcDialog = Cast<UGsUIDialogNpc>(_uiDialog))
			{
				//uiNpcDialog->SetTextEmpty();
				uiNpcDialog->HideAll(true);
			}
		}

		// FadeIn 연출 시작
		float fadeInTime = GData()->GetGlobalData()->_cutSpotWarpFadeInTime;
		FGsUIHelper::TrayFadeIn(fadeInTime);
	}

	PlayCutAction(cutData);
}

void FGsNpcDialog::PlayCutAction(const FGsSchemaNpcDialogCutData* inCutData)
{
	for (auto& iter : inCutData->actortList)
	{
		const FGsSchemaNpcDialogActorData* schemaNpcDialogActorData = iter.GetRow();
		if (nullptr == schemaNpcDialogActorData)
			continue;

		// 디스폰은 안한다
		if (schemaNpcDialogActorData->preActorState != EGsNpcDialogActorStateType::DIALOG_DESPAWN)
		{
			// 연출 : 이모티콘, 모션...
			StartEffect(schemaNpcDialogActorData);
		}
	}

	PlayUiDialog(inCutData);
}

void FGsNpcDialog::PlayUiDialog(const FGsSchemaNpcDialogCutData* inCutData)
{
	if (_uiDialog.IsValid())
	{
		if (class UGsUIDialogNpc* uiNpcDialog = Cast<UGsUIDialogNpc>(_uiDialog))
		{
			if (!uiNpcDialog->_delegateRemainTimeOver.IsBound())
			{
				uiNpcDialog->_delegateRemainTimeOver.BindRaw(this, &FGsNpcDialog::NextCutAction);
			}

			if (!uiNpcDialog->_delegateSkip.IsBound())
			{
				uiNpcDialog->_delegateSkip.BindRaw(this, &FGsNpcDialog::onSkip);
			}

			uiNpcDialog->StartAction(inCutData, _isAutoNextCut);
		}

		--_remainCut;
	}
}

void FGsNpcDialog::EndAction()
{
	EndEmoticon();
	//CloseDialog();

	if (_uiDialog.IsValid())
	{
		if (class UGsUIDialogNpc* uiDialog = Cast<UGsUIDialogNpc>(_uiDialog))
		{
			uiDialog->OnStopVoice();

			// 사운드 믹서 변경
			if (UGsSoundManager* soundManager = GSound())
			{
				if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
					soundMixerController->OffMixMode(EGsSoundMixType::NpcDialog);
			}
		}
	}

	if (_delegateOnFinished.IsBound())
	{
		_delegateOnFinished.ExecuteIfBound();
	}
}

void FGsNpcDialog::SetCameraDialogView(const FGsSchemaNpcDialogCutData* inSchemaNpcDialogCutData)
{
	if (nullptr == inSchemaNpcDialogCutData)
		return;

	FGsSchemaCameraDialogViewDataRow cameraDialogViewDataRow = inSchemaNpcDialogCutData->dialogViewType;
	const FGsSchemaCameraDialogViewData* cameraDialogViewData = cameraDialogViewDataRow.GetRow();
	if (nullptr == cameraDialogViewData)
		return;

	TArray<UGsGameObjectBase*> arrTarget;
	EGsDialogCameraViewType cameraViewType = cameraDialogViewData->type;	
	
	bool isTargetExist = true;
	// 카메라 연출		
	if (cameraViewType == EGsDialogCameraViewType::DialogBackLocalMultiNpc)
	{
		for (auto& iter : inSchemaNpcDialogCutData->cameraViewList)
		{
			if (const FGsSchemaNpcData* schemaNpcData = iter.GetRow())
			{
				UGsGameObjectBase* targetNpc =
					GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(schemaNpcData->id);

				// 해당 타겟이 없으면 실패
				if (targetNpc == nullptr)
				{
					isTargetExist = false;
				}
				else
				{
					arrTarget.Add(targetNpc);
				}

			}
		}
		// 여러명 보여주는 뷰는 무조건 2명 이상이어야 한다
		if (arrTarget.Num() < 2)
		{
			isTargetExist = false;
		}
	}
	else
	{
		if (inSchemaNpcDialogCutData->cameraViewList.Num() > 0)
		{
			const FGsSchemaNpcDataRow& viewTargetData = inSchemaNpcDialogCutData->cameraViewList[0];
		
			if (const FGsSchemaNpcData* schemaNpcData = viewTargetData.GetRow())
			{
				UGsGameObjectBase* targetNpc =
					GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(schemaNpcData->id);

				// 해당 타겟이 없으면 실패
				if (targetNpc == nullptr)
				{
					isTargetExist = false;
				}
				else
				{
					arrTarget.Add(targetNpc);
				}
			}				
		}
		else
		{
			UGsGameObjectBase* npcTarget = _npcObject.Get();
			if (npcTarget != nullptr)
			{
				arrTarget.Add(npcTarget);
			}			
		}

		if (arrTarget.Num() == 0)
		{
			isTargetExist = false;
		}
	}

	if (isTargetExist == true)
	{
		TArray<int> dialogSpawnObjects = GSClientSpawn()->GetDialogAllSpawnID();
		FGsCameraDialogMessageParam cameraDialogParam(arrTarget, 
			EGsCamearDialogEndCallBackType::Quest,
			cameraDialogViewData, dialogSpawnObjects, false, 
			inSchemaNpcDialogCutData->playerPosSpotId, 
			false);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::CAMERA_DIALOG_MODE_VIEW_CHANGE, &cameraDialogParam);
		// 카메라 타입 저장
		_cameraViewType = cameraDialogViewData;
	}
	
}
// game object 스폰 상태 변경(스폰 ,디스폰)
// 1. FGsSchemaNpcDialogCutData 컷 데이터
// 2. 전처리인지, 후처리인지
void FGsNpcDialog::ChangeGameObjectSpawnState(const FGsSchemaNpcDialogCutData* inSchemaNpcDialogCutData, bool inIsPre)
{
	if (inSchemaNpcDialogCutData == nullptr ||
		inSchemaNpcDialogCutData->actortList.Num() == 0)
	{
		return;
	}

	// 스폰 배열(대사 연출 내용)
	TArray<const FGsSchemaNpcDialogActorData*> arrSpawnDialog;
	// 디스폰 배열(대사 연출 내용)
	TArray<int> arrDespawnDialog;
	// 디스폰 배열(전역: 연출이 끝나도 상태유지)
	TArray<int> arrDespawnGlobal;
	// 스폰 배열(전역: 연출이 끝나도 상태유지)
	TArray<int> arrSpawnGlobal;

	for (auto& iter: inSchemaNpcDialogCutData->actortList)
	{
		const FGsSchemaNpcDialogActorData* schemaNpcDialogActorData = iter.GetRow();
		if (nullptr == schemaNpcDialogActorData)
			continue;
		
		int32 npcTblId = -1;
		if (const FGsSchemaNpcData* schemaNpcData = schemaNpcDialogActorData->npcData.GetRow())
		{
			npcTblId = schemaNpcData->id;
		}


		if (inIsPre == true)
		{
			// 대사 스폰
			if (schemaNpcDialogActorData->preActorState == EGsNpcDialogActorStateType::DIALOG_SPAWN)
			{
				arrSpawnDialog.Add(schemaNpcDialogActorData);
			}
			// 대사 디스폰
			else if(schemaNpcDialogActorData->preActorState == EGsNpcDialogActorStateType::DIALOG_DESPAWN)
			{
				arrDespawnDialog.Add(npcTblId);				
			}
			// 글로벌 스폰
			else if (schemaNpcDialogActorData->preActorState == EGsNpcDialogActorStateType::GLOBAL_SPAWN)
			{
				arrSpawnGlobal.Add(npcTblId);
			}
			// 글로벌 디스폰
			else if (schemaNpcDialogActorData->preActorState == EGsNpcDialogActorStateType::GLOBAL_DESPAWN)
			{
				arrDespawnGlobal.Add(npcTblId);
			}
		}
		else
		{
			// 대사 스폰
			if (schemaNpcDialogActorData->PostActorState == EGsNpcDialogActorStateType::DIALOG_SPAWN)
			{
				arrSpawnDialog.Add(schemaNpcDialogActorData);
			}
			// 대사 디스폰
			else if (schemaNpcDialogActorData->PostActorState == EGsNpcDialogActorStateType::DIALOG_DESPAWN)
			{
				arrDespawnDialog.Add(npcTblId);				
			}
			// 글로벌 스폰
			else if (schemaNpcDialogActorData->PostActorState == EGsNpcDialogActorStateType::GLOBAL_SPAWN)
			{
				arrSpawnGlobal.Add(npcTblId);
			}
			// 글로벌 디스폰
			else if (schemaNpcDialogActorData->PostActorState == EGsNpcDialogActorStateType::GLOBAL_DESPAWN)
			{
				arrDespawnGlobal.Add(npcTblId);
			}
		}
	}
	if (arrDespawnDialog.Num() != 0)
	{
		GSClientSpawn()->DespawnDialogObject(arrDespawnDialog);
	}

	if (arrDespawnGlobal.Num() != 0)
	{
		GSClientSpawn()->DespawnGlobalObject(arrDespawnGlobal);
	}

	if (arrSpawnDialog.Num() != 0)
	{
		UGsGameObjectBase* localObject =
			GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		GSClientSpawn()->SpawnDialogObject(arrSpawnDialog, _npcObject.Get(), localObject);
	}

	if (arrSpawnGlobal.Num() != 0)
	{
		GSClientSpawn()->SpawnGlobalObject(arrSpawnGlobal);
	}
}
// 연출시작 : 이모티콘, 모션(소셜)
void FGsNpcDialog::StartEffect(const FGsSchemaNpcDialogActorData* inSchema)
{
	TWeakObjectPtr<UGsGameObjectBase> actor = nullptr;

	switch (inSchema->actorType)
	{
	case EGsNpcDialogActorType::ACTOR_MY:	
		{
			actor = _playerObject;
		}		
		break;
	case EGsNpcDialogActorType::ACTOR_TARGET:
		actor = _npcObject;
		break;
	case EGsNpcDialogActorType::ACTOR_NON_TARGET:
		if (const FGsSchemaNpcData* schemaNpcData = inSchema->npcData.GetRow())
		{
			actor = GSGameObject()->FindObjectLocalNearestNonPlayerByNPCId(schemaNpcData->id);
		}
		break;
	default:
		break;
	}

	if (actor == nullptr)
	{
		return;
	}

	if (inSchema->isMotionPlay)
	{
		int motionId = 0;
		int motionIndex = 0;
		if (0 < inSchema->motionList.Num())
		{
			if (inSchema->actorType == EGsNpcDialogActorType::ACTOR_MY)
			{
				FGsGameObjectDataLocalPlayer* dataLocalPlayer = actor->GetCastData<FGsGameObjectDataLocalPlayer>();
				motionIndex = (dataLocalPlayer->GetCreatureGenderType() == CreatureGenderType::MALE) ? 0 : 1;				
			}

			if (motionIndex < inSchema->motionList.Num())
			{
				FGsSchemaSkillCommonRow row = inSchema->motionList[motionIndex];
				if (const FGsSchemaSkillCommon* skillCommon = row.GetRow())
				{
					motionId = skillCommon->id;
				}
			}
		}
		
		StartMotion(actor, motionId);
	}
	
	if (inSchema->expressionType != EGsEmoticonType::EMOTICON_EMPTY)
	{
		StartEmoticon(actor, inSchema->expressionType);
	}
}

// 연출 멈추기 : 이모티콘, 모션(소셜)
void FGsNpcDialog::EndEffect()
{
	EndMotion();
	EndEmoticon();

	int index = _cutCount - (_remainCut + 1);

	if (_npcDialogGroupData->cutDataId.IsValidIndex(index))
	{
		FGsSchemaNpcDialogCutDataRow cutDataRow = _npcDialogGroupData->cutDataId[index];
		const FGsSchemaNpcDialogCutData* cutData = cutDataRow.GetRow();

		if (nullptr != cutData)
		{
			// 스폰 , 디스폰 처리(컷 전처리 시점)
			ChangeGameObjectSpawnState(cutData, false);
		}
	}
	else
	{
		GSLOG(Error, TEXT("FGsNpcDialog - cutGroupCount : %d \t remainGroupCount : %d \t cutCount : %d \t remainCut : %d \t npcGameId : %d"),
			_cutGroupCount, _remainGroupCount, _cutCount, _remainCut, _npcGameId);
	}
}

// npc 모션 시작
void FGsNpcDialog::StartMotion(TWeakObjectPtr<UGsGameObjectBase> inObject, int32 inMotionId)
{
	FGsSkillHandlerBase* skillHandler = GetNpcSkillHandler(inObject);
	if (nullptr == skillHandler)
	{
		EndAction();
	}
	else
	{
		// 모션 시작	
		skillHandler->OnSocial(inMotionId);
	}
		
	// 오브젝트 저장
	_motionObjList.Emplace(inObject);
}

// npc 모션 끝
void FGsNpcDialog::EndMotion(bool isIsFinished)
{
	for (TWeakObjectPtr<UGsGameObjectBase> object : _motionObjList)
	{
		FGsSkillHandlerBase* skillHandler = GetNpcSkillHandler(object);
		if (nullptr == skillHandler)
		{
			EndAction();
		}
		else
		{
			// 모션 끝	
			skillHandler->StopSocial(isIsFinished);
		}			
	}

	_motionObjList.Empty();
}

void FGsNpcDialog::EndSocial()
{
	TFunction<void(TWeakObjectPtr<UGsGameObjectBase> inObject)> func = [this](TWeakObjectPtr<UGsGameObjectBase> object)
	{
		if (object.IsValid())
		{
			if (FGsSkillHandlerBase * skillHandler = this->GetNpcSkillHandler(object))
			{
				// 모션 끝	
				skillHandler->StopSocial(true);				
			}
		}
	};

	FGsClientSpawnManager* clientSpawnManager =  GSClientSpawn();
	UGsGameObjectManager* gameObjectManager =  GSGameObject();

	if (clientSpawnManager == nullptr || gameObjectManager == nullptr)
	{
		return;
	}

	// 참여한 npc
	TArray<int> dialogSpawnObjects = clientSpawnManager->GetDialogAllSpawnID();
	for (int tID : dialogSpawnObjects)
	{
		func(gameObjectManager->FindObjectLocalNearestNonPlayerByNPCId(tID));
	}

	// 나
	func(_playerObject);

	_motionObjList.Empty();
}

// 이모티콘 시작
void FGsNpcDialog::StartEmoticon(TWeakObjectPtr<UGsGameObjectBase> inObject, EGsEmoticonType inType)
{
	_uiEmoticonList.Emplace(UGsBlueprintFunctionLibraryUI::AddEmoticon(inType, _npcObject.Get()));
}

// 이모티콘 끝
void FGsNpcDialog::EndEmoticon()
{
	for(TWeakObjectPtr<UGsUIHUDEmoticon> uiEmoticon : _uiEmoticonList)
	{
		if (nullptr == uiEmoticon)
			return;

		if (uiEmoticon.IsValid())
		{
			uiEmoticon->Release();
		}
	}

	_uiEmoticonList.Empty();
}

// 그룹 컷 스킵
void FGsNpcDialog::onSkip()
{
	_remainCut = 0;	
	NextCutAction();
}

// 다음 컷 시작
void FGsNpcDialog::NextCutAction()
{
	ClearUIEvent();
	// 다음 컷 연출 전에 전 컷 연출 끝내기
	EndEffect();

	if (0 < _remainCut)
	{
		StartCutAction();
	}
	else
	{
		if (0 < _remainGroupCount)
		{
			SetCutGroupData();
		}
		else
		{			
			EndSocial();
			EndAction();
		}		
	}
}

FGsSkillHandlerBase* FGsNpcDialog::GetNpcSkillHandler(TWeakObjectPtr<UGsGameObjectBase> inObject)
{
	if (false == inObject.IsValid())
		return nullptr;

	return inObject->GetSkillHandler();
}

// 한 컷의 대한 정보
const FGsSchemaNpcDialogData* FGsNpcDialog::GetNpcDialogData(int32 InRowKey)
{
	const UGsTableNpcDialogData* table = Cast<UGsTableNpcDialogData>(FGsSchemaNpcDialogData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaNpcDialogData* schemaNpcDialogData = nullptr;
	if (false == table->FindRowById(InRowKey, schemaNpcDialogData))
	{
		return nullptr;
	}

	return schemaNpcDialogData;
}

// 컷 그룹에 대한 정보
const FGsSchemaNpcDialogCutGroupData* FGsNpcDialog::GetNpcDialogCutGroupData(int32 InRowKey)
{
	const UGsTableNpcDialogCutGroupData* table = Cast<UGsTableNpcDialogCutGroupData>(FGsSchemaNpcDialogCutGroupData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaNpcDialogCutGroupData* schemaNpcDialogCutGroupData = nullptr;
	if (false == table->FindRowById(InRowKey, schemaNpcDialogCutGroupData))
	{
		return nullptr;
	}

	return schemaNpcDialogCutGroupData;
}

void FGsNpcDialog::OnCloseDialog()
{
	if (_delegateOnFinished.IsBound())
	{
		_delegateOnFinished.Unbind();
	}

	ClearUIEvent();

	_remainCut = 0;
	_remainGroupCount = 0;

	EndSocial();
	EndAction();

	CloseDialog();
}

void FGsNpcDialog::ClearUIEvent()
{
	if (_uiDialog.IsValid())
	{
		if (class UGsUIDialogNpc* uiNpcDialog = Cast<UGsUIDialogNpc>(_uiDialog))
		{
			if (uiNpcDialog->_delegateRemainTimeOver.IsBound())
			{
				uiNpcDialog->_delegateRemainTimeOver.Unbind();
			}

			if (uiNpcDialog->_delegateSkip.IsBound())
			{
				uiNpcDialog->_delegateSkip.Unbind();
			}
		}
	}
}

