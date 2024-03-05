#pragma once

#include "Classes/GsManager.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/TextDecorator/GsRichTextBlockLinkDecorator.h"
#include "Unlock/GsUnlockDefine.h"

class FGsBaseDemandHelper;
class FGsTextDemandHelper;
class URichTextBlock;
struct FGsSchemaCurrencyDemandInfoSet;
struct FGsSchemaItemDemandInfoSet;

/**
 * 사용처 매니저
 */
class T1PROJECT_API FGsUtilizingContentsManager final : public IGsManager
{

private:
	static const FString HYPERLINK_ID;

public:
	static const FText HYPERLINK_DELIMETER;

private:
	/** 수요 헬퍼 순환 순서. 링크텍스트의 순서로도 사용함. */
	static const EGsUnlockContentsType HELPERS_ORDER[];
	static const TMap<EGsUnlockContentsType, TSharedRef<FGsBaseDemandHelper>> HELPERS;

private:
	TSharedRef<FGsTextDemandHelper> _textDemandHelper;

public:
	FGsUtilizingContentsManager();
	void RegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock);

	// 링크 텍스트 생성 FText MakeDemandRichText(CostType, CurrencyType, ItemId)
	FText MakeRichText(const CurrencyType InCurrencyType) const;
	FText MakeRichText(const ItemId InItemId) const;

	// 이동 시도
	bool TryToNavigate(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType, const bool InAlertOnFail = true) const;
	bool TryToNavigate(const EGsUnlockContentsType InContentsType, const ItemId InItemId, const bool InAlertOnFail = true) const;

	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType) const;
	UGsSupplyManager::EGsNavigationStatus GetNavigationStatus(const EGsUnlockContentsType InContentsType, const ItemId InItemId) const;

private:
	void OnClickHyperLink(const UGsRichTextBlockLinkDecorator::FGsOnClickParamMap& InParamMap);

private:
	static FText MakeRichText(const UGsSupplyManager::EGsNavigationStatus InNavigationStatus, const FText& InTitle, const EGsUnlockContentsType InContentsType, const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId);
	static const FGsSchemaCurrencyDemandInfoSet* FindDemandInfoSet(const CurrencyType InCurrencyType);
	static const FGsSchemaItemDemandInfoSet* FindDemandInfoSet(const ItemId InItemId);

};

#define GSUtilizingContents() UGsScopeHolder::GetGameManagerFType<FGsUtilizingContentsManager>(UGsScopeGame::EManagerType::UtilizingContents)
