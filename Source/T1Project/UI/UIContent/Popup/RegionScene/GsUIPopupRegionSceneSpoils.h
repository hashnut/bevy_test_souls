#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "Map/WorldMap/GsWorldMapDefine.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "SlateCore/Public/Layout/Geometry.h"
#include "SlateCore/Public/Input/Events.h"

#include "Message/GsMessageGameObject.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapSpoilsItem.h"
#include "UI/UILib/Base/GsUIPopup.h"

#include "GsUIPopupRegionSceneSpoils.generated.h"

UCLASS()
class T1PROJECT_API UGsUIPopupRegionSceneSpoils : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
		TSubclassOf<class UUserWidget> _entryWidgetClass;
	UPROPERTY()
		class UGsDynamicPanelSlotHelper* _slotHelper;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _panelScrollRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _closeButton;

protected:
	//위젯이 스크롤 사이즈로 인해 줄어드는 현상을 없애기 위해 블루프린터에서 위젯 갯수 이상 되면 스크롤 보이도록 설정하는 구간
	UPROPERTY(EditDefaultsOnly, Category = "GsUI", meta = (BindWidget))
		int32 _numberOfWidgetForScroll = 5;

private:
	FGsToggleGroup _toggleGroup;

private:
	TArray<FGsSchemaWorldMapSpoilsMonsterLevel> _dataArray;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

public:
	void InvalidateAllInternal();

private:
	//키보드 인풋 백키 적용
	virtual void OnInputCancel() override;
	UFUNCTION()
		void OnClickClose();
	UFUNCTION()
		void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

public:
	void SetMonsterData(const TArray<FGsSchemaWorldMapSpoilsMonsterLevel>& inData);

public:
	virtual bool OnBack() override;
};