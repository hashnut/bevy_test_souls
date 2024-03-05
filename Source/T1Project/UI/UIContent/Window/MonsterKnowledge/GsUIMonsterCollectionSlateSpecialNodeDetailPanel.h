// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIMonsterCollectionSlateNodeDetailPanelBase.h"
#include "Message/GsMessageContents.h"
#include "GsUIMonsterCollectionSlateSpecialNodeDetailPanel.generated.h"

class UGsButton;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsUICurrencyButton;
class UGsUISpiritShotMaterialListItem;
class UGsUIMonsterCollectionSpecialNode;
struct IGsMessageParam;


/**
 * 사운드 연출 리소스 이름 (DTGsSoundResData 로부터 조회)
 */
namespace
{
	const FString SOUND_SPECIAL_OPEN = TEXT("UI_MB_SpecialNodeTry"); // (특수노드 개방 연출 사운드)
}

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateSpecialNodeDetailPanel : public UGsUIMonsterCollectionSlateNodeDetailPanelBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSpecialNodeTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterCollectionSpecialNode* _specialNodeState;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSpecialNodeStatName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSpecialNodeStatValue;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISpiritShotMaterialListItem* _materialInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSpecialNodeMaterialInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOpenNode;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBtnTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textProbability;

private:
	bool _isNodeVfxPlaying = false;
	bool _isNodeOpenSuccess = false;
	TPair<MessageContentMonsterKnowledge, FDelegateHandle> _messageUiDelegate;

	FTimerHandle _openTimer;

protected:
	const class FGsSlateSpecialNode* _focusNodeData = nullptr;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClickedOpenNode();

	UFUNCTION(BlueprintCallable)
	void OnStartSpecialNodeOpen();

	UFUNCTION(BlueprintCallable)
	void OnFinishedSpecialNodeOpen();

	UFUNCTION()
	void OnFinishedSpecialNodeResult();

public:
	virtual void SetData(const class FGsMonsterKnowledgeSlate* InData, TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> InCallback) override;
	virtual void Invalidate() override;
	virtual void InvalidateCurrency() override;

	void SetDetailData(const class FGsSlateSpecialNode* InData);
	const class FGsSlateSpecialNode* GetDetailData() const { return _focusNodeData; }

	void PlaySpecialNodeResult(const IGsMessageParam* InParam);
};
