// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponentEx/GsWidgetComponentFairyTalk.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "UI/UIContent/Billboard/GsUIBillboardFairyTalkBalloon.h"
#include "Fairy/GsFairyData.h"


UGsWidgetComponentFairyTalk::UGsWidgetComponentFairyTalk()
{
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCastShadow(false);
}

void UGsWidgetComponentFairyTalk::BeginPlay()
{
	Super::BeginPlay();
}

void UGsWidgetComponentFairyTalk::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UGsWidgetComponentFairyTalk::SetData(TWeakPtr<FGsFairyData> data)
{
	_data = data;

	if (nullptr == GetWidgetClass().Get())
	{
		if (UGsPathManager* pathMgr = GPath())
		{
			UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("FairyTalkBalloon"));
			if (widgetClass)
			{
				SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
			}
		}
	}

	if (UGsUIBillboardFairyTalkBalloon* talkBalloon = Cast<UGsUIBillboardFairyTalkBalloon>(GetWidget()))
	{
		if (auto fairyData = _data.Pin())
		{
			talkBalloon->SetName(fairyData->GetFairyName(), fairyData->GetFairyGrade());
		}
		talkBalloon->SetFairyActor(GetAttachmentRootActor());
	}
}

void UGsWidgetComponentFairyTalk::SetSkillTalk()
{
	if (auto fairyData = _data.Pin())
	{
		if (UGsUIBillboardFairyTalkBalloon* talkBalloon = Cast<UGsUIBillboardFairyTalkBalloon>(GetWidget()))
		{
			const FText& skillTalkMessage = fairyData->GetOnSkillTalkMessage();
			if (false == skillTalkMessage.IsEmpty())
			{
				talkBalloon->SetTalk(skillTalkMessage);
			}
		}
	}
}

void UGsWidgetComponentFairyTalk::SetVehicleTalk(EGsFairyTalkType TalkType)
{
	if (auto fairyData = _data.Pin())
	{
		if (UGsUIBillboardFairyTalkBalloon* talkBalloon = Cast<UGsUIBillboardFairyTalkBalloon>(GetWidget()))
		{
			const FText& skillTalkMessage = fairyData->GetOnSkillTalkMessage(TalkType);
			if (false == skillTalkMessage.IsEmpty())
			{
				talkBalloon->SetTalk(skillTalkMessage);
			}
		}
	}
}
