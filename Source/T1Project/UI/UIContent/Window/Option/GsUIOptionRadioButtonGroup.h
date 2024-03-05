// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsSchemaEnums.h"
#include "DataSchema/Option/GsSchemaOptionButtonGroup.h"
#include "../UI/UIControlLib/Control/GsToggleGroup.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "GsUIOptionRadioButtonGroup.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIOptionRadioButtonGroup : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnClickSlot, EGsOptionItem /*InType*/, int /*InIndex*/);

public:
	FOnClickSlot OnClickSlot;

protected:
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

	// 버튼 리스트( 하, 중, 상, 최상 ) 은 가로로 차례차례 생성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UHorizontalBox* _HorizontalBoxList;

	// 선택 on/off 헬퍼
	FGsToggleGroup _toggleGroup;

	// 옵션목록 타입
	EGsOptionItem _itemType;
	// 활성화
	bool _isEnabled = true;

	bool _isVislble = true;

	// 리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _slotEntryClass;

	// 아이템 버튼그룹 정보
	const FGsSchemaOptionButtonGroup* _buttonGroup = nullptr;

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
	UFUNCTION()
	void OnClickButton(int32 InIndex, bool bIsSelected);

public:
	void SetData(EGsOptionItem InType, const FGsSchemaOptionButtonGroup* InSchemaButtonGroup, 
		int InOptionIndex, bool InisEnabled, bool InisVisible = true);
	void SetValueIndex(int InOptionIndex);
};
