// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStageLobbyBase.h"
#include "Shared/Shared/SharedInclude/SharedPrimitiveTypes.h"
#include "UI/UIContent/Popup/GsUIPopupLobbyOption.h"
#include "UI/UIContent/Window/GsUIWindowTitle.h"
#include "Message/GsMessageSystem.h"

class UGsUIWindowTitle;
//------------------------------------------------------------------------------
// 게임모드를 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsStageLobbyTitle : public FGsStageLobbyBase
{
private:
	MsgSystemHandleArray					_listSystemDelegate;

private:
	TWeakObjectPtr<class UGsUIWindowTitle>		_windowUI;
	TWeakObjectPtr<class UGsUIPopupLobbyOption> _popupLobbyOptionUI;

	bool _isAccountChanging = false;

public:
	FGsStageLobbyTitle();
	virtual ~FGsStageLobbyTitle();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

protected:
	void OnClickServerSelect();
	void OnClickOption();
	void OnClickStart(uint16 In_planetWorldId);
	void OnClickAccountChange();
	void OnClickPatchCheck(); // BSAM_PATCH_DATA_REPAIR
	void OnClickDeviceRegister();

	void OnPopupClickServerSelect(uint16 In_PlanetWorldId);

	void BindMessages();
	void UnbindMessages();

	// 하이브 SignIn 실패
	void HiveAuthResultFailed(const IGsMessageParam* InParam);
	// 하이브 SignIn 성공
	void HiveAuthResultSucceeded(const IGsMessageParam* InParam);

	// 하이브 LogOut 실패
	void HiveAuthLogoutResultFailed(const IGsMessageParam* InParam);
	// 하이브 LogOut 성공
	void HiveAuthLogoutResultSucceeded(const IGsMessageParam* InParam);

	//// 기기등록 성공
	//void HiveDeviceRegisterSucceeded(const IGsMessageParam* InParam);
	//// 기기등록 실패
	//void HiveDeviceRegisterFailed(const IGsMessageParam* InParam);

	// 기기등록 여부 서버에서 받음	
	void HiveDeviceRegisterResult(const IGsMessageParam* InParam);

	// 서버선택 성공/실패, 게이트웨이 로그인 실패
	void UpdateServerName(const IGsMessageParam* InParam);

	// 기기등록 hive open 성공
	void HiveDeviceRegisterPopupSuccessded(const IGsMessageParam* InParam);
	// 기기등록 hive open 실패
	void HiveDeviceRegisterPopupFailed(const IGsMessageParam* InParam);

	// 에러 상황 (게임 다시시작)
	void ErrorExit();

	void HIVEShowSignIn(const IGsMessageParam* InParam);
private:
	// 계정변경
	void SetIsAccountChanging(bool In_Value);
	void AccountChagedResult(bool inValue);
	bool GetIsAccountChanging() const { return _isAccountChanging; }
};
