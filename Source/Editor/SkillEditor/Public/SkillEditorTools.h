// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

//~=============================================================================
// SkillEditorTools
// 해당 함수, 클래스들은 엔진, 에디터, 클라이언트, 서버, 기획 데이터등 변환, 수정 변환을하는
// 여러 동작들의 모임이다.
//~=============================================================================

#pragma once

struct FGsSkillSetData;
struct FGsCommonActionData;
struct FCompositeSection;
struct FGsDC_SkillSet;

class USkillEditorDataAsset;
class UCollisionSkillNotify;
class UAnimMontage;
class USkillNotify;

namespace SkillEditorTools
{
	SKILLEDITOR_API void MontageEditorDataToEditorData(USkillEditorDataAsset* SkillEditorDataAsset);
	SKILLEDITOR_API TArray<USkillNotify*> GetSkillNotifys(UAnimMontage* InAnimMontage);
}