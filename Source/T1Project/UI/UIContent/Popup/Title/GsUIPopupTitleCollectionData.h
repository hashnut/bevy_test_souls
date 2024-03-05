#pragma once

#include "CoreMinimal.h"
#include "Title/ETitleEnum.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUI.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "../Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "GsUIPopupTitleCollectionData.generated.h"


class UGsButton;
class UScrollBox;
class UTextBlock;
class UGsDynamicPanelSlotHelper;


UCLASS()
class T1PROJECT_API UGsUIPopupTitleCollectionData : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRetentionStatus;

	// 2022/01/21 PKT - 칭호 보유 갯수
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textRetentionStatusCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxCollection;

	// 2022/01/20 PKT - Stat Info
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfCollection;

	/**
	 * 2022/02/04 PKT - 연출 관련 변수
	 */
	FTimerHandle _timerHandler;

	TArray<TPair<int32, ETitleRewardState>> _updateEffectList;
	// 2022/02/04 PKT - 연출 Index
	int32 _nextEffectIndex = INDEX_NONE;
	// 2022/02/04 PKT - 연출이 진행 중인가?
	bool _isPlayingEffect = false;

protected:
	TPair<MessageContentTitle, FDelegateHandle> _messageUiDelegate;

	/************************************************************************/
	/* override                                                             */
	/************************************************************************/
public:
	void BeginDestroy() override;

protected:
	void NativeOnInitialized() override;	
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

protected:
	void SetRetentionStatus();
	void SetEffectPlay();

	/************************************************************************/
	/* bind                                                                 */
	/************************************************************************/
protected:
	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, class UWidget* InEntry);

	UFUNCTION()
	void OnClickedClose();

	UFUNCTION()
	void OnFinishedVFX();

	void InvalidCollectionList(const struct IGsMessageParam* InParam);

protected:
	virtual void OnInputCancel() override;
};