#include "GsSupplyManager.h"

#include "Runtime/UMG/Public/Components/RichTextBlockDecorator.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"

#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/SupplyInfo/GsSchemaCurrencySupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaDropSupplyInfo.h"
#include "DataSchema/SupplyInfo/GsSchemaDropSupplyInfoCreatureIds.h"
#include "DataSchema/SupplyInfo/GsSchemaItemSupplyInfoSet.h"
#include "DataSchema/SupplyInfo/GsSchemaSkillSupplyInfoSet.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Message/GsMessageContents.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "SupplyHelper/GsBaseSupplyHelper.h"
#include "SupplyHelper/GsBMShopSupplyHelper.h"
#include "SupplyHelper/GsCraftSupplyHelper.h"
#include "SupplyHelper/GsDropSupplyHelper.h"
#include "SupplyHelper/GsTextSupplyHelper.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "UI/UIControlLib/TextDecorator/GsRichTextBlockLinkDecorator.h"
#include "Unlock/GsUnlockDefine.h"

FString LexToString(const ESupplyLinkActionType InSupplyLinkActionType)
{
	const UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESupplyLinkActionType"), true);
	return (nullptr != Enum) ? Enum->GetNameStringByValue(StaticCast<int64>(InSupplyLinkActionType)) : TEXT("");
}

void LexFromString(OUT ESupplyLinkActionType& OutSupplyLinkActionType, const TCHAR* InBuffer)
{
	const UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESupplyLinkActionType"), true);
	OutSupplyLinkActionType = StaticCast<ESupplyLinkActionType>((nullptr != Enum) ? Enum->GetValueByNameString(InBuffer) : 0LL);
}

const FString UGsSupplyManager::HYPERLINK_ID = TEXT("supply");
const FText UGsSupplyManager::HYPERLINK_DELIMETER = FText::FromString(TEXT("\n"));

const EGsUnlockContentsType UGsSupplyManager::HELPERS_ORDER[] = {
	EGsUnlockContentsType::WORLDMAP,
	EGsUnlockContentsType::CRAFT,
	EGsUnlockContentsType::BMSHOP,
};

const TMap<EGsUnlockContentsType, TSharedRef<FGsBaseSupplyHelper>> UGsSupplyManager::HELPERS = {
	{ EGsUnlockContentsType::WORLDMAP, MakeShared<FGsDropSupplyHelper>() },
	{ EGsUnlockContentsType::CRAFT, MakeShared<FGsCraftSupplyHelper>() },
	{ EGsUnlockContentsType::BMSHOP, MakeShared<FGsBMShopSupplyHelper>() },
};

void UGsSupplyManager::Initialize()
{
	_textSupplyHelper = MakeShared<FGsTextSupplyHelper>();
}

void UGsSupplyManager::RegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock)
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

	GsDecorator->AddUniqueOnClickDelegate(UGsSupplyManager::HYPERLINK_ID, UGsRichTextBlockLinkDecorator::FGsOnClickRichTextBlockLinkDecorator::CreateUObject(this, &UGsSupplyManager::OnClickHyperLink));
}

FText UGsSupplyManager::MakeRichTextCurrencyType(const CurrencyType InCurrencyType) const
{
	TArray<FText> Texts;

	const FGsSchemaCurrencySupplyInfoSet* CurrencySupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetCurrencyType(InCurrencyType);
	if (nullptr != CurrencySupplyInfoSet)
	{
		for (const EGsUnlockContentsType ContentsType : UGsSupplyManager::HELPERS_ORDER)
		{
			TSharedRef<FGsBaseSupplyHelper> SupplyHelper = UGsSupplyManager::HELPERS[ContentsType];

			const EGsNavigationStatus NavigationStatus = SupplyHelper->GetNavigationStatus(*CurrencySupplyInfoSet);
			const FText Title = SupplyHelper->GetTitleText();
			const FText Text = UGsSupplyManager::MakeRichText(NavigationStatus, Title, ContentsType, CostType::CURRENCY, InCurrencyType, INVALID_ITEM_ID);

			if (!Text.IsEmpty())
			{
				Texts.Add(Text);
			}
		}
	}

	if (_textSupplyHelper.IsValid())
	{
		const FText Text = _textSupplyHelper->GetTitleTextCurrencyType(InCurrencyType);
		if (!Text.IsEmpty())
		{
			Texts.Add(Text);
		}
	}

	return FText::Join(UGsSupplyManager::HYPERLINK_DELIMETER, Texts);
}

