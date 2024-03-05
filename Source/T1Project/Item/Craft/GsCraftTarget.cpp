#include "GsCraftTarget.h"

#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

#include "Currency/GsCostPackage.h"
#include "Currency/GsCostPackageMultiple.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftMaterial.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftRecipe.h"
#include "DataSchema/Item/Craft/GsSchemaItemCraftRestrict.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainChapter.h"
#include "DataSchema/Quest/Main/GsSchemaQuestMainStory.h"
#include "DataSchema/Reward/GsSchemaRewardBox.h"
#include "DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "GsCraftTab.h"
#include "GsCraftTargetObject.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Management/ScopeGame/GsCraftManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsValidatorChainManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Quest/Table/GsQuestMainTableCacheSet.h"
#include "Shared/Client/SharedEnums/PD_ResultEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UTIL/GsTimeSyncUtil.h"

/** rewardBoxList 의 인덱스로, 항상 이 값을 사용함. 이 RewardBox 의 ItemBagList 요소의 인덱스로 성공/대성공 등을 판단함 */
static constexpr int32 REWARD_BOX_INDEX = 0;

FGsCraftTarget::FGsCraftTarget(const FGsSchemaItemCraftRecipe* InTableData, const FGsSchemaItemCraftRestrict* InTableDataRestrict)
	: _tableData(InTableData)
	, _tableDataRestrict(InTableDataRestrict)
	, _craftTab(FGsCraftTarget::MakeCraftTab(_tableData->itemCraftTabId))
	, _craftTargetId(_tableData->itemCraftRecipeId)
	, _displayUtcRange(FGsCraftTarget::MakeUtcRange(_tableDataRestrict))
	, _costPackage(CreateCostPackage())
{
	check(!!_tableData);
	check(_craftTargetId != INVALID_CRAFT_ID);
	// GSLOG(Log, TEXT("FGsCraftTarget(), _displayUtcRange.IsEmpty():%d"), _displayUtcRange.IsEmpty());
}

CraftTabId FGsCraftTarget::GetCraftTabId() const
{
	return (nullptr != _craftTab) ? _craftTab->_craftTabId : INVALID_CRAFT_TAB_ID;
}

FText FGsCraftTarget::GetCraftName() const
{
	check(!!_tableData);
	return _tableData->recipeText;
}

bool FGsCraftTarget::CanDisplay() const
{
	const FDateTime& ServerUtcNow = FGsTimeSyncUtil::GetServerNowDateTimeUTC();

	//GSLOG(Log, TEXT("FGsCraftTarget::CanDisplay(), _craftTargetId:%d, !HasDisplayUtcRange():%d, ServerUtcNow:%s, _displayUtcRange.Contains(ServerUtcNow):%d"), _craftTargetId, !HasDisplayUtcRange(), *ServerUtcNow.ToString(), _displayUtcRange.Contains(ServerUtcNow));
	//GSLOG(Log, TEXT("_displayUtcRange.GetLowerBound().GetValue():%s, _displayUtcRange.GetLowerBound().IsClosed()%d, _displayUtcRange.GetLowerBound().IsExclusive():%d"), *_displayUtcRange.GetLowerBound().GetValue().ToString(), _displayUtcRange.GetLowerBound().IsClosed(), _displayUtcRange.GetLowerBound().IsExclusive());
	//GSLOG(Log, TEXT("_displayUtcRange.GetUpperBound().GetValue():%s, _displayUtcRange.GetUpperBound().IsClosed()%d, _displayUtcRange.GetUpperBound().IsExclusive():%d"), *_displayUtcRange.GetUpperBound().GetValue().ToString(), _displayUtcRange.GetUpperBound().IsClosed(), _displayUtcRange.GetUpperBound().IsExclusive());

	if (!HasDisplayUtcRange())
	{
		return true;
	}

	return _displayUtcRange.Contains(ServerUtcNow);
}

