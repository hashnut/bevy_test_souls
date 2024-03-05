// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildDiplomacyConfirm.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Guild/GsGuildDiplomacyData.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildEmblem.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsGuildMessageParam.h"
#include "Engine/Texture2D.h"


void UGsUIPopupGuildDiplomacyConfirm::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildDiplomacyConfirm::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildDiplomacyConfirm::OnClickCancel);
}

void UGsUIPopupGuildDiplomacyConfirm::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();
}

void UGsUIPopupGuildDiplomacyConfirm::NativeDestruct()
{
	UnbindMessages();

	_resultFunc = nullptr;
	_guildDBId = INVALID_GUILD_DB_ID;

	Super::NativeDestruct();
}

void UGsUIPopupGuildDiplomacyConfirm::OnInputOk()
{
	ProcessClose(true);
}

void UGsUIPopupGuildDiplomacyConfirm::OnInputCancel()
{
	ProcessClose(false);
}

void UGsUIPopupGuildDiplomacyConfirm::BindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();

	MGuild& msgGuild = messageMgr->GetGuild();
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIPLOMACY_ENEMY_STATE_NOTIFY,
		this, &UGsUIPopupGuildDiplomacyConfirm::OnEnemyStateNotify));
	_msgHandleGuildList.Emplace(msgGuild.AddUObject(MessageContentGuild::DIPLOMACY_ALLY_STATE_NOTIFY,
		this, &UGsUIPopupGuildDiplomacyConfirm::OnAllyStateNotify));
}

void UGsUIPopupGuildDiplomacyConfirm::UnbindMessages()
{
	FGsMessageHolder* messageMgr = GMessage();

	MGuild& msgGuild = messageMgr->GetGuild();
	for (MsgHandleGuild& handle : _msgHandleGuildList)
	{
		msgGuild.Remove(handle);
	}
	_msgHandleGuildList.Empty();
}

void UGsUIPopupGuildDiplomacyConfirm::SetData(const FGsGuildDiplomacyDataBase* InData, const FText& InMessage, 
	bool bIsAlly, TFunction<void(bool)> InFunc)
{
	_guildDBId = InData->_guildDBId;

	if (const FGsSchemaGuildEmblem* tableEmblem = FGsGuildHelper::GetGuildEmblem(InData->_emblemId))
	{
		// 큰 아이콘 사용
		UTexture2D* loadedTexture = FGsUIHelper::LoadTextureFromPath(tableEmblem->iconPathBig);
		if (loadedTexture)
		{
			_imgEmblem->SetBrushFromSoftTexture(loadedTexture);
		}
	}

	FText textGuildName;
	FGsGuildHelper::GetFormattedGuildName(InData->_guildName, InData->GetWorldId(), false, textGuildName);
	_tbGuildName->SetText(textGuildName);

	_tbGuildName->SetColorAndOpacity(bIsAlly ? 
		FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_ALLY_COLOR) : 
		FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_ENEMY_COLOR));

	_tbMasterName->SetText(FText::FromString(InData->_masterName));

	_tbMessage->SetText(InMessage);

	_resultFunc = InFunc;
}

void UGsUIPopupGuildDiplomacyConfirm::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupGuildDiplomacyConfirm::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildDiplomacyConfirm::OnEnemyStateNotify(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamDiplomacyNotify* param = InParam->Cast<const FGsGuildMsgParamDiplomacyNotify>())
	{
		if (param->_bIsDelete)
		{
			if (param->_guildDBId == _guildDBId)
			{
				// TEXT: 해당 길드의 상태가 변경되었습니다.
				FText textMsg;
				if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Changed_GuildState"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}

				ProcessClose(false);
			}
		}
	}
}

void UGsUIPopupGuildDiplomacyConfirm::OnAllyStateNotify(const IGsMessageParam* InParam)
{
	if (const FGsGuildMsgParamDiplomacyNotify* param = InParam->Cast<const FGsGuildMsgParamDiplomacyNotify>())
	{
		if (param->_bIsDelete)
		{
			if (param->_guildDBId == _guildDBId)
			{
				// TEXT: 해당 길드의 상태가 변경되었습니다.
				FText textMsg;
				if (FText::FindText(TEXT("GuildText"), TEXT("Alert_Diplomacy_Changed_GuildState"), textMsg))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}

				ProcessClose(false);
			}
		}
	}
}

void UGsUIPopupGuildDiplomacyConfirm::ProcessClose(bool bIsOk)
{
	Close();

	if (_resultFunc)
	{
		_resultFunc(bIsOk);
	}
}