#include "Management/ScopeGame/GsUtilizingContentsManager.h"

#include "Runtime/UMG/Public/Components/RichTextBlockDecorator.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"

#include "Classes/GsSingleton.h"
#include "DataSchema/DemandInfo/GsSchemaCurrencyDemandInfoSet.h"
#include "DataSchema/DemandInfo/GsSchemaItemDemandInfoSet.h"
#include "DataSchema/UtilizeInfo/GsSchemaItemUtilizeInfoSet.h"
#include "DataSchema/UtilizeInfo/GsSchemaUtilizeMapInfo.h"
#include "DemandHelper/GsBaseDemandHelper.h"
#include "DemandHelper/GsBMShopDemandHelper.h"
#include "DemandHelper/GsCommonDemandHelper.h"
#include "DemandHelper/GsCraftDemandHelper.h"
#include "DemandHelper/GsTextDemandHelper.h"
#include "DemandHelper/GsWorldMapDemandHelper.h"
#include "GsSupplyManager.h"
#include "GsUnlockManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsCraftMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupItemDropNavigator.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "UI/UIControlLib/TextDecorator/GsRichTextBlockLinkDecorator.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "Unlock/GsUnlockDefine.h"

const FString FGsUtilizingContentsManager::HYPERLINK_ID = TEXT("demand");
const FText FGsUtilizingContentsManager::HYPERLINK_DELIMETER = FText::FromString(TEXT("\n"));

const EGsUnlockContentsType FGsUtilizingContentsManager::HELPERS_ORDER[] = {
	EGsUnlockContentsType::BMSHOP,
	EGsUnlockContentsType::SKILL,
	EGsUnlockContentsType::CRAFT,
	EGsUnlockContentsType::COSTUME,
	EGsUnlockContentsType::FAIRY,
	EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION,
	EGsUnlockContentsType::WORLDMAP,
	EGsUnlockContentsType::SEAL,
};

const TMap<EGsUnlockContentsType, TSharedRef<FGsBaseDemandHelper>> FGsUtilizingContentsManager::HELPERS = {
	{ EGsUnlockContentsType::BMSHOP, MakeShared<FGsBMShopDemandHelper>() },
	{ EGsUnlockContentsType::SKILL, MakeShared<FGsCommonDemandHelper>(EGsUnlockContentsType::SKILL) },
	{ EGsUnlockContentsType::CRAFT, MakeShared<FGsCraftDemandHelper>() },
	{ EGsUnlockContentsType::COSTUME, MakeShared<FGsCommonDemandHelper>(EGsUnlockContentsType::COSTUME) },
	{ EGsUnlockContentsType::FAIRY, MakeShared<FGsCommonDemandHelper>(EGsUnlockContentsType::FAIRY) },
	{ EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION, MakeShared<FGsCommonDemandHelper>(EGsUnlockContentsType::MONSTER_KNOWLEDGE_COLLECTION) },
	{ EGsUnlockContentsType::WORLDMAP, MakeShared<FGsWorldMapDemandHelper>() },
	{ EGsUnlockContentsType::SEAL, MakeShared<FGsCommonDemandHelper>(EGsUnlockContentsType::SEAL) },
};

FGsUtilizingContentsManager::FGsUtilizingContentsManager()
	: _textDemandHelper(MakeShared<FGsTextDemandHelper>())
{
}

void FGsUtilizingContentsManager::RegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock)
{
	if (nullptr == InRichTextBlock)
	{
		GSLOG(Warning, TEXT("nullptr == InRichTextBlock"));
		return;
	}

	URichTextBlockDecorator* Decorator = InRichTextBlock->GetDecoratorByClass(UGsRichTextBlockLinkDecorator::StaticClass());
	if (nullptr == Decorator)
	{
		GSLOG(Error, TEXT("nullptr == Decorator"));
		return;
	}

	UGsRichTextBlockLinkDecorator* GsDecorator = Cast<UGsRichTextBlockLinkDecorator>(Decorator);
	if (nullptr == GsDecorator)
	{
		GSLOG(Error, TEXT("nullptr == GsDecorator"));
		return;
	}

	GsDecorator->AddUniqueOnClickDelegate(FGsUtilizingContentsManager::HYPERLINK_ID, UGsRichTextBlockLinkDecorator::FGsOnClickRichTextBlockLinkDecorator::CreateRaw(this, &FGsUtilizingContentsManager::OnClickHyperLink));
}

