// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageContents.h"

struct IGsMessageParam;
/**
 * 
 */
class T1PROJECT_API FGsPickOneBoxHandler
{
public:
	TArray<TPair<MessageContentItem, FDelegateHandle>> _itemActionMsgHandler;


public:
	void Init();
	void Close();
	void OnReconnection();

private:
	void InitMsg();
	void RemoveMsg();

private:
	void OpenPopup(const struct IGsMessageParam* InMessageParam);
	void ConfirmPickItem(const struct IGsMessageParam* InMessageParam);

};