Level GetPlayerCharacterLevel()
{
	const FGsGameDataManager* GameDataManager = GGameData();
	check(GameDataManager);

	const FGsNetUserData* NetUserData = GameDataManager->GetUserData();
	if (nullptr == NetUserData)
	{
		GSLOG(Warning, TEXT("nullptr == NetUserData"));
		return INVALID_LEVEL;
	}

	return NetUserData->mLevel;
}

StoryId GetCurrentStoryId()
{
	const UGsQuestManager* QuestManager = GSQuest();
	check(!!QuestManager);

	const FGsQuestMainTableCacheSet* QuestMainTableCacheSet = QuestManager->GetQuestMainTableCache();
	if (nullptr == QuestMainTableCacheSet)
	{
		// 어떤 상태인지 알 수 없음.
		GSLOG(Error, TEXT("nullptr == QuestMainTableCacheSet"));
		return INVALID_STORY_ID;
	}

	const int32 StoryIndex = QuestMainTableCacheSet->GetStoryIndex();
	TSharedPtr<FGsQuestStoryTableCache> StoryTableCache = QuestMainTableCacheSet->GetQuestStoryTableCache(StoryIndex);

	return StoryTableCache->GetSchemaQuestStory()->id;
}

// TODO: 개선 필수
FText GetCurrentStoryName(const StoryId InStoryId)
{
	const UGsQuestManager* QuestManager = GSQuest();
	check(!!QuestManager);

	FText StoryName;

	const UGsTable* Table = FGsSchemaQuestMainStory::GetStaticTable();
	if (nullptr == Table)
	{
		GSLOG(Error, TEXT("nullptr == Table"));
	}
	else
	{
		const UGsTableQuestMainStory* TableQuestMainStory = Cast<UGsTableQuestMainStory>(Table);
		if (nullptr == TableQuestMainStory)
		{
			GSLOG(Error, TEXT("nullptr == TableQuestMainStory"));
		}
		else
		{
			const FGsSchemaQuestMainStory* QuestMainStory = nullptr;
			if (TableQuestMainStory->FindRowById(InStoryId, QuestMainStory) && nullptr != QuestMainStory)
			{
				StoryName = QuestMainStory->storyTitleText;
			}
		}
	}

	return StoryName;
}

bool FGsCraftTarget::IsCraftUnlocked(OUT FText* OutLockMessage /*= nullptr*/) const
{
	if (ItemCraftUnlockType::LEVEL == _tableData->unlockCondition)
	{
		const int32 PlayerCharacterLevel = StaticCast<int32>(GetPlayerCharacterLevel());
		if (INVALID_LEVEL == PlayerCharacterLevel)
		{
			GSLOG(Warning, TEXT("INVALID_LEVEL == PlayerCharacterLevel"));
			return false;
		}

		const bool Unlocked = _tableData->unlockValue <= PlayerCharacterLevel;
		if (!Unlocked && nullptr != OutLockMessage)
		{
			*OutLockMessage = FText::FromString(FString::Printf(TEXT("[로컬라이즈필요] 캐릭터 레벨 제한: %d"), _tableData->unlockValue));
		}

		return Unlocked;
	}
	else if (ItemCraftUnlockType::QUEST == _tableData->unlockCondition)
	{
		const int32 CurrentStoryId = StaticCast<int32>(GetCurrentStoryId());
		if (INVALID_STORY_ID == CurrentStoryId)
		{
			GSLOG(Warning, TEXT("INVALID_STORY_ID == CurrentStoryId"));
			return false;
		}

		const bool Unlocked = _tableData->unlockValue <= CurrentStoryId;
		if (!Unlocked && nullptr != OutLockMessage)
		{
			const FText StoryName = GetCurrentStoryName(CurrentStoryId);
			*OutLockMessage = FText::FromString(FString::Printf(TEXT("[로컬라이즈필요] 퀘스트 제한: %s"), *StoryName.ToString()));
		}

		return Unlocked;
	}

	if (nullptr != OutLockMessage)
	{
		*OutLockMessage = FText::GetEmpty();
	}

	return true;
}

