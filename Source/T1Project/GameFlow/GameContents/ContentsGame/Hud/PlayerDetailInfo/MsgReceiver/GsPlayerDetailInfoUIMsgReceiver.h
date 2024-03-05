//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Message/GsMessageContents.h"
//#include "UI/UIContent/Popup/GsUIPopupPlayerDetailInfo.h"
//#include "GameObject/Define/GsGameObjectDefine.h"
//#include "Message/MessageParam/GsPlayerDetailInfoMessageParam.h"
//
///**
// * 
// */
//class T1PROJECT_API FGsPlayerDetailInfoUIMsgReceiver
//{
//private:
//	TWeakObjectPtr<UGsUIPopupPlayerDetailInfo> _playerDetailUI;
//	TArray<TPair<MessageContentPlayerInfo, FDelegateHandle>>	_listEventDelegate;
//
//public:
//	FGsPlayerDetailInfoUIMsgReceiver() = default;
//	~FGsPlayerDetailInfoUIMsgReceiver();
//
//public:
//	void RegisterMessage();
//	void RemoveMessage();
//
//private:
//	void UpdateEquippedItemList(FGsPlayerDetailInfoMessageParamBase* InData);
//	void UpdateEquippedItem(FGsPlayerDetailInfoMessageParamBase* InData);
//	void UpdateClassImage(FGsPlayerDetailInfoMessageParamBase* InData);
//	void UpdatePlayerStatList(FGsPlayerDetailInfoMessageParamBase* InData);
//
//public:
//	void Send_UpdateEquippedItemList() const;
//	void Send_UpdateClassImage() const;
//	void Send_UpdatePlayerStatList() const;
//
//public:
//	void SetUIObject(UGsUIPopupPlayerDetailInfo* InUI) { _playerDetailUI = InUI; }
//
//public:
//	bool IsValidObject() { return _playerDetailUI.IsValid(); }
//};
