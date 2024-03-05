#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Fairy/GsBasePortrait.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsUISummonItemPortrait.generated.h"

class UGsButton;
class UGsUISummonPortraitBase;
class UImage;

UCLASS()
class T1PROJECT_API UGsUISummonItemPortrait : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSelected, TWeakPtr<IGsBasePortrait>);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _portraitName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FLinearColor _gradeColor;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _summonPercent;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCollection;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISummonPortraitBase* _portrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _bgNotCollectedFairy;

public:
	FOnSelected OnItemSelected;

private:
	TWeakPtr<IGsBasePortrait> _data;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickPortrait();

	virtual void NativeOnInitialized() override;

public:
	void SetData(TWeakPtr<IGsBasePortrait> baseData, float percent, CreatureGenderType gender = CreatureGenderType::ALL);
	void RefreshUI();
};