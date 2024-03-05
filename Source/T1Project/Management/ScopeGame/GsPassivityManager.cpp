#include "GsPassivityManager.h"
#include "T1Project.h"

#include "Runtime/UMG/Public/Components/RichTextBlockDecorator.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeSkillNotifySet.h"

#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "UI/UIContent/Common/Item/GsUIItemDetailInfo.h"
#include "UI/UIControlLib/TextDecorator/GsRichTextBlockLinkDecorator.h"

const FString FGsPassivityManager::HYPERLINK_ID = TEXT("passivity");
const FText FGsPassivityManager::HYPERLINK_DELIMETER = FText::FromString(TEXT("\n"));

void FGsPassivityManager::RegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock, UObject* InOwner)
{
	if (nullptr == InRichTextBlock)
	{
		GSLOG(Warning, TEXT("nullptr == InRichTextBlock"));
		return;
	}

	if (nullptr == InOwner)
	{
		GSLOG(Warning, TEXT("nullptr == InOwner"));
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

	//GsDecorator->AddUniqueOnClickDelegateUObject(FGsPassivityManager::HYPERLINK_ID, UGsRichTextBlockLinkDecorator::FGsOnClickRichTextBlockLinkDecorator::CreateRaw(this, &FGsPassivityManager::OnClickHyperLink));
	GsDecorator->AddUniqueOnClickDelegateId(InOwner->GetUniqueID(), UGsRichTextBlockLinkDecorator::FGsOnClickRichTextBlockLinkDecoratorId::CreateRaw(this, &FGsPassivityManager::OnClickHyperLinkId));
	//GsDecorator->AddUniqueOnClickDelegateUObject(InOwner, UGsRichTextBlockLinkDecorator::FGsOnClickRichTextBlockLinkDecoratorUObject::CreateRaw(this, &FGsPassivityManager::OnClickHyperLink));
}

void FGsPassivityManager::UnRegisterHyperLinkDelegate(URichTextBlock* InRichTextBlock, UObject* InOwner)
{
	if (nullptr == InRichTextBlock)
	{
		GSLOG(Warning, TEXT("nullptr == InRichTextBlock"));
		return;
	}

	if (nullptr == InOwner)
	{
		GSLOG(Warning, TEXT("nullptr == InOwner"));
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

	GsDecorator->RemoveOnClickDelegate(InOwner->GetUniqueID());
	//GsDecorator->RemoveOnClickDelegateUObject(InOwner);
}

// 파라미터 키 이름
static constexpr TCHAR PASSIVITY_KEY_TITLE[] = TEXT("Title");
static constexpr TCHAR PASSIVITY_KEY_DESC[] = TEXT("Desc");
static constexpr TCHAR PASSIVITY_KEY_PASSIVE[] = TEXT("Passive");

void FGsPassivityManager::OnClickHyperLinkId(const UGsRichTextBlockLinkDecorator::FGsOnClickParamId& InParamIdPair)
{
	// InParamMap 에 들어 있는 스킬 관련 설명 뽑아내기
	const FString* titleString = InParamIdPair.Value.Find(PASSIVITY_KEY_TITLE);
	if (nullptr == titleString)
	{
		GSLOG(Error, TEXT("nullptr == titleString"));
		return;
	}

	const FString* descString = InParamIdPair.Value.Find(PASSIVITY_KEY_DESC);
	if (nullptr == descString)
	{
		GSLOG(Error, TEXT("nullptr == descString"));
		return;
	}

	bool isPassive = false;
	if (InParamIdPair.Value.Contains(PASSIVITY_KEY_PASSIVE))
	{
		isPassive = true;
	}

	// 1. 메시지 방식 : 메시지를 보내는 방식은 ItemDetailInfo 가 여러 개 열려 있을 때, 해당 RichTextBlock 이
	// 속해 있는 팝업 창에서의 툴팁을 제대로 띄워주기 위해 UniqueId 와 스트링을 함께 보낸다.
	uint32 uniqueId = InParamIdPair.Key;

	FGsItemDetailSkillInfoMessage itemDetailSkillInfo(uniqueId, *titleString, *descString, isPassive);

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetItemContentSkill().SendMessage(MessageContentItemSkill::DETAIL_OPEN_ITEM_SKILL, &itemDetailSkillInfo);
	}
}

void FGsPassivityManager::OnClickHyperLink(const UGsRichTextBlockLinkDecorator::FGsOnClickParamUObject& InParamUObjectPair)
{
	// InParamMap 에 들어 있는 스킬 관련 설명 뽑아내기
	const FString* titleString = InParamUObjectPair.Value.Find(PASSIVITY_KEY_TITLE);
	if (nullptr == titleString)
	{
		GSLOG(Error, TEXT("nullptr == titleString"));
		return;
	}

	const FString* descString = InParamUObjectPair.Value.Find(PASSIVITY_KEY_DESC);
	if (nullptr == descString)
	{
		GSLOG(Error, TEXT("nullptr == descString"));
		return;
	}

	bool isPassive = false;
	if (InParamUObjectPair.Value.Contains(PASSIVITY_KEY_PASSIVE))
	{
		isPassive = true;
	}

	// 2. UObject* 를 넘기는 방식 : PassivityManager 가 GsUIItemDetailInfo 와 결합되는 단점이 있다.
	// RichTextBlock 이 속해 있는 팝업에서 툴팁을 제대로 띄워줄 수 있음 (아이템 비교 창 같은 케이스 해결 가능)
	UGsUIItemDetailInfo* itemDetailInfo = Cast<UGsUIItemDetailInfo>(InParamUObjectPair.Key);
	if (nullptr == itemDetailInfo)
	{
		GSLOG(Error, TEXT("nullptr == itemDetailInfo"));
		return;
	}

	itemDetailInfo->SetWeaponSkillInfo(FText::FromString(*titleString),
		(FText::FromString(*descString)), isPassive);
}

FText FGsPassivityManager::MakeRichText(const FGsSchemaPassivitySet* passivitySet) const
{
	FText skillText(GetSkillText(passivitySet));

	// 스킬 효과 텍스트 채우기
	FText descText;
	descText = passivitySet->descText;

	// 링크 바로 연결
	FText RichText;
	TMap<FString, FString> ParamMap = {
		{PASSIVITY_KEY_TITLE, skillText.ToString()},
		{PASSIVITY_KEY_DESC, descText.ToString()}
	};

	if (IsSkillPassive(passivitySet))
	{
		ParamMap.Add(PASSIVITY_KEY_PASSIVE, FString());
	}

	RichText = UGsRichTextBlockLinkDecorator::MakeLinkText(FGsPassivityManager::HYPERLINK_ID, skillText, ParamMap);

	return RichText;
}

FText FGsPassivityManager::GetSkillText(const FGsSchemaPassivitySet* passivitySet) const
{
	FText skillText;

	bool isPassive = IsSkillPassive(passivitySet);

	// 발동형 스킬일 경우 스킬명에 확률을 추가
	if (false == isPassive)
	{
		// PassivityEffectInvokeSkillNotifySet 테이블 조회
		const UGsTablePassivityEffectInvokeSkillNotifySet* passivityEffectInvokeSkillNotifySetTable = Cast<UGsTablePassivityEffectInvokeSkillNotifySet>(FGsSchemaPassivityEffectInvokeSkillNotifySet::GetStaticTable());
		if (nullptr == passivityEffectInvokeSkillNotifySetTable)
		{
			GSLOG(Error, TEXT("nullptr == passivityEffectInvokeSkillNotifySetTable"));
			return FText::FromString("passivityEffectInvokeSkillNotifySetTable not found");
		}

		const FGsSchemaPassivityEffectInvokeSkillNotifySet* invokeSkill = nullptr;
		passivityEffectInvokeSkillNotifySetTable->FindRowById(passivitySet->id, invokeSkill);

		if (nullptr == invokeSkill)
		{
			GSLOG(Error, TEXT("nullptr == invokeSkill"));
			return FText::FromString("InvokeSkill not found");
		}

		int32 ratePct = invokeSkill->param.ratePct;

		skillText = FText::FromString(passivitySet->nameText.ToString() + " " + FString::FromInt(ratePct) + "%");
	}
	else
	{
		skillText = passivitySet->nameText;
	}

	return skillText;
}

bool FGsPassivityManager::IsSkillPassive(const FGsSchemaPassivitySet* passivitySet) const
{
	if (PassivityType::INVOKE_ABNORMALITY == passivitySet->type ||
		PassivityType::INVOKE_COOLDOWN == passivitySet->type ||
		PassivityType::INVOKE_SKILL_NOTIFY == passivitySet->type)
	{
		return false;
	}
	else
	{
		return true;
	}
}