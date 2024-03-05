#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIMaterialSetGainInfoCraftTargetListEntry.h"

#include "GsUIMaterialSetGainInfoCraftTargetList.generated.h"

class FGsCraftTarget;
class UGsDynamicPanelSlotHelper;
class UScrollBox;
class UWidget;

UCLASS()
class UGsUIMaterialSetGainInfoCraftTargetList final : public UUserWidget
{

	GENERATED_BODY()

public:
	/** 초기화 파라미터. 멤버 변수 생명 주기가 불안정하므로, 일부 제한 사항을 추가함. */
	struct Parameters final : public FNoncopyable
	{

		// 본 멤버 변수는 raw pointer 로서 생
		const TArray<FGsItemCollectionCondition>* _itemCollectionConditions = nullptr;

		// TArray<UGsUIMaterialSetGainInfoCraftTargetListEntry::Parameters> _collectionSlotInfos;

	};

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootList;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	UGsDynamicPanelSlotHelper* _slotHelper;

private:
	TWeakPtr<Parameters> _parameters;

	/************************************************************************/
	/* overrides															*/
	/************************************************************************/

protected:
	void NativeOnInitialized() final;

	void NativeConstruct() final;

	void NativeDestruct() final;

	/************************************************************************/
	/* behavior                                                             */
	/************************************************************************/

private:
	void ResetAllDatas();

	/************************************************************************/
	/* setter																*/
	/************************************************************************/

public:
	/** 제작창을 설정 */
	void SetParameters(TWeakPtr<Parameters> InParameters);

	/************************************************************************/
	/* event                                                                */
	/************************************************************************/

private:

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);

};
