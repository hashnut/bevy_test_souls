#include "GsUIMonsterCollectionSlateNodeFadeInVFX.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperSprite.h"
#include "UMG/Public/Components/Image.h"

void UGsUIMonsterCollectionSlateNodeFadeInVFX::SetData(UPaperSprite* InData)
{
	_imgSymbol->SetBrushFromAtlasInterface(InData);
}