bool FGsCraftTarget::CanCraftNow(OUT FText* OutFailMessage /*= nullptr*/) const
{
	PD::Result CurrentResult = PD::Result::RESULT_SUCCESS;

	if (!CanDisplay())
	{
		CurrentResult = PD::Result::CRAFT_ERROR_EXPIRE_DATE_ITEM_CRAFT;
	}

	if (PD::Result::RESULT_SUCCESS == CurrentResult && !IsCraftUnlocked(OutFailMessage))
	{
		CurrentResult = PD::Result::CRAFT_ERROR_NOT_MEET_THE_UNLOCK_CONDITION;
	}

	if (PD::Result::RESULT_SUCCESS == CurrentResult)
	{
		if (0 <= GetLimitCountServer() && GetLimitCountServer() <= GetLimitCountServerCrafted())
		{
			CurrentResult = PD::Result::CRAFT_ERROR_EXCEED_LIMIT_COUNT_SERVER;
		}
		else if (0 <= GetLimitCountAccountByServer() && GetLimitCountAccountByServer() <= GetLimitCountAccountByServerCrafted())
		{
			CurrentResult = PD::Result::CRAFT_ERROR_EXCEED_LIMIT_COUNT_ACCOUNT;
		}
	}

	if (nullptr != OutFailMessage && PD::Result::RESULT_SUCCESS != CurrentResult)
	{
		FText::FindText(TEXT("NetText"), PD::ResultEnumToString(CurrentResult), *OutFailMessage);

#if WITH_EDITOR
		// 로컬라이즈 안된 경우
		if (OutFailMessage->IsEmpty())
		{
			*OutFailMessage = FText::FromString(PD::ResultEnumToString(CurrentResult));
		}
#endif

	}

	return (PD::Result::RESULT_SUCCESS == CurrentResult);
}

int32 FGsCraftTarget::GetMaterialNum() const
{
	return _costPackage->GetCostElementsCount();
}

float FGsCraftTarget::GetSuccessRatio() const
{
	check(!!_tableData);

	if (!_ratioSuccess.IsSet())
	{
		FGsCraftTarget::FillSuccessRatio(GetRewardItemBagDatas(), _ratioSuccess, _ratioGreatSuccess);
	}

	return _ratioSuccess.GetValue();
}

float FGsCraftTarget::GetGreatSuccessRatio() const
{
	check(!!_tableData);

	if (!_ratioSuccess.IsSet())
	{
		FGsCraftTarget::FillSuccessRatio(GetRewardItemBagDatas(), _ratioSuccess, _ratioGreatSuccess);
	}

	return _ratioGreatSuccess;
}

bool FGsCraftTarget::CanBeFail() const
{
	return !FMath::IsNearlyEqual(GetSuccessRatio() + GetGreatSuccessRatio(), 1.f);
}

bool FGsCraftTarget::CanBeSuccess() const
{
	return !FMath::IsNearlyZero(GetSuccessRatio());
}

bool FGsCraftTarget::CanBeGreatSuccess() const
{
	return !FMath::IsNearlyZero(GetGreatSuccessRatio());
}

int32 FGsCraftTarget::GetPlayerCanCraftMaxCount() const
{
	// 플레이어 재화로 구매 가능한 갯수
	int32 PlayerCanCraftMaxCount = GetCostPackage()->GetPlayerCanUseMaxCount();

	// 서버 갯수 제한
	{
		const int32 LimitCount = GetLimitCountServer();
		if (0 <= LimitCount)
		{
			const int32 RemainCount = FMath::Clamp(LimitCount - GetLimitCountServerCrafted(), 0, LimitCount);
			if (PlayerCanCraftMaxCount > RemainCount)
			{
				PlayerCanCraftMaxCount = RemainCount;
			}
		}
	}

	// 서버 계정 갯수 제한
	{
		const int32 LimitCount = GetLimitCountAccountByServer();
		if (0 <= LimitCount)
		{
			const int32 RemainCount = FMath::Clamp(LimitCount - GetLimitCountAccountByServerCrafted(), 0, LimitCount);
			if (PlayerCanCraftMaxCount > RemainCount)
			{
				PlayerCanCraftMaxCount = RemainCount;
			}
		}
	}

	// hard cap
	return FMath::Clamp(PlayerCanCraftMaxCount, 0, StaticCast<int32>(CRAFT_MAX_COUNT));
}

