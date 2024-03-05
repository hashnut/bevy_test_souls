// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimNotifies/AnimNotify.h"
//#include "Data/GsDC_UNotifyHitInfo.h"
#include "SkillNotifyData/SkillNotifyDataBase.h"
#include "SkillNotifyBase.generated.h"

//UCLASS()
//class T1PROJECT_API UGsSkillNotifyDataBase : public UObject
//{
//	GENERATED_BODY()
//
//public:
//	// 미리 지정해둔 타입 정보
//	// 상속받은 생성자에서 설정
//	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
//	EGsSkillNotifyType NotifyType;
//
//	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
//	int SkillNotifyId;
//
//	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
//		float Offset;
//
//	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
//		int RepeatCount;
//
//public:
//	UGsSkillNotifyDataBase() {};
//	virtual ~UGsSkillNotifyDataBase() = default;
//};


UCLASS(Abstract)
class USkillNotifyBase : public USkillNotify
{
	GENERATED_BODY()
	
public:
	//int32 SkillNotifyId;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillNotify)
	float Offset;*/

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKillNotifyData(Client)")
	//UGsSkillNotifyDataBase* NotifyData;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKillNotifyData")
	//USkillNotifyDataBase* NewNotifyData;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKillNotifyData")
	//UGsDC_UNotifyHitInfo* testNotifyData;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DataCenter SKillNotifyData")
	//struct FGsDC_NotifyHitInfo testNotifyData;
	//class UGsTable* DCNotifyData;
};
