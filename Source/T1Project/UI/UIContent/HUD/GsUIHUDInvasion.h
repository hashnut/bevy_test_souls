// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageInvasion.h"
#include "GsUIHUDInvasion.generated.h"


UCLASS()
class T1PROJECT_API UGsUIHUDInvasion : public UGsUIHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _userCountTextBlock;

	int32 _maxUserCount;
	MapId _currentMapId;
	MsgGameObjHandleArray _gameObjectMsg;
	MsgInvasionHandleArray _invasionMsg;

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:	
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;
	void InvalidUserCount();

private:
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam*);
	void OnInvalidUserCount(const struct IGsMessageParam*);

};
