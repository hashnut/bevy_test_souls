// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class T1PROJECT_API FGsInvenHandler
{
public:
	FGsInvenHandler();
	virtual ~FGsInvenHandler();

public:
	void Init();
	void Close();
	void OnReconection();

	/*
public:
	
	void OpenInventory(EInventoryTab InTab);
	
	void ChangeInvenTabState(EInventoryTab InTab);
	void CloseInventory();
	void OnClickItem();

public:
	void CreateItem(const uint64 InItemDBID);
	void DeleteItem(const uint64 InItemDBID);
	void UpdateItem(const uint64 InItemDBID);
	void OnChangeInvenSlotCount(uint32 InAddedCount);

private:
	void BeginGetItemList();
	void ResetContainValue();

public:
	// 보유 아이템 Get 함수( 가변 템플릿 사용 )
	template<typename T1, typename... T2>
	void GetOwnedItem(T1 InType, T2... InTypes);
	void GetOwnedItem(); // 후처리
	void FindItem(ItemType InItemType);
	void FindItem(uint32 InItemTID);

public:
	TArray<TWeakPtr<FGsItem>> FindItemByPresetNum(ItemType InType);

};

template<typename T1, typename... T2>
void FGsInvenHandler::GetOwnedItem(T1 InType, T2... InTypes)
{
	FindItem(InType);
	GetOwnedItem(InTypes...);
}
*/

};