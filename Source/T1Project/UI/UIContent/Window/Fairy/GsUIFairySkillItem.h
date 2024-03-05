#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIFairySkillItem.generated.h"

class UImage;
class UGsButton;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;
struct FGsSchemaPassivitySet;

struct FGsFairyPassivityInfo
{
	FairyLevelId level{ INVALID_FAIRY_LEVEL_ID };
	const FGsSchemaPassivitySet* passivity{ nullptr };
	bool isVehicle{ false };
	bool isUpgradable{ true };

	FGsFairyPassivityInfo(FairyLevelId lv, const FGsSchemaPassivitySet* pv, bool vehicle, bool upgradable)
		: level(lv), passivity(pv), isVehicle(vehicle), isUpgradable(upgradable) {}
};

UCLASS()
class T1PROJECT_API UGsUIFairySkillItem : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnClickSkillIcon, FairyLevelId);
	DECLARE_DELEGATE_OneParam(FOnClickSkillTooltip, TWeakPtr<FGsFairyPassivityInfo>);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _skillIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSkillName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSkillLevel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _level;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _upgradeEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _upgradeEffectBackLight;

	UPROPERTY(BlueprintReadOnly, Category = "GsUIFx", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* _enchantEnable;

private:
	TWeakPtr<FGsFairyPassivityInfo> _data;
	FairyLevelId _skillLevel{ INVALID_FAIRY_LEVEL_ID };

public:
	FOnClickSkillIcon OnClickFairySkillIcon;
	FOnClickSkillTooltip OnSkillTooltip;

protected:
	UFUNCTION()
	void OnClickSkillButton();

	virtual void NativeOnInitialized() override;

public:
	void SetData(TWeakPtr<FGsFairyPassivityInfo> infoData);
	void SetData(const FGsSchemaPassivitySet* data, FairyLevelId lv);
	void RefreshUI();
};

