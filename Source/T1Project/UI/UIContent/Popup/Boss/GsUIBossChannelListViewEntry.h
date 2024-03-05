#pragma once
#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "GsUIBossChannelListViewEntry.generated.h"

class UTextBlock;
class UGsButton;
class UGsUIPopupBossContribution;
class UGsUIBossChannelPanel;

UCLASS()
class UGsUIBossChannelListViewEntryData : public UObject
{
	GENERATED_BODY()

public:
	FString _channelName;	
	int32 _channel;
	TWeakObjectPtr<UGsUIPopupBossContribution> _parents;
	TWeakObjectPtr<UGsUIBossChannelPanel> _channelPanel;
};

UCLASS()
class T1PROJECT_API UGsUIBossChannelListViewEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _textBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _button;

public:
	int32 _channel;
	TWeakObjectPtr<UGsUIBossChannelPanel> _channelPanel;
	TWeakObjectPtr<UGsUIPopupBossContribution> _parents;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;
	UFUNCTION()
	void OnClickButton();
};
