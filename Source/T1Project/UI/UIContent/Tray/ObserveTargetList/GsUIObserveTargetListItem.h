#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"

#include "GsUIObserveTargetListItem.generated.h"

/*
* tray 관전 타겟 리스트 아이템
*/

class UGsButton;
class UGsGameObjectBase;

UCLASS()
class UGsUIObserveTargetListItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()
	// 멤버 변수
protected:
	// 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItem;
	// 타겟 레벨
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textTargetLevel;
	// 타겟 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textTargetName;

private:
	// 아이템 대상
	UGsGameObjectBase* _itemTarget;

	// 가상 함수
public:
	virtual void NativeOnInitialized() override;

	// 로직 함수
public:
	// 아이템 데이터 세팅
	void SetObserveTargetListItemData(UGsGameObjectBase* In_target);

	// ui 이벤트
public:
	UFUNCTION()
	void OnClickItem();


};