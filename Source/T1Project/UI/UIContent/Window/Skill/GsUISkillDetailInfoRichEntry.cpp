#include "GsUISkillDetailInfoRichEntry.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"
#include "T1Project.h"

void UGsUISkillDetailInfoRichEntry::NativeConstruct()
{
	Super::NativeConstruct();

	// 습득처 하이퍼링크 딜리게이트 등록
	UGsSupplyManager* SupplyManager = GSSupply();
	if (nullptr == SupplyManager || nullptr == _textBlockDesc)
	{
		GSLOG(Error, TEXT("nullptr == SupplyManager || nullptr == _textBlockDesc"));
	}
	else
	{
		SupplyManager->RegisterHyperLinkDelegate(_textBlockDesc);
	}
}

void UGsUISkillDetailInfoRichEntry::SetTitle(const FText& InText)
{
	_textBlockTitle->SetText(InText);
}

void UGsUISkillDetailInfoRichEntry::SetDesc(const FText& InText)
{
	_textBlockDesc->SetText(InText);
}

URichTextBlock* UGsUISkillDetailInfoRichEntry::GetDescRichTextBlock()
{
	return _textBlockDesc;
}
