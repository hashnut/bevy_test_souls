// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameClientVersion.h"

UGsGameClientVersion::UGsGameClientVersion()
{
}

void UGsGameClientVersion::Initialize()
{
	for (EGsGameClientBranchType el : _BranchType)
	{
		_branchFlag[(uint8)el] = true;
	}
}

bool UGsGameClientVersion::CheckType(EGsGameClientBranchType Type)
{
	return _branchFlag[(uint8)Type];
}