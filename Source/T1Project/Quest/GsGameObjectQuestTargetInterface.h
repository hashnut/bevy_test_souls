// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"
#include "Map/Spawn/GsSchemaSpawnElement.h"
#include "GsGameObjectQuestTargetInterface.generated.h"

class USphereComponent;
class UWidgetComponent;
class UGsUIBillboardInteractionButton;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGsGameObjectQuestTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class T1PROJECT_API IGsGameObjectQuestTargetInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//퀘스트 진행 가능 상태
	virtual void SetQuestTargetActive(bool In_isActive, int In_Index = 0) {}
	virtual bool GetQuestTargetActive() const { return false; }

	// 퀘스트 마크 액티브
	virtual void SetQuestMarkActive(bool In_isActive, int In_typeIndex = 0) {}
	// 퀘스트 대상인가?
	virtual bool IsQuestTarget(){return false;}
	virtual void InvalidateQuestMark(){}
	
};
