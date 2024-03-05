#include "GsUIPopupBattleArenaGuildWarGuide.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIPopupBattleArenaGuildWarGuide::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUIPopupBattleArenaGuildWarGuide::OnClickedConfirm);
}

void UGsUIPopupBattleArenaGuildWarGuide::OnClickedConfirm()
{
	Close();
}

void UGsUIPopupBattleArenaGuildWarGuide::OnInputCancel()
{
	Close();
}