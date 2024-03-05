#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "ItemDropNavigator/GsUIItemDropListContainer.h"

#include "GsUIPopupItemDropNavigator.generated.h"

class UGsButton;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsUIIconBase;
class UGsUIItemDropElement;
class UTextBlock;
class UWidget;
struct FGsSchemaWorldMapMonsterElementDataRow;

UCLASS()
class UGsUIPopupItemDropNavigator final : public UGsUIPopup
{

	GENERATED_BODY()

public:
	struct Parameters final
	{
		/** 타이틀 텍스트 선택. true 면 획득처, false 면 사용처로 출력함. */
		bool _titleSupply;
		/** 연관 타입 */
		CostType _costType = CostType::MAX;
		/** 연관 재화 타입 */
		CurrencyType _currencyType = CurrencyType::NONE;
		/** 연관 아이템ID */
		ItemId _itemId = INVALID_ITEM_ID;
	};

	/**
	 * instance members
	 */

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _closeBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _targetIconSelector;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _targetNameTxt;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIItemDropListContainer* _itemDropListContainer;

	UPROPERTY()
	UGsDynamicIconSlotHelper* _targetIconSlotHelper;

protected:
	Parameters _parameters;

protected:
	void NativeOnInitialized() final;

public:
	/** 아이콘 파리미터 설정 */
	void SetParameters(Parameters InParameters);

	/** 목록에 출력할 맵ID와 크리쳐ID 쌍.
	 *  크리쳐ID는 필요없을 경우 INVALID_CREATURE_ID 일 수 있음.
	 */
	void SetParametersDropMapCreatures(TArray<TPair<MapId, CreatureId>> InDropMapCreatures);

	/** 목록에 출력할 맵ID와 크리쳐ID 쌍.
	 *  크리쳐ID는 필요없을 경우 INVALID_CREATURE_ID 일 수 있음.
	 */
	void SetParametersDropMapCreatures(TArray<FGsSchemaWorldMapMonsterElementDataRow> InDropMapCreatures);

	void SetParametersCraftIds(TArray<CraftId> InCraftIds);

private:
	void Invalidate();

private:
	UFUNCTION()
	void OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon);

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedClose();

	/** 내부 등 어떤 버튼이든 "팝업닫기" 가 아닌 다른 창으로의 이동을 시도할 때, 본 팝업 등을 닫기 위해 이 콜백을 호출함. */
	void OnClickNavigateButton();

	// 2022/10/20 PKT - KeyBoard Mapping
protected:
	virtual void OnInputCancel() final;

};
