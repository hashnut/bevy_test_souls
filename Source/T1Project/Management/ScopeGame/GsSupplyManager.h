#pragma once

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/SupplyInfo/GsSchemaCraftSupplyInfo.h"
#include "GsManager.h"
#include "Management/GsScopeGame.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/TextDecorator/GsRichTextBlockLinkDecorator.h"
#include "Unlock/GsUnlockDefine.h"

#include "GsSupplyManager.generated.h"

class FGsBaseSupplyHelper;
class FGsTextSupplyHelper;
class URichTextBlock;
struct FGsSchemaCurrencySupplyInfoSet;
struct FGsSchemaDropSupplyInfo;
struct FGsSchemaDropSupplyInfoCreatureIds;
struct FGsSchemaItemSupplyInfoSet;
struct FGsSchemaSkillSupplyInfoSet;

UENUM(BlueprintType)
enum class ESupplyLinkActionType : uint8
{
	Currency,
	Item,
	Skill,
};

FString LexToString(const ESupplyLinkActionType InSupplyLinkActionType);
void LexFromString(OUT ESupplyLinkActionType& OutSupplyLinkActionType, const TCHAR* InBuffer);

UCLASS()
class T1PROJECT_API UGsSupplyManager final :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

public:
	/**
	 * 바로가기 상태
	 */
	enum class EGsNavigationStatus : uint8
	{
		None,
		Displayable,
		Movable,
	};

private:
	static const FString HYPERLINK_ID;
	static const FText HYPERLINK_DELIMETER;

	/** 공급헬퍼 순환 순서. 링크텍스트의 순서로도 사용함. */
	static const EGsUnlockContentsType HELPERS_ORDER[];
	static const TMap<EGsUnlockContentsType, TSharedRef<FGsBaseSupplyHelper>> HELPERS;

private:
	TSharedPtr<FGsTextSupplyHelper> _textSupplyHelper;

public:
	void Initialize() final;
	void RegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock);

	// 링크 텍스트 생성 FText MakeSupplyRichText(CostType, CurrencyType, ItemId)
	FText MakeRichTextCurrencyType(const CurrencyType InCurrencyType) const;
	FText MakeRichTextItemId(const ItemId InItemId) const;
	FText MakeRichTextSkillId(const SkillId InUserSkillSetId) const;

	// 이동 시도 bool TryToNavigate(CostType, CurrencyType, ItemId, EGsUnlockContentsType)
	bool TryToNavigateCurrencyType(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType, const bool InAlertOnFail = true) const;
	bool TryToNavigateItemId(const EGsUnlockContentsType InContentsType, const ItemId InItemId, const bool InAlertOnFail = true) const;
	bool TryToNavigateSkillId(const EGsUnlockContentsType InContentsType, const SkillId InSkillId, const bool InAlertOnFail = true) const;

	EGsNavigationStatus GetNavigationStatus(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType) const;
	EGsNavigationStatus GetNavigationStatus(const EGsUnlockContentsType InContentsType, const ItemId InItemId) const;

private:
	void OnClickHyperLink(const UGsRichTextBlockLinkDecorator::FGsOnClickParamMap& InParamMap) const;

private:
	static FText MakeRichText(const EGsNavigationStatus InNavigationStatus, const FText& InTitle, const EGsUnlockContentsType InContentsType, const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId);
	static FText MakeRichText(const EGsNavigationStatus InNavigationStatus, const FText& InTitle, const EGsUnlockContentsType InContentsType, const SkillId InUserSkillSetId);

	static const FGsSchemaCurrencySupplyInfoSet* FindSupplyInfoSetCurrencyType(const CurrencyType InCurrencyType);
	static const FGsSchemaItemSupplyInfoSet* FindSupplyInfoSetItemId(const ItemId InItemId);
	static const FGsSchemaSkillSupplyInfoSet* FindSupplyInfoSetSkillId(const SkillId InUserSkillSetId);

public:
	/** 대상 정보로 공급처 테이블을 조회함. */
	template<typename TGsSchema>
	const TGsSchema* LookupSupplyInfoData(const ItemId InItemId) const
	{
		return nullptr;
	}
};

template<>
const TSet<int32>* UGsSupplyManager::LookupSupplyInfoData(const ItemId InItemId) const;

template<>
const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>* UGsSupplyManager::LookupSupplyInfoData(const ItemId InItemId) const;

template<>
const TMap<int32, FGsSchemaDropSupplyInfo>* UGsSupplyManager::LookupSupplyInfoData(const ItemId InItemId) const;

#define GSSupply() UGsScopeHolder::GetGameManager<UGsSupplyManager>(UGsScopeGame::EManagerType::Supply)
