// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDFrameTestClient.h"
#include "Components/TextBlock.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "GsUIHUDTestQuest.h"
#include "Message/GsMessageUI.h"
#include "Widget.h"
#include "T1Project/Level/LevelScriptActor/GsLevelScriptActor.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIHUDFrameTestClient::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_hudList.Emplace(_questUI);

	_btn_F1->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F1);
	_btn_F2->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F2);
	_btn_F3->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F3);
	_btn_F4->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F4);
	_btn_F5->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F5);
	_btn_F6->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F6);
	_btn_F7->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F7);
	_btn_F8->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F8);
	_btn_F9->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F9);
	_btn_F10->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F10);
	_btn_F11->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F11);
	_btn_F12->OnClicked.AddDynamic(this, &UGsUIHUDFrameTestClient::OnClick_F12);
	
}

void UGsUIHUDFrameTestClient::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UGsUIHUDFrameTestClient::KeyEventTrigger(int i)
{
	ALevelScriptActor* level = GetWorld()->GetLevelScriptActor();
	if (level)
	{
		AGsLevelScriptActor* worldLevel = Cast<AGsLevelScriptActor>(level);
		worldLevel->EventFunctionKeyClick(i);
	}
}

void UGsUIHUDFrameTestClient::OnClick_F1()
{
	KeyEventTrigger(1);
}

void UGsUIHUDFrameTestClient::OnClick_F2()
{
	KeyEventTrigger(2);
}

void UGsUIHUDFrameTestClient::OnClick_F3()
{
	KeyEventTrigger(3);
}

void UGsUIHUDFrameTestClient::OnClick_F4()
{
	KeyEventTrigger(4);
}

void UGsUIHUDFrameTestClient::OnClick_F5()
{
	KeyEventTrigger(5);
}

void UGsUIHUDFrameTestClient::OnClick_F6()
{
	KeyEventTrigger(6);
}

void UGsUIHUDFrameTestClient::OnClick_F7()
{
	KeyEventTrigger(7);
}

void UGsUIHUDFrameTestClient::OnClick_F8()
{
	KeyEventTrigger(8);
}

void UGsUIHUDFrameTestClient::OnClick_F9()
{
	KeyEventTrigger(9);
}

void UGsUIHUDFrameTestClient::OnClick_F10()
{
	KeyEventTrigger(10);
}

void UGsUIHUDFrameTestClient::OnClick_F11()
{
	KeyEventTrigger(11);
}

void UGsUIHUDFrameTestClient::OnClick_F12()
{
	KeyEventTrigger(12);
}

void UGsUIHUDFrameTestClient::InvalidateAllMenu()
{
	// 각 메뉴들이 InvalidateAllInternal을 이용하지 않는 이유:
	// 외부(AGsHUDLocalPlayer)에서 InvalidateAll을 불러주면 1틱 뒤에 InvalidateAllInternal 함수가 불리는데,
	// 그때 각 메뉴들의 InvalidateAll을 불러주면, 1틱을 또 기다려 갱신하므로 2틱 뒤에 갱신된다.
	// AGsHUDLocalPlayer에서 각 메뉴들의 InvalidateAll을 부른 후, 전체 갱신을 요청했을때
	// 같은 틱에 dirty 체크가 되기 위해서 별도 함수를 통해 갱신한다.

	for (UGsUIHUD* hud : _hudList)
	{
		hud->InvalidateAll();
	}
}




