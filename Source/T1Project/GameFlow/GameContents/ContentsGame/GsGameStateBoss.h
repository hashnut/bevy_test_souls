#pragma once
#include "CoreMinimal.h"

#define  USE_3D_BOSS false
#include "GsContentsGameBase.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageBoss.h"

#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedBossEnum.h"


/*
	지역맵 컨텐츠 코드
*/
class UGsUIPopupBossInfo;
class UGsUIPopupBossContribution;
class AGsActorBossObject;
class AGsCharacterNonPlayer;
class AGsActorBossScene;

struct IGsMessageParam;
struct FGsSchemaFieldBossData;
struct FGsSchemaNpcData;

enum class EGsBossTab
{
	FieldBoss,
	WorldBoss,
	None
};

/// <summary>
/// 보스 정보창의 매니저 역활
/// </summary>
class T1PROJECT_API FGsGameStateBoss : public FGsContentsGameBase
{	

private:	
	TWeakObjectPtr<class UGsUIPopupBossInfo> _popupBossInfoUI;
	TWeakObjectPtr<class UGsUIPopupBossContribution> _popupBossContribution;

	// 메시지(해지용)
private: 
	MsgUIHandleArray _uiDelegates;
	TArray<TPair<MessageBoss, FDelegateHandle>> _bossDelegates;	

private:	
	const struct FGsSchemaFieldBossData* _selectedBossData;
	const struct FGsSchemaFieldBossData* _selectedMutantData;
	bool _isShowMutant = false;	
	time_t _lastDeadTime = 0;
	EGsBossTab _selectBossType = EGsBossTab::FieldBoss;

private:
	const struct FGsSchemaWorldBossData* _selectWorldBossData;

private:
	const struct FGsSchemaFieldBossData* _savedBossData;
	const struct FGsSchemaFieldBossData* _savedMutantData;
	bool _savedIsShowMutant = false;
	EGsBossTab _savedBossType = EGsBossTab::FieldBoss;

private:
	const struct FGsSchemaWorldBossData* _saveWorldBossData;

public:
	FGsGameStateBoss();
	virtual ~FGsGameStateBoss();

	// 가상 함수
public:
	virtual void Enter() override;
	virtual void Exit() override;

	// 로직 함수
private:
	void RegisterMessages();
	void UnregisterMessages();

private:
	void OpenBossUI();
	void CloseBossUI();					

private:	
	void ClearSelect();	

	//select
public:
	void SelectFirstFieldBoss(bool inShowMutant = false);	
	void SelectedFieldBoss(FieldBossId inBossId = 0, bool inShowMutant = false);
	void SelectedBossByMap(int32 inLevelId, bool inShowMutant = false);	
	void InvalidateBoss();
	void InvalidateDeathTime();
	void SaveSelectBoss();
	void RecoverySelectBoss();
	void ClearSelectBossData();

public:	
	void SelectFirstWorldBoss();
	void SelectedWorldBoss(WorldBossId inWorldBossId);
	static void OpenSelectedWorldBoss(const MapId InMapId, const CreatureId InCreatureId);

	//set get
public:
	void SetSelectedBossData(const struct FGsSchemaFieldBossData* inSelectedBoss = nullptr){ _selectedBossData = inSelectedBoss;}
	const struct FGsSchemaFieldBossData* GetSelectedBossData(){return _selectedBossData;}
	void SetSelectedMutantData(const struct FGsSchemaFieldBossData* inSelectedMutant = nullptr) { _selectedMutantData = inSelectedMutant; }
	const struct FGsSchemaFieldBossData* GetSelectedMutantData() { return _selectedMutantData; }
	bool IsShowMutant(){return _isShowMutant;}
	void SetShowMutant(bool inShow = false){_isShowMutant = inShow;}
	void SetLastDeadTime(time_t inTime) {
		_lastDeadTime = inTime;
	}
	time_t GetLastDeadTime(){return _lastDeadTime;}
	bool TryGetFieldBossArray(TArray<const struct FGsSchemaFieldBossData*>& outFieldBossArray);
	TArray<const struct FGsSchemaWorldBossData*> GetWorldBossArray();

public:
	void SetSelectedWorldBossData(const struct FGsSchemaWorldBossData* inWorldBoss = nullptr) { _selectWorldBossData = inWorldBoss; }	
	const struct FGsSchemaWorldBossData* GetSelectedWorldBossData() { return _selectWorldBossData; }
	EGsBossTab GetSavedBossType() { return _savedBossType; }
	static FGsGameStateBoss* GetInstance();
};