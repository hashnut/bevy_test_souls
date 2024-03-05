// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupKeywordVoteMy.generated.h"

class UGsButton;
class UScrollBox;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaSocialKeyword;
/**
 * 유저 프로필 한마디 투표/정보 팝업 UI
 */
UCLASS()
class T1PROJECT_API UGsUIPopupKeywordVoteMy : public UGsUIPopup
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollKeyword;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfKeyword;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperKeyword;
	
protected:
	TArray<const FGsSchemaSocialKeyword*> _keywordList;
	
	uint64 _mainKeywordCount;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void UpdateMainKeywordCount();

	uint64 GetKeywordId(int32 InIndex);

protected:
	UFUNCTION()
	void KeywordListRefresh(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickOk();

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};
