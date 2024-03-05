#pragma once

/**
* file		GsUITitleEquipSlotEffect.h
* @brief	window title ¿¡ ÀåÂø ½½·Ô È¿°ú
* @author	PKT
* @date		2022/01/24
**/


#include "CoreMinimal.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "GsUITitleEquipSlotStatInfo.generated.h"

class UGsUIVFX;
class UTextBlock;
class FGsTitleData;
class UHorizontalBox;
class UGsSwitcherButton;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUITitleEquipSlotStatInfo : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _horizontalBoxFirst;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFirstStatName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textFirstStatValue;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _horizontalBoxSecond;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSecondStatName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSecondStatValue;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textEmptyStat;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _effectUnEquip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _effectEquip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUITitleEquipGradeLoopEffect* _animationGradeEffect;

	class UWidgetAnimation* _animationData = nullptr;

	const class FGsTitleData* _titleData = nullptr;

	bool _isSelected = false;	
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetDisplay(const FGsTitleData* InData);
	void SetChangedEquipTitle(bool InIsEquip, bool InIsShowEffect = false);

protected:
	void SetPlayAnimation(float InAtTime, bool InIsForward);
	
public:
	void SetData(const FGsTitleData* InData, bool InIsShowEffect);
	void PlayEffect(bool InIsSelected);
};
