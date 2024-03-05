#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


class T1PROJECT_API FGsDungeonData
{
protected:
	const struct FGsSchemaDungeonData* _schemaData = nullptr;
	
	// 2023/2/21 PKT - 해당 던전에 대한 컨텐츠 언락을 검사.
	TFunction<bool(const FGsDungeonData*,bool)> _unlockContentsFunc = nullptr;	

	bool _isPrevUnlockState = false;

public:
	FGsDungeonData() = default;
	virtual ~FGsDungeonData() = default;

	explicit FGsDungeonData(const struct FGsSchemaDungeonData* InDungeonData
		, TFunction<bool(const FGsDungeonData*,bool)> InUnlockContentsFunc = nullptr);
public:
	bool IsUnLockState() const;
	bool IsUnLockContents(bool InShowTicker = false);

	// 2023/9/14 PKT - 운영툴등에 입장료가 조작(?)가능 함. 때문에 별도 함수 만듬.
	TTuple<CurrencyType,Currency> GetEnterdCurrencyData() const;

	const struct FGsSchemaDungeonData* GetSchema() const;
};