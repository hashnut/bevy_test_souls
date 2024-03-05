#pragma once
#include "CoreMinimal.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Quest/GsQuestData.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "Runtime/DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"
#include "Quest/Table/GsQuestSubTableCacheSet.h"
#include "GsUIWindowSubAreaEntry.generated.h"


class UWidgetSwitcher;
/*
	Area item
*/
UCLASS()
class UGsUIWindowSubAreaEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherSelectBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherUnSelectBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _nameText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _numText;
	
	bool _bIsSelected = false;
	int32 _mapId = 0;

public:
	virtual void NativeOnInitialized() override;

public:
	// 데이터 세팅
	void SetSelect(bool inIsSelect);
	void SetData(int inSlotIndex, int32 inMapId);

	void Refresh();

private:
	bool GetDungeonGroupName(DungeonGroupId inDungeonGroupId, OUT FText& outDungeonGroupName);
	bool IsDungeonMap(IN int32 inDungeonGroupId);
};
