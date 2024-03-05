// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavAreas/NavArea.h"
#include "NavArea_Corner.generated.h"

/**
 * NavMeshModifierVolume이 사용하는 기능(Area 내에 탐색 코스트를 지정) 클래스
 *
 * 이클래스는 UNavArea_Default의 가중치보다 탐색 코스트를 UNavArea_Default 보다 높게 지정하여 
 * 코너(외각면)에 붙지 않게 이동하게 할때 사용한다.
 * 
 * 즉 길외각을 표현할 때 사용한다.
 * https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=204210180
 */
UCLASS()
class T1PROJECT_API UNavArea_Corner : public UNavArea
{
	GENERATED_UCLASS_BODY()
	
};
