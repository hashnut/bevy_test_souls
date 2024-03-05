#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../Quest/GsQuestData.h"
#include "GsUISubNPCList.generated.h"

class UUserWidget;
class UScrollBox;
class UVerticalBox;
class UWidget;
class UGsDynamicPanelSlotHelper;
/*
	NPC LIST
*/
UCLASS(meta = (DisableNativeTick))
class UGsUISubNPCList : public UUserWidget
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	// 스크롤
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;

	// 아래로 차례차례 추가
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherListType;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	TArray<TSharedPtr<FGsSubQuestNpcMoveData>> _subQuestNpcMoveDataList;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	virtual void BeginDestroy() override;

	// 로직 함수
public:
	// 데이터 세팅
	void SetNpcListData();
protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
};