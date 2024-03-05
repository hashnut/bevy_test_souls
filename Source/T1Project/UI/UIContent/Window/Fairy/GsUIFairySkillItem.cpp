#include "GsUIFairySkillItem.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Classes/PaperSprite.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Data/GsResourceManager.h"


void UGsUIFairySkillItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTooltip->OnClicked.AddDynamic(this, &UGsUIFairySkillItem::OnClickSkillButton);
}

void UGsUIFairySkillItem::SetData(const FGsSchemaPassivitySet* data, FairyLevelId lv)
{
	if (data)
	{
		_skillLevel = lv;

		FText levelFormat;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

		_textSkillName = data->nameText;
		_textSkillLevel = FText::Format(levelFormat, lv);

		_skillIcon->SetVisibility(ESlateVisibility::Visible);

		if (UPaperSprite* loadObject = GResource()->LoadSync<UPaperSprite>(data->iconPath))
		{
			static FVector2D sz = _skillIcon->Brush.GetImageSize();
			if (auto material = _skillIcon->GetDynamicMaterial())
			{
				FSlateAtlasData atlasData = loadObject->GetSlateAtlasData();
				material->SetTextureParameterValue(TEXT("SpriteTexture"), atlasData.AtlasTexture);
				material->SetScalarParameterValue(TEXT("UPos"), atlasData.StartUV.X);
				material->SetScalarParameterValue(TEXT("VPos"), atlasData.StartUV.Y);
				material->SetScalarParameterValue(TEXT("UTiling"), atlasData.SizeUV.X);
				material->SetScalarParameterValue(TEXT("VTiling"), atlasData.SizeUV.Y);
				_skillIcon->SetBrushSize(sz);
			}
		}
		_level->SetVisibility(lv > 10000 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
		_upgradeEffect->SetVisibility(ESlateVisibility::Collapsed);
		_upgradeEffectBackLight->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIFairySkillItem::SetData(TWeakPtr<FGsFairyPassivityInfo> infoData)
{
	_data = infoData;

	if (auto data = _data.Pin())
	{
		_skillLevel = data->level;

		FText levelFormat;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

		_textSkillLevel = FText::Format(levelFormat, _skillLevel);

		if (const auto passivity = data->passivity)
		{
			_textSkillName = passivity->nameText;
			
			_skillIcon->SetVisibility(ESlateVisibility::Visible);

			if (UPaperSprite* loadObject = GResource()->LoadSync<UPaperSprite>(passivity->iconPath))
			{
				static FVector2D sz = _skillIcon->Brush.GetImageSize();
				if (auto material = _skillIcon->GetDynamicMaterial())
				{
					FSlateAtlasData atlasData = loadObject->GetSlateAtlasData();
					material->SetTextureParameterValue(TEXT("SpriteTexture"), atlasData.AtlasTexture);
					material->SetScalarParameterValue(TEXT("UPos"), atlasData.StartUV.X);
					material->SetScalarParameterValue(TEXT("VPos"), atlasData.StartUV.Y);
					material->SetScalarParameterValue(TEXT("UTiling"), atlasData.SizeUV.X);
					material->SetScalarParameterValue(TEXT("VTiling"), atlasData.SizeUV.Y);
					_skillIcon->SetBrushSize(sz);
				}
			}
		}

		_level->SetVisibility(data->isVehicle || _skillLevel > 10000 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
		_upgradeEffect->SetVisibility(data->isUpgradable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		_upgradeEffectBackLight->SetVisibility(data->isUpgradable ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

		if (data->isUpgradable)
			PlayAnimation(_enchantEnable, 0.f, 0);
	}
}

void UGsUIFairySkillItem::RefreshUI()
{
	
}

void UGsUIFairySkillItem::OnClickSkillButton()
{
	OnClickFairySkillIcon.ExecuteIfBound(_skillLevel);
	OnSkillTooltip.ExecuteIfBound(_data);
}
