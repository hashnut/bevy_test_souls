#pragma once

#include "GsMessage.h"
#include "GsMessageLog.h"
#include "CoreMinimal.h"
#include "DelegateInstanceInterface.h"
#include "Delegates/DelegateBase.h"

//------------------------------------------------------------------------------
// 이벤트를 전달하기위한 기본 객체 클래스
// GSTMessage 와 같이 쓰인다.
//------------------------------------------------------------------------------
template <typename TMessageId, typename TParam>
class TGsMessageHandlerOneParam : public MessageHandlerLog<TMessageId>
{
	static_assert(TIsEnum<TMessageId>::Value, "TMessageId is not enumerator type");
	static_assert(
		TIsArithmetic<TParam>::Value || TIsLValueReferenceType<TParam>::Value || TIsRValueReferenceType<TParam>::Value || TIsPointer<TParam>::Value,
		"TParam can only support Arithmetic, LValue, RValue and Pointer types."
	);

	using TParamStoreType = typename TChooseClass<TIsRValueReferenceType<TParam>::Value, typename TRemoveReference<TParam>::Type, TParam>::Result;
	using TParamInvokeType = typename TChooseClass<TIsRValueReferenceType<TParam>::Value, typename TRValueToLValueReference<TParam>::Type, TParam>::Result;

public:
	DECLARE_EVENT_OneParam(TGsMessageHandlerOneParam, MessageDelegator, TParamInvokeType);

private:
	TMap<TMessageId, MessageDelegator>		_router;
	TArray<TGsMessage<TMessageId, TParamStoreType>>	_asyncMessage;
	FRWLock									_asyncRWLock;
	int										_lastMessage = -1;

	TArray<TPair<TMessageId, FDelegateHandle>> _asyncDelete;

public:
	TGsMessageHandlerOneParam() : MessageHandlerLog<TMessageId>() { }
	virtual ~TGsMessageHandlerOneParam() { RemoveAll(); }

public:
	virtual void RemoveAll() { _router.Empty(); }
	
public:
	TMap<TMessageId, MessageDelegator>& GetRouter() { return _router; }

public:
	//------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------
	template <typename UserClass, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddRaw(
		TMessageId MessageId, 
		UserClass* InUserObject,
		typename TMemFunPtrType<false, UserClass, void(TParamInvokeType, VarTypes...)>::Type InFunc,
		VarTypes... Vars	)
	{
		FDelegateHandle result = _router.FindOrAdd(MessageId).AddRaw(InUserObject, InFunc, Forward<VarTypes>(Vars)...);
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}


