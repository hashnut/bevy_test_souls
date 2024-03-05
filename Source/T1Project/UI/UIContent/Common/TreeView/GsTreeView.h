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
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsTreeView.generated.h"


class UVerticalBox;
class UGsDynamicPanelSlotHelper;
class UGsUITreeViewBaseElement;
class UGsUITreeViewMainElement;
class UGsUITreeViewSubElement;

DECLARE_DELEGATE_TwoParams(FOnTreeViewDelegate, const UUserWidget*, bool);

UCLASS()
class T1PROJECT_API UGsUITreeView : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

	const static int32 ELEMENT_HIDE_ALL_INDEX = 0;

	/************************************************************************/
	/* BP Bind                                                              */
	/************************************************************************/
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITreeViewMainElement* _baseElement;

	/**
	 * Sub Element는 Toggle 관리 안함. 항상 무언가 선택이 되어있어야 하지 않다.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalSubElement;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperBtnSubElement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfBtnSubElement;

	// 2021/11/23 PKT - Tree Element Data Only
	TSharedPtr<const FGsTreeViewData> _data;

	/************************************************************************/
	/* Animation                                                            */
	/************************************************************************/
	const static int32 DONE_ANIMATION_FOLD_VALUE = -1;

	enum class FoldAnimtion
	{
		Force,
		Fold,
		UnFold,
	};

	FoldAnimtion _animtionType = FoldAnimtion::Force;

	float _deltaTime = 0.f;

	int32 _animPlaySlotIndex = DONE_ANIMATION_FOLD_VALUE;

public:
	// 2021/12/05 PKT - Main Element 상태 변화
	FOnTreeViewDelegate OnSelectedMainElementDelegate;
	// 2021/12/05 PKT - Sub Element 상태 변화
	FOnTreeViewDelegate OnSelectedSubElementDelegate;
	// 2021/12/13 PKT - RedDot 변경
	FOnTreeViewDelegate OnRedDotDelegate;

protected:
	virtual void NativeOnInitialized() override;
	virtual void BeginDestroy() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void SetIsSelected(bool bInIsSelected);
	virtual bool GetIsSelected() const;

	 
	/************************************************************************/
	/* Event Bind                                                           */
	/************************************************************************/
protected:
	// 2021/11/23 PKT - sub element create & Add ...
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnCreateSubElement(UWidget* InEntry);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshSubElement(int32 InIndex, UWidget* InEntry);

	void OnClickedMainElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected);

	void OnSelectedMainElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected);

	void OnRedDotMainElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected);

	void OnClickedSubElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected);

	void OnSelectedSubElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected);

	void OnRedDotSubElement(const UGsUITreeViewBaseElement* InElement, bool InIsSelected);

	/************************************************************************/
	/* Animation                                                            */
	/************************************************************************/
protected:
	void ForceFold(bool InIsFold);

	/************************************************************************/
	/* Set                                                                  */
	/************************************************************************/
public:
	// 2021/11/24 PKT - Set Data
	void SetData(TSharedPtr<const FGsTreeViewData> InData);

	// 2021/12/05 PKT - On/Off Fold Animation
	void SetOnAnimation(bool InIsActivate);

	// 2021/12/05 PKT - Leave Focus
	void SetSubElementReleaseSelected(const UWidget* InFocusOhterWidget );
	
	int32 GetSubElementCount() const;

	bool GetIsRedDot() const;

	UGsUITreeViewBaseElement* GetSubElementByIndex(int32 InIndex) const;
};
