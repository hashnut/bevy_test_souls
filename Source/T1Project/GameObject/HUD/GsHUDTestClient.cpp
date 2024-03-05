// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/HUD/GsHUDTestClient.h"
#include "UI/UIContent/HUD/TestClient/GsUIHUDFrameTestClient.h"
#include "Management/ScopeGlobal/GsPathManager.h"


void AGsHUDTestClient::BeginPlay()
{
	Super::BeginPlay();

	Reset();

	// 아트 테스트용 UI가 빌드에 포함되지 않도록 Path 테이블에서 삭제하여 직접 경로를 사용
	FSoftObjectPath hudUIPath(TEXT("WidgetBlueprint'/Game/ArtTest/UI/HUD_TestClient.HUD_TestClient"));
	if (UClass* hudClass = UGsPathManager::GetClassStatic(hudUIPath))
	{
		if (UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), hudClass))
		{
			_hudWidget = Cast<UGsUIHUDFrameTestClient>(widget);

			_hudWidget->AddToViewport();
		}
	}
}

void AGsHUDTestClient::EndPlay(const EEndPlayReason::Type in_type)
{
	if (_hudWidget.IsValid())
	{
		_hudWidget->RemoveFromParent();
	}
	_hudWidget = nullptr;
	
	Super::EndPlay(in_type);
}

void AGsHUDTestClient::InvalidateHud()
{
	if (_hudWidget.IsValid())
	{
		_hudWidget->InvalidateAllMenu();
	}
}
