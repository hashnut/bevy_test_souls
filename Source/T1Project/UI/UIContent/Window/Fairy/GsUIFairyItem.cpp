#include "GsUIFairyItem.h"
#include "Fairy/GsFairyData.h"

#include "Management/ScopeGame/GsFairyManager.h"
#include "Management/ScopeGame/GsPresetManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"
#include "Kismet/GameplayStatics.h"

#include "Preset/GsPresetHelper.h"

#include "Engine/AssetManager.h"

#include "Components/AudioComponent.h"
#include "PaperSprite.h"
#include "Image.h"
#include "TextBlock.h"
#include "CanvasPanel.h"
#include "WidgetSwitcher.h"
#include "Classes/PaperSprite.h"
#include "Materials/MaterialInstanceDynamic.h"


void UGsUIFairyItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSelect->OnClicked.AddDynamic(this, &UGsUIFairyItem::OnSelected);
	_btnSelectCollection->OnClicked.AddDynamic(this, &UGsUIFairyItem::OnSelectedCollection);
}

void UGsUIFairyItem::NativeDestruct()
{
	OnHide();

	Super::NativeDestruct();
}

void UGsUIFairyItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{	
	if (_isBlocked)
	{
		_blockTime -= InDeltaTime;
		if (_blockTime <= 0.f)
		{
			SetBlockSelected(false, 0.f);
		}
	}
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIFairyItem::OnHide()
{
	ResetData();

	// 재생 중이던 사운드 중지
	if (_soundId > 0)
	{
		UGsSoundPlayer* soundPlayer = GSoundPlayer();
		if (soundPlayer && soundPlayer->IsPlaying(_soundId))
			soundPlayer->StopSound(_soundId);
		_soundId = 0;
	}
}

void UGsUIFairyItem::RefreshUI()
{
	Super::RefreshUI();

	if (_data.IsValid())
	{
		SetVisibility(ESlateVisibility::Visible);
		CommonRefreshUI();

		switch (WidgetType)
		{
		case UGsUIFairyItem::UIItemType::NONE:
		case UGsUIFairyItem::UIItemType::ITEM:
			FairyDataRefreshUI();
			break;
		case UGsUIFairyItem::UIItemType::ENCHANT:
			FairyDataEnchantUI();
			break;
		case UGsUIFairyItem::UIItemType::COMPOSITION:
			FairyDataComposeUI();
			break;
		case UGsUIFairyItem::UIItemType::COLLECTION:
			FairyDataCollectionUI();
			break;
		case UGsUIFairyItem::UIItemType::RESOURCE:
			FairyDataResourceUI();
			break;
		case UGsUIFairyItem::UIItemType::SUMMONWAIT:
			FairyDataSummonWaitUI();
			break;
		case UGsUIFairyItem::UIItemType::ENCHANT_RES:
			FairyDataEnchantResUI();
			break;
		case UGsUIFairyItem::UIItemType::POPUP_COLLECTION:
			FairyDataPopupCollectionUI();
			break;
		default:
			break;
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIFairyItem::CommonRefreshUI()
{
	if (auto fairyData = _data.Pin())
	{
		FText levelFormat;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

		_btnSelectCollection->SetVisibility(ESlateVisibility::Collapsed);
		_btnSelectCollection->SetIsEnabled(false);
		_btnSelect->SetIsEnabled(false);
		_checkIcon->SetVisibility(ESlateVisibility::Hidden);
		_redDot->SetVisibility(ESlateVisibility::Hidden);
		_completeMark->SetVisibility(ESlateVisibility::Hidden);
		_summoned->SetVisibility(ESlateVisibility::Hidden);
		_imagePreset->SetVisibility(ESlateVisibility::Hidden);
		_textFairyAmount->SetVisibility(ESlateVisibility::Hidden);
		_textEnchantCount->SetVisibility(ESlateVisibility::Hidden);

		_bgSelected->SetVisibility(_isSelected ?
			ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_bgNotCollectedFairy->SetVisibility(fairyData->isCollectedFairy() ? 
			ESlateVisibility::Hidden : ESlateVisibility::Visible);

		_textFairyAmount->SetText(FText::AsNumber(fairyData->GetAmount()));
		_textEnchantCount->SetText(FText::Format(levelFormat, fairyData->GetCurrentLevel()));

		//SetPortraiteImage(fairyData->GetFairyPortraitPath());
		//SetFairyGrade(fairyData->GetFairyGrade());
		//_bgImg->SetColorAndOpacity(FGsUIColorHelper::GetColorBGGradeWidthItemBGGrade(fairyData->GetFairyGrade()));
	}
}

void UGsUIFairyItem::FairyDataRefreshUI()
{
	if (auto fairyData = _data.Pin())
	{
		auto equipFairy = GSFairy()->GetEquipFairyData();
		bool isEquipped = (equipFairy.IsValid() && equipFairy == fairyData->_fairyItem);
		bool isPreset = !isEquipped && FGsPresetHelper::IsPreset(fairyData->GetPresetValue()) && GSPreset()->IsActivePreset();

		_btnSelect->SetIsEnabled(true);

		_summoned->SetVisibility(isEquipped ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_imagePreset->SetVisibility(isPreset ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_redDot->SetVisibility(fairyData->_isNew ? 
			ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_textEnchantCount->SetVisibility(fairyData->GetCurrentLevel() == INVALID_LEVEL ? 
			ESlateVisibility::Hidden : ESlateVisibility::Visible);
		_textFairyAmount->SetVisibility(fairyData->GetAmount() > 0 ? 
			ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UGsUIFairyItem::FairyDataEnchantUI()
{
	if (auto fairyData = _data.Pin())
	{
		_btnSelect->SetIsEnabled(true);
		_textEnchantCount->SetVisibility(fairyData->GetCurrentLevel() == INVALID_LEVEL ? 
			ESlateVisibility::Hidden : ESlateVisibility::Visible);
		_textFairyAmount->SetVisibility(ESlateVisibility::Hidden);
		_textFairyAmount->SetText(FText::AsNumber(fairyData->GetAmount()));
	}
}

void UGsUIFairyItem::FairyDataComposeUI()
{
	if (auto fairyData = _data.Pin())
	{
		_btnSelect->SetIsEnabled(fairyData->GetAmount() - _useCount > 0);
		_bgSelected->SetVisibility(ESlateVisibility::Hidden);
		_textEnchantCount->SetVisibility(ESlateVisibility::Hidden);
		_textFairyAmount->SetText(FText::AsNumber(fairyData->GetAmount() - _useCount));
		_textFairyAmount->SetVisibility(_useCount >= 0 && fairyData->GetAmount() - _useCount > 0 ? 
			ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_bgNotCollectedFairy->SetVisibility(_useCount > 0 && fairyData->GetAmount() - _useCount == 0 ? 
			ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UGsUIFairyItem::FairyDataCollectionUI()
{
	FText levelFormat;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

	_btnSelectCollection->SetVisibility(ESlateVisibility::Visible);
	_btnSelectCollection->SetIsEnabled(true);
	_bgSelected->SetVisibility(ESlateVisibility::Hidden);
	_redDot->SetVisibility(_isNewComplete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_textEnchantCount->SetVisibility(_needLv == INVALID_LEVEL ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	_completeMark->SetVisibility(!_complete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_checkIcon->SetVisibility(_complete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	_bgNotCollectedFairy->SetVisibility(ESlateVisibility::Hidden);
	_textEnchantCount->SetText(FText::Format(levelFormat, _needLv ));
}

void UGsUIFairyItem::FairyDataResourceUI()
{
	if (auto fairyData = _data.Pin())
	{
		_btnSelect->SetIsEnabled(fairyData->GetAmount() - _useCount > 0);
		_bgSelected->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIFairyItem::FairyDataSummonWaitUI()
{
	_btnSelectCollection->SetVisibility(ESlateVisibility::Visible);
	_btnSelectCollection->SetIsEnabled(true);
	_redDot->SetVisibility(ESlateVisibility::Hidden);
	_bgNotCollectedFairy->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIFairyItem::FairyDataEnchantResUI()
{
	if (auto fairyData = _data.Pin())
	{
		auto equipFairy = GSFairy()->GetEquipFairyData();

		_btnSelect->SetVisibility(ESlateVisibility::Hidden);
		_btnSelect->SetIsEnabled(false);
		_textFairyAmount->SetVisibility(ESlateVisibility::Hidden);
		_textEnchantCount->SetVisibility(fairyData->GetCurrentLevel() == INVALID_LEVEL ?
			ESlateVisibility::Hidden : ESlateVisibility::Visible);
	}
}

void UGsUIFairyItem::FairyDataPopupCollectionUI()
{
	FText levelFormat;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

	_bgSelected->SetVisibility(ESlateVisibility::Hidden);
	_redDot->SetVisibility(_isNewComplete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_textEnchantCount->SetVisibility(_needLv == INVALID_LEVEL ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	_completeMark->SetVisibility(!_complete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	_checkIcon->SetVisibility(_complete ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	_bgNotCollectedFairy->SetVisibility(ESlateVisibility::Hidden);
	_textEnchantCount->SetText(FText::Format(levelFormat, _needLv));
}

//void UGsUIFairyItem::SetFairyGrade(FairyGrade grade)
//{
//	_gradeSwitcher->SetVisibility(grade == FairyGrade::NONE ? 
//		ESlateVisibility::Hidden : ESlateVisibility::Visible);
//	_gradeBack->SetVisibility(grade == FairyGrade::NONE ?
//		ESlateVisibility::Hidden : ESlateVisibility::Visible);
//
//	static TMap<FairyGrade, uint8> gradeSelect = {
//		{ FairyGrade::NONE, 0 },
//		{ FairyGrade::NORMAL, 0 },
//		{ FairyGrade::MAGIC, 1 },
//		{ FairyGrade::RARE, 2 },
//		{ FairyGrade::EPIC, 3 },
//		{ FairyGrade::UNIQUE, 4 },
//		{ FairyGrade::LEGEND, 5 }
//	};
//
//	_gradeSwitcher->SetActiveWidgetIndex(gradeSelect[grade]);
//	_gradeBack->SetActiveWidgetIndex(gradeSelect[grade]);
//}

FairyId UGsUIFairyItem::GetFairyId()
{
	if (auto fairyData = _data.Pin())
	{
		return fairyData->_id;
	}
	return INVALID_FAIRY_ID;
}

void UGsUIFairyItem::SetSelected(bool isSelected, bool isNotiSelect)
{ 
	_isSelected = isSelected; 

	SetBlockSelected(false, 0.f);
	if (_isSelected && isNotiSelect)
	{
		if (auto fairyData = _data.Pin())
		{
			OnItemSelected.ExecuteIfBound(fairyData->_id);
		}
	}
	else if (!_isSelected && _soundId > 0)
	{
		UGsSoundPlayer* soundPlayer = GSoundPlayer();
		if (soundPlayer && soundPlayer->IsPlaying(_soundId)) 
			soundPlayer->StopSound(_soundId);

		_soundId = 0;
	}
}

void UGsUIFairyItem::SetBlockSelected(bool selectBlock /*= true*/, float blockTime /*= 0.5f*/)
{
	_isBlocked = selectBlock;
	_blockTime = blockTime;

	_btnSelect->SetIsEnabled(!selectBlock);
}

void UGsUIFairyItem::OnSelected()
{	
	if (_isBlocked) return;

	if (auto fairyData = _data.Pin())
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			if (_soundId > 0 && !soundPlayer->IsPlaying(_soundId))
				_soundId = 0;
		}

		if (_soundId == 0)
		{
#ifdef USE_LOCALIZED_SOUND
			if (USoundBase* sound = GLocalization()->GetCharacterSound(fairyData->GetFairyVoiceFilePath()))
			{
				if (UAudioComponent* audioCom = UGameplayStatics::SpawnSound2D(GetWorld(), sound))
				{
					_soundId = audioCom->GetAudioComponentID();
				}
			}
#else			
			if (UObject* soundObject = UAssetManager::GetStreamableManager().LoadSynchronous(FSoftObjectPath(fairyData->GetFairUISoundPath())))
			{
				TWeakObjectPtr<USoundBase> pressSound = Cast<USoundBase>(soundObject);
				if (pressSound.IsValid())
				{
					if (UAudioComponent* audioCom = UGameplayStatics::SpawnSound2D(GetWorld(), pressSound.Get()))
					{
						_soundId = audioCom->GetAudioComponentID();
					}
				}
			}
#endif
		}

		OnItemSelected.ExecuteIfBound(fairyData->_id);
	}
}

void UGsUIFairyItem::OnSelectedCollection()
{
	if (auto fairyData = _data.Pin())
	{
		OnItemSelected.ExecuteIfBound(fairyData->_id);
	}
}

