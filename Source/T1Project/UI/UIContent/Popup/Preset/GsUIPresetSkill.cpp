#include "GsUIPresetSkill.h"

#include "Components/Image.h"
#include "Components/PanelWidget.h"

#include "Paper2D/Classes/PaperSprite.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Skill/GsSkill.h"

#include "Materials/MaterialInstanceDynamic.h"


void UGsUIPresetSkill::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSkill->OnClicked.AddDynamic(this, &UGsUIPresetSkill::OnClickSkill);
}

void UGsUIPresetSkill::NativeConstruct()
{
	Super::NativeConstruct();

	_panelSkill->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPresetSkill::SetSkill(const FGsSkill* InSkill, PresetId InPresetId)
{
	if (nullptr == InSkill)
	{
		_panelSkill->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		TWeakObjectPtr<UImage> weakThis(_imageSkill);
		TWeakObjectPtr<UImage> weakAuto(_imageAuto);
		TWeakObjectPtr<UPanelWidget> weakPanel(_panelSkill);
		bool isAuto = InSkill->_presetData[InPresetId].Value;
		FGsUIHelper::AsyncLoadSpriteFromPath(*InSkill->GetIconPath(), FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, weakAuto, weakPanel, isAuto](UObject* inRes)
			{
				if (UPanelWidget* strongPanel = weakPanel.Get())
				{
					strongPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}

				if (UImage* strongAuto = weakAuto.Get())
				{
					strongAuto->SetVisibility(isAuto ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
				}

				if (UImage* strongThis = weakThis.Get())
				{
					if (UMaterialInstanceDynamic* mat = strongThis->GetDynamicMaterial())
					{
						if (UPaperSprite* icon = Cast<UPaperSprite>(inRes))
						{
							FSlateAtlasData atlasData = icon->GetSlateAtlasData();
							mat->SetTextureParameterValue(TEXT("Texture"), atlasData.AtlasTexture);
							mat->SetScalarParameterValue(TEXT("OffsetU"), atlasData.StartUV.X);
							mat->SetScalarParameterValue(TEXT("OffsetV"), atlasData.StartUV.Y);
							mat->SetScalarParameterValue(TEXT("TilingU"), atlasData.SizeUV.X);
							mat->SetScalarParameterValue(TEXT("TilingV"), atlasData.SizeUV.Y);
						}
					}
				}
			}));
	}
}

void UGsUIPresetSkill::OnClickSkill()
{

}