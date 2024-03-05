// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GsDialogBase.h"

class FGsDialogBase;
class UGsUIManager;
class UGsUITraySubDialog;
struct FGsSchemaSubDialogData;

/**
 *  서브 대화스크립트
 */
class T1PROJECT_API FGsSubDialog final : public FGsDialogBase
{	
	using Super = FGsDialogBase;

public:
	FGsSubDialog() = default;
	virtual ~FGsSubDialog() = default;

protected:
	const FGsSchemaSubDialogData* _subDialogData = nullptr;

	// 총 컷수
	int32 _cutCount;
	// 남은 컷수
	int32 _remainCut;
	// 보이스 출력가능한 상태인가?
	bool _isVoicePlayable = true;
	// 믹서 교체 했는가?
	bool _isSoundMixerChanged = false;

protected:
	const FGsSchemaSubDialogData* GetSubDialogData(int32 InRowKey);

public:
	// 연출 시작
	void StartCutAction();
	// 다음 컷 연출 시작
	void NextCutAction();
	
public:
	virtual void CloseDialog() override;
	virtual void OpenDialog(const FName& InKey, int32 inDialogId) override;
	void OpenDialog(const FName & InKey, const FGsSchemaSubDialogData * inSubDialogData, bool inIsVoicePlayable = true);

public:
	void OnStopVoice();
	void ForceStopDialog();
};
