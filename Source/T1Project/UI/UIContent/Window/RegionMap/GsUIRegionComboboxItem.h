#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUIRegionComboboxItem.generated.h"

/*
	지역맵: 콤보 박스 리스트 아이템
*/

class UGsButton;

class UImage;

UCLASS()
class UGsUIRegionComboboxItem :
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
	// town image
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgInTown;

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
	// 현재 맵 item 표시
	void SetCurrentMapItem(bool In_isCurrentMap);

	void SetIsInTown(bool In_isInTown);
};