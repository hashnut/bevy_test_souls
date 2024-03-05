// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Option/GsOptionEnumType.h"
#include "GameFramework/PlayerInput.h"
#include "Message/GsMessageUI.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsUIWindowKeyBoardMapping.generated.h"

/**
 *  윈도우 키보드 매핑
 */

class UInputKeySelector;
class UGsButton;
class UImage;
class UTextBlock;
class UGsUIKeyBoardMappingCircle;
class UGsUIKeyBoardMappingRect;
class UGsUIKeyBoardMappingEntry;
class UScrollBox;

UCLASS()
class T1PROJECT_API UGsUIWindowKeyBoardMapping : public UGsUIWindow
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnButtonClick, int32);
	
protected:
	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnClose;
	// back 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnBack;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnRefresh;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _selectKeyName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSkillLeftArrow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnSkillRightArrow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollSkill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _moveUp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _moveDown;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _moveLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _moveRight;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _normalAttack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _pickUp;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _scan;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _switchSpiritshot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _switchAuto;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _switchAutopotion;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_5;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_6;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_7;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_8;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_9;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_10;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_11;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skillslot_12;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _costumeskillslot_1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_2;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_3;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_4;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_5;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_6;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_7;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_8;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_9;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslot_10;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslotLeft;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _quickslotRight;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _camChange;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _minimap;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _bookmark;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _hudTapPrev;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _hudTapNext;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _chat;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _character;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _event;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _bmshop;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _inventory;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _skill;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _menu;
	/*UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UInputKeySelector* _savemode;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UInputKeySelector* _screenshotmode;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UInputKeySelector* _bufflist;*/
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _ok;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingRect* _no;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIKeyBoardMappingCircle* _targetChange;

	TMap<EGsKeyMappingType, class UGsUIKeyBoardMappingEntry*> _keySelectors;
	TMap<EGsKeyMappingType, FInputActionKeyMapping> _inputActionBaseKeyMappings;
	TMap<EGsKeyMappingType, FInputActionKeyMapping> _inputActionKeyMappings;
	TMap<EGsKeyMappingType, FInputAxisKeyMapping> _inputAxisKeyMappings;

	MsgUIHandle _msgHandle;

	int _scrollSkillIndex = 0;
	int _scrollQuickIndex = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void CloseInternal() override;
	virtual bool SyncContentState() override;

	virtual void OnClickBack() override;
	virtual void OnClickCloseAllStack() override;

public:
	// UObject override
	virtual void BeginDestroy() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickRefresh();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSkillLeftArrow();
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickSkillRightArrow();

	void OnKeySelectFunc(FInputChord InputChord);
	void OnIsSelectingKeyChangedFunc();
	void OnUIUpdate();
	void OnUpdate(const IGsMessageParam*);
	void OpenConsentPopup(EGsKeyMappingType In_Type, EGsKeyMappingType In_SameKeyType, 
		FInputChord In_InputChordNew, FInputChord In_InputChordOld);

	void SetData(EGsKeyMappingType In_Type, FInputChord In_InputChord);
	void SetSelectKeyName(bool In_isSelect, EGsKeyMappingType In_Type);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnKeySelected(EGsKeyMappingType In_Type, FInputChord inInputChord);
};

