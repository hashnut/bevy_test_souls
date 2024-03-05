#pragma once
#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUICostumeSkillIconItem.generated.h"

class UGsButton;
class UImage;
class UWidgetSwitcher;
class UTextBlock;
class UPanelWidget;

struct FGsSchemaPassivitySet;
struct FGsSchemaSkillSet;

DECLARE_DELEGATE_TwoParams(FOnSelectIcon, bool, int);

UCLASS()
class T1PROJECT_API UGsUICostumeSkillIconItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSkillIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _skillIcon;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSkillMark;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textWeapon;

	bool _isActiveSkill{ false };
	int _passivityId{ -1 };

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnClickSkillDetailInfo();
public:

	FOnSelectIcon _OnSelectedIcon{ nullptr };
	void SetData(bool activeSkill = false, int32 id = 0, 
		CreatureWeaponType weaponType = CreatureWeaponType::NONE, const FSoftObjectPath iconPath = FSoftObjectPath());

	void SetIconImageSprite(class UPaperSprite* InSprite);
};

