// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUISkillSlotEquipBase.generated.h"

class UImage;
class UGsButton;
class UPaperSprite;
class FGsSkill;

/**
 * 스킬창 장착슬롯, 상세정보의 업그레이드 스킬슬롯의 Base 클래스
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUISkillSlotEquipBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 편집 모드에서만 켜지는 버튼?
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSlot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSkill;

protected:
	SkillSlotId _slotId = 0;
	const FGsSkill* _skill = nullptr;
		
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	virtual void InitializeSlot(SkillSlotId InSlotId);
	// nullptr 넣으면 empty slot 처리
	virtual bool SetData(const FGsSkill* InData);

protected:
	virtual void SetEmpty();

protected:
	UFUNCTION()
	virtual void OnClickSlot() {}

public:
	void SetIconImageSprite(class UPaperSprite* InSprite);

protected:
	void SetIconDimmed(bool bIsDimmed);
	
public:
	bool IsEmpty() const;
	SkillSlotId GetSlotId() const { return _slotId; }
	const FGsSkill* GetSkill() const { return _skill; }
	SkillId GetSkillId() const;
};
