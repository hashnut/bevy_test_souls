// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsSchemaEnums.h"
#include "DataSchema/Option/GsSchemaOptionButtonGroup.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "Option/GsOptionEnumType.h"
#include "GsUIOptionCheckButtonGroup.generated.h"

struct FGsSchemaOptionCheckGroup;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIOptionCheckButtonGroup : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickCheckBox, EGsOptionItem /*InType*/, bool /*InIndex*/);

public:
	FOnClickCheckBox OnClickCheckBox;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UHorizontalBox* _HorizontalBoxList;

	// 옵션목록 타입
	EGsOptionItem _itemType;
	// 활성화
	bool _isEnabled = true;
	bool _isVislble = true;

	// 리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _slotEntryClass;

	// 아이템 버튼그룹 정보
	const FGsSchemaOptionCheckGroup* _checkButtonGroup = nullptr;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// 리스트 업데이트
	UFUNCTION()
	void OnRefreshEntrySlot(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickButton(EGsOptionItem InType, bool bIsSelected);

public:
	void SetData(EGsOptionItem InType, const FGsSchemaOptionCheckGroup* InSchemaButtonGroup, bool InisEnabled = true, bool InisVisible = true);
};
