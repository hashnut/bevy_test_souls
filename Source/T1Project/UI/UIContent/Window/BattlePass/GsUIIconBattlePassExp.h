// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "GsUIIconBattlePassExp.generated.h"

struct FGsBattlePassMissionData;
class UGsButton;
class UImage;

struct IconData
{
	enum class DataType : uint8 { BattlePassExp };
	DataType _type{ DataType::BattlePassExp };

	IconData() = default;
	IconData(DataType type) : _type(type) {};
};

struct IconBattlePassExpData : public IconData
{
	TWeakPtr<FGsBattlePassMissionData> _data;

	IconBattlePassExpData(DataType type, TWeakPtr<FGsBattlePassMissionData> data)
		: IconData(type), _data(data) {}
};
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIIconBattlePassExp : public UGsUIIconDefault
{
	GENERATED_BODY()
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLongPressEventBattlePass, TWeakPtr<IconData>);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UGsButton* _slotButton;


	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon")
	FText _countText;

protected:
	TWeakPtr<IconData> _data;
	FDelegateHandle _longPressDelegate;

public:
	FOnLongPressEventBattlePass  OnLongPressEvent;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void OnFinishedLongPress();

public:

	void SetData(TWeakPtr<IconData> data);
	void RefreshUI();

};
