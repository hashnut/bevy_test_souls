// Fill out your copyright notice in the Description page of Project Settings.


#include "GsWidgetComponentTalkBalloon.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIContent/Billboard/GsUIBillboardTalkBalloon.h"


UGsWidgetComponentTalkBalloon::UGsWidgetComponentTalkBalloon()
{
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCastShadow(false);
}

void UGsWidgetComponentTalkBalloon::BeginPlay()
{
    Super::BeginPlay();

	_talkBalloonData = nullptr;

	if (_talkBalloonDataRow.IsNull())
		return;

    const FGsSchemaTalkBalloonData* data = _talkBalloonDataRow.GetRow();
    if (nullptr == data)
        return;

	if (nullptr == GetWidgetClass().Get())
	{
		if (UGsPathManager* pathMgr = GPath())
		{
			UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("TalkBalloon"));
			if (widgetClass)
			{
				SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
			}
		}
	}

	if (UGsUIBillboardTalkBalloon* talkBalloon = Cast<UGsUIBillboardTalkBalloon>(GetWidget()))
	{
		_talkBalloonData = data;
		talkBalloon->SetTalkBalloonData(data);
		talkBalloon->SetActor(GetAttachmentRootActor());
	}
}

void UGsWidgetComponentTalkBalloon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

bool UGsWidgetComponentTalkBalloon::IsAvailable()
{
	if (_talkBalloonDataRow.IsNull() && _talkBalloonData == nullptr)
		return false;

	return true;
}

void UGsWidgetComponentTalkBalloon::SetData(const FGsSchemaTalkBalloonData* InTalkBalloonData)
{
	_talkBalloonData = InTalkBalloonData;

	if (nullptr == _talkBalloonData)
		return;

	if (nullptr == GetWidgetClass().Get())
	{
		if (UGsPathManager* pathMgr = GPath())
		{
			UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("TalkBalloon"));
			if (widgetClass)
			{
				SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
			}
		}
	}

	if (UGsUIBillboardTalkBalloon* talkBalloon = Cast<UGsUIBillboardTalkBalloon>(GetWidget()))
	{
		talkBalloon->SetTalkBalloonData(_talkBalloonData);
		talkBalloon->SetActor(GetAttachmentRootActor());
	}
}

void UGsWidgetComponentTalkBalloon::SetIsEnabled(bool bInIsEnabled)
{
	if (UGsUIBillboardTalkBalloon* talkBalloon = Cast<UGsUIBillboardTalkBalloon>(GetWidget()))
	{
		talkBalloon->EndTalkBalloon();
		talkBalloon->SetIsEnabled(bInIsEnabled);
	}
}