FText UGsSupplyManager::MakeRichTextItemId(const ItemId InItemId) const
{
	TArray<FText> Texts;

	const FGsSchemaItemSupplyInfoSet* SupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetItemId(InItemId);
	if (nullptr != SupplyInfoSet)
	{
		for (const EGsUnlockContentsType ContentsType : UGsSupplyManager::HELPERS_ORDER)
		{
			TSharedRef<FGsBaseSupplyHelper> SupplyHelper = UGsSupplyManager::HELPERS[ContentsType];

			const EGsNavigationStatus NavigationStatus = SupplyHelper->GetNavigationStatus(*SupplyInfoSet);
			const FText Title = SupplyHelper->GetTitleText();
			const FText Text = UGsSupplyManager::MakeRichText(NavigationStatus, Title, ContentsType, CostType::ITEM, CurrencyType::NONE, InItemId);

			if (!Text.IsEmpty())
			{
				Texts.Add(Text);
			}
		}
	}

	if (_textSupplyHelper.IsValid())
	{
		const FText Text = _textSupplyHelper->GetTitleTextItemId(InItemId);
		if (!Text.IsEmpty())
		{
			Texts.Emplace(Text);
		}
	}

	return FText::Join(UGsSupplyManager::HYPERLINK_DELIMETER, Texts);
}

FText UGsSupplyManager::MakeRichTextSkillId(const SkillId InUserSkillSetId) const
{
	TArray<FText> Texts;

	const FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetSkillId(InUserSkillSetId);
	if (nullptr != SupplyInfoSet)
	{
		for (const EGsUnlockContentsType ContentsType : UGsSupplyManager::HELPERS_ORDER)
		{
			TSharedRef<FGsBaseSupplyHelper> SupplyHelper = UGsSupplyManager::HELPERS[ContentsType];

			const EGsNavigationStatus NavigationStatus = SupplyHelper->GetNavigationStatus(*SupplyInfoSet);
			const FText Title = SupplyHelper->GetTitleText();
			const FText Text = UGsSupplyManager::MakeRichText(NavigationStatus, Title, ContentsType, InUserSkillSetId);

			if (!Text.IsEmpty())
			{
				Texts.Add(Text);
			}
		}
	}

	if (_textSupplyHelper.IsValid())
	{
		const FText Text = _textSupplyHelper->GetTitleTextSkillId(InUserSkillSetId);
		if (!Text.IsEmpty())
		{
			Texts.Emplace(Text);
		}
	}

	return FText::Join(UGsSupplyManager::HYPERLINK_DELIMETER, Texts);
}

bool UGsSupplyManager::TryToNavigateCurrencyType(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType, const bool InAlertOnFail /*= true*/) const
{
	const FGsSchemaCurrencySupplyInfoSet* SupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetCurrencyType(InCurrencyType);
	if (nullptr == SupplyInfoSet)
	{
		GSLOG(Error, TEXT("nullptr == SupplyInfoSet"));
		return false;
	}

	FText FailMessage;
	const bool Result = UGsSupplyManager::HELPERS[InContentsType]->TryToNavigate(*SupplyInfoSet, InAlertOnFail ? &FailMessage : nullptr);
	if (InAlertOnFail && !FailMessage.IsEmptyOrWhitespace())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FailMessage);
	}

	return Result;
}

bool UGsSupplyManager::TryToNavigateItemId(const EGsUnlockContentsType InContentsType, const ItemId InItemId, const bool InAlertOnFail /*= true*/) const
{
	const FGsSchemaItemSupplyInfoSet* SupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetItemId(InItemId);
	if (nullptr == SupplyInfoSet)
	{
		GSLOG(Error, TEXT("nullptr == SupplyInfoSet"));
		return false;
	}

	FText FailMessage;
	const bool Result = UGsSupplyManager::HELPERS[InContentsType]->TryToNavigate(*SupplyInfoSet, InAlertOnFail ? &FailMessage : nullptr);
	if (InAlertOnFail && !FailMessage.IsEmptyOrWhitespace())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FailMessage);
	}

	return Result;
}

