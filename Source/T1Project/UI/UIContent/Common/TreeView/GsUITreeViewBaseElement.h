#pragma once

/**
* file		GsUIMonsterKnolwedgeCategoryButton.h
* @brief	Tree 카테고리
* @author	PKT
* @date		2021/11/23
**/

#include "CoreMinimal.h"
#include "GsTreeViewElementData.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Common/TreeView/GsTreeViewElementData.h"
#include "GsUITreeViewBaseElement.generated.h"


class UGsButton;
class UTextBlock;
class UWidgetSwitcher;
class UGsUIRedDotDefault;
class UGsUITreeViewBaseElement;

DECLARE_DELEGATE_TwoParams(FOnTreeViewElementDelegate, const UGsUITreeViewBaseElement*, bool);

UCLASS()
class T1PROJECT_API UGsUITreeViewBaseElement : public UUserWidget
{
	GENERATED_BODY()

protected:
	/************************************************************************/
	/* BP Bind                                                              */
	/************************************************************************/
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherState;		// 2021/11/23 PKT - 0 : Normal, 1 : Selected

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _btnFirstStateTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _btnSecondStateTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotBase* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTrigger;

	// 2021/11/23 PKT - Tree Element Data Only
	const FGsTreeViewElementData* _data = nullptr;

public:
	// 2021/12/05 PKT - Clicked Element
	FOnTreeViewElementDelegate OnClickedDelegate;
	// 2021/12/05 PKT - State Changed Element
	FOnTreeViewElementDelegate OnSelectedDelegate;
	// 2021/11/23 PKT -  RedDot Delegate
	FOnTreeViewElementDelegate OnRedDotDelegate;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;

	// 2021/11/23 PKT - 상속 받는 곳에서 재정의
	virtual bool IsSelected() const { return false; }

	/************************************************************************/
	/* Event Bind                                                           */
	/************************************************************************/
protected:
	UFUNCTION()
	void OnClickedElement();

	/************************************************************************/
	/* Set / Get                                                            */
	/************************************************************************/
public:		
	virtual void SetSelected(bool InIsSelected);
	virtual bool GetIsSelected() const;

	void SetData(const FGsTreeViewElementData* InData);

	void SetIsRedDot(bool InIsRedDot);

	bool GetIsRedDot() const;

	const FGsTreeViewElementData* GetData() const;
};