FText FGsUtilizingContentsManager::MakeRichText(const CurrencyType InCurrencyType) const
{
	TArray<FText> Texts;

	const FGsSchemaCurrencyDemandInfoSet* DemandInfoSet = FGsUtilizingContentsManager::FindDemandInfoSet(InCurrencyType);
	if (nullptr != DemandInfoSet)
	{
		for (const EGsUnlockContentsType ContentsType : FGsUtilizingContentsManager::HELPERS_ORDER)
		{
			const TSharedRef<FGsBaseDemandHelper>& DemandHelper = FGsUtilizingContentsManager::HELPERS.FindRef(ContentsType);

			const UGsSupplyManager::EGsNavigationStatus NavigationStatus = DemandHelper->GetNavigationStatus(*DemandInfoSet);
			const FText Title = DemandHelper->GetTitleText(CostType::CURRENCY, InCurrencyType, INVALID_ITEM_ID);
			const FText Text = FGsUtilizingContentsManager::MakeRichText(NavigationStatus, Title, ContentsType, CostType::CURRENCY, InCurrencyType, INVALID_ITEM_ID);

			if (!Text.IsEmpty())
			{
				Texts.Add(Text);
			}
		}
	}

	const FText Text = _textDemandHelper->GetTitleText(CostType::CURRENCY, InCurrencyType, INVALID_ITEM_ID);
	if (!Text.IsEmpty())
	{
		Texts.Add(Text);
	}

	return FText::Join(FGsUtilizingContentsManager::HYPERLINK_DELIMETER, Texts);
}

FText FGsUtilizingContentsManager::MakeRichText(const ItemId InItemId) const
{
	TArray<FText> Texts;

	const FGsSchemaItemDemandInfoSet* ItemDemandInfoSet = FGsUtilizingContentsManager::FindDemandInfoSet(InItemId);
	if (nullptr != ItemDemandInfoSet)
	{
		for (const EGsUnlockContentsType ContentsType : FGsUtilizingContentsManager::HELPERS_ORDER)
		{
			const TSharedRef<FGsBaseDemandHelper>& DemandHelper = FGsUtilizingContentsManager::HELPERS.FindRef(ContentsType);

			const UGsSupplyManager::EGsNavigationStatus NavigationStatus = DemandHelper->GetNavigationStatus(*ItemDemandInfoSet);
			const FText Title = DemandHelper->GetTitleText(CostType::ITEM, CurrencyType::NONE, InItemId);
			const FText Text = FGsUtilizingContentsManager::MakeRichText(NavigationStatus, Title, ContentsType, CostType::ITEM, CurrencyType::NONE, InItemId);

			if (!Text.IsEmpty())
			{
				Texts.Add(Text);
			}
		}
	}

	const FText Text = _textDemandHelper->GetTitleText(CostType::ITEM, CurrencyType::NONE, InItemId);
	if (!Text.IsEmpty())
	{
		Texts.Add(Text);
	}

	return FText::Join(FGsUtilizingContentsManager::HYPERLINK_DELIMETER, Texts);
}

bool FGsUtilizingContentsManager::TryToNavigate(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType, const bool InAlertOnFail /*= true*/) const
{
	const FGsSchemaCurrencyDemandInfoSet* DemandInfoSet = FGsUtilizingContentsManager::FindDemandInfoSet(InCurrencyType);
	if (nullptr == DemandInfoSet)
	{
		GSLOG(Error, TEXT("nullptr == DemandInfoSet"));
		return false;
	}

	FText FailMessage;
	const bool Result = FGsUtilizingContentsManager::HELPERS[InContentsType]->TryToNavigate(*DemandInfoSet, InAlertOnFail ? &FailMessage : nullptr);
	if (InAlertOnFail && !FailMessage.IsEmptyOrWhitespace())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FailMessage);
	}

	return Result;
}

