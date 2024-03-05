#pragma once

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUICraftShortCutListEntry.generated.h"

class FGsGameStateRegionMap;
class IGsToggleGroupEntry;
class UGsButton;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsUIIconBase;
class UGsWrapBoxIconSelector;
class UObject;
class UTextBlock;

UCLASS()
class UGsUICraftShortCutListItem final : public UObject
{

	DECLARE_DELEGATE_OneParam(OnClickMoveBtn, const UGsUICraftShortCutListItem& /*InListItem*/);

	GENERATED_BODY()

public:
	CraftId _craftId;
	OnClickMoveBtn _onClickMoveBtn;

};

UCLASS()
class UGsUICraftShortCutListEntry final : public UUserWidget, public IUserObjectListEntry
{

	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _targetIconSelector;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UGsDynamicIconSlotHelper* _targetSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsWrapBoxIconSelector* _materialIconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UGsDynamicIconSlotHelper* _materialSlotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _txtTitle;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _moveBtn;

protected:
	void NativeOnInitialized() final;

	void NativeDestruct() final;

protected:
	void NativeOnListItemObjectSet(UObject* InListItemObject) final;

private:
	void InvalidateListItem(const UObject* InListItemObject = nullptr);

	UFUNCTION()
	void OnRefreshIconTarget(int32 InIndex, UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnRefreshIconMaterial(int32 InIndex, UGsUIIconBase* InIcon);

	UFUNCTION()
	void OnClickedMoveBtn();

};
