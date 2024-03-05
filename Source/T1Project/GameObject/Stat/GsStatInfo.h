// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/SecureVar/GsSecureVar.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

class FGsStatBase;
class FGsDisplayData;

/**
 *
 */
class T1PROJECT_API FGsStatInfo
{
	friend FGsStatBase;
	friend FGsDisplayData;

private:
	StatType _statType;
	int32 _maxStatValue;

public:
	FGsStatInfo()
	{
	}

	virtual ~FGsStatInfo()
	{
	}

protected:
	virtual void SetStatValue(int32 InValue);

private:
	void SetMaxStatValue(int32 InValue);
	void SetStatType(StatType InType);

public:
	virtual int32 GetStatValue() const;
	int32 GetMaxStatValue() const;
	StatType GetStatType() const;
};


class FGsStatInfoCommon final : public FGsStatInfo
{
private:
	int32 _statValue;

protected:
	void SetStatValue(int32 InValue) final;

public:
	int32 GetStatValue() const final;
};


class FGsStatInfoSecure final : public FGsStatInfo
{
private:
	FGsSecureInt32 _statValue;

protected:
	void SetStatValue(int32 InValue) final;

public:
	int32 GetStatValue() const final;
};
