// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GsUIPopupKeywordVote.generated.h"

class UGsButton;
class UTextBlock;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaSocialKeyword;
/**
 * 유저 프로필 한마디 투표/정보 팝업 UI
 */
UCLASS()
class T1PROJECT_API UGsUIPopupKeywordVote : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDesc;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollKeyword;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfKeyword;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperKeyword;
	
protected:
	FGsToggleGroup _toggleGroupKeyword;
	TArray<const FGsSchemaSocialKeyword*> _keywordList;
	
	uint64 _selectedKeywordId;
	uint64 _mainKeywordCount;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void UpdateMainKeywordCount();

	void OnClickSlotKeyword(int32 InIndex, bool bIsSelected);
	void OnSelectChangedKeyword(int32 InIndex);

	uint64 GetKeywordId(int32 InIndex);

protected:
	UFUNCTION()
	void KeywordListCreate(UWidget* InEntry);
	UFUNCTION()
	void KeywordListRefresh(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickOk();
	UFUNCTION()
	void OnClickCancel();

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
