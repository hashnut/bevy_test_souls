#pragma once

#include "GsCraftTab.h"

#include "GsCraftTargetObject.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsCostPackage;
class FGsCostPackageMultiple;
class FGsCraftTab;
class FGsItem;
struct FGsSchemaItemCommon;
struct FGsSchemaItemCraftMaterial;
struct FGsSchemaItemCraftRecipe;
struct FGsSchemaItemCraftRestrict;
struct FGsSchemaRewardItemBagData;
struct FSoftObjectPath;

/**
 * 제작 대상
 */
class FGsCraftTarget final
{
public:
	const FGsSchemaItemCraftRecipe* _tableData = nullptr;

	const FGsSchemaItemCraftRestrict* _tableDataRestrict = nullptr;

	const FGsCraftTab* _craftTab = nullptr;

	const CraftId _craftTargetId = INVALID_CRAFT_ID;

private:
	const FDateRange _displayUtcRange;

	/** 현재 서버에 제작된 총 갯수 */
	int32 _limitCountServerCrafted = 0;

	/** 현재 서버 계정에 제작된 총 갯수 */
	int32 _limitCountAccountByServerCrafted = 0;

	mutable TOptional<float> _ratioSuccess;

	mutable float _ratioGreatSuccess = 0.f;

	mutable TMap<CraftResult, FGsCraftTargetObject> _craftTargetObjects;

	TSharedRef<const FGsCostPackage> _costPackage;

public:
    FGsCraftTarget(const FGsSchemaItemCraftRecipe* InTableData, const FGsSchemaItemCraftRestrict* InTableDataRestrict);

public:
	CraftTabId GetCraftTabId() const;

	FText GetCraftName() const;

	/** 현재 출력 가능한 제작 대상인지 여부를 반환. */
	bool CanDisplay() const;

	/** 언락되었는지 여부 */
	bool IsCraftUnlocked(OUT FText* OutLockMessage = nullptr) const;

	/** 현재 제작 가능한 지 여부를 반환. */
	bool CanCraftNow(OUT FText* OutFailMessage = nullptr) const;

	/** 재료의 종류 수 */
	int32 GetMaterialNum() const;

	/** 성공 확률. 1.f 가 100% */
	float GetSuccessRatio() const;

	/** 대성공 확률. 1.f 가 100% */
	float GetGreatSuccessRatio() const;

	/** 실패 가능한 지 여부. */
	bool CanBeFail() const;

	/** 성공이 가능한 지 여부 */
	bool CanBeSuccess() const;

	/** 대성공이 가능한 지 여부 */
	bool CanBeGreatSuccess() const;

	/** 현재 플레이어가 제작 가능한 최대 갯수 */
	int32 GetPlayerCanCraftMaxCount() const;

	/************************************************************************/
	/* 갯수 제한                                                            */
	/************************************************************************/

public:
	bool HasAnyLimitCount() const;

	/** 서버 제작 제한 갯수. 음수면 제한 없음. */
	int32 GetLimitCountServer() const;

	/** 서버 제작 현재 갯수 */
	int32 GetLimitCountServerCrafted() const;

	void SetLimitCountServerCrafted(const int32 InValue);

	/** 서버별 계정 제작 제한 갯수. 음수면 제한 없음. */
	int32 GetLimitCountAccountByServer() const;

	/** 서버별 계정 제작 현재 갯수 */
	int32 GetLimitCountAccountByServerCrafted() const;

	void SetLimitCountAccountByServerCrafted(const int32 InValue);

	ResetTimeTicket GetLimitCountResetTimeTicket() const;

	/************************************************************************/
	/* 노출 제한                                                            */
	/************************************************************************/

public:
	FString GetDisplayEndLocalString() const;
	FString GetDisplayEndUtcString() const;
	const FText GetDisplayEndUtcText() const;

	bool HasDisplayUtcRange() const;
	bool IsEndTimeMagicTime() const;

	/************************************************************************/
	/* 제작 대상 오브젝트                                                   */
	/************************************************************************/

public:
	const FGsCraftTargetObject* GetCraftTargetObjectByResult(const CraftResult InCraftResult) const;

	const FGsCraftTargetObject* GetDefaultCraftTargetObject() const;

	FSoftObjectPath GetDefaultCraftTargetObjectIconPath() const;

private:
	const TArray<FGsSchemaRewardItemBagData>* GetRewardItemBagDatas() const;

	/************************************************************************/
	/* 즐겨찾기                                                             */
	/************************************************************************/

public:
	bool IsFavorite() const;

	/************************************************************************/
	/* cost package                                                         */
	/************************************************************************/

	TSharedRef<const FGsCostPackage> GetCostPackage() const;

	TSharedRef<FGsCostPackageMultiple> MakeCostPackageMultiple() const;

	TSharedRef<FGsCostPackage> CreateCostPackage() const;

private:
	static TSharedRef<FGsCostPackage> MakeCostPackage(const CraftId InCraftRecipeId);

	static const FGsCraftTab* MakeCraftTab(const CraftTabId InCraftTabId);

	static FDateRange MakeUtcRange(const FGsSchemaItemCraftRestrict* InTableDataRestrict);

	static void FillSuccessRatio(const TArray<FGsSchemaRewardItemBagData>* InRewardItemBagDatas, OUT TOptional<float>& OutSuccessRatio, OUT float& OutGreatSuccessRatio);

};