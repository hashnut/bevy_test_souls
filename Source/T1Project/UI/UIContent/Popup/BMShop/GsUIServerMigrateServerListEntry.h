// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIServerMigrateServerListEntry.generated.h"

class UGsButton;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;

struct MigrateServer;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIServerMigrateServerListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textServerNormalTypeName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSelected;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textServerSelectedTypeName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRecommend;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelNew;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _connectedServer;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherServerState;
	
	TFunction<void(TWeakPtr<MigrateServer>)> _callbackFunc = nullptr;

protected:
	TWeakPtr<MigrateServer> _data = nullptr;
	bool _myServer{ false };

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClickedNormal();

public:
	void SetData(TWeakPtr<MigrateServer> InData, WorldId InSelectedWorldId, TFunction<void(TWeakPtr<MigrateServer>)> InCallbackFunc);
	void Invalidate(WorldId InSelectedWorldId);
	bool IsSelected() const;

public:
	TWeakPtr<MigrateServer> GetData() const { return _data; }
};