bool FGsCraftTarget::HasAnyLimitCount() const
{
	if (nullptr == _tableDataRestrict)
	{
		return false;
	}

	return ItemCraftRestrict::NONE != _tableDataRestrict->limitCount1 || ItemCraftRestrict::NONE != _tableDataRestrict->limitCount2;
}

int32 FGsCraftTarget::GetLimitCountServer() const
{
	if (nullptr == _tableDataRestrict)
	{
		return -1;
	}

	if (ItemCraftRestrict::SERVER == _tableDataRestrict->limitCount1)
	{
		return _tableDataRestrict->limitCount1Amount;
	}
	else if (ItemCraftRestrict::SERVER == _tableDataRestrict->limitCount2)
	{
		return _tableDataRestrict->limitCount2Amount;
	}

	return -1;
}

int32 FGsCraftTarget::GetLimitCountServerCrafted() const
{
	return _limitCountServerCrafted;
}

void FGsCraftTarget::SetLimitCountServerCrafted(const int32 InValue)
{
	_limitCountServerCrafted = InValue;
	FGsCraftMessageParamOnChangeCraftTarget(_craftTargetId).Send();
}

int32 FGsCraftTarget::GetLimitCountAccountByServer() const
{
	if (nullptr == _tableDataRestrict)
	{
		return -1;
	}

	if (ItemCraftRestrict::ACCOUNT == _tableDataRestrict->limitCount1)
	{
		return _tableDataRestrict->limitCount1Amount;
	}
	else if (ItemCraftRestrict::ACCOUNT == _tableDataRestrict->limitCount2)
	{
		return _tableDataRestrict->limitCount2Amount;
	}

	return -1;
}

int32 FGsCraftTarget::GetLimitCountAccountByServerCrafted() const
{
	return _limitCountAccountByServerCrafted;
}

void FGsCraftTarget::SetLimitCountAccountByServerCrafted(const int32 InValue)
{
	_limitCountAccountByServerCrafted = InValue;
	FGsCraftMessageParamOnChangeCraftTarget(_craftTargetId).Send();
}

ResetTimeTicket FGsCraftTarget::GetLimitCountResetTimeTicket() const
{
	return (nullptr != _tableDataRestrict) ? _tableDataRestrict->limitCountResetTimeTicket : ResetTimeTicket::RTT_NONE;
}

FString FGsCraftTarget::GetDisplayEndLocalString() const
{
	if (!_displayUtcRange.HasUpperBound())
	{
		return FString();
	}

	FString DateTimeString;
	FGsTimeStringHelper::GetTimeStringNYMDHMS(FGsTimeSyncUtil::ConvertSeverDateTimeToClientDateTime(_displayUtcRange.GetUpperBoundValue()), DateTimeString);

	return DateTimeString;
}

FString FGsCraftTarget::GetDisplayEndUtcString() const
{
	if (!_displayUtcRange.HasUpperBound())
	{
		return FString();
	}

	FString DateTimeString;
	FGsTimeStringHelper::GetTimeStringNYMDHMS(_displayUtcRange.GetUpperBoundValue(), DateTimeString);

	return DateTimeString;
}

const FText FGsCraftTarget::GetDisplayEndUtcText() const
{
	return FText::FromString(GetDisplayEndUtcString());
}

bool FGsCraftTarget::HasDisplayUtcRange() const
{
	return !_displayUtcRange.IsEmpty();
}

