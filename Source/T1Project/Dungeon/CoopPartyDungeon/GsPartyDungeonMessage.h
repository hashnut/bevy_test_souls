#pragma once

#include "CoreMinimal.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "Public/Shared/Client/SharedEnums/PD_ResultEnum.h"



class T1PROJECT_API FGsPartyDungeonMessage
{
	static void OnPopupMessage(PD::Result InResult)
	{
		// 2022/06/07 PKT - ¾È³» ÆË¾÷Ã¢À» ¶ç¿î´Ù.
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupOK"));
		if (!widget.IsValid())
		{
			GSLOG(Error, TEXT("!widget.IsValid()"));
			return;
		}

		if (UGsUIPopupOK* popup = Cast<UGsUIPopupOK>(widget.Get()))
		{
			FText message;
			FindTextByResult(InResult, message);
			popup->SetData(InMessage, nullptr);
		}
	}

	static bool FindTextByResult(const PD::Result InResult, FText& OutText)
	{
		OutText = FText::GetEmpty();

		switch (InResult)
		{
		default:
			break;
		}

		return (false == OutText.IsEmpty()) ? true : false;
	}
};