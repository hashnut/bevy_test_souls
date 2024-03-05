// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"

struct IGsMessageParam;

/**
 * 
 */
class T1PROJECT_API FGsChangeNickNameHandler final
{
public:
	using MsgHandleItemContents = TPair<MessageContentItem, FDelegateHandle>;

private:
	uint64 _ticketItemDBId = 0;
	FString _strTempNickName;

private:
	TArray<MsgHandleItemContents> _msgHandleItemContentsList;

public:
	FGsChangeNickNameHandler() = default;
	~FGsChangeNickNameHandler() {}

public:
	void Init();
	void Close();

private:
	void BindMessages();
	void UnbindMessages();

private:
	// 닉네임 변경 티켓 사용 시. ItemDBId저장, 변경 팝업 오픈
	void OnUseTicket(const IGsMessageParam* InParam);
	// 닉네임 체크 요청, 임시 닉네임 저장
	void OnRequestCheckNickName(const IGsMessageParam*);
	// 변경 팝업에서 이름 검사 요청 보낸 후 성공 시. 확인 팝업 오픈
	void OnCheckValidNickName(const IGsMessageParam*);
	// 닉네임 확정 요청
	void OnRequestConfirmNickName(const IGsMessageParam*);
	// 확정 요청 성공 시. 타이틀로 보내기 팝업 오픈
	void OnConfirmNickName(const IGsMessageParam* InParam);

private:
	bool IsValidPlayerState();
};