bool FGsCraftTarget::IsEndTimeMagicTime() const
{
	return _displayUtcRange.GetUpperBoundValue() == FDateTime::MaxValue();
}

const FGsCraftTargetObject* FGsCraftTarget::GetCraftTargetObjectByResult(const CraftResult InCraftResult) const
{
	if (0 == _craftTargetObjects.Num())
	{
		const TArray<FGsSchemaRewardItemBagData>* ItemBagListPtr = GetRewardItemBagDatas();
		if (nullptr != ItemBagListPtr)
		{
			const TArray<FGsSchemaRewardItemBagData>& ItemBagList = *ItemBagListPtr;
			if (CRAFT_REWARD_SUCCESS_INDEX < ItemBagList.Num() && (ItemBagList[CRAFT_REWARD_SUCCESS_INDEX].itemId != INVALID_ITEM_ID || ItemBagList[CRAFT_REWARD_SUCCESS_INDEX].currencyType != CurrencyType::NONE))
			{
				_craftTargetObjects.Add(CraftResult::CRAFT_SUCCEEDED, FGsCraftTargetObject::MakeCraftTargetObject(ItemBagList[CRAFT_REWARD_SUCCESS_INDEX]));
			}
			else
			{
				GSLOG(Error, TEXT("DTGsCraftTaret 테이블 '%s' 의 보상데이터의 ItemBagList[%d] 를 입력해주세요."), *_tableData->GetRowName().ToString(), CRAFT_REWARD_SUCCESS_INDEX);
			}

			if (CRAFT_REWARD_GREAT_SUCCESS_INDEX < ItemBagList.Num() && (ItemBagList[CRAFT_REWARD_GREAT_SUCCESS_INDEX].itemId != INVALID_ITEM_ID || ItemBagList[CRAFT_REWARD_GREAT_SUCCESS_INDEX].currencyType != CurrencyType::NONE))
			{
				_craftTargetObjects.Add(CraftResult::CRAFT_SUCCEEDED_GREAT, FGsCraftTargetObject::MakeCraftTargetObject(ItemBagList[CRAFT_REWARD_GREAT_SUCCESS_INDEX]));
			}
			else
			{
				//GSLOG(Log, TEXT("NOT CRAFT_REWARD_GREAT_SUCCESS_INDEX < ItemBagList.Num() && (ItemBagList[CRAFT_REWARD_GREAT_SUCCESS_INDEX].itemId != INVALID_ITEM_ID || ItemBagList[CRAFT_REWARD_GREAT_SUCCESS_INDEX].currencyType != CurrencyType::NONE), RowName:%s"), *_tableData->GetRowName().ToString());
			}

			if (CRAFT_REWARD_FAIL_INDEX < ItemBagList.Num() && (ItemBagList[CRAFT_REWARD_FAIL_INDEX].itemId != INVALID_ITEM_ID || ItemBagList[CRAFT_REWARD_FAIL_INDEX].currencyType != CurrencyType::NONE))
			{
				_craftTargetObjects.Add(CraftResult::CRAFT_FAIL, FGsCraftTargetObject::MakeCraftTargetObject(ItemBagList[CRAFT_REWARD_FAIL_INDEX]));
			}
			else
			{
				//GSLOG(Log, TEXT("NOT CRAFT_REWARD_FAIL_INDEX < ItemBagList.Num() && (ItemBagList[CRAFT_REWARD_FAIL_INDEX].itemId != INVALID_ITEM_ID || ItemBagList[CRAFT_REWARD_FAIL_INDEX].currencyType != CurrencyType::NONE), RowName:%s"), *_tableData->GetRowName().ToString());
			}
		}
	}

	return _craftTargetObjects.Find(InCraftResult);
}

const FGsCraftTargetObject* FGsCraftTarget::GetDefaultCraftTargetObject() const
{
	const FGsCraftTargetObject* CraftTargetObject = GetCraftTargetObjectByResult(CraftResult::CRAFT_SUCCEEDED);
	if (nullptr == CraftTargetObject)
	{
		GSLOG(Error, TEXT("nullptr == CraftTargetObject"));
	}
	return CraftTargetObject;
}

