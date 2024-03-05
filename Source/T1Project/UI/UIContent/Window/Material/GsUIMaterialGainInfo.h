#pragma once
#include "../Message/GsMessageContents.h"
#include "Unlock/GsUnlockDefine.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMaterialGainInfo.generated.h"

class UGsButton;
class UTextBlock;
class URichTextBlock;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsUIItemDefaultInfo;
class UGsDynamicPanelSlotHelper;
class UGsUIItemDropListContainer;
class UGsUIMaterialSetGainInfoCraftMaterialList;
struct IGsMessageParam;

UCLASS()
class T1PROJECT_API UGsUIMaterialGainInfo : public UUserWidget
{
	GENERATED_BODY()
	/**
	* 추후 다른 시스템에서 사용하게 되면 그에 맞는 Dirve Owner 를 추가
	*/
public:
	enum class DriveOwner { None, ItemCollection, };

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemDefaultInfo* _itemDefaultInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnMarketDetailTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCraftDetailTab;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnDropNPCDetailTab;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetailInfo;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMaterialSetGainInfoCraftMaterialList* _detailInfoCraft;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemDropListContainer* _detailInfoDropNPC;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherEnchantBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherItemCollectionBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherMarketBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherCraftBtn;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherController;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItemCollectionSubstituteMaterials;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGoEnchant;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGoItemCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGoMarket;	

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGoCraft;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItemCollectionRegist;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URichTextBlock* _textLockReason;

	FGsToggleGroup _toggleGroupMainTabs;

	DriveOwner _driveOwner = DriveOwner::None;
	
	const void* _data = nullptr;

	ItemId _materialId = INVALID_ITEM_ID;
	Level _level = 0;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:	
	void InvalidateMarket();
	void InvalidateCraft();
	void InvalidateDropNPC();

	void SetController(DriveOwner InOwner);
	void InvalidateItemCollectionController();

protected:
	void OnChangedToogleMainTab(int32 InIndex);

	UFUNCTION()
	void OnGoMarket();

	UFUNCTION()
	void OnGoCraft();

	UFUNCTION()
	void OnGoItemCollection();

	UFUNCTION()
	void OnGoEnchant();

	// 2023/1/11 PKT - 아이템 컬렉션 대체 아이템 리스트 팝업 오픈
	UFUNCTION()
	void OnClickedItemCollectionSubstituteMaterials();

	// 2023/1/11 PKT - 아이템 컬렉션 등록 버튼 클릭
	UFUNCTION()
	void OnClickedItemCollectionRegister();

	// "획득처" 목록의 이동하기 버튼 클릭 시 이 이벤트를 항상 발생시킴.
	void OnClickItemDropNavigateButton();

public:
	void OnReceiveItemPriceInfo(const IGsMessageParam*);
	void OnReceiveCollectionPriceList(const IGsMessageParam* InParam);
	void OnRequestRefreshPrice(const IGsMessageParam*);

public:
	void SetData(const class FGsItemCollectionCondition* InData, const int32 InTabIndex = -1);
	void Invalidate();
	void ContentsUnlock(EGsUnlockContentsType InType);

	void Capture(int32& OutTabIndex);

	ItemId GetMaterialId() const { return _materialId; }
};