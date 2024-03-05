#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/BattleArena/GsBattleArenaData.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIPopupBattleArenaGuildWarDetailScore.generated.h"

UCLASS()
class  T1PROJECT_API UGsUIPopupBattleArenaGuildWarDetailScore : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	/**
	 * Rend Team
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgRedGuildEmblem;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRedGuildName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRedPrefix;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textRedTotalScore;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _redRootPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIBattleArenaRankListEntry* _redMyStatusBoard;

	/**
	 * Blue Team
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgBlueGuildEmblem;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlueGuildName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBluePrefix;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlueTotalScore;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _blueRootPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUIBattleArenaRankListEntry* _blueMyStatusBoard;

	/**
	 * etc
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textNoriceRewardCondition;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnRefresh;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnOK;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _redEntryWidgetClass;
	
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _redPanelSlotHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _blueEntryWidgetClass;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _bluePanelSlotHelper;	

protected:
	FGsBattleArenaGuildWarTeamInfo _redTeamInfo;
	FGsBattleArenaGuildWarTeamInfo _blueTeamInfo;
public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClickedOK();
	UFUNCTION()
	void OnClcikedRefresh();

	virtual void OnInputCancel() override;

protected:
	UFUNCTION()
	void OnRefreshRedTeamEntry(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnRefreshBlueTeamEntry(int32 InIndex, UWidget* InEntry);

public:
	void SetData(EBattleArenaGuildWarState InState, const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam);
	void Invalidate(const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam);
};
