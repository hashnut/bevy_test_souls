

#include "GsUIPKBookDetailContributorEntry.h"
#include "../PKBook/GsPKBookData.h"
#include "../PKBook/GsPKBookHelper.h"
#include "../Management/ScopeGame/GsPKBookManager.h"

#include "Image.h"
#include "TextBlock.h"
#include "PaperSprite.h"
#include "WidgetSwitcher.h"
#include "Classes/Engine/Texture2D.h"
#include "../../Helper/GsUIHelper.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"

#include "../../Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "DataSchema/PKBook/GsSchemaPKBookConfig.h"

#include "../Net/GsNetSendServiceWorld.h"

#include "../Party/GsPartyFunc.h"
#include "../UTIL/GsText.h"
#include "DataSchema/GameObject/GsSchemaWeaponUIInfo.h"
#include "T1Project.h"

void UGsUIPKBookDetailContributorEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSetDetectionTarget->OnClicked.AddDynamic(this, &UGsUIPKBookDetailContributorEntry::OnClickedSetDetectionTarget);
	_btnDetectionTargetClear->OnClicked.AddDynamic(this, &UGsUIPKBookDetailContributorEntry::OnClickedDetectionClear);
	_btnDetectionTargetAdd->OnClicked.AddDynamic(this, &UGsUIPKBookDetailContributorEntry::OnClickedDetectionAdd);
}

void UGsUIPKBookDetailContributorEntry::BeginDestroy()
{
	if (_btnDetectionTargetClear)
	{
		_btnDetectionTargetClear->OnClicked.RemoveDynamic(this, &UGsUIPKBookDetailContributorEntry::OnClickedDetectionClear);
		_btnDetectionTargetClear = nullptr;
	}

	if (_btnDetectionTargetAdd)
	{
		_btnDetectionTargetAdd->OnClicked.RemoveDynamic(this, &UGsUIPKBookDetailContributorEntry::OnClickedDetectionAdd);
		_btnDetectionTargetAdd = nullptr;
	}

	if (_btnSetDetectionTarget)
	{
		_btnSetDetectionTarget->OnClicked.RemoveDynamic(this, &UGsUIPKBookDetailContributorEntry::OnClickedSetDetectionTarget);
		_btnSetDetectionTarget = nullptr;
	}

	Super::BeginDestroy();
}

bool UGsUIPKBookDetailContributorEntry::IsSetDetectionTarget() const
{
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return false;
	}

	// 2022/09/28 PKT - 유저의 정보 이면서 자신을 공격한 대상에 대한 정보라면 경계 대상 을 설정 할 수가 있다.
	return (PKContributorType::USER == _data->_contributorType && PKBookResult::DIE == _recordReason);
}

void UGsUIPKBookDetailContributorEntry::SetSwitcherState(int32 InSwiterIndex)
{
	if (false == IsSetDetectionTarget())
	{	// 2022/10/12 PKT - NPC 정보나 내 자신에 대한 정보라면 경계 해제 / 추가를 할 수 없음. 무조건 데미지만 표기
		_switcherState->SetActiveWidgetIndex(SWITCHER_STATE_DAMEGE_INDEX);
		
		// 2022/10/12 PKT - Interaction 끔
		_switcherState->SetVisibility(ESlateVisibility::HitTestInvisible);

		// 2022/10/12 PKT - 경계 설정에 대한 버튼 인터렉션도 끔.
		_btnSetDetectionTarget->SetVisibility(ESlateVisibility::HitTestInvisible);

		return;
	}

	// 2022/10/12 PKT - 무조건 활성화
	_btnSetDetectionTarget->SetVisibility(ESlateVisibility::Visible);


	if (InSwiterIndex == SWITCHER_STATE_DAMEGE_INDEX)
	{	// 2022/10/12 PKT - 데미지 보기 설정이면..		
		_switcherState->SetActiveWidgetIndex(InSwiterIndex);
		// 2022/10/12 PKT - Interaction 끔
		_switcherState->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{	// 2022/10/12 PKT - Interaction 켬
		_switcherState->SetVisibility(ESlateVisibility::SelfHitTestInvisible);


		_switcherState->SetActiveWidgetIndex(InSwiterIndex);
	}
}

void UGsUIPKBookDetailContributorEntry::OnClickedSetDetectionTarget()
{
	if (false == IsSetDetectionTarget())
	{
		GSLOG(Error, TEXT("false == IsTargetPlayer()"));
		return;
	}

	if (SWITCHER_STATE_DAMEGE_INDEX == _switcherState->GetActiveWidgetIndex())
	{
		if (nullptr == GsPKBook()->FindDetectionTargetDataById(StaticCast<UserDBId>(_data->_contributorId)))
		{
			SetSwitcherState(SWITCHER_STATE_DETECTION_ADD);
		}
		else
		{
			SetSwitcherState(SWITCHER_STATE_DETECTION_CLEAR);
		}
	}
	else
	{
		SetSwitcherState(SWITCHER_STATE_DAMEGE_INDEX);
	}
}

void UGsUIPKBookDetailContributorEntry::OnClickedDetectionClear()
{
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;		
	}

	FGsNetSendServiceWorld::Send_ReqPKBookDeleteDetectionTarget(_data->_contributorId);	
}

