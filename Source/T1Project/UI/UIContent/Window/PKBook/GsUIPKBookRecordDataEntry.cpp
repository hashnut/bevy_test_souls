

#include "GsUIPKBookRecordDataEntry.h"
#include "PKBook/GsPKBookData.h"
#include "PKBook/GsPKRecordData.h"
#include "Management/ScopeGame/GsPKBookManager.h"
#include "PKBook/GsPKBookHelper.h"

#include "Image.h"
#include "TextBlock.h"
#include "PaperSprite.h"
#include "WidgetSwitcher.h"
#include "Classes/Engine/Texture2D.h"

#include "../../Helper/GsUIHelper.h"
#include "../../Common/RedDot/GsUIRedDotDefault.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UTIL/GsText.h"
#include "../T1Project.h"




void UGsUIPKBookRecordDataEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	_btnRecordBox->OnClicked.AddDynamic(this, &UGsUIPKBookRecordDataEntry::OnClicked);
}


void UGsUIPKBookRecordDataEntry::BeginDestroy()
{
	if (_btnRecordBox)
	{
		_btnRecordBox->OnClicked.RemoveDynamic(this, &UGsUIPKBookRecordDataEntry::OnClicked);
		_btnRecordBox = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPKBookRecordDataEntry::SetData(const FGsPKRecordData* InData)
{
	if (nullptr == InData || INVALID_PK_RECORD_ID >= InData->GetId())
	{
		GSLOG(Error, TEXT("nullptr == InData || INVALID_PK_RECORD_ID >= InData->GetId()"));
		return;
	}

	_PKRecordId = InData->GetId();

	FText text;
	// 2022/09/22 PKT - 발생 시간
	FGsPKBookHelper::TextDate(InData->GetOccurrenceTime(), text);
	_textBlockDate->SetText(text);

	if (PKBookResult::KILL == InData->GetReason())
	{
		const static int32 SWITHCER_REASON_KILL_INDEX = 1;
		_switcherRecordReason->SetActiveWidgetIndex(SWITHCER_REASON_KILL_INDEX);
	}
	else if (PKBookResult::DIE == InData->GetReason())
	{
		const static int32 SWITHCER_REASON_DIE_INDEX = 0;
		_switcherRecordReason->SetActiveWidgetIndex(SWITHCER_REASON_DIE_INDEX);
	}
	else if (PKBookResult::ASSIST == InData->GetReason())
	{
		const static int32 SWITHCER_REASON_ASSIST_INDEX = 2;
		_switcherRecordReason->SetActiveWidgetIndex(SWITHCER_REASON_ASSIST_INDEX);
	}
	
	// 2022/09/22 PKT - 발생 지역
	FGsPKBookHelper::TextMapName(InData->GetOccurrenceMapId(), text);
	_textBlockMapName->SetText(text);

	// 2022/09/22 PKT - main target 정보
	const FGsPKRecordingContributorData& mainTarget = InData->GetMainContributorData();

	// 2022/09/22 PKT - 메인 대상의 이름
	_textBlockName->SetText(FText::FromString(FGsTextUtil::CombineUserAndPrefix(mainTarget._name, mainTarget._homeWorldId)));

	_iconGuildEmblem->SetVisibility(ESlateVisibility::Hidden);
	_textBlockGuildName->SetVisibility(ESlateVisibility::Hidden);

	// 2022/09/26 PKT - Emblem Icon
	if (INVALID_GUILD_EMBLEM_ID != mainTarget._guildEmblemId)
	{	
		_iconGuildEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FSoftObjectPath path;
		if (FGsPKBookHelper::FindPathGuildIcon(mainTarget._guildEmblemId, true, path))
		{
			_iconGuildEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(path));
		}
	}

	// 2022/09/26 PKT - Guild Name
	if (false == mainTarget._guildName.IsEmpty())
	{
		_textBlockGuildName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_textBlockGuildName->SetText(mainTarget._guildName);
	}

	// 2022/09/22 PKT - RedDot
	FGsPKBookManager* PKBookManager = GsPKBook();
	int64 checkOutTime = (nullptr != PKBookManager) ? PKBookManager->GetCheckOutTime() : 0;
	_redDotNormal->SetRedDot(checkOutTime < StaticCast<int64>(InData->GetOccurrenceTime()));
}

void UGsUIPKBookRecordDataEntry::OnClicked()
{
	OnTryOpenPopUpDelegate.ExecuteIfBound(_PKRecordId);
}