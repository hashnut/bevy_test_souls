// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/Operation/GsUIGuildOperationBase.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "GsUIGuildOperationBuilding.generated.h"

/**
 * 길드 건물 페이지
 * - 주의: 다른 길드 정보 상세보기 시에도 동일UI 활용함에 유의
 */
UCLASS()
class T1PROJECT_API UGsUIGuildOperationBuilding : public UGsUIGuildOperationBase, public IGsInvalidateUIInterface
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UPanelWidget* _panelGuildAsset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockGuildAsset;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UUniformGridPanel* _panelGrid;

	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	TArray<const struct FGsSchemaGuildBuilding*> _dataList;
	bool _bIsAuthority = false;
	bool _bIsMyGuild = false;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	virtual void InvalidateAllInternal() override;

public:
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
	virtual void Hide() override;
	
	virtual void OnReceiveBuildingList() override;
	virtual void OnChangeGuildLevel() override;
	virtual void OnChangeCurrency(CurrencyType InType) override;
	virtual void OnUpdateBuilding(GuildBuildingId InBuildingId) override;
	virtual void OnChangeMyAuthority() override;

protected:
	void InvalidateList();

protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

protected:
	void SetUIGuildAsset();
};
