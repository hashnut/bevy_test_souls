// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GsDialogBase.h"
#include "GsSchemaEnums.h"

class FGsDialogBase;
struct FGsSchemaNpcDialogData;
struct FGsSchemaNpcDialogCutGroupData;
class UGsGameObjectBase;
class FGsSkillHandlerBase;
class UGsUIHUDEmoticon;
struct FGsSchemaNpcDialogActorData;
struct FGsSchemaNpcDialogCutData;
struct FGsSchemaCameraDialogViewData;

/**
 *  npc 대화스크립트
 */
class T1PROJECT_API FGsNpcDialog final : public FGsDialogBase
{
	using Super = FGsDialogBase;

public:
	FGsNpcDialog() = default;
	virtual ~FGsNpcDialog() = default;

protected:
	// 컷 그룹 갯수
	int8 _cutGroupCount = 0;
	// 그룹에 속한 컷 수
	int8 _cutCount = 0;
	// 남은 컷 그룹 갯수
	int8 _remainGroupCount = 0;
	// 남은 컷 갯수
	int8 _remainCut = 0;
	// 글자 나오는 속도
	float _velocity = 0.03f;
	// npc Id
	int64 _npcGameId = 0;
	// default CameraViewType
	const FGsSchemaCameraDialogViewData* _cameraViewType = nullptr;
	// 대화 진행 중 대화를 자동으로 넘기는가??
	bool _isAutoNextCut = true;
	   	
	// 대화 상대 npc
	TWeakObjectPtr<class UGsGameObjectBase> _playerObject;
	// 대화 상대 npc
	TWeakObjectPtr<class UGsGameObjectBase> _npcObject;

	// 모션
	TArray<TWeakObjectPtr<class UGsGameObjectBase>>	_motionObjList;
	// 이모티콘
	TArray<TWeakObjectPtr<class UGsUIHUDEmoticon>>	_uiEmoticonList;
	// 대화 데이터
	const FGsSchemaNpcDialogData*			_npcDialogData = nullptr;
	// 대화 그룹 데이터
	const FGsSchemaNpcDialogCutGroupData*	_npcDialogGroupData = nullptr;

protected:
	const FGsSchemaNpcDialogData* GetNpcDialogData(int32 InRowKey);
	const FGsSchemaNpcDialogCutGroupData* GetNpcDialogCutGroupData(int32 InRowKey);

	// 한컷의 시작
	void StartCutAction();
	// 다음컷 시작
	void NextCutAction();
	// 연출 끝
	void EndAction();
	// 컷 그룹 데이터 셋팅
	void SetCutGroupData();
	// 스킵 버튼을 눌렀을 때
	void onSkip();
	// 카메라 view 변경
	void SetCameraDialogView(const FGsSchemaNpcDialogCutData* inSchemaNpcDialogCutData);
	// game object 스폰 상태 변경(스폰 ,디스폰)
	// 1. FGsSchemaNpcDialogCutData 컷 데이터
	// 2. 전처리인지, 후처리인지
	void ChangeGameObjectSpawnState(const FGsSchemaNpcDialogCutData* inSchemaNpcDialogCutData, bool inIsPre);
	// 연출시작 : 이모티콘, 모션(소셜)
	void StartEffect(const FGsSchemaNpcDialogActorData* inSchema);
	// 연출끝내기 : 이모티콘, 모션(소셜)
	void EndEffect();
	// npc 모션 시작
	void StartMotion(TWeakObjectPtr<UGsGameObjectBase> inObject, int32 inMotionId);
	// npc 모션 끝
	void EndMotion(bool isIsFinished = false);
	// 모든 대화가 끝났을 때 대화에 참여한 모든 오브젝트의 소셜상태를 꺼준다.
	void EndSocial();
	// 이모티콘 시작
	void StartEmoticon(TWeakObjectPtr<UGsGameObjectBase> inObject, EGsEmoticonType inType);
	// 이모티콘 끝
	void EndEmoticon();
	// 캐릭터 애니메이션 연출을 위한 스킬핸들러
	FGsSkillHandlerBase* GetNpcSkillHandler(TWeakObjectPtr<UGsGameObjectBase> inObject);
	// 연출 시작
	void PlayCutAction(const FGsSchemaNpcDialogCutData* inCutData);
	// ui 자막
	void PlayUiDialog(const FGsSchemaNpcDialogCutData* inCutData);

	void ClearUIEvent();

public:
	// 대화창 오픈
	virtual void OpenDialog(const FName& InKey, int32 inDialogId) override;
	void OpenDialog(const FName& InKey, int32 inDialogId, UGsGameObjectBase* objectBase, const FGsSchemaCameraDialogViewData* inViewData);
	void OpenDialog(const FName& InKey, int32 inDialogId, float inVelocity);
	void OpenDialog(const FName& InKey, const FGsSchemaNpcDialogData* inNpcDialogData, UGsGameObjectBase* objectBase, const FGsSchemaCameraDialogViewData* inViewData);	

	void OnCloseDialog();
};
