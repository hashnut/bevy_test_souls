// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsSchemaEnums.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Option/GsOptionEnumType.h"
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionDivision.h"
#include "GsUIOptionSubArea.generated.h"

/**
 * 
 */

class UVerticalBox;
struct FGsSchemaOptionDivisionRow;
class UGsUIOptionPreset;
class UTextBlock;
UCLASS()
class T1PROJECT_API UGsUIOptionSubArea : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 중분류 이하 목록 리스트
	// 리소스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _scrollBoxEntryClass;

	// 스크롤헬퍼
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _scrollBoxHelper;
	
	//// 스크롤
	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//class UScrollBox* _scrollBox;

	// 소분류는 세로로 차례차례 생성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UVerticalBox* _verticalBox;	

	// 중분류는 가로로 차례차례 생성
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UVerticalBox* _verticalBoxSub;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UHorizontalBox* _horizontalBoxSub;

	// 선택 on/off 헬퍼
	FGsToggleGroup _toggleGroup;

	// 대분류 인덱스
	UPROPERTY()
	int32 _divisionIndex;

	// 중분류 탭을 생성할 것 인가?
	UPROPERTY()
	bool _isDivision;

	// 중분류 리스트
	TArray<const FGsSchemaOptionDivision*> _divisionList;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UCanvasPanel* _itemGroup;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionToggle* _itemGroupToggle;

	//  프리셋
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIOptionItemArea* _itemArea;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textMain;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSub;

	MsgUIHandle _msgHandle;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	// 스크롤 업데이트
	UFUNCTION()
	void OnRefreshEntryScrollBox(int32 InIndex, class UWidget* InEntry);	

	// 메뉴 클릭
	UFUNCTION()
	void OnClickSlot(int32 InIndex);

	// 토글이 클릭 됐을 때
	UFUNCTION()
	void OnClickItemGroupToggle(EGsOptionItem InType, bool InIsSelect);

	void OnUpdate(const IGsMessageParam*);
	
public:
	// 중분류 이하 업데이트
	void Update(bool InIsDivision, const TArray<FGsSchemaOptionDivisionRow>* InDivisionList);
	void UpdataItem(int32 InIndex);
};
