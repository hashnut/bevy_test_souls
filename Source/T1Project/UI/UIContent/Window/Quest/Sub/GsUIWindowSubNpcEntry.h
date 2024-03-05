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
#include "GsUIWindowSubNpcEntry.generated.h"

struct FGsQuestSubStoryTableCache;
class UImage;
class UWidgetSwitcher;
/*
	npc list item
*/

UCLASS()
class UGsUIWindowSubNpcEntry : public UUserWidget, public IGsEntryInterface
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

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgSelectFace;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgUnSelectFace;
		
	bool _bIsSelected = false;
	int32 _npcId = 0;

public:
	virtual void NativeOnInitialized() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickItem();

public:
	// 데이터 세팅
	void SetData(int32 inNpcId);
	void SetSelect(bool inIsSelect);	

	void Refresh();

private:
	void SetImg(UImage* inImg, const FSoftObjectPath& iconPath);
};