FSoftObjectPath FGsCraftTarget::GetDefaultCraftTargetObjectIconPath() const
{
	const FGsCraftTargetObject* CraftTargetObject = GetDefaultCraftTargetObject();
	if (nullptr == CraftTargetObject)
	{
		GSLOG(Warning, TEXT("nullptr == CraftTargetObject, _tableData->itemCraftRecipeId:%d"), (nullptr != _tableData ? _tableData->itemCraftRecipeId : 0))
		return FSoftObjectPath();
	}

	return CraftTargetObject->GetCommonIconPath();
}

const TArray<FGsSchemaRewardItemBagData>* FGsCraftTarget::GetRewardItemBagDatas() const
{
	check(!!_tableData);

	const UGsTableRewardData* RewardDataTable = Cast<UGsTableRewardData>(FGsSchemaRewardData::GetStaticTable());
	if (nullptr == RewardDataTable)
	{
		GSLOG(Error, TEXT("nullptr == RewardDataTable"));
		return nullptr;
	}

	const FGsSchemaRewardData* RewardData = nullptr;
	if (RewardDataTable->FindRowById(_tableData->rewardDataId, RewardData) && nullptr != RewardData)
	{
		if (REWARD_BOX_INDEX < RewardData->rewardBoxList.Num())
		{
			if (const FGsSchemaRewardBoxData* RewardBoxData = RewardData->rewardBoxList[REWARD_BOX_INDEX].rewardBoxDataRow.GetRow())
			{
				return &RewardBoxData->itemBagList;
			}
		}
	}

	GSLOG(Error, TEXT("DTGsCraftTaret 테이블 '%s' 에 보상데이터를 입력해주세요."), *_tableData->GetRowName().ToString());
	return nullptr;
}

bool FGsCraftTarget::IsFavorite() const
{
	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return false;
	}

	return CraftManager->IsFavorite(_craftTargetId);
}

TSharedRef<const FGsCostPackage> FGsCraftTarget::GetCostPackage() const
{
	return _costPackage;
}

TSharedRef<FGsCostPackageMultiple> FGsCraftTarget::MakeCostPackageMultiple() const
{
	return GetCostPackage()->MakeCostPackageMultiple();
}

TSharedRef<FGsCostPackage> FGsCraftTarget::CreateCostPackage() const
{
	return FGsCraftTarget::MakeCostPackage(_craftTargetId);
}

