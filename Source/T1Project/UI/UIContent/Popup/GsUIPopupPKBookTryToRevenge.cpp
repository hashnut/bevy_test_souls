
#include "GsUIPopupPKBookTryToRevenge.h"
#include "../PKBook/GsPKBookHelper.h"
#include "../PKBook/GsPKBookData.h"
#include "../Management/ScopeGame/GsPKBookManager.h"
#include "../Sequence/GsSequencePlayer.h"
#include "Image.h"
#include "TextBlock.h"
#include "PaperSprite.h"
#include "Classes/Engine/Texture2D.h"
#include "../Helper/GsUIHelper.h"
#include "../UTIL/GsText.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "../T1Project.h"




void UGsUIPopupPKBookTryToRevenge::TryToAction(PKRecordId InRecordId)
{
	if (INVALID_PK_RECORD_ID != InRecordId)
	{
		if (true == UGsSequencePlayer::IsSequencePlaying())
		{	// 2023/07/07 PKT - 미란차석님 요청사항 https://jira.com2us.com/jira/browse/C2URWQ-5782
			return;
		}
		
		FGsNetSendServiceWorld::Send_ReqPKBookGoForRevenge(InRecordId);
	}
	
	Super::TryToAction(InRecordId);
}

void UGsUIPopupPKBookTryToRevenge::SetData(PKRecordId InRecordId, const struct FGsPKRecordingContributorData* InData)
{
	Super::SetData(InRecordId, InData);

	if (INVALID_PK_RECORD_ID == _recordId)
	{
		GSLOG(Error, TEXT("INVALID_PK_RECORD_ID == _recordId"));
		return;
	}

	// 2022/09/28 PKT - Target Name
	_textBlockName->SetText(FText::FromString(FGsTextUtil::CombineUserAndPrefix(InData->_name, InData->_homeWorldId)));

	_iconGuildEmblem->SetVisibility(ESlateVisibility::Hidden);
	_textBlockGuildName->SetVisibility(ESlateVisibility::Hidden);

	// 2022/09/28 PKT - Target Guild Emblem
	if (INVALID_GUILD_EMBLEM_ID != InData->_guildEmblemId)
	{	// 2022/09/27 PKT - 길드 엠블럼
		FSoftObjectPath path;
		if (FGsPKBookHelper::FindPathGuildIcon(InData->_guildEmblemId, true, path))
		{
			_iconGuildEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_iconGuildEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(path));
		}
	}

	// 2022/09/28 PKT - Target Guild Name
	if (false == InData->_guildName.IsEmpty())
	{
		_textBlockGuildName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_textBlockGuildName->SetText(InData->_guildName);
	}
}