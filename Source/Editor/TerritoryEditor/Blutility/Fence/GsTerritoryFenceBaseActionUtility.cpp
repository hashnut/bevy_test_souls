// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceBaseActionUtility.h"
#include "Blutility/Public/EditorUtilityLibrary.h"
#include "Actor/Fence/GsTerritoryFenceBase.h"
#include "Util/GsTerritoryEditorUtil.h"


void UGsTerritoryFenceBaseActionUtility::DeleteFence()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	for (AActor* actor : selectArray)
	{
		if (actor)
		{
			if (AGsTerritoryFenceBase* fence = Cast<AGsTerritoryFenceBase>(actor))
			{
				fence->DestroyManually();
			}
		}
	}
}

void UGsTerritoryFenceBaseActionUtility::EditName(FText inName)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	for (AActor* actor : selectArray)
	{
		if (actor)
		{
			if (AGsTerritoryFenceBase* fence = Cast<AGsTerritoryFenceBase>(actor))
			{
				fence->SetName(inName);
			}
		}
	}
}

class AGsTerritoryFenceBase* UGsTerritoryFenceBaseActionUtility::GetSelectedFence()
{
	TArray<AActor*> selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() == 1)
	{
		if (AGsTerritoryFenceBase* zone = Cast<AGsTerritoryFenceBase>(selectArray[0]))
		{
			return zone;
		}
	}

	return nullptr;
}
