#pragma once

#include "GsMessage.h"
#include "GsMessageLog.h"
#include "CoreMinimal.h"
#include "Util/GsEnum.h"
#include "DelegateInstanceInterface.h"
#include "Delegates/DelegateBase.h"

//------------------------------------------------------------------------------
// 메시지(델리게이트) 객체
// 선언된 타입의 메시지를 전달하는 객체를 생성한다
// 사용예) GsMessageManager 참조
//------------------------------------------------------------------------------

template <typename TMessageId>
class TGsMessageHandler final : public MessageHandlerLog<TMessageId>
{
	static_assert(TIsEnum<TMessageId>::Value, "TMessageId is not enumerator type");
	DECLARE_EVENT(TGsMessageHandler, MessageDelegator);

private:
	TMap<TMessageId, MessageDelegator>				_router;

	TArray<TMessageId>								_asyncMessage;
	FRWLock											_asyncRWLock;

	int												_lastMessage = -1;
	TArray<TPair<TMessageId, FDelegateHandle>>		_asyncDelete;

public:	
	TGsMessageHandler() : MessageHandlerLog<TMessageId>() { }
	virtual ~TGsMessageHandler()					{ RemoveAll(); }

public:
	void RemoveAll()								{ _router.Empty(); }	

protected:
	TMap<TMessageId, MessageDelegator>& GetRouter() { return _router; }

	
public:
	//------------------------------------------------------------------------------------------------
	// 메시지 외부등록
	//------------------------------------------------------------------------------------------------
	template <typename UserClass, typename... ParamTypes, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddRaw(
		TMessageId MessageId,
		UserClass* InUserObject,
		typename TMemFunPtrType<false, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc,
		VarTypes... Vars
	)
	{		
		FDelegateHandle result;
		if (MessageDelegator* findFunc = _router.Find(MessageId))
		{
			result = findFunc->AddRaw(InUserObject, InFunc, Forward<VarTypes>(Vars)...);
		}
		else
		{
			typename TGsMessageHandler<TMessageId>::MessageDelegator delFunc;
			result = delFunc.AddRaw(InUserObject, InFunc, Forward<VarTypes>(Vars)...);
			_router.Add(MessageId, delFunc);
		}
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 메시지 외부등록
	//------------------------------------------------------------------------------------------------
	template <typename UserClass, typename... ParamTypes, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddUObject(
		TMessageId MessageId,
		UserClass* InUserObject,
		typename TMemFunPtrType<false, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc,
		VarTypes... Vars
	)
	{	
		FDelegateHandle result;
		if (MessageDelegator* findFunc = _router.Find(MessageId))
		{
			result = findFunc->AddUObject(InUserObject, InFunc, Forward<VarTypes>(Vars)...);
		}
		else
		{
			typename TGsMessageHandler<TMessageId>::MessageDelegator delFunc;
			result = delFunc.AddUObject(InUserObject, InFunc, Forward<VarTypes>(Vars)...);
			_router.Add(MessageId, delFunc);
		}
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 메시지 외부등록
	//------------------------------------------------------------------------------------------------
	template <typename FunctorType, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddLambda(TMessageId MessageId, FunctorType&& InFunctor, VarTypes... Vars)
	{
		FDelegateHandle result;
		if (MessageDelegator* findFunc = _router.Find(MessageId))
		{
			result = findFunc->AddLambda(InFunctor, Forward<VarTypes>(Vars)...);
		}
		else
		{
			typename TGsMessageHandler<TMessageId>::MessageDelegator delFunc;
			result = delFunc.AddLambda(InFunctor, Forward<VarTypes>(Vars)...);
			_router.Add(MessageId, delFunc);
		}
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 메시지 외부등록
	//------------------------------------------------------------------------------------------------
	template <typename UObjectTemplate, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddUFunction(
		TMessageId MessageId, UObjectTemplate* InUserObject, const FName& InFunctionName, VarTypes... Vars
	)
	{
		FDelegateHandle result;
		if (MessageDelegator* findFunc = _router.Find(MessageId))
		{
			result = findFunc->AddUFunction(InUserObject, InFunctionName, Forward<VarTypes>(Vars)...);
		}
		else
		{
			typename TGsMessageHandler<TMessageId>::MessageDelegator delFunc;
			result = delFunc.AddUFunction(InUserObject, InFunctionName, Forward<VarTypes>(Vars)...);
			_router.Add(MessageId, delFunc);
		}
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 메시지 외부등록
	//------------------------------------------------------------------------------------------------
	template <typename... ParamTypes, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddStatic(
		TMessageId MessageId,
		typename TBaseStaticDelegateInstance<void(ParamTypes...), FDefaultDelegateUserPolicy, VarTypes...>::FFuncPtr InFunc,
		VarTypes... Vars
	)
	{
		FDelegateHandle result;
		if (MessageDelegator* findFunc = _router.Find(MessageId))
		{
			result = findFunc->AddStatic(InFunc, Forward<VarTypes>(Vars)...);
		}
		else
		{
			typename TGsMessageHandler<TMessageId>::MessageDelegator delFunc;
			result = delFunc.AddStatic(InFunc, Forward<VarTypes>(Vars)...);
			_router.Add(MessageId, delFunc);
		}
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 메시지 외부등록
	//------------------------------------------------------------------------------------------------
	template <typename UserClass, typename FunctorType, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddWeakLambda(
		TMessageId MessageId, UserClass* InUserObject, FunctorType&& InFunctor, VarTypes... Vars
	)
	{
		FDelegateHandle result;
		if (MessageDelegator* findFunc = _router.Find(MessageId))
		{
			result = findFunc->AddWeakLambda(InUserObject, InFunctor, Forward<VarTypes>(Vars)...);
		}
		else
		{
			typename TGsMessageHandler<TMessageId>::MessageDelegator delFunc;
			result = delFunc.AddWeakLambda(InUserObject, InFunctor, Forward<VarTypes>(Vars)...);
			_router.Add(MessageId, delFunc);
		}
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------
	template <typename UserClass, typename... ParamTypes, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddSP(
		TMessageId MessageId,
		const TSharedRef<UserClass, ESPMode::Fast>& InUserObjectRef,
		typename TMemFunPtrType<false, UserClass, void(ParamTypes..., VarTypes...)>::Type InFunc,
		VarTypes... Vars
	)
	{
		FDelegateHandle result;
		if (MessageDelegator* findFunc = _router.Find(MessageId))
		{
			result = findFunc->AddSP(InUserObjectRef, InFunc, Forward<VarTypes>(Vars)...);
		}
		else
		{
			typename TGsMessageHandler<TMessageId>::MessageDelegator delFunc;
			result = delFunc.AddSP(InUserObjectRef, InFunc, Forward<VarTypes>(Vars)...);
			_router.Add(MessageId, delFunc);
		}
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

public:
	//------------------------------------------------------------------------------------------------
	// 삭제 
	//------------------------------------------------------------------------------------------------
	void Remove(const TPair<TMessageId, FDelegateHandle>& Handle)
	{
		if (ensure(Handle.Value.IsValid()) && _router.Num() != 0)
		{
			if (_lastMessage == (int)Handle.Key)
			{
				_asyncDelete.Add(Handle);
				return;
			}

			MessageDelegator* delegateFunc = _router.Find(Handle.Key);
			check(delegateFunc);
			delegateFunc->RemoveDelegateInstance(Handle.Value);
		}
	}

	//------------------------------------------------------------------------------------------------
	// 삭제 
	//------------------------------------------------------------------------------------------------
	void RemoveIF(const TPair<TMessageId, FDelegateHandle>& Handle)
	{
		if (ensure(Handle.Value.IsValid()) && _router.Num() != 0)
		{
			if (_lastMessage == (int)Handle.Key)
			{
				_asyncDelete.Add(Handle);
				return;
			}

			MessageDelegator* delegateFunc = _router.Find(Handle.Key);
			if (nullptr != delegateFunc)
			{
				delegateFunc->RemoveDelegateInstance(Handle.Value);
			}
		}
	}

public:
	//------------------------------------------------------------------------------------------------
	// 메시지 발동
	//------------------------------------------------------------------------------------------------
	virtual void SendMessage(TMessageId MessageId)
	{
		if (const MessageDelegator* delegateFunc = _router.Find(MessageId))
		{

			
			_lastMessage = (int)MessageId;
			
			MessageHandlerLog<TMessageId>::Log(MessageId);


			if (delegateFunc->IsBound())
			{
				delegateFunc->Broadcast();
			}

			_lastMessage = -1;
		}
#ifdef LOG_MESSAGE
		else
		{
			GSLOG(Warning, TEXT("[%s] [%s] Cant Find Router!!"), *_handlerName, (int)MessageId);
		}
#endif
	}
	//------------------------------------------------------------------------------------------------
	// 메세지 전송(ASync 용) : 다른 Thread에서 메인쓰레드로 요청할때 (Hive, 다운로드, ThreadJob 등)
	// 큐에 메시지를 담고 Update에서 일괄호출하는 방식사용
	//------------------------------------------------------------------------------------------------
	virtual void SendMessageAsync(TMessageId MessageId)
	{
		FRWScopeLock ScopeWriteLock(_asyncRWLock, SLT_Write);
		_asyncMessage.Add(MessageId);
	}

public:
	//------------------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------------------
	virtual void Update()
	{
		TArray<TMessageId> copyQueue;
		{
			FRWScopeLock ScopeReadLock(_asyncRWLock, SLT_ReadOnly);
			Swap(copyQueue, _asyncMessage); 
		}

		for (const TMessageId& messageId : copyQueue)
		{
			SendMessage(messageId);
		}

		//한틱에 들어온 삭제 메시지를 버퍼에 담아두고 업데이트에서 처리
		for (const TPair<TMessageId, FDelegateHandle>& handle : _asyncDelete)
		{
			Remove(handle);
		}

		_asyncDelete.Empty();
	}
};