void UGsUIPKBookDetailContributorEntry::OnClickedDetectionAdd()
{
	if (nullptr == _data)
	{
		GSLOG(Error, TEXT("nullptr == _data"));
		return;
	}

	FGsPKBookManager* PKBookManager = GsPKBook();
	if (nullptr == PKBookManager)
	{
		GSLOG(Error, TEXT("nullptr == PKBookManager"));
		return;
	}

	const FGsSchemaPKBookConfig* PKBookConfig = PKBookManager->GetConfig();
	if (nullptr == PKBookConfig)
	{
		GSLOG(Error, TEXT("nullptr == PKBookConfig"));
		return;
	}

	const int32 maxCount = PKBookConfig->maximumCountOfDetectionTarget;
	if (maxCount <= PKBookManager->GetDetectionTargetCountAll())
	{
		FText message;
		// 2022/10/07 PKT - "등록 최대 인원을 초과하여 경계대상을 추가할 수 없습니다."
		FGsPKBookHelper::FindText(TEXT("Error_Msg_DetectionTargetList_OverCount"), message);
		FGsUIHelper::TraySectionMessageDelayTicker(EGsNotiSection::EXTSYSTEM, message);
		return;
	}

	FGsNetSendServiceWorld::Send_ReqPKBookAddDetectionTarget(_data->_homeWorldId, _data->_contributorId);
}

void UGsUIPKBookDetailContributorEntry::SetData(PKBookResult InRecordReason, const struct FGsPKRecordingContributorData* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
		return;
	}

	_data = InData;
	
	// 2022/09/28 PKT - 기여자가 어떤 일에 대한 기여자인지에 대한 값(Kill / Die / Assist)
	_recordReason = InRecordReason;

	if (PKBookResult::KILL == InData->_reason)
	{
		const static int32 SWITHCER_REASON_KILL_INDEX = 1;
		_switcherRecordReason->SetActiveWidgetIndex(SWITHCER_REASON_KILL_INDEX);
	}
	else if (PKBookResult::DIE == InData->_reason)
	{
		const static int32 SWITHCER_REASON_DIE_INDEX = 0;
		_switcherRecordReason->SetActiveWidgetIndex(SWITHCER_REASON_DIE_INDEX);
	}
	else if (PKBookResult::ASSIST == InData->_reason)
	{
		const static int32 SWITHCER_REASON_ASSIST_INDEX = 2;
		_switcherRecordReason->SetActiveWidgetIndex(SWITHCER_REASON_ASSIST_INDEX);
	}

	// 2022/09/26 PKT - 이름
	_textBlockName->SetText(FText::FromString(FGsTextUtil::CombineUserAndPrefix(_data->_name, _data->_homeWorldId)));
	
	// 2022/09/27 PKT - Default
	_iconGuildEmblem->SetVisibility(ESlateVisibility::Hidden);
	_textBlockGuildName->SetVisibility(ESlateVisibility::Hidden);

	// 2022/09/26 PKT - 무기 / 길드 설정
	if (PKContributorType::USER == _data->_contributorType )
	{
		const static int32 SWITCHER_USER_INDEX = 0;
		_switcherContributorType->SetActiveWidgetIndex(SWITCHER_USER_INDEX);

		const FGsSchemaWeaponUIInfo* tableData = FGsPartyFunc::GetWeaponUIInfo(_data->_weaponType);
		if (tableData)
		{
			UPaperSprite* spriteImg = FGsUIHelper::LoadSpriteFromPath(tableData->_weaponTypeImagePath);
			if (spriteImg)
			{
				_imgWeapon->SetBrushFromAtlasInterface(spriteImg);
			}
			_imgWeapon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			_imgWeapon->SetVisibility(ESlateVisibility::Hidden);
		}
		
		if (INVALID_GUILD_EMBLEM_ID != _data->_guildEmblemId)
		{	// 2022/09/27 PKT - 길드 엠블럼
			FSoftObjectPath path;
			if (FGsPKBookHelper::FindPathGuildIcon(_data->_guildEmblemId, true, path))
			{
				_iconGuildEmblem->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_iconGuildEmblem->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(path));
			}
		}

		if (false == _data->_guildName.IsEmpty())
		{
			_textBlockGuildName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_textBlockGuildName->SetText(_data->_guildName);
		}
	}
	else
	{
		const static int32 SWITCHER_NPC_INDEX = 1;

		_switcherContributorType->SetActiveWidgetIndex(SWITCHER_NPC_INDEX);
	}

	FText format;
	FGsPKBookHelper::FindText(TEXT("Popup_Detail_Damege"), format);

	FText message = FText::Format(format, { _data->_damegeAmount });
	_textBlockDamege->SetText(message);

	// 2022/09/27 PKT - 
	SetSwitcherState(SWITCHER_STATE_DAMEGE_INDEX);
}

void UGsUIPKBookDetailContributorEntry::Invalidate()
{
	if (false == IsSetDetectionTarget())
	{	
		return;
	}

	// 2022/09/27 PKT - 초기 상태로 돌린다.
	_switcherState->SetActiveWidgetIndex(SWITCHER_STATE_DAMEGE_INDEX);
}