#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "UMG/Public/Blueprint/IUserObjectListEntry.h"
#include "GSUIMonsterCollectionPortrait.generated.h"

UCLASS()
class T1PROJECT_API UGsMonsterCollectionPortraitEntryData : public UObject
{
	GENERATED_BODY()

protected:
	const class FGsMonsterKnowledgeBook* _data = nullptr;
	mutable bool _isImmediateAnimation = false;

public:
	void SetData(const class FGsMonsterKnowledgeBook* InData);
	void SetData(const class FGsMonsterKnowledgeBook* InData, bool IsIsImmediateAnimation);

	bool IsImmediateAnimation() const;
	const class FGsMonsterKnowledgeBook* Get() const;
};

class UGsUIVFX;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;
class UGsUIProgressBar;
class UGsUIRedDotDefault;
class UGsUIMonsterPortrait;

UCLASS()
class T1PROJECT_API UGSUIMonsterCollectionPortrait : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMonsterPortrait* _npcPortrait;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherGradeEdge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _levelUpImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUiVFX", meta = (BindWidget))
	UPanelWidget* _panelLevleUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUiVFX", meta = (BindWidget))
	UTextBlock* _textRewardPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDot;

	// 2021/12/08 PKT - Max Level ´Þ¼º ½Ã VFX
	UPROPERTY(BlueprintReadOnly, Category = "GsUiVFX", meta = (BindWidget))
	UGsUIVFX* _effectFinalLevelUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUiVFX", meta = (BindWidget))
	UGsUIVFX* _effectNormalLevelUp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUiVFX", meta = (BindWidget))
	UPanelWidget* _panelCompleted;

	UPROPERTY(BlueprintReadOnly, Category = "GsUiVFX", meta = (BindWidget))
	UGsUIProgressBar* _expProgressBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUiVFX", meta = (BindWidget))
	UTextBlock* _textLevel;

protected:
	const class FGsMonsterKnowledgeBook* _data = nullptr;
	Level _prevLevel = 0;
	bool _isOnlyDisplay = false;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* inListItemObject) override;
	
	void PlayImmediateAnimation(bool IsIsImmediateAnimation);
public:
	void SetData(const class FGsMonsterKnowledgeBook* InData, bool InIsOnlyDisplay = false);
	void Invalidate();

	const class FGsMonsterKnowledgeBook* GetData() const;
};
