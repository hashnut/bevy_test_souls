#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossRewardData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "GameObject/Boss/GsFieldBossHandler.h"
#include "GsUIPopupBossContribution.generated.h"

class UTextBlock;
class UListView;
class UComboBox;
class UGsButton;
class UGsUIBossChannelPanel;
struct FGsSchemaFieldBossData;
struct FGsSchemaNpcData;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUIPopupBossContribution : public UGsUIPopup
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _contributionTitleTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _bossSpawnTimeTextBlock; 
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _channelTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _channelButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _closeButton;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UListView* _contributionListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsUIBossChannelPanel* _channelPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UWidgetSwitcher* _widgetSwitcher;

private:
	FText _textChannelFormat;
	FText _textTitleResultForamt;

private:
	const FGsSchemaFieldBossData* _bossData;
	const FGsSchemaNpcData* _bossNpcData;	
	int32 _currentChannel = 1;
	MapId _mapId;
	TSharedPtr<FGsResultDepeatFieldBoss> _bossDepeatData;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	void InitContribution(const FGsSchemaFieldBossData* inBossData, TSharedPtr<FGsResultDepeatFieldBoss> inData);

private:
	UFUNCTION()
	void OnClose();
	UFUNCTION()
		void OnOpenChannel();	
	void InvalidBossContribution();

private:
	void SetResultDepeatBoss(TSharedPtr<FGsResultDepeatFieldBoss> inData);	
	void SetBossData(const FGsSchemaFieldBossData* inBossData, const FGsSchemaNpcData* inBossNpcData);
	const FGsSchemaFieldBossRewardData* GetBossRewardData(const FGsSchemaFieldBossData* inBossData, int32 inRank);

public:
	void SetChannel(int32 inChannel);
	void InitChannel();

protected:
	virtual void OnInputCancel() override;
};