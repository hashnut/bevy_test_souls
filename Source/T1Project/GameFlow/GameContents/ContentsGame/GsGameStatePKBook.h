// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "../Message/GsMessagePKBook.h"
#include "../UI/UIContent/Window/GsUIWindowPKBook.h"

/**
 *
 */
class T1PROJECT_API FGsGameStatePKBook : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<UGsUIWindowPKBook>	_windowUI;

	MsgPKBookArray	_listPKBookDelegate;
public:
	FGsGameStatePKBook();
	virtual ~FGsGameStatePKBook();

protected:
	void RegisterMessages();
	void UnregisterMessages();

	// 2022/09/21 PKT - 리스트 리스트 갱신
	void ON_MSG_UI_INVALID_RECORD(const struct IGsMessageParam* InParam);
	// 2022/09/21 PKT - 디테일 데이터
	void ON_MSG_UI_OPEN_DETAIL_POPUP(const struct IGsMessageParam* InParam);
	// 2022/09/21 PKT - 디테일 데이터 갱신
	void ON_MSG_UI_INVALID_RECORD_DETAILDATA(const struct IGsMessageParam* InParam);
	// 2022/09/21 PKT - 추격 대상 갱신	
	void ON_MSG_UI_INVALID_DETECTIONTARGET(const struct IGsMessageParam* InParam);	
	// 2022/09/21 PKT - 추격 대상 리스트 갱신
	void ON_MSG_UI_INVALID_DETECTIONTARGET_LIST(const struct IGsMessageParam* InParam);

public:
	virtual void Enter() override;
	virtual void Exit() override;
};
