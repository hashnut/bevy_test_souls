#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIWorldBossClearPanel.generated.h"


UCLASS()
class T1PROJECT_API UGsUIWorldBossClearPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UListView* _rankListView;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIWorldBossRankListViewEntry* _lastHitRankData;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIWorldBossRankListViewEntry* _playerRankData;

public:
	virtual void NativeConstruct() override;
	void InvalidResult();

};

