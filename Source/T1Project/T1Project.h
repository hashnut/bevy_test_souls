// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(T1Project, Log, All);

#define GS_LOG_ACTIVE	1 

#if GS_LOG_ACTIVE
#define GSLOG_CALLINFO								(FString(__FUNCTION__) + TEXT(" ----Line(") + FString::FromInt(__LINE__) + TEXT(")"))
#define GSLOG_S(Verbosity)							UE_LOG(T1Project,Verbosity, TEXT("----%s"), *GSLOG_CALLINFO)
#define GSLOG(Verbosity, Format, ...)				UE_LOG(T1Project, Verbosity, TEXT("----%s ----%s"), *GSLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define GS_MSSAGE_LOG(Verbosity, Format, ...)		UE_LOG(T1Project, Verbosity, TEXT("%s"), *FString::Printf(Format, ##__VA_ARGS__))
#define GSALOG(Verbosity, Format, ...)				UE_LOG(T1Project, Verbosity, TEXT("%s"), *FString::Printf(Format, ##__VA_ARGS__))
#else
#define GSLOG_S(Verbosity)							do{} while(0)
#define GSLOG(Verbosity, Format, ...)				do{} while(0)
#define GS_MSSAGE_LOG(Verbosity, Format, ...)		do{} while(0)
#define GSALOG(Verbosity, Format, ...)				do{} while(0)
#endif

// 붉은색 로깅
#define GSCHECK(Expr, ...)												\
	{																	\
		if (!(Expr))													\
		{																\
			GSLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'"));	\
			return __VA_ARGS__;											\
		}																\
	}

#define _QUOTE(X)	#X
#define QUOTE(X)	_QUOTE(X)
#define todo(MSG)	message(__FILE__ "(" QUOTE(__LINE__) ") : todo => " MSG) 

#define SCREEN_MESSAGE(key, time, color, Format, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(key, time, color, FString::Printf(TEXT(Format), __VA_ARGS__));}

// 서버 판정에 의한 Hit 이펙트 출력(클라 선판정 막기)
#define HIT_EFFECT_BY_SERVER

#define SAFE_DELETE(t)			\
{								\
	if (nullptr != t)			\
	{							\
		delete t;				\
		t = nullptr;			\
	}							\
}				


#define NAVIMESH_TEST_LOCATION_LOG


 /*
 클래스 순서

 class Typename : public parent
 {
   UE4 Macro
   각종 Macro
   friend 선언
   타입 별칭 선언

 private:
   멤버 변수들(Property)

 public:
   생성자
   소멸자

 public:
   static

 public:
   각종 가상 함수들

 public:
   Mutator 함수들(Logical)

 public:
   Getter/Setter
 }
 */