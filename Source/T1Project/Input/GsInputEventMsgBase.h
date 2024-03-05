#pragma once

#include "CoreMinimal.h"
#include "Message/MessageParam/GsMessageParam.h"

class T1PROJECT_API FGsInputEventMsgBase
{
public:
	FGsInputEventMsgBase();
	FGsInputEventMsgBase(uint32 In_Id) : Id(In_Id) {}
	virtual ~FGsInputEventMsgBase();

public:
	uint32 Id = 0;
};

class T1PROJECT_API FGsInputEventMsgLocation : public IGsMessageParam
{
public:
	FVector Pos;
	FVector Dir;

public:
	FGsInputEventMsgLocation(const FVector& InPos, const FVector& InDir) : Pos(InPos), Dir(InDir)
	{
		/**/
	}
};

/*
class T1PROJECT_API FGsInputEventMsgAction
{
public:
	int32 ID;
};
*/

class T1PROJECT_API FGsInputEventMsgTouchRelease : public FGsInputEventMsgBase
{
public:
	bool IsSimpleTouch;

public:
	FGsInputEventMsgTouchRelease(bool In_isSimpleTouch) :IsSimpleTouch(In_isSimpleTouch)
	{
	}
};

// Flag 를 활용할 수 있는 Input Event Message 타입 (템플릿 버전)
//template <typename TEnumFlag>
//class T1PROJECT_API TGsInputEventMsgFlag : public FGsInputEventMsgBase
//{
//public:
//	using uFlagType = __underlying_type(TEnumFlag);
//
//	static_assert(TIsEnum<TEnumFlag>::Value, "TGsMapping is not enumerator class");
//	static_assert(TAreTypesEqual<uFlagType, decltype(Id)>, "TEnumFlag's type should be same as that of Id");
//
//public:
//	TGsInputEventMsgFlag(TEnumFlag In_Flag) : Id(StaticCast<decltype(Id)>(In_Flag))
//	{
//	}
//};

/**
 * 인풋 블락을 담당하는 Flag 를 잠금 및 해제할 때 사용할 메시지 타입
 * 
 * @IsAdd : true 일 경우 블락을 추가, false 일 경우 추가된 녀석을 해제한다
 */
class T1PROJECT_API FGsInputEventMsgFlag : public FGsInputEventMsgBase
{
public:
	bool IsAdd = true;

public:
	FGsInputEventMsgFlag() = default;

	template<typename EnumFlag>
	explicit FGsInputEventMsgFlag(EnumFlag In_Flag, bool InIsAdd) : FGsInputEventMsgBase(StaticCast<uint32>(In_Flag)), IsAdd(InIsAdd)
	{
		static_assert(TIsEnum<EnumFlag>::Value, "EnumFlag is not enumerator class");
	}
	//FGsInputEventMsgFlag(uint32 In_Flag) : FGsInputEventMsgBase(In_Flag) {}

public:
	template<typename EnumFlag>
	void SetFlag(EnumFlag In_Flag)
	{
		static_assert(TIsEnum<EnumFlag>::Value, "EnumFlag is not enumerator class");
		Id = StaticCast<uint32>(In_Flag);
	}
};

class T1PROJECT_API FGsInputEventMsgWindowKey : public IGsMessageParam
{
public:
	int32 WindowKeyType;
	EInputEvent InputEvent;	

public:
	FGsInputEventMsgWindowKey() = default;
	FGsInputEventMsgWindowKey(int32 In_Type, EInputEvent In_InputEvent) :
		WindowKeyType(In_Type), InputEvent(In_InputEvent) {}
	virtual ~FGsInputEventMsgWindowKey() {}	
};