#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Client/SharedEnums/SharedBossEnum.h"
#include "Crc.h"
#include "GsUIPopupBossRankReward.generated.h"

struct FGsSchemaFieldBossData;
struct FGsSchemaNpcData;
struct FGsSchemaFieldBossRewardData;
struct FGsSchemaRewardDataRow;
struct FGsSchemaRewardData;
class UListView;
class UGsButton;
class UWidget;
class UTextBlock;
class UGsUITooltipDesc;

USTRUCT()
struct FGsRange
{
	GENERATED_BODY()

public:
	int32 _min;
	int32 _max;	

public:
	FGsRange()=default;
	virtual ~FGsRange()=default;
	FGsRange(const FGsRange& inRange)
	: _min(inRange._min), _max(inRange._max)
	{

	}
	FGsRange(int32 inMin, int32 inMax)
		: _min(inMin), _max(inMax)
	{
	}

	bool Equals(const FGsRange& inRank) const
	{
		return (_min == inRank._min) && (_max == inRank._max);
	}

	bool operator==(const FGsRange& inRank) const
	{
		return (inRank._max == _max) && (inRank._min == _min);
	}
};

FORCEINLINE uint32 GetTypeHash(const FGsRange& inRange)
{
	uint32 Hash = FCrc::MemCrc32(&inRange, sizeof(FGsRange));
	return Hash;
}

struct FGsFieldBossRewardArray
{
public:
	FGsFieldBossRewardArray()=default;
	virtual ~FGsFieldBossRewardArray()=default;

public:	
	TArray<const FGsSchemaRewardData*> _rewardDataArray;
};


UCLASS()
class T1PROJECT_API UGsUIPopupBossRankReward : public UGsUIPopup
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UListView* _rewardListView;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _closeButton;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _guidButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsUITooltipDesc* _guidPanel;
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _nameTextBlock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _titleTextBlock;

private:
	TMap<FGsRange, FGsFieldBossRewardArray>  bossRankRewardMap;

private:	
	FText _mailTitleFormatText;
	FText _dorpTitleFormatText;
	FText _mailTitleText;
	FText _dropTitleText;
	FText _guidText;

private:
	const FGsSchemaFieldBossData* _selectBossData;
	const FGsSchemaNpcData* _selectNpcData;
	BossRewardGiveType _giveType;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void OnClose();

private:
	UFUNCTION()
		void OnClickGuid();

public:
	void InitReward(const FGsSchemaFieldBossData* inBossInfo, const FGsSchemaNpcData* inNpcData, BossRewardGiveType inGiveType);
	void InitRewardList();
	void Add(int32 inMinRank, int32 inMaxRank, const FGsSchemaRewardData* inBossInfo);

protected:
	virtual void OnInputCancel() override;
};