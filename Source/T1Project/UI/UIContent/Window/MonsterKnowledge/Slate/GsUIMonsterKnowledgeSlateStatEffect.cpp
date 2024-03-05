#include "GsUIMonsterKnowledgeSlateStatEffect.h"
#include "UI/UILib/Base/GsUIVFX.h"



void UGsUIMonsterKnowledgeSlateStatEffect::NativeOnInitialized()
{
	UUserWidget::NativeOnInitialized();
}

void UGsUIMonsterKnowledgeSlateStatEffect::BeginDestroy()
{
	UUserWidget::BeginDestroy();
}

void UGsUIMonsterKnowledgeSlateStatEffect::NativeConstruct()
{
	UUserWidget::NativeConstruct();
	
	if (false == _effectVFX->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_effectVFX->OnVFXAnimationFinished.BindUObject(this, &UGsUIMonsterKnowledgeSlateStatEffect::Finish);
	}
	Hide();
}

void UGsUIMonsterKnowledgeSlateStatEffect::NativeDestruct()
{
	if (true == _effectVFX->OnVFXAnimationFinished.IsBoundToObject(this))
	{
		_effectVFX->OnVFXAnimationFinished.Unbind();
	}

	UUserWidget::NativeDestruct();
}

void UGsUIMonsterKnowledgeSlateStatEffect::Reset()
{
	/*_effectVFX->StopAllAnimations();
	_effectVFX->StoppedSequencePlayers.Empty();*/
}

void UGsUIMonsterKnowledgeSlateStatEffect::Finish()
{
	Reset();
	Hide();
}

void UGsUIMonsterKnowledgeSlateStatEffect::Show()
{
	//Reset();
	_effectVFX->ShowVFX();
}

void UGsUIMonsterKnowledgeSlateStatEffect::Hide()
{
	_effectVFX->HideVFX();
}