	//------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------
	template <typename UserClass, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddUObject(
		TMessageId MessageId,
		UserClass* InUserObject,
		typename TMemFunPtrType<false, UserClass, void(TParamInvokeType, VarTypes...)>::Type InFunc,
		VarTypes... Vars	)
	{
		FDelegateHandle result = _router.FindOrAdd(MessageId).AddUObject(InUserObject, InFunc, Forward<VarTypes>(Vars)...);
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------
	template<typename FunctorType, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddLambda(
		TMessageId MessageId, FunctorType&& InFunctor, VarTypes... Vars	)
	{
		FDelegateHandle result = _router.FindOrAdd(MessageId).AddLambda(InFunctor, Forward<VarTypes>(Vars)...);
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------
	template <typename UObjectTemplate, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddUFunction(
		TMessageId MessageId, UObjectTemplate* InUserObject, const FName& InFunctionName, VarTypes... Vars	)
	{
		FDelegateHandle result = _router.FindOrAdd(MessageId).AddUFunction(InUserObject, InFunctionName, Forward<VarTypes>(Vars)...);
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------
	template <typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddStatic(
		TMessageId MessageId,
		typename TBaseStaticDelegateInstance<void(TParamInvokeType), FDefaultDelegateUserPolicy, VarTypes...>::FFuncPtr InFunc,
		VarTypes... Vars )
	{
		FDelegateHandle result = _router.FindOrAdd(MessageId).AddStatic(InFunc, Forward<VarTypes>(Vars)...);
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------
	template <typename UserClass, typename FunctorType, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddWeakLambda(
		TMessageId MessageId, UserClass* InUserObject, FunctorType&& InFunctor, VarTypes... Vars )
	{
		FDelegateHandle result = _router.FindOrAdd(MessageId).AddWeakLambda(InUserObject, InFunctor, Forward<VarTypes>(Vars)...);
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

	//------------------------------------------------------------------------------------------------
	/* * Adds a shared pointer-based (fast, not thread-safe) member function delegate.
	 *
	 * Shared pointer delegates keep a weak reference to your object.
	 *
	 * @param	InUserObjectRef	User object to bind to
	 * @param	InFunc			Class method function address*/
	 //------------------------------------------------------------------------------------------------
	template <typename UserClass, typename... VarTypes>
	inline TPair<TMessageId, FDelegateHandle> AddSP(
		TMessageId MessageId,
		const TSharedRef<UserClass, ESPMode::Fast>& InUserObjectRef,
		typename TMemFunPtrType<false, UserClass, void(TParamInvokeType, VarTypes...)>::Type InFunc,
		VarTypes... Vars
	)
	{
		FDelegateHandle result = _router.FindOrAdd(MessageId).AddSP(InUserObjectRef, InFunc, Forward<VarTypes>(Vars)...);
		check(result.IsValid());
		return TPairInitializer<TMessageId, FDelegateHandle>(MessageId, result);
	}

public:

	//------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------
	virtual void Remove(const TPair<TMessageId, FDelegateHandle>& Handle)
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
	// 
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
	// 메세지 전송(Sync 용)
	//------------------------------------------------------------------------------------------------
	virtual void SendMessage(TMessageId MessageId, TParam inData, bool inShow = true)
	{
		if (const MessageDelegator* delegateFunc = _router.Find(MessageId))
		{
			_lastMessage = (int)MessageId;

			if (inShow == true)
			{
				MessageHandlerLog<TMessageId>::Log(MessageId);
			}

			if (delegateFunc->IsBound())
			{
				//delegateFunc->Broadcast(TIsPointer<TParam>::Value ? inData : Forward<TParam>(inData));
				delegateFunc->Broadcast(inData);
			}

			_lastMessage = -1;
		}		
#ifdef LOG_MESSAGE
		else
		{
			GSLOG(Warning, TEXT("[%d] Cant Find Router!!"), (int)MessageId);
		}
#endif
	}
	//------------------------------------------------------------------------------------------------
	// 메세지 전송(ASync 용) : 다른 Thread에서 메인쓰레드로 요청할때 (Hive, 다운로드, ThreadJob 등)
	// 큐에 메시지를 담고 Update에서 일괄호출하는 방식사용
	//------------------------------------------------------------------------------------------------
	virtual void SendMessageAsync(TMessageId MessageId, TParam inData) 
	{
		FRWScopeLock ScopeWriteLock(_asyncRWLock, SLT_Write);
		_asyncMessage.Emplace(TGsMessage<TMessageId, TParamStoreType>(MessageId, TIsPointer<TParam>::Value ? inData : Forward<TParam>(inData)));
	}

public:
	//------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------
	virtual void Update()
	{
		TArray<TGsMessage<TMessageId, TParamStoreType>> copyQueue;
		{
			FRWScopeLock ScopeReadLock(_asyncRWLock, SLT_ReadOnly);
			Swap(copyQueue, _asyncMessage); 
			//_asyncMessage가 멤버이므로 재초기화나 move 대신 swap 해준다. move 시에 바로 접근하면 문제있을 수 있음
			// copyQueue = MoveTemp(_asyncMessage); _asyncMessage = TArray<TMessageId>(); 해줘도 된다
		}

		for (TGsMessage<TMessageId, TParamStoreType>& message : copyQueue)
		{
			SendMessage(message.GetId(), (TIsPointer<TParam>::Value ? message.GetParam() : Forward<TParam>(message.GetParam())));
		}


		for (const TPair<TMessageId, FDelegateHandle>& handle : _asyncDelete)
		{
			Remove(handle);
		}

		_asyncDelete.Empty();
	}
};