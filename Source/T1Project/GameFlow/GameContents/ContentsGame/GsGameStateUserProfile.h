// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageGameObject.h"


class UGsUIWindowUserProfile;
class AGsActorUserProfileSceneObject;
struct IGsMessageParam;
/**
 * 유저 프로필 로직
 */
class T1PROJECT_API FGsGameStateUserProfile : public FGsContentsGameBase
{
private:
	TArray<TPair<MessageContentUserProfile, FDelegateHandle>> _userProfileDelegates;
	TArray<TPair<MessageItem, FDelegateHandle>> _itemDelegates;
	TArray<TPair<MessageUserInfo, FDelegateHandle>> _userInfoDelegates;
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegates;
	MsgSystemHandleArray _systemDelegates;
	MsgGameObjHandleArray _gameObjectDelegates;

private:
	TWeakObjectPtr<UGsUIWindowUserProfile> _windowUI;
	TWeakObjectPtr<AGsActorUserProfileSceneObject> _actorScene;

public:
	FGsGameStateUserProfile();
	virtual ~FGsGameStateUserProfile();

public:
	virtual void Enter() override;
	virtual void Exit() override;

private:
	void RegisterMessages();
	void RemoveMessages();

private:
	void OnInvalidateAll();
	void OnInvalidateComment();
	void OnInvalidateIntroduction();
	void OnInvalidateKeyword();
	void OnInvalidateNickNameHistory();
	void OnInvalidateEquipItem(struct FGsItemMessageParamBase& InParam);

	void OnLookInfoChanged(uint64 InGameId);

	void OnOpenKeywordVote();

	void OnOptionSaveCompleted(const IGsMessageParam*);
	
	void OnGameServerReconnectionSuccess();

	void OnUserPresetEquipUpdate(const struct IGsMessageParam* inParam);

	void SpawnScene();
	void DespawnScene();

	void OpenWindowUI();
	void CloseWindowUI();

	void OnErrorInvalidUser();

	void ShowJoystick();
	void HideJoystick();
};
