// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsSchemaEnums.h"
#include "DataCenter/Public/Option/GsOptionEnumType.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "DataSchema/Option/GsSchemaOptionButtonGroup.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "Option/GsOptionEnumType.h"
#include "GsUIOptionToggleGroup.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIOptionToggleGroup : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickSlot, EGsOptionItem /*InType*/, bool /*InIsSelected*/);

public:
	FOnClickSlot OnClickSlot;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UHorizontalBox* _HorizontalBoxList;

	// 리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _slotEntryClass;

	// 아이템 버튼그룹 정보
	const struct FGsSchemaOptionToggleGroup* _schemaToggleGroup = nullptr;
	// 활성화?
	bool _isEnabled = true;

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

	// 버튼그룹의 버튼이 클릭 됐을 때
	void OnClickToggleButton(EGsOptionItem InType, bool InIsSelect);

public:
	void SetData(const FGsSchemaOptionToggleGroup* InSchemaToggleGroup, bool InIsEnabled);
};