TSharedRef<FGsCostPackage> FGsCraftTarget::MakeCostPackage(const CraftId InCraftRecipeId)
{
	TSharedRef<FGsCostPackage> CostPackage = MakeShared<FGsCostPackage>(ECachedValueItemDelegateType::CONSUMABLE_INVEN_DEPOT);
	CostPackage->SetJointCostCandidateElements(true);

	const UGsTableItemCraftRecipe* TableItemCraftRecipe = Cast<UGsTableItemCraftRecipe>(FGsSchemaItemCraftRecipe::GetStaticTable());
	if (nullptr == TableItemCraftRecipe)
	{
		GSLOG(Error, TEXT("nullptr == TableItemCraftRecipe"));
		return MoveTemp(CostPackage);
	}

	const FGsSchemaItemCraftRecipe* ItemCraftRecipe = nullptr;
	if (!TableItemCraftRecipe->FindRowById(InCraftRecipeId, ItemCraftRecipe))
	{
		GSLOG(Error, TEXT("nullptr == ItemCraftRecipe, InCraftRecipeId:%d"), InCraftRecipeId);
		return MoveTemp(CostPackage);
	}

	const UGsTableItemCraftMaterial* TableItemCraftMaterial = Cast<UGsTableItemCraftMaterial>(FGsSchemaItemCraftMaterial::GetStaticTable());
	if (nullptr == TableItemCraftMaterial)
	{
		GSLOG(Error, TEXT("nullptr == TableItemCraftMaterial"));
		return MoveTemp(CostPackage);
	}

	// 재화 비용 추가
	if (CurrencyType::NONE != ItemCraftRecipe->currencyType)
	{
		CostPackage->AddCost(ItemCraftRecipe->currencyType, ItemCraftRecipe->currencyAmount);
	}

	// 아이템 비용 추가
	{

		const TArray<TTuple<int32, int32>> CraftMaterialRawDatas = {
			MakeTuple(ItemCraftRecipe->material1, ItemCraftRecipe->material1Amount),
			MakeTuple(ItemCraftRecipe->material2, ItemCraftRecipe->material2Amount),
			MakeTuple(ItemCraftRecipe->material3, ItemCraftRecipe->material3Amount),
			MakeTuple(ItemCraftRecipe->material4, ItemCraftRecipe->material4Amount),
			MakeTuple(ItemCraftRecipe->material5, ItemCraftRecipe->material5Amount),
			MakeTuple(ItemCraftRecipe->material6, ItemCraftRecipe->material6Amount),
			MakeTuple(ItemCraftRecipe->material7, ItemCraftRecipe->material7Amount),
			MakeTuple(ItemCraftRecipe->material8, ItemCraftRecipe->material8Amount),
			MakeTuple(ItemCraftRecipe->material9, ItemCraftRecipe->material9Amount),
			MakeTuple(ItemCraftRecipe->material10, ItemCraftRecipe->material10Amount),
		};

#define CRAFT_MATERIAL_ID_INDEX 0
#define CRAFT_MATERIAL_AMOUNT_INDEX 1

		for (const TTuple<int32, int32>& CraftMaterialRawData : CraftMaterialRawDatas)
		{
			const int32 MaterialId = CraftMaterialRawData.Get<CRAFT_MATERIAL_ID_INDEX>();
			const ItemAmount MaterialAmount = StaticCast<ItemAmount>(CraftMaterialRawData.Get<CRAFT_MATERIAL_AMOUNT_INDEX>());
			if (0 == MaterialId)
			{
				// 재료 지정되지 않은 경우 스킵
				continue;
			}

			if (0 == MaterialAmount)
			{
				GSLOG(Warning, TEXT("0 == MaterialAmount, 재료 지정되어 있으나 갯수가 0 개임, MaterialId:%d"), MaterialId);
				continue;
			}

			const FGsSchemaItemCraftMaterial* ItemCraftMaterial = nullptr;
			if (!TableItemCraftMaterial->FindRowById(MaterialId, ItemCraftMaterial))
			{
				GSLOG(Error, TEXT("!TableItemCraftMaterial->FindRowById(%d, ItemCraftMaterial)"), MaterialId);
				continue;
			}

			TArray<TTuple<ItemId, Level>> CandidateMaterailItems;
			for (int32 i = 0; i < ItemCraftMaterial->itemIds.Num(); ++i)
			{
				const Level ItemMinLevel = ItemCraftMaterial->itemLevels.IsValidIndex(i) ? ItemCraftMaterial->itemLevels[i] : INVALID_LEVEL;
				CandidateMaterailItems.Emplace(MakeTuple(ItemCraftMaterial->itemIds[i], ItemMinLevel));
			}

			CostPackage->AddCostCandidates(CandidateMaterailItems, MaterialAmount, ItemCraftMaterial->includeItemGroup);
		}

#undef CRAFT_MATERIAL_ID_INDEX
#undef CRAFT_MATERIAL_AMOUNT_INDEX

	}

#if WITH_EDITOR
	if (0 == CostPackage->GetCostElementsCount())
	{
		GSLOG(Error, TEXT("[비용 정보 생성 실패] 제작 ID:%d 의 재료 목록을 확인해주세요."), InCraftRecipeId);
	}
#endif

	return MoveTemp(CostPackage);
}

