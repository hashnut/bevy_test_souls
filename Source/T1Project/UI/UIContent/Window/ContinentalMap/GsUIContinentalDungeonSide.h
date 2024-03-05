#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUIContinentalDungeonSide.generated.h"

class UGsButton;

/*
	대륙맵 던전 우측(자동이동, 즉시 이동)
*/


UCLASS()
class T1PROJECT_API UGsUIContinentalDungeonSide : public UUserWidget
{
	GENERATED_BODY()
protected:
	// 자동 이동 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoMove;
	// 텔레포트 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTeleport;
	// 던전 정보 보기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDungeonInfo;
	// 닫기 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	// 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textName;

private:

	// 가상 함수
public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

	// 로직 함수
public:
	void SetData(const FText& In_name)
	{
		_textName = In_name;
	}
	UFUNCTION()
	void OnClickAutoMove();
	UFUNCTION()
	void OnClickTeleport();
	UFUNCTION()
	void OnClickDungeonInfo();
	UFUNCTION()
	void OnClickClose();
};