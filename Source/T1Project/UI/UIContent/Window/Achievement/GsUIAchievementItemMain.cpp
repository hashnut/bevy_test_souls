#include "GsUIAchievementItemMain.h"
#include "Engine/AssetManager.h"

#include "Achievement/GsAchievementData.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "WidgetSwitcher.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Popup/GsUIPopupAchievementGroupList.h"

#include "Net/GsNetSendServiceWorld.h"

namespace FStringExt
{
	static FString FromInt64(int64 Num)
	{
		const TCHAR* DigitToChar = TEXT("9876543210123456789");
		constexpr int64 ZeroDigitIndex = 9;
		bool bIsNumberNegative = Num < 0;
		const int64 TempBufferSize = 24; // 16 is big enough
		TCHAR TempNum[TempBufferSize];
		int64 TempAt = TempBufferSize; // fill the temp string from the top down.

		// Convert to string assuming base ten.
		do
		{
			TempNum[--TempAt] = DigitToChar[ZeroDigitIndex + (Num % 10)];
			Num /= 10;
		} while (Num);

		if (bIsNumberNegative)
		{
			TempNum[--TempAt] = TEXT('-');
		}

		const TCHAR* CharPtr = TempNum + TempAt;
		const int64 NumChars = TempBufferSize - TempAt;

		FString Ret;
		Ret.Append(CharPtr, NumChars);
		return Ret;
	}

	static FString FormatAsNumber64(int64 InNumber)
	{
		FString Number = FromInt64(InNumber), Result;

		int64 dec = 0;
		for (int64 x = Number.Len() - 1; x > -1; --x)
		{
			Result += Number.Mid(x, 1);

			dec++;
			if (dec == 3 && x > 0)
			{
				Result += TEXT(",");
				dec = 0;
			}
		}

		return Result.Reverse();
	}
};

void UGsUIAchievementItemMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	_btnRecv->OnClicked.AddDynamic(this, &UGsUIAchievementItemMain::OnReceive);
	_btnDetail->OnClicked.AddDynamic(this, &UGsUIAchievementItemMain::OnAchievementGroupList);
}

void UGsUIAchievementItemMain::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIAchievementItemMain::RefreshUI()
{
	if (auto achievement = _data.Pin())
	{
		Super::RefreshUI();

		_receivedAchievementId = 0;
		_multiProgressBar->SetPercent(achievement->GetPreProgressRate());
		_multiProgressBar->SetProgress(achievement->GetProgressRate(), 0, achievement->_IsProgressUp);
		_multiProgressBar->SetProgressText(FText::FromString(FString::Format(TEXT("{0}/{1}"),
			{ FStringExt::FormatAsNumber64(achievement->CurrentPoint()), FStringExt::FormatAsNumber64(achievement->CompletePoint()) })));
		_btnRecv->SetIsEnabled(achievement->IsComplete() && !achievement->IsRewarded());
		_completeSwitcher->SetActiveWidgetIndex(achievement->IsRewarded() ? 1 : 0);
	}
}


void UGsUIAchievementItemMain::OnReceive()
{
	if (auto achievement = _data.Pin())
	{
		if (INVALID_ACHIEVEMENT_ID == achievement->_achievementId)
		{
			GSLOG(Error, TEXT("Achievement ID is Invalid!"));
			return;
		}

		//// 이미 방금 수령한 업적일 경우 (광클 이슈 해결)
		if (_receivedAchievementId == achievement->_achievementId)
			return;

		_receivedAchievementId = achievement->_achievementId;
		FGsNetSendServiceWorld::SendReqAchievementRewards(_receivedAchievementId);
	}
}

void UGsUIAchievementItemMain::OnAchievementGroupList()
{
	if (!_data.IsValid()) return;

	if (auto PopupDetail = Cast<UGsUIPopupAchievementGroupList>(GUI()->OpenAndGetWidget(TEXT("PopupAchievementDetail"))))
	{
		UGsUIPopupAchievementGroupList::PopupInitData param = { _data.Pin()->_groupId };
		PopupDetail->InitializeData(&param);
		PopupDetail->RefreshUI();
	}
}
