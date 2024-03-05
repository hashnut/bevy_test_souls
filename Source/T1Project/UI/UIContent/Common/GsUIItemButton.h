#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIItemButton.generated.h"

/*
* 재화 대신 아이템 아이콘 출력하는 버튼
* - 사용처(다시 뽑기 버튼)
*/

class UGsButton;
class UTextBlock;
class UImage;

UCLASS()
class T1PROJECT_API UGsUIItemButton : public UUserWidget
{
	GENERATED_BODY()

public:

	DECLARE_DELEGATE(FOnClickItemButton);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnMain;
	// 버튼 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textButtonName;
	// 아이템 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textItemCount;

public:
	FOnClickItemButton OnClickItemButton;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetButtonName(const FText& InText);
	void SetItemCount(int In_count);
	// 아이템 id 세팅
	void SetItemId(int In_itemId);

protected:
	UFUNCTION()
	void OnClickMainButton();
};