bool UGsSupplyManager::TryToNavigateSkillId(const EGsUnlockContentsType InContentsType, const SkillId InSkillId, const bool InAlertOnFail /*= true*/) const
{
	const FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetSkillId(InSkillId);
	if (nullptr == SupplyInfoSet)
	{
		GSLOG(Error, TEXT("nullptr == SupplyInfoSet"));
		return false;
	}

	FText FailMessage;
	const bool Result = UGsSupplyManager::HELPERS[InContentsType]->TryToNavigate(*SupplyInfoSet, InAlertOnFail ? &FailMessage : nullptr);
	if (InAlertOnFail && !FailMessage.IsEmptyOrWhitespace())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FailMessage);
	}

	return Result;
}

UGsSupplyManager::EGsNavigationStatus UGsSupplyManager::GetNavigationStatus(const EGsUnlockContentsType InContentsType, const CurrencyType InCurrencyType) const
{
	const TSharedRef<FGsBaseSupplyHelper>* SupplyHelper = UGsSupplyManager::HELPERS.Find(InContentsType);
	if (nullptr == SupplyHelper)
	{
		return EGsNavigationStatus::None;
	}

	const FGsSchemaCurrencySupplyInfoSet* SupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetCurrencyType(InCurrencyType);
	if (nullptr == SupplyInfoSet)
	{
		return EGsNavigationStatus::None;
	}

	return (*SupplyHelper)->GetNavigationStatus(*SupplyInfoSet);
}

UGsSupplyManager::EGsNavigationStatus UGsSupplyManager::GetNavigationStatus(const EGsUnlockContentsType InContentsType, const ItemId InItemId) const
{
	const TSharedRef<FGsBaseSupplyHelper>* SupplyHelper = UGsSupplyManager::HELPERS.Find(InContentsType);
	if (nullptr == SupplyHelper)
	{
		return EGsNavigationStatus::None;
	}

	const FGsSchemaItemSupplyInfoSet* SupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetItemId(InItemId);
	if (nullptr == SupplyInfoSet)
	{
		return EGsNavigationStatus::None;
	}

	return (*SupplyHelper)->GetNavigationStatus(*SupplyInfoSet);
}

// 파라미터 키 이름
static constexpr TCHAR SUPPLY_KEY_ACTION_TYPE[] = TEXT("action");
static constexpr TCHAR SUPPLY_KEY_CONTENTS_TYPE[] = TEXT("contents");
static constexpr TCHAR SUPPLY_KEY_CURRENCYTYPE[] = TEXT("currency");
static constexpr TCHAR SUPPLY_KEY_ITEMID[] = TEXT("item");
static constexpr TCHAR SUPPLY_KEY_SKILLID[] = TEXT("skillid");

void UGsSupplyManager::OnClickHyperLink(const UGsRichTextBlockLinkDecorator::FGsOnClickParamMap& InParamMap) const
{
	const FString* ActionTypeStr = InParamMap.Find(SUPPLY_KEY_ACTION_TYPE);
	if (nullptr == ActionTypeStr)
	{
		GSLOG(Error, TEXT("nullptr == ActionTypeStr"));
		return;
	}

	ESupplyLinkActionType ActionType;
	LexFromString(ActionType, **ActionTypeStr);

	const FString* ContentsTypeStr = InParamMap.Find(SUPPLY_KEY_CONTENTS_TYPE);
	if (nullptr == ContentsTypeStr)
	{
		GSLOG(Error, TEXT("nullptr == ContentsTypeStr"));
		return;
	}

	// EGsUnlockContentsType 가 데이터센터 스키마로 변경되면 아래도 수정 가능.
	uint8 ContentsTypeNum;
	LexFromString(ContentsTypeNum, **ContentsTypeStr);
	const EGsUnlockContentsType ContentsType = StaticCast<EGsUnlockContentsType>(ContentsTypeNum);

	switch (ContentsType)
	{
	case EGsUnlockContentsType::WORLDMAP:
	{
		//침공전 공격자일 경우 몬스터 처치로 인해 월드맵 창 열리는 기능 예외
		bool isInvade = GGameData()->IsInvadeWorld();
		if (isInvade)
		{
			FText findText;
			if (FText::FindText(TEXT("WorldMapText"), TEXT("ErrorCannotmove"), findText))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
			}
			return;
		}
		break;
	}
	}

	if (ESupplyLinkActionType::Currency == ActionType)
	{
		const FString* CurrentCurrencyTypeStr = InParamMap.Find(SUPPLY_KEY_CURRENCYTYPE);
		if (nullptr == CurrentCurrencyTypeStr)
		{
			GSLOG(Error, TEXT("nullptr == CurrentCurrencyTypeStr"));
			return;
		}

		const CurrencyType CurrentCurrencyType = CurrencyTypeStringToEnum(*CurrentCurrencyTypeStr);
		TryToNavigateCurrencyType(ContentsType, CurrentCurrencyType);
	}
	else if (ESupplyLinkActionType::Item == ActionType)
	{
		const FString* CurrentItemIdStr = InParamMap.Find(SUPPLY_KEY_ITEMID);
		if (nullptr == CurrentItemIdStr)
		{
			GSLOG(Error, TEXT("nullptr == CurrentItemIdStr"));
			return;
		}

		ItemId CurrentItemId;
		LexFromString(CurrentItemId, **CurrentItemIdStr);
		TryToNavigateItemId(ContentsType, CurrentItemId);
	}
	else if (ESupplyLinkActionType::Skill == ActionType)
	{
		const FString* CurrentSkillIdStr = InParamMap.Find(SUPPLY_KEY_SKILLID);
		if (nullptr == CurrentSkillIdStr)
		{
			GSLOG(Error, TEXT("nullptr == CurrentSkillIdStr"));
			return;
		}

		SkillId CurrentSkillId;
		LexFromString(CurrentSkillId, **CurrentSkillIdStr);
		TryToNavigateSkillId(ContentsType, CurrentSkillId);
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, nullptr);
	}
}

