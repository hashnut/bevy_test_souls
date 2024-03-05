#pragma once

#include "CoreMinimal.h"
#include "../UI/UIControlLib/Interface/GsEntryInterface.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIBattleArenaRankListEntry.generated.h"

UCLASS()
class  T1PROJECT_API UGsUIBattleArenaRankListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherRanking;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRanking;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textKill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textAssist;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textDeath;

public:
	void SetData(int32 InRank, const struct FGsBattleArenaGuildWarMemberInfo& InData);
};
