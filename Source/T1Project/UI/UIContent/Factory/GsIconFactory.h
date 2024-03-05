// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIControlLib/Define/GsIconDefine.h"
#include "UI/UIControlLib/Define/EGsIconType.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"

/**
 * 아이콘 생성 팩토리
 * - InIconType: 생성할 아이콘
 * - InIconKey: 추후 아이콘 Enum이 부족해질 때를 대비한 변수.
 * - InOwner: 아이콘 생성할 위젯(없으면 World)
 * - bInUsePool: Pool관리 받는 아이콘 생성.
 *
 * 주의: bInUsePool가 True일 경우 아이콘 Pool에 반환할 것
 */

class T1PROJECT_API FGsIconFactory
{
public:
	static class UGsUIIconBase* CreateIcon(EGsIconType InIconType, int32 InIconKey = 0, class UWidget* InOwner = nullptr, 
		bool bInUsePool = true);

	// 아이콘 만들기 (공통)
	static UGsUIIconBase* CreateIcon(FSoftObjectPath In_path);

    // 해제
    static void ReleaseIcon(UGsUIIconBase* In_icon);

	static void WarmUpIcon(EGsIconType InIconType, int32 InWarmUpCount);

private:
	// 풀에서 가져오기
	static UGsUIIconBase* Claim(UClass* In_class);
	static UGsUIIconBase* CreateCommonIcon(EGsIconType InIconType, class UWidget* InOwner, bool bInUsePool);
#if WITH_EDITORONLY_DATA
	static UClass* GetClassByObjectPath(const FSoftObjectPath& InPath);
#endif
};
