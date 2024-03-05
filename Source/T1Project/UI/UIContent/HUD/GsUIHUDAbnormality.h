// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageGameObject.h"
#include "GsUIHUDAbnormality.generated.h"

struct IGsMessageParam;

/**
 * HUD 좌상단 버프 아이콘 표시 
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDAbnormality : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	int32 _maxShowDebuffCount = 10;

	// Buff
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnBuff;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockBuffCount;

	// Debuff
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;

	UPROPERTY()
	class UGsDynamicIconSlotHelper* _slotHelper;

protected:
	MsgGameObjHandleArray _msgHandleList;

	// 디버프 아이콘 표시 위한 데이터
	TArray<struct FGsPassivityData*> _passivityDebuffList;
	TArray<struct FGsAbnormalityData*> _abnormalityDebuffList;

protected:
	bool _isSaveBatteryMode = false;

protected:
	// UUserWidget override.
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// IGsInvalidateUIInterface override.
	virtual void InvalidateAllInternal() override;

protected:
	void RegisterMessages();
	void DeregisterMessages();

protected:
	void OnAddAbnormality(const IGsMessageParam* InParam);
	void OnRemoveAbnormality(const IGsMessageParam* InParam);
	void OnAddPassivity(const IGsMessageParam* InParam);
	void OnRemovePassivity(const IGsMessageParam* InParam);

	UFUNCTION()
	void OnClickBuff();

	// 아이콘 생성 시
	UFUNCTION()
	void OnCreateIcon(class UGsUIIconBase* InIcon);

	// 아이콘 갱신 시
	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon);

	// 아이콘 클릭 시
	UFUNCTION()
	void OnClickIcon(class UGsUIIconAbnormality* InIcon);

protected:
	void SetUIBuffCount(int32 InCount);
	// bRefreshAbnormality는 타이밍 이슈로 추가함
	void InvalidateDebuffList(class FGsAbnormalityHandlerBase* InHandler, bool bRefreshAbnormality);
	void OpenPopupAbnormalityInfo();

	// 절전 모드용으로 설정
public:
	void SetSaveBatteryMode();
};
