#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "SlateCore/Public/Layout/Geometry.h"

#include "GsUIContinentalBP.generated.h"

/*
	대륙 bp
*/

class UImage;
class UCanvasPanel;

class UGsUIRegionItem;
class UGsButton;
class UGsUIContinentalBPDragZone;

UCLASS()
class UGsUIContinentalBP : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UGsUIRegionItem*> _arrayRegionItems;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIContinentalBPDragZone* _dragZone;


	// get
public:

	UGsUIContinentalBPDragZone* GetDragZone()
	{
		return _dragZone;
	}

	// 지역 item 찾기
	UGsUIRegionItem* FindRegionItem(int In_mapId);
	// 지역 item 찾기(그룹 정보도 참조)
	// https://jira.com2us.com/jira/browse/C2URWQ-7502
	UGsUIRegionItem* FindRegionItemMapIdAndGroupIds(int In_mapId);

	bool IsRegionItemUseable();
};