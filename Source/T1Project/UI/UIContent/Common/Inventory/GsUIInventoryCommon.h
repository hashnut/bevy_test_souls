// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageContents.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "T1Project.h"
#include "Runtime/Core/Public/Misc/EnumClassFlags.h"
#include "Message/GsMessageContents.h"
#include "GsUIInventoryCommon.generated.h"

class UGsUIIconBase;
class UGsUIIconItem;
class FGsInventoryLogicBase;

/************************************************************************/
/* Property                                                             */
/************************************************************************/
enum class EInventoryCommonProperty : uint8
{
	Default = 0,
	ItemActionMessageUnBind = 1 << 0,					// 2021/08/09 PKT - MessageBind을 하지 않겠다.
	RefreshPartitions = 1 << 1,				// 2021/08/09 PKT - Icon 로드시 분할 생성 방식 여부
	SelectedNFocusEffectOverlap = 1 << 2,	// 2021/08/09 PKT - Slected 효과와 포커싱 효과의 중복 표기를 허용 할 것인가?
	HideInventoryCount = 1 << 3,			// 2021/08/18 PKT - 인벤토리 갯수 표기를 하지 않겠다.
	InventoryRedDot = 1 << 4,			// 인벤토리 탭 레드닷을 표시한다.
};
ENUM_CLASS_FLAGS(EInventoryCommonProperty);

/**
 * 거래소 판매탭 인벤토리 
 */
UCLASS()
class T1PROJECT_API UGsUIInventoryCommon : public UUserWidget
{
	GENERATED_BODY()

protected:
	/**
	 * 2021/08/09 PKT - 분할 생성 상숫값
	 */
	const float GENERATED_PARTION_RATE = 0.031f;
	const int32 GENERATE_PARTION_MIN_COUNT = 5;
	const int32 GENERATE_PARTION_MAX_COUNT = GENERATE_PARTION_MIN_COUNT * 12;

	using ItemCollectionMsgHandler = TArray<TPair<MessageContentItemCollection, FDelegateHandle>>;
	
public:
	DECLARE_DELEGATE_OneParam(FOnClickItemIcon, uint64 /*ItemDBId*/);
	DECLARE_DELEGATE(FOnMultiEnchantReset);
	DECLARE_DELEGATE(FOnMultiEnchantRegisterItem);

public:
	using MsgHandleItem = TPair<MessageItem, FDelegateHandle>;
	using MsgHandleInventoryCommon = TPair<MessageContentCommonInven, FDelegateHandle>;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imageRedDotAll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imageRedDotEquip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imageRedDotComsume;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imageRedDotEtc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelTab;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockSlotCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxInventoryIconSelector* _iconSelector = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelEmptyList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsCheckBox* _checkBoxAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsCheckBox* _checkBoxEquip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsCheckBox* _checkBoxConsume;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsCheckBox* _checkBoxMaterial;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnItemSlotLock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textEmptyDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _multiEnchantOptionRootPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnMultiEnchantReset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnMultiEnchantRegisterItem;

	UPROPERTY(EditAnywhere, Category = "GsIcon")
	EGsIconType _iconType;

	UPROPERTY(EditAnywhere, Category = "GsIcon")
	uint8 _iconKey;

	UPROPERTY(EditAnywhere, Category = "GsIcon")
	int32 _previewCount;

protected:
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _iconSlotHelper;

protected:
	FGsToggleGroup _toggleGroupTab;
	TArray<MsgHandleItem> _msgHandleItemList;
	TArray<MsgHandleInventoryCommon> _msgHandleInventoryCommon;
	
	// 각 컨텐츠에서 FGsInventoryLogicBase을 상속하여 구현 후 SetData 해준다
	class FGsInventoryLogicBase* _logic = nullptr;

	// 2021/08/09 PKT - 프로퍼티
	EInventoryCommonProperty _property = EInventoryCommonProperty::Default;

	// 2021/10/08 PKT - 분할 생성 정보
	struct FGsItemIconGeneratedPartion
	{
		const static int32 MIN_COUNT_PER_FRAME = 2;
		const static int32 MAX_COUNT_PER_FRAME = MIN_COUNT_PER_FRAME * 4;

