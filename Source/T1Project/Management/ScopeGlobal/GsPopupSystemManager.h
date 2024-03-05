// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Management/GsScopeHolder.h"
#include "../PopupSystem/GsPopupSystemEnum.h"
#include "GsPopupSystemManager.generated.h"

class UGsUIPopup;
class FPopupInfo
{
private:
	EPopupType _popupType = EPopupType::None;
	EPopupButtonType _buttonType = EPopupButtonType::Yes;
	FText _message;
	TFunction<void()> _callbackYesFunc;
	TFunction<void(bool)> _callbackYesNoFunc;

public:
	FPopupInfo() = default;
	~FPopupInfo() = default;
	explicit FPopupInfo(EPopupType InPopupTypec, EPopupButtonType InButtonType,
		const FText& InMessage,
		TFunction<void()> InCloseCallbackYes = nullptr,
		TFunction<void(bool)> InCloseCallbackYesNo = nullptr)
		: _popupType(InPopupTypec), _buttonType(InButtonType), _message(InMessage),
		_callbackYesFunc(InCloseCallbackYes),
		_callbackYesNoFunc(InCloseCallbackYesNo) {}

public:
	// getter
	const EPopupType GetPopupType() const { return _popupType; }
	const EPopupButtonType GetButtonType() const { return _buttonType; }
	const FText& GetMessage() const { return _message; }
	const TFunction<void()>& GetCallbackYesFunc() const { return _callbackYesFunc; }
	const TFunction<void(bool)>& GetCallbackYesNoFunc() const { return _callbackYesNoFunc; }
};
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsPopupSystemManager final :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<class UGsUIPopup> _popupSystemUI;
	TArray<TSharedPtr<FPopupInfo>> _popupInfoList;

	FName _popupOkKey;
	FSoftObjectPath _popupOkPath;

	FName _popupYesNoKey;
	FSoftObjectPath _popupYesNoPath;
	
public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	void OnLauncherStart();

private:
	bool IsVaildPopupType(EPopupType InPopupType);

public:
	void RemoveAll();
	// 시스템 팝업을 띄운다. (시스템 팝업: Depth 높음, Stack 최상단, 로딩중 닫히지 않음)
	void PopupSystemMsg(const FText& InMessage, TFunction<void()> InCloseCallback = nullptr, 
		EPopupType InPopupType = EPopupType::None, EPopupOpenType InPopupOpenType = EPopupOpenType::Serial);

	// 시스템 YesNo 팝업을 띄운다.
	void PopupSystemYesNo(const FText& InMessage, TFunction<void(bool)> InCloseCallback = nullptr, 
		 EPopupType InPopupType = EPopupType::None, EPopupOpenType InPopupOpenType = EPopupOpenType::Serial);

	void OpenPopupSystemUI(const FText& InMessage);
	void OpenPopupSystemYesNoUI(const FText& InMessage);

	void OnPopUpClose(bool InIsOk = true);

private:
	TWeakObjectPtr<class UGsUIPopupSystem> GetPopupSystemUI();
	TWeakObjectPtr<class UGsUIPopupSystemYesNo> GetPopupSystemYesNo();
};

#define GPopupSystem() UGsScopeHolder::GetGlobalManager<UGsPopupSystemManager>(UGsScopeGlobal::EManagerType::PopupSystem)