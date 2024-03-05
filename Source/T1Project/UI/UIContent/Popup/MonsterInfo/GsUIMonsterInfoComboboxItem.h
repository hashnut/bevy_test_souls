#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUIMonsterInfoComboboxItem.generated.h"


// 몬스터 팝업 레벨 표시 콤보 박스 아이템
// https://jira.com2us.com/jira/browse/CHR-24415

class UGsButton;

class UImage;

UCLASS()
class UGsUIMonsterInfoComboboxItem :
	public UUserWidget,
	public IGsToggleGroupEntry,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:

	// 아이템 버튼(전체)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItem;
	// 선택 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textName;
	

	bool _bIsSelected = false;

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	// IGsToggleGroupEntry override
	virtual void SetIsSelected(bool bInIsSelected) override;
	virtual bool GetIsSelected() const override
	{
		return _bIsSelected;
	}

	// 로직 함수
public:
	// 아이템 클릭
	UFUNCTION()
	void OnClickItem();

	void SetText(const FText& In_name);
	// 현재 레벨 item 표시
	void SetCurrentMonsgerLevelItem(bool In_isCurrentLevel);

};