		float frameRate = 0.f;
		int32 MaxCount = 0;
		int32 currentCount = 0;
		int32 countPerFrame = 0;
		float prevDeltaSeconds = 0.f;

		void Clear()
		{
			frameRate = 0.f;
			MaxCount = 0;
			currentCount = 0;
			countPerFrame = 0;
			prevDeltaSeconds = 0.f;
		}
	};

	FGsItemIconGeneratedPartion _generatedPartion;

	// 2021/08/09 PKT - 분할 생성 시 "OnUpdateItem()"가 호출되면 예약을 걸어둔다.
	TSet<ItemDBId> _reservedUpdateItem;
	// 2021/10/12 PKT - 분할 생성 시 Selected 이벤트가 오면 예약을 걸어둔다.
	int32 _reservedSelectedIndex = INDEX_NONE;

protected:
	FOnClickItemIcon _onClickItemIcon;
	FOnMultiEnchantReset _onMultiEnchantReset;
	FOnMultiEnchantRegisterItem _onMultiEnchantRegisterItem;

protected:
	TMap<EInventoryTab, UImage*> _imageRedDotMap;

public:
	virtual void BeginDestroy() override;
	virtual void SynchronizeProperties() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void BindMessages();
	void UnbindMessages();
	void InvalidateItemList();
	void InvalidateInvenTabSlotCount();
		
public:
	void SetProPerty(EInventoryCommonProperty InProperts);
	void SetData(FGsInventoryLogicBase* InData);
	void InvalidateData();
	void InvalidateInvenTabRedDot();
	void ChangeInvenSlotCount(uint32 InMaxSlotCount, uint32 InTotalExtendCount);
	void InvalidateBySlotIndex(int32 InSlotIndex);
	void SetInvenSlotDimmed(const bool InDimmedState, const ItemDBId InItemDbid);
	void SetInvenSlotSelectionImg(const bool InIsSelected, const ItemDBId InItemDbid);
	void SetMultiEnchantInvenState(const bool InState);

	void SetEmptyDesc(const FText& InDesc);
	//void SetFocus(int InIndex);

public:
	void SetEnchantInvenState();
	void SetItemFusionInvenState();
	void SetInvenTab(const uint8 InIndex);

protected:
	void SetInvenTabRedDot(EInventoryTab InTab, bool InValue);
	
protected:
	void OnAddItem(struct FGsItemMessageParamBase& InParam);
	void OnUpdateItem(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItem(struct FGsItemMessageParamBase& InParam);

	void OnInvalidateData(const struct IGsMessageParam* InParam);
	void OnInvalidateSelectedEffect(const struct IGsMessageParam* InParam);

	void GeneratedPartionItemIcon();

protected:	
	UFUNCTION()
	void OnCreateItemIcon(UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnRefreshItemIcon(int32 InIndex, UGsUIIconBase* InIcon);

	void OnSelectTab(int32 InIndex);
	void OnClickItemIcon(UGsUIIconItem& InIcon);

	UFUNCTION()
	void OnClickSlotLock();

	UFUNCTION()
	void OnClickMultiEnchantResetBtn();
	UFUNCTION()
	void OnClickMultiEnchantRegisterBtn();

	// 아이템 강화 쪽에서 별도 탭변경 처리에 따른 딤드/선택 처리가 필요하여
	// 별도로 빼는 처리함
public:
	FGsToggleGroup& GetToggleGroupTab() { return _toggleGroupTab; }

public:
	FOnClickItemIcon& OnClickItem() { return _onClickItemIcon; }
	FOnMultiEnchantReset& OnClickMultiEnchantReset() { return _onMultiEnchantReset; }
	FOnMultiEnchantRegisterItem& OnClickMultiEnchantRegister() { return _onMultiEnchantRegisterItem; }

//----------------------------------------------------------------------------------------------------------------------
	// Tutorial
private:
	bool _bIsTutorial = false;

public:
	DECLARE_DELEGATE_OneParam(FOnTutorialListEvent, UWidget* /*TargetWidget*/);
	FOnTutorialListEvent OnTutorialListEvent;

public:
	bool IsTutorial() const { return _bIsTutorial; }
	void SetTutorial(bool bIsTutorial);

	UFUNCTION()
	void OnScrollList(float InCurrentOffset);

};

