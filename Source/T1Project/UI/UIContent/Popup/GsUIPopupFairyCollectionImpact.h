#pragma once
#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "GsUIPopupFairyCollectionImpact.generated.h"

class UScrollBox;
class UGsButton;
class UTextBlock;
class FGsFairyCollection;
class UGsUIFairyStat;
class UPanelWidget;

UCLASS()
class T1PROJECT_API UGsUIPopupFairyCollectionImpact :
    public UGsUIPopup
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "GsUI", meta = (BindWidget))
    UGsButton* _closeButton;

    UPROPERTY(EditDefaultsOnly, Category = "GsUI", meta = (BindWidget))
    UTextBlock* _textProgressRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
    UScrollBox* _listRewardStat;

    UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
    UPanelWidget* _listPanel;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
    TSubclassOf<UUserWidget> _entryWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
    UPanelWidget* _emptyPanel;
    
    UPROPERTY()
    class UGsDynamicPanelSlotHelper* _scrollBoxHelper;

protected:
    UFUNCTION(BlueprintCallable, Category = "GsUI")
    void OnClickClose();

    UFUNCTION()
    void OnRefreshEntry(int32 index, UWidget* entry);

protected:
    virtual void NativeOnInitialized() override;
    virtual void CloseInternal() override;

public:
    struct PopupInitData
    {
        PassivityOwnerType _collecionOwnType{ PassivityOwnerType::FAIRY_COLLECTION };
        size_t _completeCount{ 0 };
        size_t _collectionCount{ 0 };
        
        PopupInitData(PassivityOwnerType collecionOwnType, size_t completeCount, size_t collectionCount)
            : _collecionOwnType(collecionOwnType), _completeCount(completeCount), _collectionCount(collectionCount) {}
    };

    virtual void InitializeData(PopupInitData* initParam = nullptr);

protected:
    TArray<TPair<FText, FText>> _collectionRewards;

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() override;
};