FText UGsSupplyManager::MakeRichText(const EGsNavigationStatus InNavigationStatus, const FText& InTitle, const EGsUnlockContentsType InContentsType, const CostType InCostType, const CurrencyType InCurrencyType, const ItemId InItemId)
{
	FText RichText;

	if ((CostType::CURRENCY == InCostType && CurrencyType::NONE == InCurrencyType)
		|| (CostType::ITEM == InCostType && INVALID_ITEM_ID == InItemId)
		|| EGsNavigationStatus::None == InNavigationStatus
		|| InTitle.IsEmpty()
		)
	{
		return RichText;
	}

	switch (InNavigationStatus)
	{
	case EGsNavigationStatus::Displayable:
		RichText = InTitle;
		break;

	case EGsNavigationStatus::Movable:
	{
		TMap<FString, FString> ParamMap;

		if (CostType::CURRENCY == InCostType)
		{
			ParamMap.Emplace(SUPPLY_KEY_ACTION_TYPE, LexToString(ESupplyLinkActionType::Currency));
			ParamMap.Emplace(SUPPLY_KEY_CURRENCYTYPE, CurrencyTypeEnumToString(InCurrencyType));
		}
		else if (CostType::ITEM == InCostType)
		{
			ParamMap.Emplace(SUPPLY_KEY_ACTION_TYPE, LexToString(ESupplyLinkActionType::Item));
			ParamMap.Emplace(SUPPLY_KEY_ITEMID, LexToString(InItemId));
		}

		ParamMap.Emplace(SUPPLY_KEY_CONTENTS_TYPE, LexToString(StaticCast<uint8>(InContentsType)));

		RichText = UGsRichTextBlockLinkDecorator::MakeLinkText(UGsSupplyManager::HYPERLINK_ID, InTitle, ParamMap);
		break;
	}

	default:
		// do nothing. return empty text
		break;
	}

	return RichText;
}

FText UGsSupplyManager::MakeRichText(const EGsNavigationStatus InNavigationStatus, const FText& InTitle, const EGsUnlockContentsType InContentsType, const SkillId InUserSkillSetId)
{
	FText RichText;

	if (EGsNavigationStatus::None == InNavigationStatus || InTitle.IsEmpty())
	{
		return RichText;
	}

	switch (InNavigationStatus)
	{
	case EGsNavigationStatus::Displayable:
		RichText = InTitle;
		break;

	case EGsNavigationStatus::Movable:
	{
		TMap<FString, FString> ParamMap;
		ParamMap.Emplace(SUPPLY_KEY_ACTION_TYPE, LexToString(ESupplyLinkActionType::Skill));
		ParamMap.Emplace(SUPPLY_KEY_SKILLID, LexToString(InUserSkillSetId));
		ParamMap.Emplace(SUPPLY_KEY_CONTENTS_TYPE, LexToString(StaticCast<uint8>(InContentsType)));
		RichText = UGsRichTextBlockLinkDecorator::MakeLinkText(UGsSupplyManager::HYPERLINK_ID, InTitle, ParamMap);
		break;
	}

	default:
		// do nothing. return empty text
		break;
	}

	return RichText;
}

