#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupBossContribution.h"
#include "GsUIBossChannelPanel.generated.h"

class UTextBlock;
class UListView;
class UComboBox;
class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIBossChannelPanel : public UUserWidget
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UListView* _channelListView;

private:
	FText _textChannelFormat;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:	
	void InitChannel(TArray<TSharedPtr<FGsChannelDefeatList>> inChannelDefeatArray, TWeakObjectPtr<UGsUIPopupBossContribution> inParents);
	void Clear();

public:
	void Close();
	void Open();
	bool IsOpen();
};
