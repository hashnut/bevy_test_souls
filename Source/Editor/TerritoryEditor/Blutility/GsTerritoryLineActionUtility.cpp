// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryLineActionUtility.h"
#include "EditorUtilityLibrary.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "TerritoryEditor.h"
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Actor/Line/GsTerritoryLine.h"
#include "Actor/Line/GsTerritoryLinePatrol.h"
#include "GsTerritoryEditorSetting.h"
#include "UObject/UObjectGlobals.h"

const FString ONLY_ONE_LINE_SELECT = TEXT("하나의 Line만 선택해 주세요");
const FString NONE_LINE_SELECT = TEXT("Line이 선택되지 않았습니다.");
const FString MOUSE_NOT_ON_LINE = TEXT("마우스가 Line위에 있지 않습니다.");

void UGsTerritoryLineActionUtility::DeleteLine()
{
	AGsTerritoryLine* line = nullptr;
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	for (AActor* actor : selectArray)
	{
		if (nullptr == actor)
		{
			continue;
		}

		line = Cast<AGsTerritoryLine>(actor);
		if(nullptr == line)
		{
			continue;
		}
		
		line->DestroyManually();
	}
}

void UGsTerritoryLineActionUtility::ChangeLineShape(float inRot, float inDistance)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);
		return;
	}

	AGsTerritoryLine* line = nullptr;
	for (AActor* actor : selectArray)
	{
		line = Cast<AGsTerritoryLine>(actor);
		if (nullptr == line)
		{
			continue;
		}

		line->ChangeLineShape(inRot, inDistance);
	}
}

void UGsTerritoryLineActionUtility::ChangeCircleShape(float inRadius)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);
		return;
	}

	AGsTerritoryLine* line = nullptr;
	for (AActor* actor : selectArray)
	{
		line = Cast<AGsTerritoryLine>(actor);
		if (nullptr == line)
		{
			continue;
		}

		line->ChangeCircleShape(inRadius);
	}
}

void UGsTerritoryLineActionUtility::ChangePolygonShape()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);
		return;
	}

	AGsTerritoryLine* line = nullptr;
	for (AActor* actor : selectArray)
	{
		line = Cast<AGsTerritoryLine>(actor);
		if (nullptr == line)
		{
			continue;
		}
		
		line->ChangePolygonShape();
	}
}

void UGsTerritoryLineActionUtility::OptimizeLine()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);
		return;
	}

	AGsTerritoryLine* line = nullptr;
	for (AActor* actor : selectArray)
	{
		line = Cast<AGsTerritoryLine>(actor);
		if (nullptr == line)
		{
			continue;
		}

		line->OptimizePath();
	}
}

AGsTerritoryLine* UGsTerritoryLineActionUtility::GetSelectedTerritoryLine()
{
	TArray<AActor*> selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() == 1)
	{
		if (AGsTerritoryLine* line = Cast<AGsTerritoryLine>(selectArray[0]))
		{
			return line;
		}
	}

	FString log = TEXT("영역이 없습니다.");
	FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, log);

	return nullptr;
}

bool UGsTerritoryLineActionUtility::IsSelectedOnlyOneLine()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() == 1)
	{
		if (AGsTerritoryLine* line = Cast<AGsTerritoryLine>(selectArray[0]))
		{
			return true;
		}
	}

	FString log = TEXT("하나의 영역만 선택해 주세요.");

	UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);	

	return false;
}

bool UGsTerritoryLineActionUtility::IsSelectedLine()
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();

	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);

		return false;
	}

	for (AActor* actor : selectArray)
	{
		if (AGsTerritoryLine* line = Cast<AGsTerritoryLine>(actor))
		{
			if (nullptr == line)
			{
				FString log = TEXT("터레인 영역만 선택해 주세요");					
				UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);

				return false;
			}
		}
	}

	return true;
}

class AGsTerritoryLinePatrol* UGsTerritoryLineActionUtility::CreatePatrolLine(const FVector& in_location)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	FTransform trs;
	trs.SetLocation(in_location);
	AActor* temp = nullptr;

	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return nullptr;
	}

	temp = GEditor->AddActor(world->GetCurrentLevel(), settings->_patrolLineClass, trs);

	return Cast<AGsTerritoryLinePatrol>(temp);
}

void UGsTerritoryLinePatrolActionUtility::SetReturn(bool inReturn)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);
		return;
	}

	for (AActor* actor : selectArray)
	{
		if (AGsTerritoryLinePatrol* line = Cast<AGsTerritoryLinePatrol>(actor))
		{
			line->SetReturn(inReturn);
		}
	}	
}

void UGsTerritoryLinePatrolActionUtility::SetRepeat(bool inRepeat)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);
		return;
	}

	for (AActor* actor : selectArray)
	{
		if (AGsTerritoryLinePatrol* line = Cast<AGsTerritoryLinePatrol>(actor))
		{
			line->SetRepeat(inRepeat);
		}
	}
}

void UGsTerritoryLinePatrolActionUtility::SetRepeatDelay(float inDelay)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);
		return;
	}

	for (AActor* actor : selectArray)
	{
		if (AGsTerritoryLinePatrol* line = Cast<AGsTerritoryLinePatrol>(actor))
		{
			line->SetRepeatDelay(inDelay);
		}
	}
}

void UGsTerritoryLinePatrolActionUtility::SetSuicide(bool inSuicide)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() <= 0)
	{
		FString log = TEXT("아무것도 선택되지 않았습니다.");
		UE_LOG(LogTerritoryEditor, Error, TEXT("%s"), *log);
		return;
	}

	for (AActor* actor : selectArray)
	{
		if (AGsTerritoryLinePatrol* line = Cast<AGsTerritoryLinePatrol>(actor))
		{
			line->SetSuicide(inSuicide);
		}
	}
}
