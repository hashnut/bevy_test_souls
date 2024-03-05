// Fill out your copyright notice in the Description page of Project Settings.

#include "GsInputBindingBase.h"

UGsInputBindingBase::UGsInputBindingBase()
{
}

UGsInputBindingBase::~UGsInputBindingBase()
{
}

void UGsInputBindingBase::Initialize()
{
}

void UGsInputBindingBase::Update(float Delta)
{
}

void UGsInputBindingBase::Finalize()
{
}

void UGsInputBindingBase::SetBinding(UInputComponent* input)
{
	_inputComponent = input;
}