const FGsSchemaCurrencySupplyInfoSet* UGsSupplyManager::FindSupplyInfoSetCurrencyType(const CurrencyType InCurrencyType)
{
	const UGsTableCurrencySupplyInfoSet* CurrencySupplyInfoSetTable = StaticCast<const UGsTableCurrencySupplyInfoSet*>(FGsSchemaCurrencySupplyInfoSet::GetStaticTable());
	if (nullptr == CurrencySupplyInfoSetTable || CurrencyType::NONE == InCurrencyType)
	{
		GSLOG(Warning, TEXT("nullptr == CurrencySupplyInfoSetTable || CurrencyType::NONE == %s"), *CurrencyTypeEnumToString(InCurrencyType));
		return nullptr;
	}

	const FGsSchemaCurrencySupplyInfoSet* CurrencySupplyInfoSet = nullptr;
	CurrencySupplyInfoSetTable->FindRowById(InCurrencyType, CurrencySupplyInfoSet);
	return CurrencySupplyInfoSet;
}

const FGsSchemaItemSupplyInfoSet* UGsSupplyManager::FindSupplyInfoSetItemId(const ItemId InItemId)
{
	const UGsTable* ItemSupplyInfoSetTable = FGsSchemaItemSupplyInfoSet::GetStaticTable();
	if (nullptr == ItemSupplyInfoSetTable || INVALID_ITEM_ID == InItemId)
	{
		//GSLOG(Warning, TEXT("nullptr == ItemSupplyInfoSetTable || INVALID_ITEM_ID == %d"), InItemId);
		return nullptr;
	}

	const FName RowName(LexToString(InItemId));
	const FGsSchemaItemSupplyInfoSet* ItemSupplyInfoSet = nullptr;
	ItemSupplyInfoSetTable->FindRow(RowName, ItemSupplyInfoSet);
	return ItemSupplyInfoSet;
}

const FGsSchemaSkillSupplyInfoSet* UGsSupplyManager::FindSupplyInfoSetSkillId(const SkillId InUserSkillSetId)
{
	const UGsTable* SupplyInfoSetTable = FGsSchemaSkillSupplyInfoSet::GetStaticTable();
	if (nullptr == SupplyInfoSetTable || INVALID_SKILL_ID == InUserSkillSetId)
	{
		//GSLOG(Warning, TEXT("nullptr == SupplyInfoSetTable || INVALID_ITEM_ID == %d"), InItemId);
		return nullptr;
	}

	const FName RowName(LexToString(InUserSkillSetId));
	const FGsSchemaSkillSupplyInfoSet* SupplyInfoSet = nullptr;
	SupplyInfoSetTable->FindRow(RowName, SupplyInfoSet);
	return SupplyInfoSet;
}

template<>
const TSet<int32>* UGsSupplyManager::LookupSupplyInfoData(const ItemId InItemId) const
{
	const FGsSchemaItemSupplyInfoSet* ItemSupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetItemId(InItemId);
	return (nullptr != ItemSupplyInfoSet) ? &ItemSupplyInfoSet->craftIdSet : nullptr;
}

template<>
const TMap<int32, FGsSchemaDropSupplyInfoCreatureIds>* UGsSupplyManager::LookupSupplyInfoData(const ItemId InItemId) const
{
	const FGsSchemaItemSupplyInfoSet* ItemSupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetItemId(InItemId);
	return (nullptr != ItemSupplyInfoSet) ? &ItemSupplyInfoSet->mapCreatureSetMap : nullptr;
}

template<>
const TMap<int32, FGsSchemaDropSupplyInfo>* UGsSupplyManager::LookupSupplyInfoData(const ItemId InItemId) const
{
	const FGsSchemaItemSupplyInfoSet* ItemSupplyInfoSet = UGsSupplyManager::FindSupplyInfoSetItemId(InItemId);
	return (nullptr != ItemSupplyInfoSet) ? &ItemSupplyInfoSet->contentsEventMapCreatureSetMap : nullptr;
}
