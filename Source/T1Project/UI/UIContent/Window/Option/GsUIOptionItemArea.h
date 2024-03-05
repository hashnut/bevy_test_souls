// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsSchemaEnums.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "GsUIOptionItemArea.generated.h"

/**
 * 
 */
class UVerticalBox;
struct FGsOptionSelectInfoRow;
struct FGsSchemaOptionItemRow;

UCLASS()
class T1PROJECT_API UGsUIOptionItemArea : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 아이템 목록 리스트
	// 리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _scrollBoxEntryClass;

	// 스크롤헬퍼
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	// 스크롤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UVerticalBox* _verticalBox;

	// 아이템 슬롯 초기값 셋팅
	EGsOptionItem _itemType = EGsOptionItem::Resolution;

	// 아이템 리스트
	const TArray<FGsSchemaOptionItemRow>* _itemList = nullptr;
	// 스크롤 크기
	float _scrollOffset = 0;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// 스크롤 업데이트
	UFUNCTION()
	void OnRefreshEntryScrollBox(int32 InIndex, class UWidget* InEntry);

public:
	// 아이템 리스트 업데이트
	void Update(const TArray<FGsSchemaOptionItemRow>* InOptionList);
	void UpdateList(const TArray<FGsSchemaOptionItemRow>* InOptionList);
};
