#include "GsUIEmojiChatEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Image.h"

void UGsUIEmojiChatEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnEmoji->OnClicked.AddDynamic(this, &UGsUIEmojiChatEntry::OnSelected);
}

void UGsUIEmojiChatEntry::NativeConstruct()
{
	Super::NativeConstruct();
	//_btnEmoji->OnClicked.AddDynamic(this, &UGsUIEmojiChatEntry::OnSelected);
}

void UGsUIEmojiChatEntry::NativeDestruct()
{
	//_btnEmoji->OnClicked.RemoveDynamic(this, &UGsUIEmojiChatEntry::OnSelected);
	Super::NativeDestruct();
}

void UGsUIEmojiChatEntry::OnSelected()
{
	if (_emojiID > 0)
	{	
		OnClickSlot.ExecuteIfBound(_emojiID);
	}
}

void UGsUIEmojiChatEntry::SetEmojiID(int32 emojiID, FSoftObjectPath pathEmoticon)
{ 
	_emojiID = emojiID; 
	_iconImage->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(pathEmoticon), true);
}