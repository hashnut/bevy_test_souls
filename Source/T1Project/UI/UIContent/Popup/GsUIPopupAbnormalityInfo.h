// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "GsUIPopupAbnormalityInfo.generated.h"

/**
 * 로컬 플레이어의 상태이상 정보 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupAbnormalityInfo : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsAbnormality")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	// 토글버튼의 Root패널
	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	class UPanelWidget* _panelTabRoot;

	// 리스트의 Root패널
	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	class UPanelWidget* _panelListRoot;
	
	// 적용 중인 효과가 없습니다 UI의 Root패널
	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	class UPanelWidget* _panelEmptyRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality", meta = (BindWidget))
	class UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsAbnormality")
	FText _textEmptyList;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	FGsToggleGroup _toggleGroupTab;
	TArray<struct FGsAbnormalityData*> _abnormalityList;
	TArray<struct FGsPassivityData*> _passivityList;
	MsgGameObjHandleArray _msgHandleList;
	MsgUIHandle _msgUIHandle;
	TWeakObjectPtr<class UGsGameObjectBase> _localPlayer;
	
public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void InvalidateAllInternal() override;

protected:
	virtual void OnInputCancel() override;

protected:
	void RegisterMessages();
	void DeregisterMessages();

protected:
	void InvalidateList(int32 InIndex);

	void OnAddAbnormality(const IGsMessageParam* InParam);
	void OnRemoveAbnormality(const IGsMessageParam* InParam);
	void OnAddPassivity(const IGsMessageParam* InParam);
	void OnRemovePassivity(const IGsMessageParam* InParam);
	void OnArcasPoint(const IGsMessageParam*);

	// 탭 메뉴 클릭 시
	UFUNCTION()
	void OnClickTab(int32 InIndex);

	// 리스트 앤트리 갱신 시
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickClose();

protected:	
	class UGsGameObjectBase* GetLocalPlayer();
	class FGsAbnormalityHandlerBase* GetAbnormalityHandler();	
};
