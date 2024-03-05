// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "../GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GsUIHUDQuestDistance.generated.h"

class UImage;
class UPanelWidget;

class UGsGameObjectLocalPlayer;
class FGsAIQuestHandler;
/**
 *  퀘스트 자동이동 거리 표시
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDQuestDistance : public UGsUIHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _distanceText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _distUpdateSec = 0.25f;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _findText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _findQuestText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgTargetNpc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textNpcName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textNpcFunctionDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelNpcListMoveInfo;

protected:
	FTimerHandle _updateDistTimerHandle;
	TWeakObjectPtr<UGsGameObjectLocalPlayer> _owner;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void UpdateQuestDist();

public:
	FGsAIQuestHandler* GetAIQuestHandler();

	// 거리값 가져오기
	bool GetDistAndMovementAutoContentsType(OUT float& Out_dist, OUT EGsMovementAutoContentsType& Out_movementAutoContentsType);

	void ShowNpcListMoveInfo(int In_npcTblId);
	void HideNpcListMoveInfo();
};
