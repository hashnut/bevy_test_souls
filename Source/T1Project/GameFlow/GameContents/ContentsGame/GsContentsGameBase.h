// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GsContentsMode.h"
#include "../../../Classes/GsState.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/ScopeGlobal/GsUIManager.h"

//------------------------------------------------------------------------------
// Lobby 이후, InGame 컨텐츠를 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class T1PROJECT_API FGsContentsGameBase : public TGsState<FGsContentsMode::InGame>
{
public:
	FGsContentsGameBase() : TGsState<FGsContentsMode::InGame>(FGsContentsMode::InGame::GameMax)
	{
	}
	explicit FGsContentsGameBase(FGsContentsMode::InGame inMode) : TGsState<FGsContentsMode::InGame>(inMode)
	{
	}
	virtual ~FGsContentsGameBase()
	{
	}

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override {};

public:
	virtual void OnReconectionEnd() {};

protected:
	virtual bool IsChangeSoundMixer() const { return true; }
	virtual void OnResetUI(UGsUIManager::FOnUIManagerResetUI& resetHandler) {}
	
protected:
	// 네비게이션바의 추가 슬롯 표시
	// CurrncyType::NONE : 표시 안함
	// CurrencyType::MAX_USER_TYPE : 표시안함, 드롭 다운메뉴 On (StateHud만 사용)
	void SetNavigationBar(CurrencyType InType);
	void SetNavigationBarItemId(ItemId InItemId);

	void CloseNavigationBar();
	void OpenNavigationBar();
};
