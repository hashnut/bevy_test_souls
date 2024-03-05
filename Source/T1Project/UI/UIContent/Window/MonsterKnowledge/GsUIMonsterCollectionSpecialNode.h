// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"
#include "../UI/UIControlLib/Interface/GsEntryInterface.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeUICaptureData.h"
#include "GsUIMonsterCollectionSpecialNode.generated.h"

class UGsButton;
class UWidgetSwitcher;
class UImage;
class UCanvasPanel;
class UGsUIVFX;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGsNodeUseFinish);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGsNodeResultFinish);


/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSpecialNode : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UGsButton* _btnSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UWidgetSwitcher* _widgetSwitcherNodeState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageNodeIconLock;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageLock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UImage* _imageNodeIconUnlock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UImage* _imageNodeIconComplete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UCanvasPanel* _selectedOverlay;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UGsUIVFX* _vfxNodeUse;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UGsUIVFX* _vfxNodeSuccess;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta=(BindWidget))
	UGsUIVFX* _vfxNodeFail;


private:
	const class FGsSlateSpecialNode* _data = nullptr;

	TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> _selectedCallBack = nullptr;

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "GsUI")
	FGsNodeUseFinish OnFinishNodeUse;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "GsUI")
	FGsNodeResultFinish OnFinishNodeResult;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnClcikedBtn();

	UFUNCTION(BlueprintCallable)
	void OnFinishedNodeUse();

	UFUNCTION()
	void OnFinishedNodeResult();

public:
	void SetData(const class FGsSlateSpecialNode* InData, TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> InSelectedCallBack = nullptr);
	void Invalidate();

	const class FGsSlateSpecialNode* GetData() const;

	bool IsSelected() const;
	void SetSelectedEffect(bool InIsSelected);

	void PlayNodeUse() const;
	void PlayNodeSuccess() const;
	void PlayNodeFail() const;
};
