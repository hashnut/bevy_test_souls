#pragma once
#include "CoreMinimal.h"
#include "Map/GsMapIconType.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUINPCListItem.generated.h"

class UGsButton;
class UGsUIIconBase;

class UImage;
class UCanvasPanel;

/*
	npc list item
*/

UCLASS()
class UGsUINPCListItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textNPCName;
	// functionType 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgFunctionType;
	
	// 가상 함수
public:
	virtual void NativeOnInitialized() override;

	// 로직 함수
public:
	UFUNCTION()
	void OnClickItem();
	// 데이터 세팅
	void SetData(FString In_npcName);

};
