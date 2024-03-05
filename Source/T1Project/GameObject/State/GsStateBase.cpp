// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateBase.h"

IGsStateBase::IGsStateBase()
{
}

IGsStateBase::~IGsStateBase()
{
}

bool IGsStateBase::Ready(UGsGameObjectBase* Owner)
{
	return true;
}

void IGsStateBase::Enter(UGsGameObjectBase* Owner)
{

}

void IGsStateBase::ReEnter(UGsGameObjectBase* Owner)
{

}

void IGsStateBase::Update(UGsGameObjectBase* Owner, float Delta)
{

}

void IGsStateBase::Exit(UGsGameObjectBase* Owner)
{

}

void IGsStateBase::IgnoreState(UGsGameObjectBase* Owner, IGsStateBase* ChangeState)
{

}

bool IGsStateBase::CanTransition(UGsGameObjectBase*, int StateID) const
{
	return true;
}

bool IGsStateBase::IsSameState(uint8 StateID) const
{
	return (GetStateID() == StateID);
}

bool IGsStateBase::IsSameState(EGsStateBase StateType) const
{
	return IsSameState((uint8)StateType);
}

bool IGsStateBase::IsMontagePlayState() const
{
	return GetBPStateName() == NAME_None;
}
