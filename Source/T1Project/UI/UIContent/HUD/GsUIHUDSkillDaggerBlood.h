#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIHUDSkillDaggerBlood.generated.h"

class FGsSkill;
class UWidgetSwitcher;
class UGsUIDaggerBlood;
class UProgressBar;
struct IGsMessageParam;
class FGsStatInfo;

UCLASS()
class T1PROJECT_API UGsUIHUDSkillDaggerBlood : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _daggerBloodSwitcher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _daggerBloodSkillProgress;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIDaggerBlood* _blood1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIDaggerBlood* _blood2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIDaggerBlood* _blood3;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIDaggerBlood* _blood4;

protected:
	MsgUIHandleArray _msgUIHandleList;
	MsgGameObjHandleArray _msgGameObjectHandleList;

	TArray<UGsUIDaggerBlood*> _bloodProgress;

	CreatureWeaponType _weaponType = CreatureWeaponType::MAX;
	
	int   _curBloodCount{ 0 };
	bool  _runStatConsumSkill{ false };
	//bool _needCheckAbnormalityEvent = false;

	float _endTime{ 0.f };
	float _coolTime{ 0.f };

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void BindMessages();
	void UnbindMessages();

	void OnChangeSkillSet(const IGsMessageParam* InParam);
	void OnChangeSkillSlot(const IGsMessageParam* InParam);
	void OnUpdatePlayerStatList(const IGsMessageParam* InParam);
	void OnAddAbnormality(const IGsMessageParam* InParam);
	void OnRemoveAbnormality(const IGsMessageParam* InParam);

	//void OnStartBuffSkill(const IGsMessageParam* InParam);	

	void UpdateBloodDotInfo(bool abnormalityAdded = false);
	//void RunningStatConsumSkill();

	const FGsStatInfo* GetSensitiveStatInfo() const;
	int32 GetStatConsumableSkillSlot(StatType statType);
	bool IsConsumableSkillSlot(StatType statType);
	bool IsStatConsumableActiveSkill(const FGsSkill* skill, StatType statType = StatType::SENSITIVE);
	bool IsEnableSensitiveSkill();

public:
	void SetUseDaggerBloodSkill(bool set);
};

