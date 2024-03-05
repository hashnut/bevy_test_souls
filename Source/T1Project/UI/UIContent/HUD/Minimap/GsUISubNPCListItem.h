#pragma once
#include "CoreMinimal.h"
#include "../UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../Quest/GsQuestData.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "GsUISubNPCListItem.generated.h"

/*
	npc list item
*/

UCLASS()
class UGsUISubNPCListItem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

	// 멤버 변수
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnItem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textNPCName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherNpcStateIcon;
	
	TSharedPtr<FGsSubQuestNpcMoveData> _moveData;
	int _npcId;
	// 가상 함수
public:
	virtual void NativeOnInitialized() override;

	// 로직 함수
public:
	UFUNCTION()
	void OnClickItem();
	// 데이터 세팅
	void SetData(TSharedPtr<FGsSubQuestNpcMoveData> inMoveData);
};