bool FGsUtilizingContentsManager::TryToNavigate(const EGsUnlockContentsType InContentsType, const ItemId InItemId, const bool InAlertOnFail /*= true*/) const
{
	const FGsSchemaItemDemandInfoSet* DemandInfoSet = FGsUtilizingContentsManager::FindDemandInfoSet(InItemId);
	if (nullptr == DemandInfoSet)
	{
		GSLOG(Error, TEXT("nullptr == DemandInfoSet"));
		return false;
	}

	FText FailMessage;
	const bool Result = FGsUtilizingContentsManager::HELPERS[InContentsType]->TryToNavigate(*DemandInfoSet, InAlertOnFail ? &FailMessage : nullptr);
	if (InAlertOnFail && !FailMessage.IsEmptyOrWhitespace())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FailMessage);
	}

	return Result;
}

UGsSupplyManager::EGsNavigationStatus FGsUtilizingContentsManager::GetNavigationStatus(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType) const
{
	const TSharedRef<FGsBaseDemandHelper>* DemandHelper = FGsUtilizingContentsManager::HELPERS.Find(InContentsType);
	if (nullptr == DemandHelper)
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	const FGsSchemaCurrencyDemandInfoSet* DemandInfoSet = FGsUtilizingContentsManager::FindDemandInfoSet(InCurrencyType);
	if (nullptr == DemandInfoSet)
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return (*DemandHelper)->GetNavigationStatus(*DemandInfoSet);
}

UGsSupplyManager::EGsNavigationStatus FGsUtilizingContentsManager::GetNavigationStatus(const EGsUnlockContentsType InContentsType, const ItemId InItemId) const
{
	const TSharedRef<FGsBaseDemandHelper>* DemandHelper = FGsUtilizingContentsManager::HELPERS.Find(InContentsType);
	if (nullptr == DemandHelper)
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	const FGsSchemaItemDemandInfoSet* DemandInfoSet = FGsUtilizingContentsManager::FindDemandInfoSet(InItemId);
	if (nullptr == DemandInfoSet)
	{
		return UGsSupplyManager::EGsNavigationStatus::None;
	}

	return (*DemandHelper)->GetNavigationStatus(*DemandInfoSet);
}

// 파라미터 키 이름
static constexpr TCHAR DEMAND_KEY_CONTENTS_TYPE[] = TEXT("ContentsType");
static constexpr TCHAR DEMAND_KEY_COSTTYPE[] = TEXT("CostType");
static constexpr TCHAR DEMAND_KEY_CURRENCYTYPE[] = TEXT("CurrencyType");
static constexpr TCHAR DEMAND_KEY_ITEMID[] = TEXT("ItemId");

void FGsUtilizingContentsManager::OnClickHyperLink(const UGsRichTextBlockLinkDecorator::FGsOnClickParamMap& InParamMap)
{
	const FString* ContentsTypeStr = InParamMap.Find(DEMAND_KEY_CONTENTS_TYPE);
	if (nullptr == ContentsTypeStr)
	{
		GSLOG(Error, TEXT("nullptr == ContentsTypeStr"));
		return;
	}
	
	// EGsUnlockContentsType 가 데이터센터 스키마로 변경되면 아래도 수정 가능.
	uint8 ContentsTypeNum;
	LexFromString(ContentsTypeNum, **ContentsTypeStr);
	const EGsUnlockContentsType ContentsType = StaticCast<EGsUnlockContentsType>(ContentsTypeNum);

	const FString* CurrentCostTypeStr = InParamMap.Find(DEMAND_KEY_COSTTYPE);
	if (nullptr == CurrentCostTypeStr)
	{
		GSLOG(Error, TEXT("nullptr == CurrentCostTypeStr"));
		return;
	}

	const CostType CurrentCostType = CostTypeStringToEnum(*CurrentCostTypeStr);
	if (CostType::CURRENCY == CurrentCostType)
	{
		const FString* CurrentCurrencyTypeStr = InParamMap.Find(DEMAND_KEY_CURRENCYTYPE);
		if (nullptr == CurrentCurrencyTypeStr)
		{
			GSLOG(Error, TEXT("nullptr == CurrentCurrencyTypeStr"));
			return;
		}

		const CurrencyType CurrentCurrencyType = CurrencyTypeStringToEnum(*CurrentCurrencyTypeStr);
		TryToNavigate(ContentsType, CurrentCurrencyType);
	}
	else if (CostType::ITEM == CurrentCostType)
	{
		const FString* CurrentItemIdStr = InParamMap.Find(DEMAND_KEY_ITEMID);
		if (nullptr == CurrentItemIdStr)
		{
			GSLOG(Error, TEXT("nullptr == CurrentItemIdStr"));
			return;
		}

		ItemId CurrentItemId;
		LexFromString(CurrentItemId, **CurrentItemIdStr);
		TryToNavigate(ContentsType, CurrentItemId);
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, nullptr);
	}
}

