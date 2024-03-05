#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeSlateStatNode.h"
#include "GsUIMonsterCollectionSlateNode.generated.h"

class UImage;
class UGsUIVFX;
class UGsButton;
class UWidgetSwitcher;
class UGsUIMonsterCollectionSlateNodeFadeInVFX;

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateNode : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherActiveSymbol;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherActiveImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterCollectionSlateNodeFadeInVFX* _effectActiveFadeIn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _effectActiveBroken;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgActive;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgDisable;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _effectFocus;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _effectSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSymbol;

	UPROPERTY(EditInstanceOnly, Category = "Node Num", meta = (BindWidget))
	FGsSchemaMonsterKnowledgeSlateStatNodeRow _nodeRow;
	
	int32 _resourceId;

protected:
	const class FGsMonsterKnowledgeSlate* _slateData = nullptr;
	const class FGsMonsterKnowledgeSlateNode* _nodeData = nullptr;

	EMonsterStoneSlateNodeState _prevState = EMonsterStoneSlateNodeState::Max;

public:
	DECLARE_DELEGATE_OneParam(FOnClickedDelegate, const class FGsMonsterKnowledgeSlateNode*);
	FOnClickedDelegate OnClickedDelegate;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClickedNode();

	void ToggleAnimation(UUserWidget* InWidget, bool InFlag);

public:
	void SetData(const class FGsMonsterKnowledgeSlate* InSlate);
	void Invalidate(const class FGsMonsterKnowledgeSlateNode* InFocusNode = nullptr, bool InIsForceSelected = false);

	const FGsMonsterKnowledgeSlateNode* GetData() const;

	UFUNCTION()
	int32 ResourceId() const;
};