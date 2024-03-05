#pragma once

#include "CoreMinimal.h"
#include "../Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "../UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "GsUIMonsterCollectionSlate.generated.h"


class UImage;
class UGsButton;
class UCanvasPanel;
class UWidgetSwitcher;
class UGsUIRedDotDefault;


UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlate : public UUserWidget, public IGsToggleGroupEntry
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSlate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _panelLock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIRedDotDefault* _redDot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _gradeImgWidgetSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageComplete;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelected;

protected:

	const class FGsMonsterKnowledgeSlate* _data = nullptr;

	FSoftObjectPath _lastSoftObjectPath;

public:
	void BeginDestroy() override;

protected:
	void NativeOnInitialized() override;
	void NativeConstruct() override;

	void LoadImage(const FSoftObjectPath& InPath);
	void SetImageSprite(class UPaperSprite* InSprite);

	UFUNCTION()
	void OnClicked();

	UFUNCTION()
	void OnLongPressedSlate();

public:
	virtual void SetIsSelected(bool bInIsSelected);
	virtual bool GetIsSelected() const;

public:
	void SetData(const class FGsMonsterKnowledgeSlate* InData);
	void Invalidate();

	const class FGsMonsterKnowledgeSlate* GetData() const;
};