FText FGsUtilizingContentsManager::MakeRichText(const UGsSupplyManager::EGsNavigationStatus InNavigationStatus, const FText& InTitle, const EGsUnlockContentsType InContentsType, const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId)
{
	FText RichText;

	if ((CostType::CURRENCY == InCostType && CurrencyType::NONE == InCurrencyType)
		|| (CostType::ITEM == InCostType && INVALID_ITEM_ID == InItemId)
		|| UGsSupplyManager::EGsNavigationStatus::None == InNavigationStatus
		|| InTitle.IsEmpty()
		)
	{
		return RichText;
	}

	switch (InNavigationStatus)
	{
	case UGsSupplyManager::EGsNavigationStatus::Displayable:
		RichText = InTitle;
		break;

	case UGsSupplyManager::EGsNavigationStatus::Movable:
	{
		TMap<FString, FString> ParamMap = {
			// EGsUnlockContentsType 가 데이터센터 스키마로 변경되면 아래도 수정 가능.
			{ DEMAND_KEY_CONTENTS_TYPE, LexToString(StaticCast<uint8>(InContentsType)) },
			{ DEMAND_KEY_COSTTYPE, CostTypeEnumToString(InCostType) },
			{ DEMAND_KEY_CURRENCYTYPE, CurrencyTypeEnumToString(InCurrencyType) },
			{ DEMAND_KEY_ITEMID, LexToString(InItemId) },
		};
		RichText = UGsRichTextBlockLinkDecorator::MakeLinkText(FGsUtilizingContentsManager::HYPERLINK_ID, InTitle, ParamMap);

		break;
	}

	default:
		// do nothing. return empty text
		break;
	}

	return RichText;
}

const FGsSchemaCurrencyDemandInfoSet* FGsUtilizingContentsManager::FindDemandInfoSet(const CurrencyType InCurrencyType)
{
	const UGsTableCurrencyDemandInfoSet* CurrencyDemandInfoSetTable = Cast<UGsTableCurrencyDemandInfoSet>(FGsSchemaCurrencyDemandInfoSet::GetStaticTable());
	if (nullptr == CurrencyDemandInfoSetTable || CurrencyType::NONE == InCurrencyType)
	{
		GSLOG(Error, TEXT("nullptr == CurrencyDemandInfoSetTable || CurrencyType::NONE == %s"), *CurrencyTypeEnumToString(InCurrencyType));
		return nullptr;
	}

	const FGsSchemaCurrencyDemandInfoSet* CurrencyDemandInfoSet = nullptr;
	CurrencyDemandInfoSetTable->FindRowById(InCurrencyType, CurrencyDemandInfoSet);
	return CurrencyDemandInfoSet;
}

const FGsSchemaItemDemandInfoSet* FGsUtilizingContentsManager::FindDemandInfoSet(const ItemId InItemId)
{
	const UGsTableItemDemandInfoSet* ItemDemandInfoSetTable = Cast<UGsTableItemDemandInfoSet>(FGsSchemaItemDemandInfoSet::GetStaticTable());
	if (nullptr == ItemDemandInfoSetTable || INVALID_ITEM_ID == InItemId)
	{
		GSLOG(Error, TEXT("nullptr == ItemDemandInfoSetTable || INVALID_ITEM_ID == %d"), InItemId);
		return nullptr;
	}

	const FGsSchemaItemDemandInfoSet* ItemDemandInfoSet = nullptr;
	ItemDemandInfoSetTable->FindRowById(InItemId, ItemDemandInfoSet);
	return ItemDemandInfoSet;
}
