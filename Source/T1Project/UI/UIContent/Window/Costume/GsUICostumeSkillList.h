// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../../../../Costume/GsCostumeData.h"
#include "GsUICostumeSkillList.generated.h"

class UScrollBox;
class UWrapBox;
class UGsDynamicPanelSlotHelper;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUICostumeSkillList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _listPassiveSkill;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWrapBox* _passiveSkillListPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryPassiveSkillIcon;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _passiveScrollHelper;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GsUI_Widget")
	float WrapSize{ 510.f };

private:
	struct TempListData
	{
		bool _isActive{ false };
		int32 _id{ 0 };
		CreatureWeaponType _type{ CreatureWeaponType::NONE };
		const FSoftObjectPath _iconPath;
	};
	TArray<TempListData> _skillList;

	CreatureGenderType _genderType{ CreatureGenderType::ALL };
	CreatureWeaponType _weaponType{ CreatureWeaponType::ALL };

protected:
	UFUNCTION()
	void OnCreatePassiveSkillIconEntry(UWidget* InEntry);

	UFUNCTION()
	void OnRefreshPassiveSkillIconEntry(int32 index, UWidget* InEntry);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;
public:
	virtual void BeginDestroy() override;

	void RefreshSkillList(const TSharedPtr<FGsCostumeData> costumeData, bool useTotalSkill = false);
	void SetWrapSize(float wrapSize);
	
};