const FGsCraftTab* FGsCraftTarget::MakeCraftTab(const CraftTabId InCraftTabId)
{
	const UGsCraftManager* CraftManager = GSCraft();
	if (nullptr == CraftManager)
	{
		GSLOG(Error, TEXT("nullptr == CraftManager"));
		return nullptr;
	}

	return CraftManager->GetTab(InCraftTabId);
}

FDateRange FGsCraftTarget::MakeUtcRange(const FGsSchemaItemCraftRestrict* InTableDataRestrict)
{
	FDateTime BeginUtc(0);
	if (nullptr != InTableDataRestrict && !InTableDataRestrict->displayBeginUtc.IsEmpty())
	{
		const FString& UtcBeginStr = InTableDataRestrict->displayBeginUtc;

		if (!FDateTime::Parse(*UtcBeginStr, BeginUtc))
		{
			GSLOG(Error, TEXT("!FDateTime::Parse('%s', BeginUtc)"), *UtcBeginStr);
		}
		else
		{
			// GSLOG(Log, TEXT("FDateTime::Parse('%s', %lld)"), *UtcBeginStr, BeginUtc.GetTicks());
		}
	}

	/** 종료 시간 값을 넣지 않았을 때, 매직 넘버를 넣어 시간이 상시 유지되도록 한다 */
	FDateTime EndUtc = FDateTime::MaxValue();
	if (nullptr != InTableDataRestrict && !InTableDataRestrict->displayEndUtc.IsEmpty())
	{
		const FString& UtcEndStr = InTableDataRestrict->displayEndUtc;
		if (!FDateTime::Parse(*UtcEndStr, EndUtc))
		{
			GSLOG(Error, TEXT("!FDateTime::Parse('%s', EndUtc)"), *UtcEndStr);
		}
		else
		{
			// GSLOG(Log, TEXT("FDateTime::Parse('%s', %lld)"), *UtcEndStr, EndUtc.GetTicks());
		}
	}

	return FDateRange(BeginUtc, EndUtc);
}

void FGsCraftTarget::FillSuccessRatio(const TArray<FGsSchemaRewardItemBagData>* InRewardItemBagDatas, OUT TOptional<float>& OutSuccessRatio, OUT float& OutGreatSuccessRatio)
{
	if (nullptr == InRewardItemBagDatas || 0 == InRewardItemBagDatas->Num())
	{
		// 제작 결과가 없음(데이터 오류)
		GSLOG(Error, TEXT("nullptr == InRewardItemBagDatas || 0 == InRewardItemBagDatas->Num()"));
		OutSuccessRatio = 0.f;
		OutGreatSuccessRatio = 0.f;
		return;
	}

	// 실패가 가능하므로 확률 계산
	int32 GreatSuccessProbability = 0;
	int32 SuccessProbability = 0;
	int32 TotalProbability = 0;
	for (int32 i = InRewardItemBagDatas->Num() - 1; 0 <= i; --i)
	{
		const FGsSchemaRewardItemBagData& RewardItemBagData = (*InRewardItemBagDatas)[i];
		if (CRAFT_REWARD_SUCCESS_INDEX == i)
		{
			SuccessProbability = RewardItemBagData.probability;
			TotalProbability += RewardItemBagData.probability;
		}
		else if (CRAFT_REWARD_GREAT_SUCCESS_INDEX == i)
		{
			GreatSuccessProbability = RewardItemBagData.probability;
			TotalProbability += RewardItemBagData.probability;
		}
		else if (CRAFT_REWARD_FAIL_INDEX == i)
		{
			TotalProbability += RewardItemBagData.probability;
		}
	}

	if (0 == TotalProbability)
	{
		GSLOG(Warning, TEXT("0 == TotalProbability"));
		OutSuccessRatio = 0.f;
		OutGreatSuccessRatio = 0.f;
		return;
	}

	OutSuccessRatio = SuccessProbability / (float)TotalProbability;
	OutGreatSuccessRatio = GreatSuccessProbability / (float)TotalProbability;
}
