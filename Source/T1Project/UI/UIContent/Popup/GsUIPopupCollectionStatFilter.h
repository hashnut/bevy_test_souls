#pragma once
#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupCollectionStatFilter.generated.h"

class UGsButton;
class UPanelWidget;
class UGsDynamicPanelSlotHelper;
class UGsUIItemCollectionStatTypeCheckbox;

/**
 * 페어리, 코스춤 Stat filter
 * 콜렉션 매니저 직접 호출없이 값을 넣어주고 받도록 제작
 */
UCLASS()
class T1PROJECT_API UGsUIPopupCollectionStatFilter : public UGsUIPopup
{
	GENERATED_BODY()

public:
	using StatFilter = TPair<StatType, bool>;
	using MenuSet = TArray<StatFilter>;

	struct PopupInitData 
	{
		MenuSet _sortStatMenu; //복사생성

		TFunction<void(const MenuSet&)>	_cbClosed{ nullptr };		//filter 선택이 끝나고 닫은 후 sort하도록 알림
		TFunction<void(const StatFilter&)> _cbChanged{ nullptr };	//stat 변화때 sort 하도록 알림
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemCollectionStatTypeCheckbox* _checkboxAll;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _listPanel;;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _scrollBoxHelper;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOk;

	MenuSet _curStatMenu;

	TFunction<void(const StatFilter&)> _onChanged{ nullptr };
	TFunction<void(const MenuSet&)>	_onClosed{ nullptr };

protected:
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickAll();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickStatTypeCheckbox(int32 index);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickOk();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickCancel();

	UFUNCTION()
	void OnCreateEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);

protected:
	void NativeOnInitialized() override;

public:
	void BeginDestroy() override;

	void InitializeData(UGsUIPopupCollectionStatFilter::PopupInitData* initParam = nullptr);

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};