#pragma once

#include "Runtime/UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUIItemDropElement.generated.h"

class UGsButton;
class UObject;
class UTextBlock;

UCLASS()
class UGsUIItemDropElementListItem final : public UObject
{
	DECLARE_DELEGATE_OneParam(OnClickMoveBtn, const UGsUIItemDropElementListItem& /*InListItem*/);

	GENERATED_BODY()

public:
	OnClickMoveBtn _onClickMoveBtn;
	MapId _mapId;
	CreatureId _creatureId;
	Level _level;

	FText _continentalName;
	FText _mapName;
	FSlateColor _mapNameColor;

	FText _creatureName;

	FText _gradeText;
	FText _offensiveText;
	FText _levelText;
};

UCLASS()
class UGsUIItemDropElement final : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _mapNameMainTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _mapNameSubTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _targetNameTxt;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _gradeTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _offensiveTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _levelTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _moveBtn;

protected:
	void NativeOnInitialized() final;

protected:
	void NativeOnListItemObjectSet(UObject* InListItemObject) final;

private:
	void Invalidate(const UObject* InListItemObject = nullptr);

private:
	UFUNCTION()
	void OnClickedBtnMove();
};
