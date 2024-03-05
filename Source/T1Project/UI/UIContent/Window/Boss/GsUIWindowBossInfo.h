#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Message/GsMessageBoss.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIFieldBossPanel.h"
#include "GsUIWorldBossPanel.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateBoss.h"
#include "GsUIWindowBossInfo.generated.h"

enum class EGsBossType : uint8
{
	Field,
	World,
	Max
};

UCLASS()
class T1PROJECT_API UGsUIWindowBossInfo : public UGsUIWindow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _btnClose;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UPanelWidget* _panelMainTab;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _guidButton;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _guidCloseButton;	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidget* _guidPanel;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _bossWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIFieldBossPanel* _fieldBossPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsUIWorldBossPanel* _worldBossPanel;

private:
	FGsToggleGroup _bossToggleGroup;
	TArray<const struct FGsSchemaFieldBossData*> _bossDataArray;	
	TArray<TPair<MessageBoss, FDelegateHandle>> _bossDelegates;
	TArray<TPair<MessageUI, FDelegateHandle>> _uiDelegates;

private:
	FGsToggleGroup _toggleGroupMainTab;
	EGsBossTab _lastTab = EGsBossTab::None;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool SyncContentState() override;

private:
	UFUNCTION()
		void OnOpenGuid();
	UFUNCTION()
		void OnCloseGuid();

public:
	void OnSelectBoss(const struct IGsMessageParam* inParam);
	bool SelectTab(EGsBossTab inBossInfo);
	void OnClickMainTab(int32 InIndex, bool inSelect);
};