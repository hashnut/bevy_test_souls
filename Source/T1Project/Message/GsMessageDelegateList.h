// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "T1Project.h"

/**
 * 등록된 Message Delegate 관리 클래스
 * 상속 받은 클래스 내에서만 호출 가능하도록 protected 선언
 */
template <typename tMessageHandler, typename tMessageId>
class T1PROJECT_API TGsMessageDelegateList
{
private:
	tMessageHandler* _handler;
	TArray<TPair<tMessageId, FDelegateHandle>>	_listDelegate;

public:
	virtual ~TGsMessageDelegateList() = default;

protected:
	void InitializeMessageDelegateList(tMessageHandler* MessageHandler)
	{
		// 예외 검사
		if (_listDelegate.Num() > 0)
		{
			// 기존에 바인딩 리스트가 삭제되지 않음
			GSLOG(Error, TEXT("Delegate Binding not Delete!"));
		}

		check(MessageHandler);
		// 기존에 바인딩된 정보가 남아있다면 제거
		RemoveAllMessageDelegate();
		_handler = MessageHandler;
	}

	virtual void FinalizeMessageDelegateList()
	{
		RemoveAllMessageDelegate();
	}

protected:
	void AddMessageDelegate(TPair<tMessageId, FDelegateHandle> DelegateHandle)
	{
		if (_handler)
		{
			_listDelegate.Emplace(DelegateHandle);
		}
		else
		{
			ensure(false);
			GSLOG(Error, TEXT("not found MessageHandler"));
		}
	}

	void RemoveMessageDelegate(tMessageId MessageId)
	{
		if (_handler)
		{
			TPair<tMessageId, FDelegateHandle>* findPair = _listDelegate.FindByPredicate(
				[MessageId](TPair<tMessageId, FDelegateHandle> pair)
				{
					return MessageId == pair.Key;
				});
			if (findPair)
			{
				_handler->Remove(*findPair);
				_listDelegate.Remove(*findPair);
			}
		}
	}

	virtual void RemoveAllMessageDelegate()
	{
		if (_handler)
		{
			for (TPair<tMessageId, FDelegateHandle>& el : _listDelegate)
			{
				_handler->Remove(el);
			}
			_listDelegate.Empty();
		}
	}
};


template <typename tMessageHandler1, typename tMessageId1, typename tMessageHandler2, typename tMessageId2>
class T1PROJECT_API TGsMessageDelegateListTwoParam : public TGsMessageDelegateList<tMessageHandler1, tMessageId1>
{
	using Super = TGsMessageDelegateList<tMessageHandler1, tMessageId1>;

private:
	tMessageHandler2* _handler2;
	TArray<TPair<tMessageId2, FDelegateHandle>>	_listDelegate2;

public:
	virtual ~TGsMessageDelegateListTwoParam() = default;

protected:
	void InitializeMessageDelegateList(tMessageHandler1* MessageHandler1, tMessageHandler2* MessageHandler2)
	{
		// 예외 검사
		if (_listDelegate2.Num() > 0)
		{
			// 기존에 바인딩 리스트가 삭제되지 않음 체크
			GSLOG(Error, TEXT("Delegate Binding not Delete!"));
		}

		Super::InitializeMessageDelegateList(MessageHandler1);
		check(MessageHandler2);
		_handler2 = MessageHandler2;
	}

protected:
	void AddMessageDelegate(TPair<tMessageId1, FDelegateHandle> DelegateHandle)
	{
		Super::AddMessageDelegate(DelegateHandle);
	}
	void AddMessageDelegate(TPair<tMessageId2, FDelegateHandle> DelegateHandle)
	{
		if (_handler2)
		{
			_listDelegate2.Emplace(DelegateHandle);
		}
		else
		{
			ensure(false);
			GSLOG(Error, TEXT("not found MessageHandler"));
		}
	}

	void RemoveMessageDelegate(tMessageId1 MessageId)
	{
		Super::RemoveMessageDelegate(MessageId);
	}
	void RemoveMessageDelegate(tMessageId2 MessageId)
	{
		if (_handler2)
		{
			TPair<tMessageId2, FDelegateHandle>* findPair = _listDelegate2.FindByPredicate(
				[MessageId](TPair<tMessageId2, FDelegateHandle> pair)
			{
				return MessageId == pair.Key;
			});
			if (findPair)
			{
				_handler2->Remove(*findPair);
				_listDelegate2.Remove(*findPair);
			}
		}
	}

	virtual void RemoveAllMessageDelegate() override
	{
		Super::RemoveAllMessageDelegate();
		if (_handler2)
		{
			for (TPair<tMessageId2, FDelegateHandle>& el : _listDelegate2)
			{
				_handler2->Remove(el);
			}
			_listDelegate2.Empty();
		}
	}
};