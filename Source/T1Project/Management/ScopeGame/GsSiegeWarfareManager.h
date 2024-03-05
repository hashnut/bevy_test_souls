// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "SiegeWarfare/GsTaxDataHandler.h"
#include "Management/GsScopeHolder.h"


/**
 * 공성전 매니저. 
 * 정식 개발 전까지는 누적 세금 집계 정보 관리 용도로 사용
 */
class T1PROJECT_API FGsSiegeWarfareManager final : public IGsManager
{
private:
	FGsTaxDataHandler* _taxHandler = nullptr;	

public:
	FGsSiegeWarfareManager() = default;
	virtual ~FGsSiegeWarfareManager();

	// IGsManager override
public:	
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void InitializeAfterResourceDownload() override;

public:
	void ResetTimeContents(ResetTimeTicket InType);

public:
	FGsTaxDataHandler* GetTaxHandler() const { return _taxHandler; }
};

#define GSSiegeWarfare() UGsScopeHolder::GetGameManagerFType<FGsSiegeWarfareManager>(UGsScopeGame::EManagerType::SiegeWarfare)
