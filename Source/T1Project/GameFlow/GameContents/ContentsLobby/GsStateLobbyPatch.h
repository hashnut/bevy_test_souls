// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsContentsLobbyBase.h"
#include "Message/GsMessageSystem.h"
#include "../Message/MessageParam/GsMessageParam.h"
/**
 * 
 */
class FGsDivergence;
class UGsUIWindowDownload;
class FText;

class T1PROJECT_API FGsStateLobbyPatch : public FGsContentsLobbyBase
{

protected:
	MsgSystemHandleArray _msgHandleLobbySystemList;

private:
	TWeakObjectPtr<class UGsUIWindowDownload>	_windowUI;
	FText _textDownloading;
	FText _textDownloadChecking;
	FTimerHandle _timerHandle;
	bool _wasCheckSized = false;

	FText _textPatchStateQueued; // 패치 프로세스가 다른 설치를 기다리고 있습니다.
	FText _textPatchStateInitializing; // 패치 프로세스를 초기화하는 중입니다.
	FText _textPatchStateResuming; // 패치 프로세스가 기존의 스테이징된 데이터를 열거하고 있습니다.
	FText _textPatchStateInstalling; // 패치 프로세스가 파일을 설치하는 중입니다.
	FText _textPatchStateMovingToInstall; // 패치 프로세스가 준비된 파일을 설치폴더로 이동하고 있습니다.
	FText _textPatchStateBuildVerification; // 패치 프로세스에서 파일을 검증하고 있습니다.
	FText _textPatchStateCleanUp; // 패치 프로세스가 임시 파일을 정리하고 있습니다.
	FText _textPatchStatePrerequisitesInstall; // 패치 프로세스에서 필수 구성 요소를 설치하고 있습니다.
	FText _textPatchStateCompleted; // 패치 프로세스가 완료되어 다음 프로세스를 준비중입니다.
	FText _textPatchStatePaused; // 패치 프로세스가 일시 중지로 설정되었습니다.

	float _curTime;

public:
	FGsStateLobbyPatch();
	virtual ~FGsStateLobbyPatch();

protected:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	virtual void Init() override;
	virtual void Close() override;	

	// send 후 타이머 시작
	virtual void StartTimer() override;
	// 타이머 끝이 불리면 다시 타이틀로 이동
	virtual void EndTimer() override;
	// 스테이트 나갈 때 타이머 삭제
	virtual void StopTimer() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

protected:
	void RequestPatch();
	void OnPatchSucceeded();
	void OnPatchFailed();

private:
	// 다운로드 실패
	void PatchResultFailed(const IGsMessageParam* InParam);
	// 다운로드 성공
	void PatchResultSucceeded(const IGsMessageParam* InParam);

	// 임시 테스트용
	void PatchClickEvent(const IGsMessageParam* InParam);
};
 