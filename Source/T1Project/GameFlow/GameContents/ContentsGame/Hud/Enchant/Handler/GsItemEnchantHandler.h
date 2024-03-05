// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "EventProgress/GsEventProgressInterface.h"
#include "EventProgress/GsEventProgressDefine.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContents.h"


/**
 * 
 */
class T1PROJECT_API FGsItemEnchantHandler : public IGsEventProgressInterface
{
private:
	// 예약 item db id
	ItemDBId _reserveItemDBId = 0;
	bool _isUseBGflag = false;
	bool _isOpenSingleEnchantFlag = true;
	ItemId _copyDestroyEnchantItemTid = 0;	
	
private:
	MsgGameObjHandleArray _gameObjectDelegateList;
	TPair<MessageItem, FDelegateHandle>	_itemDelegate;
	TArray<TPair<MessageContentEnchant, FDelegateHandle>>	_listItemEnchantDelegate;

public:
	FGsItemEnchantHandler() = default;
	virtual ~FGsItemEnchantHandler();

public:
	void Init();
	void Close();
	void OnReconection();

public:
	// 이벤트 시작
	virtual void StartEvent(EGsEventProgressType In_startType, EGsEventProgressType In_preType) override;
	// 이벤트 종료
	virtual void FinishEvent(EGsEventProgressType In_finishType, EGsEventProgressType In_nextType) override;

private:
	void RegisterMessage();
	void RemoveMessage();

private:
	void OpenItemEnchantProc(const struct IGsMessageParam* InParam);
	void CloseItemEnchant(const struct IGsMessageParam* InMessageParam);
	void CloseAllResultWidget(const struct IGsMessageParam* InMessageParam);
	void PlayerDeadState(const struct IGsMessageParam* InParam);
	void ShowEnchantEffect(const struct IGsMessageParam* InParam);
	void ShowMagicalForgeEffect(const struct IGsMessageParam* InParam);
	void OpenExpectRefineOptionPopup(const struct IGsMessageParam* InParam);
	void UpdateExpectRefineOptionPopup(const struct IGsMessageParam* InParam);
	void CloseExpectRefineOptionPopup(const struct IGsMessageParam* InParam);

private:
	void OnChangeIconDimmed(struct FGsItemMessageParamBase& InParam);

private:
	void CloseWidgetByKey(const FName& InKey);
	void CopyTempEnchantItemData(const ItemId InItemTid);
	void ConfirmEnchantWait(ItemDBId InTargetItemDbid, ItemDBId InIngredientItemDbid, ItemId InTargetItemTid);
	void ConfirmMagicalForgeWait(ItemDBId InTargetItemDbid, ItemDBId InIngredientItemDbid, ItemId InTargetItemTid);

};
