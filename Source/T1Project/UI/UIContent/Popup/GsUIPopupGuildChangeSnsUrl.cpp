#include "GsUIPopupGuildChangeSnsUrl.h"

#include "Runtime/UMG/Public/Components/EditableTextBox.h"

#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "SlateTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

const FName UGsUIPopupGuildChangeSnsUrl::WIDGET_KEY = TEXT("PopupGuildChangeSnsUrl");

void UGsUIPopupGuildChangeSnsUrl::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const FGsGuildManager* GuildManager = GGuild();
	if (nullptr == GuildManager)
	{
		GSLOG(Error, TEXT("nullptr == GuildManager"));
		return;
	}
	else
	{
		_guildSnsUrlMaxLength = GuildManager->GetSnsUrlMaxLength();
	}
	
	_btnOk->OnClicked.AddUniqueDynamic(this, &UGsUIPopupGuildChangeSnsUrl::OnClickOk);
	_btnCancel->OnClicked.AddUniqueDynamic(this, &UGsUIPopupGuildChangeSnsUrl::OnClickCancel);
	_inputTxt->OnTextChanged.AddUniqueDynamic(this, &UGsUIPopupGuildChangeSnsUrl::OnTextChanged);
	_inputTxt->OnTextCommitted.AddUniqueDynamic(this, &UGsUIPopupGuildChangeSnsUrl::OnTextCommitted);
}

void UGsUIPopupGuildChangeSnsUrl::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIPopupGuildChangeSnsUrl::NativeDestruct()
{
	_onClosePopup.ExecuteIfBound(_parameters.Get());

	Super::NativeDestruct();
}

void UGsUIPopupGuildChangeSnsUrl::SetParameters(TUniquePtr<Parameters> InParameters, FOnClosePopup InOnClosePopup)
{
	_onClosePopup = InOnClosePopup;

	_parameters = MoveTemp(InParameters);
	if (!_parameters.IsValid())
	{
		GSLOG(Error, TEXT("!_parameters.IsValid()"));
		return;
	}

	_inputTxt->SetText(FText::FromString(_parameters->_oldSnsLink));
	_inputTxt->SetFocus();
}

void UGsUIPopupGuildChangeSnsUrl::OnInputOk()
{
	if (!_parameters.IsValid())
	{
		GSLOG(Error, TEXT("!_parameters.IsValid()"));
	}
	else
	{
		_parameters->_newSnsLink = _inputTxt->GetText().ToString().TrimStartAndEnd();
		_parameters->_result = true;
	}

	Close();
}

void UGsUIPopupGuildChangeSnsUrl::OnInputCancel()
{
	if (!_parameters.IsValid())
	{
		GSLOG(Error, TEXT("!_parameters.IsValid()"));
	}
	else
	{
		_parameters->_result = false;
	}

	Close();
}

void UGsUIPopupGuildChangeSnsUrl::OnTextChanged(const FText& InText)
{
	const int32 TextLength = InText.ToString().Len();
	if (TextLength <= _guildSnsUrlMaxLength)
	{
		_inputTxt->WidgetStyle.SetForegroundColor(FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_ENOUGH_COST));
	}
	else
	{
		_inputTxt->WidgetStyle.SetForegroundColor(FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_NEED_COST));

		// 티커로 경고 출력
		if (TextLength > _guildSnsUrlMaxLength)
		{
			FText ErrorFormat;
			if (FText::FindText(TEXT("NetText"), TEXT("GUILD_SNS_MAX_LETTER_ERROR"), ErrorFormat))
			{
				FGsUIHelper::TrayMessageTicker(FText::Format(ErrorFormat, _guildSnsUrlMaxLength));
			}
		}
	}
}

void UGsUIPopupGuildChangeSnsUrl::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	// 제한 길이 넘을 경우, 뒷 문자 강제 삭제
	if (InText.ToString().Len() > _guildSnsUrlMaxLength)
	{
		_inputTxt->SetText(FText::FromString(InText.ToString().Left(_guildSnsUrlMaxLength)));

		// 티커로 경고 출력
		FText ErrorFormat;
		if (FText::FindText(TEXT("NetText"), TEXT("GUILD_SNS_MAX_LETTER_ERROR"), ErrorFormat))
		{
			FGsUIHelper::TrayMessageTicker(FText::Format(ErrorFormat, _guildSnsUrlMaxLength));
		}
	}
}

void UGsUIPopupGuildChangeSnsUrl::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupGuildChangeSnsUrl::OnClickCancel()
{
	OnInputCancel();
}
