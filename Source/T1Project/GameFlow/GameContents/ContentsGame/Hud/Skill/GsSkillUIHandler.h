// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageCostume.h"

#include "Skill/GsSkillSlotData.h"
#include "GameObject/Skill/Checker/GsSkillCheckArray.h"
#include "UI/UILib/Define/GsUIDefine.h"

class UGsGameObjectLocalPlayer;
struct IGsMessageParam;
struct FGsItemMessageParamBase;
class FGsItem;
struct FGsSchemaItemCommon;

/**
 * HUD 스킬 버튼 로직 
 */
class T1PROJECT_API FGsSkillUIHandler
{
public:	
	using MsgItemHandle = TPair<MessageItem, FDelegateHandle>;
	using MsgCostumeHandle = TPair<MessageCostume, FDelegateHandle>;

	struct FGsAutoDelayData
	{
		SkillId _skillId = INVALID_SKILL_ID;
		bool _bUseAuto = false;
		float _remainSec = 0.f;

	public:
		bool Update(float InDeltaTime);
	};

protected:
	MsgGameObjHandleArray _msgGameObjectHandleList;
	MsgUIHandleArray _msgUIHandleList;
	MsgHudHandleArray _msgReserveSkillHandleList;
	TArray<MsgItemHandle> _msgItemHandleList;
	TArray<MsgCostumeHandle> _msgCostumeHandleList;

	TWeakObjectPtr<UGsGameObjectLocalPlayer> _localPlayer;
	TMap<SkillId, FGsAutoDelayData> _reqAutoMap;

	// 스킬 체커 등록
	// https://jira.com2us.com/jira/browse/CHR-19996
	FGsSkillCheckArray _skillChecker;

public: 
	static bool isSkillSlotSet;

public:
	FGsSkillUIHandler() = default;
	virtual ~FGsSkillUIHandler();

public:
	static EGsMinimapAreaType GetSaftyZoneType();
	static bool IsSaftyZone();
	static bool IsSaftyZone(UGsGameObjectLocalPlayer* InLocalPlayer);

public:
	void Init();
	void Close();
	void OnReconnection();
	void Update(float InDeltaTime);

private:
	void InitMessages();
	void RemoveMessages();

// private:
// 	bool IsNotEnoughCostStat(const class FGsSkill* InSkill, bool InPlayNotEnoughTickerMsg);

protected:
	// msgGameObject
	void OnLocalPlayerSpawnComplete(const IGsMessageParam*);
	void OnLocalRevive(const IGsMessageParam*);
	void OnClickDimmedSkillButton(const IGsMessageParam* InParam);

	// msgUI
	void OnReqSkillAuto(const IGsMessageParam* InParam);
	void OnInvalidateAquireItem(const IGsMessageParam* InParam);

	// msgReserveSkill
	void OnActiveSkill(int32 InSlotId);

	// msgItem : 스킬북으로 인한 레드닷 체크를 위해 아이템 메시지를 받음
	void OnAddItemList(FGsItemMessageParamBase& InParam);
	void OnRemoveItem(FGsItemMessageParamBase& InParam);
	void OnUpdateItem(FGsItemMessageParamBase& InParam);

	// msgCostume
	void OnUpdateCostume(const IGsMessageParam* InParam);

protected:
	void CheckSkillBook(bool bInForced);
	
	bool IsActiveSkillWindow() const; // 스킬창 상태인지	
	bool IsSkillIngredientItem(const FGsSchemaItemCommon* InTable) const; // 강화, 업그레이드, 습득 재료

	bool GetSkillIdBySkilllBook(FGsItem* InItem, OUT TArray<SkillId>& OutList);
};
