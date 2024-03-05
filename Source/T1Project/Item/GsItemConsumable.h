// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsItem.h"

struct FGsSchemaItemConsumeDetail;
struct FGsSchemaItemEffectSkillBook;
enum class CostumItemUseErrorType : uint8 { NoError, InvdeInvalidTime = 1, InvadeInvalidMap = 2 };
/**
 * 
 */
class T1PROJECT_API FGsItemConsumable : public FGsItem
{
	using Super = FGsItem;

protected:
	const FGsSchemaItemConsumeDetail* _consumDetailData;
	TArray<const FGsSchemaBase*> _effectDetailDataBaseList;

public:
	explicit FGsItemConsumable(const int32 In_ItemDetailID);
	FGsItemConsumable() = default;
	virtual ~FGsItemConsumable() = default;

public:
	virtual void UpdateBy(const FGsItem* In_Item) override;

public:
	virtual bool IsCooldownTimeItem() const override;
	virtual bool IsUseItem() const override;
	virtual float GetMaxCoolTime() const override;
	virtual int32 GetCooldownGroupID() const override;
	virtual int32 GetTooltipDurationTime() const override;
	virtual int32 GetPickOneTypeRewardID() const override;
	virtual int32 GetRewardId() const override;

private:
	void SetReStoreEffectDetailData(const TArray<int32>& InEffectIDList);	
	void SetSkillBookEffectDetailData(const TArray<int32>& InEffectIDList);
	void SetRewardEffectDetailData(const TArray<int32>& InEffectIDList);
	
public:
	bool IsOverlabUseBuffItem(class FGsAbnormalityHandlerBase* InhandlerBase);
	bool IsAllowedAutoUse() const;
	bool IsSpawnInvadePortal() const;
	bool IsLearnedSkillBookItem() const;
	bool IsItemUseLock(OUT CostumItemUseErrorType& errorNum) const;

	bool GetUserSkillSetIdList(OUT TArray<int32>& OutList) const; // SkillBookEffectÀÏ °æ¿ì
	ItemEffectType GetEffectType() const;

	void FindReStoreStatData(OUT TArray<FGsItemRestoreStatData>& outDataList) const;
	void FindEffectIdList(OUT TArray<int32>& outEffectIdList);

private:
	const FGsSchemaItemConsumeDetail* GetDetailConsumeTableData(const int32 In_ID) const;

};