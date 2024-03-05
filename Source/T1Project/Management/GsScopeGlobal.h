// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsScopeBase.h"
#include "GsScopeGlobal.generated.h"

/**
 * 공용 매니저 모음. 로비, 인게임에서 모두 사용되는 매니저 나열.
 */
UCLASS()
class T1PROJECT_API UGsScopeGlobal final : public UGsScopeBase
{
	GENERATED_BODY()

public:
	// UGsScopeGlobal 에서 관리할 Manager 타입 열거형
	// 순서는 임의로 조정하시면 안됩니다. 순서변경시 노티해주셔야합니다.(파트장)

	// 주의 !!!) 글로벌 객체는 FType부터 인덱싱되어 있습니다.
	enum EManagerType : uint8
	{		
		Net = 0,
		DataCenter,
		GameFlow,
		PhoneInfo,
		GameData,
		Guild,						
		OfflinePlay,
		BMShop,
		FObjectMgrMax,
		//-------------------------------------------------
		UObjectMgrStart = FObjectMgrMax,
		//-------------------------------------------------
		Path = UObjectMgrStart,
		Resource,
		Data,
		GGameObjects,
		Item,
		Skill,
		Ui,
		Level,
		Sequence,
		Sound,
		Patch,		
		Hive,
		Option,
		Localization, //주의: OptionManager보다 늦게 생성되어야 합니다
		Divergence,
		PopupSystem,
		Quest,
		Platform,

		UObjectMgrMax,
		
		// Max 위에다가 타입 추가하이소~
		Max = UObjectMgrMax
	};

	
public: // UGsScopeBase
	virtual void Create() override;
	virtual void Init() override;
	virtual void InitAfterResourceDownload() override;
};
