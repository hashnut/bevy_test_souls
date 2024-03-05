// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameViewPortClient.h"
#include "SignificanceManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Framework/Application/SlateApplication.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Message/MessageParam/GsSystemMessageParam.h"
#include "Message/GsMessageSystem.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"


void UGsGameViewPortClient::Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice)
{
	Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);

	_handleResize = FViewport::ViewportResizedEvent.AddUObject(this, &UGsGameViewPortClient::OnResizeViewport);
}

void UGsGameViewPortClient::BeginDestroy()
{
	if (_handleResize.IsValid())
	{
		FViewport::ViewportResizedEvent.Remove(_handleResize);
	}

	Super::BeginDestroy();
}

void UGsGameViewPortClient::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	if (_significantManager)
	{
		if ((nullptr != GetWorld()) && (nullptr != GSGameObject()))
		{

			if (UGsGameObjectBase * player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
			{
				if (AActor * character = player->GetActor())
				{
					_viewPortArray.Empty();
					_viewPortArray.Add(character->GetTransform());
				}
			}
			_significantManager->Update(TArrayView<FTransform>(_viewPortArray));
		}
	}
}

void UGsGameViewPortClient::InitSignificanceManager()
{
	if (GetWorld())
	{
		_significantManager = USignificanceManager::Get(GetWorld());
	}	
}

void UGsGameViewPortClient::OnResizeViewport(FViewport* InViewport, uint32 InNum)
{
	UpdateDragTriggerDistance();
}

void UGsGameViewPortClient::UpdateDragTriggerDistance()
{
	if (nullptr == Viewport)
	{
		// 이 값이 없으면 UWidgetLayoutLibrary::GetViewportScale 에서 매우 작은 값이 넘어옴
		return;
	}

	// 엔진 기본값은 5. 리스트 내 버튼 클릭이 잘되게 만들기 위해 민감도를 낮춤
	float dragTriggerDistBase = 20.f;
	if (UGsUIManager* uiMgr = GUI())
	{
		if (uiMgr->IsHUDScale() ||
			uiMgr->IsLongDragTriggerDist())
		{
			// 스킬 버튼 클릭이 잘되게 만들기 위해 민감도를 더 낮춤
			dragTriggerDistBase = 50.f;
		}
	}

	// 기기에 따라 동일조건을 주기 위해 Scale을 곱함
	float viewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	// 너무 이상한 값을 넣지 않도록 않도록 자름
	float dragTriggerDist = FMath::Clamp(dragTriggerDistBase * viewportScale, 5.f, 500.f);

	FSlateApplication::Get().SetDragTriggerDistance(dragTriggerDist);
}

void UGsGameViewPortClient::LostFocus(FViewport* InViewport)
{
	Super::LostFocus(InViewport);	

	if (nullptr == GScope())
	{
		return;
	}

	if (FGsMessageHolder* message = GMessage())
	{
		FGsViewPortFocusParam param(false);
		message->GetSystemParam().SendMessage(MessageSystem::VIEWPORT_LOST_FOCUS, &param);
		GSLOG(Log, TEXT("%s"), TEXT("=== LostFocus"));
	}
}

void UGsGameViewPortClient::ReceivedFocus(FViewport* InViewport)
{
	Super::ReceivedFocus(InViewport);

	if (nullptr == GScope())
	{
		return;
	}

	if (FGsMessageHolder* message = GMessage())
	{
		FGsViewPortFocusParam param(true);
		message->GetSystemParam().SendMessage(MessageSystem::VIEWPORT_RECEIVE_FOCUS, &param);
		GSLOG(Log, TEXT("%s"), TEXT("==== ReceivedFocus"));
	}
}