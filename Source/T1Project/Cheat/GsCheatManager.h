// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define  TEST_SPAWN true

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "T1Project.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Container/GsSharedArray.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Option/GsGameUserSettings.h"
#include "Tickable.h"

#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsCheatManager.generated.h"

/**
 *
 */
class UGsCheatTestObjcet;
class UGsGameObjectBase;
class UTextBlock;

const float PLAYER_MIGHTY_WALK_SPEED = 3000.0f;


UCLASS()
class T1PROJECT_API UGsCheatManager : public UCheatManager
{

	GENERATED_BODY()

public:
	static bool IsPlayerMighty;
	static bool IsShowSpawnLocation;

	static bool IsShowCreatureDestLocation;

	static bool _isShowAnimURO;

	static bool _IsShowNaviMeshInfo;
	static bool _isShowZoneRange;
	static bool _isShowNpcTargetSightRadius;
	static bool _isShowNpcTargetInfo;

	static bool _isShowEnterCommands;
	static TArray<FString> _listCheatEnterCommands;

	static bool IsEffectEmptyPlayer;

public:
	TArray<UObject*> _listCheatGroup;
	FTimerHandle _handle;

private:
	class FGsCustomizeHandler* _customizeHandler = nullptr;
	class AGsCharacterPlayer* _testActor = nullptr;

	struct CustomizeHeightValue
	{
		float	Low = 0.0f;
		float	High = 0.0f;
	};
	CustomizeHeightValue _valueTest;
	CustomizeHeightValue _valueSpine2;

	struct HeightCalcProc
	{
		int CurrCount = 0;
		int	CurrProc = 0;
		CreatureGenderType TestGenderType;
	};
	HeightCalcProc	_processData;


public:
	virtual void InitCheatManager();
	virtual void BeginDestroy();
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor);

	// Localization util
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintUILocalization(FString InUIName);

	//bool SearchChildren(UWidget* InWidget, TArray<UTextBlock*> & InSearchTextBlocks);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowSpawnLocation(bool in_show);

	// 현재 위치 출력 (클라위치 기반)
	UFUNCTION(Exec, Category = ExecFunctions)
	void CurrLoc();

	UFUNCTION(Exec, Category = ExecFunctions)
	void LocalPlayerTargetClear();

	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintItemListLog();

	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintEquipedItemListLog();

	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintHiveProductInfo();

	// Camp
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void HitCampTest();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DestroyCamp();
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintCampAngle(bool InVal);

	// BM Shop
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void OpenBMShop();
	UFUNCTION(Exec, Category = ExecFunctions)
	void EditorZpay(bool Inval);
	UFUNCTION(Exec, Category = ExecFunctions)
	void SendProductTrans(FString InPid);
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetPurchaseCashCheatFlag(bool Inval);
	UFUNCTION(Exec, Category = ExecFunctions)
	void OnClickUserPgComplete();
	/** true 입력 시, 테이블 정보를 바탕으로 하이브상품데이터를 채움. false 는 모든 하이브상품데이터를 삭제함. */
	UFUNCTION(Exec, Category = ExecFunctions, Meta=(ToolTip="blah"))
	void FillHiveProductInfos(bool InFill);

	// UI
public:
	// Show/Hide UI
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetUIVisible(bool InVal = true);

	UFUNCTION(Exec, Category = ExecFunctions)
	void OpenUI(FString InUIPathTableKey);

	UFUNCTION(Exec, Category = ExecFunctions)
	void CloseUI(FString InUIPathTableKey);

	// npc 다이얼로그
	UFUNCTION(Exec, Category = ExecFunctions)
	void NpcDialogVelocity(uint32 inNpcDialogID, float inVelocity);

	UFUNCTION(Exec, Category = ExecFunctions)
	void NpcDialogTarget(uint32 inNpcDialogID, int32 npcTblId);

	UFUNCTION(Exec, Category = ExecFunctions)
	void StartEventDialog(int In_targetNpcTblId, int In_npcDialogId);

	UFUNCTION(Exec, Category = ExecFunctions)
	void GreetingOffDirect();


	UFUNCTION(Exec, Category = ExecFunctions)
	void SetActiveDivideDisplayStat(bool InIsOnActive);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetDivideDisplayValue(float InVal);

	//For quick test
public:
	// System
	UFUNCTION(Exec, Category = ExecFunctions)
	void GetRes();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetSignificanceDistanceForVFX(float inDist);

	//Navmesh
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowIsPlayerDestLocationValid(bool InDraw);

	//Height
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetPlayerOnLand();

	//Move
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowNpcDestLocation(bool inShow);

	static void DrawDebugCreatureDestLocation(const UGsGameObjectBase* inNpc, const FVector& inDestLocation);

	//Movie
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void PlayMovie(FString inName);
	UFUNCTION(Exec, Category = ExecFunctions)
	void StopMovie();
	UFUNCTION(Exec, Category = ExecFunctions)
	void CloseMovie();
public:
	// InType : ResurrectType
	UFUNCTION(Exec, Category = ExecFunctions)
	void PlayerRevive(int InType);

#if WITH_EDITOR
	//Effect test
public:

	// 스킬 사용 요청
	UFUNCTION(Exec, Category = ExecFunctions)
	void StartSkill(int skillId);

	// 전투, 평화 상태 설정
	UFUNCTION(Exec, Category = ExecFunctions)
	void ChangeBattleStatus(bool ApplyTarget = false);

	// 무기 변경(클래스 체인지)
	UFUNCTION(Exec, Category = ExecFunctions)
	void ChangeWeaponType(CreatureWeaponType Type);

	// 소셜 상태 이벤트 전송 
	// TId == 0 : 내캐릭터 소셜 이벤트
	// (내캐릭터에서 가장 가까운 NPC)
	UFUNCTION(Exec, Category = ExecFunctions)
	void SocialStateEvent(int TId, int SocialId);
	UFUNCTION(Exec, Category = ExecFunctions)
	void StopSocialState(int TId);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ObjectCollisionDebugShape(bool IsShow);
	UFUNCTION(Exec, Category = ExecFunctions)
	void ObjectRootmotionDebugShape(bool IsShow);
#endif

	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowNpcState(int InInfoType, int InNpcId = 0);

public:
	// 로컬라이징 Culture 변경(ko, en, ...)
	UFUNCTION(Exec, Category = ExecFunctions)
	void ChangeCulture(const FString& in_culture);

	UFUNCTION(Exec, Category = ExecFunctions)
	void GetHWInfo();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestDespawnAll();

	UFUNCTION(Exec, Category = ExecFunctions)
	void TestDisconnect();
	// npc 디스폰(tbl id로)
	UFUNCTION(Exec, Category = ExecFunctions)
	void DespawnNpcByTblId(int In_tblId);
	UFUNCTION(Exec, Category = ExecFunctions)
	void DespawnNpcByClient(float inRadius);

#if WITH_EDITOR
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetNpcCapsule(FString inName, float inRadius, float inHalfHeight);
#endif

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowAnimURO(bool inIsShow);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetPhysicsAnimation(bool inOn);

	// 카메라
public:
	// 디버그 라인 그리기
	UFUNCTION(Exec, Category = ExecFunctions)
	void CameraDebugLine();

#if WITH_EDITOR
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetTriggerVisible(bool inVisible = true);

	UFUNCTION(Exec, Category = ExecFunctions)
	void ActiveTargetRotateCamera(FString inCameraName, bool inInit = true);
	UFUNCTION(Exec, Category = ExecFunctions)
	void DeactiveTargetRotateCamera(FString inCameraName);
#endif

	// 테스트 hit 대사 그리기
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawTestHitDialog(bool In_val);
	// 대사 연출 카메라 시작
	UFUNCTION(Exec, Category = ExecFunctions)
	void StartDialogCam(int In_viewDataId, int In_npcTblId, int In_addNpcTblId = 0);
	// 대사 연출 카메라 끝
	UFUNCTION(Exec, Category = ExecFunctions)
	void EndDialogCam();
	// 대사 연출 카메라 변경
	UFUNCTION(Exec, Category = ExecFunctions)
	void ChangeDialogModeView(int In_viewDataId, int In_npcTblId, int In_addNpcTblId = 0);
	// 대사 스폰 위치
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowDialogSpawnPos(int In_targetNpcId, float In_rotate, float In_range);

	// 컷 페이드 사용하는가
	UFUNCTION(Exec, Category = ExecFunctions)
	void NpcDialogCutFadeOutUse(int In_use);

	// 카메라 본 관련 출력할지
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowCameraBoneLog(int In_use);

	// 인터랙션
public:
	// 범위 그릴지
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowInteractionRange(bool In_isvisible);

	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowTestInteractionButtonPos(bool In_val);

	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowInteracionSphere(bool In_val);

	// 시간 관련
public:
	// 서버 시간 출력
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintServerTime();

	// 퀘스트 자동 이동
public:
	// 노드 위치 그릴지
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowWaypointNodePos(bool In_isDraw);
	// 노드 중간 으로 갈지
	UFUNCTION(Exec, Category = ExecFunctions)
	void WaypointIsCenterNode(bool In_isCenter);
	// 웨이포인트 테스트 위치로 이동
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestWaypointMove();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void WarpSpot(int32 inLevelID, int32 inSpotIndex);
	UFUNCTION(Exec, Category = ExecFunctions)
	void WarpCurrentLevelSpot(int32 inSpotIndex);
	UFUNCTION(Exec, Category = ExecFunctions)
	void CheckContinuityWarp(int32 inSouceMapId, int32 inDestMapId);

	// 서브다이얼로그
	UFUNCTION(Exec, Category = ExecFunctions)
	void OpenSubDialog(int32 inSubDialogId);

	// 챕터 오픈 티커
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowCapterOpenTicker(int64 in_questId);

	// 퀘스트 초기화
	UFUNCTION(Exec, Category = ExecFunctions)
	void QuestClear();

	UFUNCTION(Exec, Category = ExecFunctions)
	void CompleteQuest(int64 in_questId);

	UFUNCTION(Exec, Category = ExecFunctions)
	void GotoQuestSubNpc(int64 in_questId);

	UFUNCTION(Exec, Category = ExecFunctions)
	void GiveUpQuest(int32 inStoryId);

	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawQuestGoalSpot(bool inIsDraw);

	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintMainQuest(int inDepth = 3);

	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowCurrentQuestId();

	UFUNCTION(Exec, Category = ExecFunctions)
	void CompleteCurrentQuest();

	UFUNCTION(Exec, Category = ExecFunctions)
	void CompletAllObjective(bool isAll, int64 inQuestId);

	//퀘스트 Npc 근처에서 플레이어끼리 충돌을 끄기 위한 스피어의 반지름을 보기위한 치트 함수
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowQuestNpcIgnoreRange(bool inShow);

	//territory 
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawTownFence();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawSafeZoneFence();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawEnvFence();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawSoundFence();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawNoCampZoneFence();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawNonPKFence();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawArenaFence();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawSafeZone();

private:
	void DrawFenceInteranl(TerritoryType inType);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowFenceState();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawZone();
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawSpot();
	UFUNCTION(Exec, Category = ExecFunctions)
	void ClearSpot();
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowZoneRange();
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintIFFInfo();
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowNpcInfo();	
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintNpc();
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetNpcTargetSightRadius(float inRange);
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetNpcSightRadius(int64 inUniqueId, float inRange);
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowTargetNpcSightRadius();
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowQuestIndex();
	UFUNCTION(Exec, Category = ExecFunctions)
	void CheckNpcInfo();
	UFUNCTION(Exec, Category = ExecFunctions)
	void CheckPlayerWeaponInfo();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawBoxCollisionFloor(const FString& inName);

	// 환경, 시간연출(AreaEnv)
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetAreaEnvTime(float InHours);
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetAreaEnvTimeCycleDuration(float InMinutes);
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetEnvWeather(int32 InWeather);
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetEnvEvent(int32 InEnvEvent, bool bIsStart);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void PlaySequence(int inSequence);

	UFUNCTION(Exec, Category = ExecFunctions)
	void StopSequence();
public:
	// bak1210 : 맵이동중에는 서버와 클라이언트간 패킷을 전송하지 않는 규칙을 깰수 있는 치트키는
	//           서버치트키라도 오버라이드해서 처리해야 문제가 생기지 않는다.
	UFUNCTION(Exec, Category = ExecFunctions)
	void UseItem(int32 inItemid);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ReconnectTest();

	// fps
public:
	//FPS(누적형 활성화)
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowFPS();
	//FPS(누적형 초기화)
	UFUNCTION(Exec, Category = ExecFunctions)
	void ResetFPS();

	// auto play
public:
	// 돌아오기 타입 세팅
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetStartPosType(int In_type);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetAIAttackCheckRadius(bool In_show);

	//UFUNCTION(Exec, Category = ExecFunctions)
	//void SetAutoPotionRate(float In_rate);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetAutoLootType(int In_type);

	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowMovetoPosRemainDist(bool In_val);

	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowDebugPath(bool In_isDraw);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetIsDrawPosSphere(bool In_isDraw);

	UFUNCTION(Exec, Category = ExecFunctions)
	void AllowPartialPath(bool In_isAllow);
	// AUTO_OPTION_TEST
	// 매너 모드 세팅
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetMannerMode(int In_mode);

	// 수동 이동시 auto 유지 할지
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetNonAutoMoveKeepAuto(bool In_value);
	// AUTO_OPTION_TEST

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ChangeBodyLocalPlayer(int inShapeId);

	UFUNCTION(Exec, Category = ExecFunctions)
	void ChangeHitEffectLocalPlayer(FString inName);

	UFUNCTION(Exec, Category = ExecFunctions)
	void TestHitLocalPlayer();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetSoundEnvTimeline(int inTimeline);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetSoundEnvWeather(int inWeather);

	UFUNCTION(Exec, Category = ExecFunctions)
	void IsSoundSpeedScale(bool inValue);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void CancelInteraction();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintUI();

	UFUNCTION(Exec, Category = ExecFunctions)
	void ClearUI();

	UFUNCTION(Exec, Category = ExecFunctions)
	void ForceGC();

	UFUNCTION(Exec, Category = ExecFunctions)
	void ForceResourceUnLoad();

	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowGUObjectArry(bool IsShow);

	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintBlockUI();

	UFUNCTION(Exec, Category = ExecFunctions)
	void HideBlockUI();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetFairyZoomArg(float zoomMaxDistance, float zoomSecond);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetFairyTouchZoomArg(float touchDistanceForce, float zoomSecond);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetFairyRotateArg(float touchDistanceForce, float touchMaxLimit);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetMoveSpeed(int32 inSpeed = 10000);
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetAttackSpeed(int32 inSpeed = 10000);
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetCastSpeed(int32 inSpeed = 10000);
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetFastTest(bool inIsBerserker);

	UFUNCTION(Exec, Category = ExecFunctions)
	void TestMoveLocationNotify(int inNotifySetId);

	// input
public:
	// 터치 이동 관련 그릴지
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetDrawTouchMove(bool In_val);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetDrawDebugArrow(bool In_val);

public:
	// 친구
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetBuddyAdd(bool In_isAdd, int In_tapIndex, uint64 In_userDBId, FString In_userName);

	// 정탄
public:
	// 모든 플레이어 정탄 active
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetSpiritShotAllActive(bool In_active);
	// 길드
public:
	// InType : ResetTimeTicket
	UFUNCTION(Exec, Category = ExecFunctions)
	void ResetTimeContents(int32 InResetTimeTicket);

	// 뽑기
public:
	// 뽑기 창 열기
	UFUNCTION(Exec, Category = ExecFunctions)
	void OpenSummon(int In_type = 0, int In_countType = 0);

	UFUNCTION(Exec, Category = ExecFunctions)
	void OpenSummonCompose(int In_type = 0, int In_resultType = 0);

	UFUNCTION(Exec, Category = ExecFunctions)
	void OpenSummonWaitChange(int In_type = 0);
	// 뽑기 창 닫기
	UFUNCTION(Exec, Category = ExecFunctions)
	void CloseSummon();

	UFUNCTION(Exec, Category = ExecFunctions)
	void StartTestAutoRetry(bool In_active);

	// 스킬 효과 텍스트 테스트
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void EffectTextSkill(int32 InSkillId, int32 InSkillLevel);
	UFUNCTION(Exec, Category = ExecFunctions)
	void EffectTextSkillNotify(int32 InSkillNotifyId, int32 InLevel = -1);
	UFUNCTION(Exec, Category = ExecFunctions)
	void EffectTextPassivity(int32 InPassivityId);
	UFUNCTION(Exec, Category = ExecFunctions)
	void EffectTextAbnormality(int32 InAbnormalityId, int32 InLevel = -1);

	UFUNCTION(Exec, Category = ExecFunctions)
	void GatherEffectTextSkill(int32 InSkillId, int32 InSkillLevel);

	// Cron 식 표현 확인
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintCron(const FString& in_value);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void CrashTest();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void CommandSet(const FString& inCommand);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowVersion();

	// 시공의 틈새 지도
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void AddSpaceCrackIcon(int In_id);
	UFUNCTION(Exec, Category = ExecFunctions)
	void DelSpaceCrackIcon(int In_id);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void OnDimmed(bool isOn);

	// 거래소 아이템 바로가기로 열기
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void OpenMarket(uint32 InItemId, int32 InOption);

	// 로비 연출 테스트
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void OnLobbyEnterGameTest();

	UFUNCTION(Exec, Category = ExecFunctions)
	void OnLobbyEnterGameClear();

	UFUNCTION(Exec, Category = ExecFunctions)
	void LobbyCamRotator();
	// lobby zoom power setting
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetLobbyZoomMobilePower(float In_value);

	// 경직 테스트
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void OnStiffen(float x, float y, float z);
	
	// 탑승 요청 시작
	UFUNCTION(Exec, Category = ExecFunctions)
	void OnVehicleStart();
	// 탑승 해제 요청
	UFUNCTION(Exec, Category = ExecFunctions)
	void OnReqVehicleCancel();

	// 퀘스트 id Show
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowQuestId(bool isShow);

	// 다운로드
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowDownloadSize(float isValue);

	UFUNCTION(Exec, Category = ExecFunctions)
	void OnClickEvent();

public:
	// 로컬 환경에서 Loop몽타주 플레이 테스트 지원
	UFUNCTION(Exec, Category = ExecFunctions)
	void LocalTestLoopMontagePlay();
public:
	// 서버TEST 지원용 치트
	//https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=251258440
	UFUNCTION(Exec, Category = ExecFunctions)
	void OnBlockOtherPlayer(bool isBlockOther);

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetLocalPlayerNetSenderInterval(float inValue);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestStartSkillPreview(bool Start = true);

	UFUNCTION(Exec, Category = ExecFunctions)
	void TestUseSkillPreview(int SkillId, int SkillLevel);

	UFUNCTION(Exec, Category = ExecFunctions)
	void TestCognitionTargetEffect();

	UFUNCTION(Exec, Category = ExecFunctions)
	void OnIgnoreSkillAutoTarget(bool IsIgnore);

	UFUNCTION(Exec, Category = ExecFunctions)
	void OnRootmotionNotSync(bool IsNotsync);

public:
	// 절전 모드
	UFUNCTION(Exec, Category = ExecFunctions)
	void StartSaveBattery();
	UFUNCTION(Exec, Category = ExecFunctions)
	void EndSaveBattery();
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetBrightness(float InBrightness);

	// 월드맵
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowRegionMapAndMovePopup(int In_mapId);
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowRegionMapTouchColor(int In_isShow);
	// 지도 땅찍고 이동 텍스쳐로 할지
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestRegionMapPickMoveTexture(int In_val);
	// 몬스터 정보 팝업과 지도 열기
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowMonsterInfoPopupAndMap( int InMapId,  int InCreatureId);

	//packectTEST
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SendPackectWarp(int32 warpId, int numSend);

	// exp test
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestExp(int In_isOn);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowRevive(ResurrectType inResurrectType);

public:
	// 컨텐츠 언락
	UFUNCTION(Exec, Category = ExecFunctions)
	void UnlockAllContents();
	UFUNCTION(Exec, Category = ExecFunctions)
	void UnlockAllContentsOnlyClient();
	UFUNCTION(Exec, Category = ExecFunctions)
	void UnlockReset();
	UFUNCTION(Exec, Category = ExecFunctions)
	void UnlockResetOnlyClient();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrinttHitMotionTarget();

	// 관전자 모드
public:
	// 근처 remote에 카메라 이동
	UFUNCTION(Exec, Category = ExecFunctions)
	void ObserveNearRemote();
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void WriteComment(FString InComment);
	// playable event
public:
	// clear cheat
	UFUNCTION(Exec, Category = ExecFunctions)
	void ClearPlayableEvent();

	// test enter play
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestPlayableEventEnter();
	// 테스트로 체크 포인트 바꾸기
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestCheckPointChange(int In_type);

public:
	// NPC 말풍선 출력 범위
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawDebugTalkBalloonRange(bool InShow);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawDebugGhost(int index = 0);

public:
	// UIScale 테스트
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestDPIScale(float InValue);

public:
	// 서버에서 보내준 네비 메시 좌표 정보
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowNaviMeshLog(int32 InShow);

	// 튜토리얼
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void StartTutorial(int32 InTutorialId);
	UFUNCTION(Exec, Category = ExecFunctions)
	void EndTutorial();
	UFUNCTION(Exec, Category = ExecFunctions)
	void NotSaveTutorial(bool bNotSave);

	// camera mode
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ChangeFixedQuarterYawMode();

	//benchmark
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestBenchmark();
	UFUNCTION(Exec, Category = ExecFunctions)
	void LoadArtLevel();
	UFUNCTION(Exec, Category = ExecFunctions)
	void UnloadArtLevel();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestReward(int64 InMainRewardId, int64 InSubRewardId = 0, int32 InQuestType = 0);
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestRandomReward(int64 InMainRewardId, int64 InSubRewardId = 0, int32 InBagIndex = 0, bool bIsSub = false, int32 InQuestType = 0);

	// 서버 Revision 및 캐릭터 위치 정보 출력
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowServerRevision(bool InShow);

	// scan button
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetScanDragDiff(float In_val);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SpawnParticleTest(float InInterval = 2.0f);
	void OnTestTimer();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowHPBar(bool In_isShow);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void OptionDevelopmentMode(bool In_isOn);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ClearEnterCommands();

	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowEnterCommands(bool In_isValue) { _isShowEnterCommands  = In_isValue;}

	static TArray<FString> GetEnterCommands();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetVoiceGender(bool IsFemail);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void OptionAlrtTest(int index);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void OpenUserContext(FString InUserName);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetHiddenLocalPlayer(bool show);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestTicker(FString In_text);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowLocalizeText(FString InNameSpace, FString InTextKey);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowArenaResult(int index);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void TestErrorMessage(int index);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void DrawDebugProp();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetIsShowCurrentWarpPos(bool In_val);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetDrawKillCenterMove(bool In_val);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void DeleteAccountTest();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void GameFocusedTest(bool In_val);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void WindowSizeTest();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void PacketLaytency();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SequencePause(bool In_IsPause);
	UFUNCTION(Exec, Category = ExecFunctions)
	void MoviePause(bool In_IsPause);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void UpdateCustomizeHeightData(int gender);

#ifdef WITH_EDITOR
	void InternalCustomizeHeightData(CreatureGenderType gender);
	void OnAnimWaitTimer();
#endif

	UFUNCTION(Exec, Category = ExecFunctions)
	void WindowScreenMode();

	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowDeviceManagement();

	//quest board
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintQuestBoard();

	
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void RemoveMyFace();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintDropobject();

	// 대륙맵 테스트
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowContinentalDungeonEnter(int In_mapId);

	// 기기 등록 정보 변경 (테스트용)
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetUnlockRegisterDevice(bool InIsRegist);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetSoundMix(EGsSoundMixType inMixType);
	UFUNCTION(Exec, Category = ExecFunctions)
	void PrintPushedSoundMix();
	UFUNCTION(Exec, Category = ExecFunctions)
	void PopSoundMix(EGsSoundMixType inMixType);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void AllCollectionComplete();

	//모바일에서만 작동하는 함수 에디터에서 치트로 재현
public:
	//모바일에서 백그라운드로 들어갈 때 호출 하는 함수
	UFUNCTION(Exec, Category = ExecFunctions)
	void ApplicationWillEnterBackground();
	//모바일에서 포그라운드로 들어갈 때 호출 하는 함수
	UFUNCTION(Exec, Category = ExecFunctions)
	void ApplicationHasEnteredForeground();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void Background(bool inValue);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ReturnToLobby();

	UFUNCTION(Exec, Category = ExecFunctions)
	void GameReStart();

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void AccountDelete();

	UFUNCTION(Exec, Category = ExecFunctions)
	void SetForceDisconnectTime(float Time);


	// 필드 보스 디스폰 티커 테스트
public: 
	UFUNCTION(Exec, Category = ExecFunctions)
	void StartFieldBossDespawnTicker(uint32 In_fieldBossId);

	// ai 테스트
public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowAIReserveDebugList(bool In_show);

public:
	UFUNCTION(Exec, Category = ExecFunctions)
	void ShowEffectEmptyPlayer(bool in_show);

	// 별의 유산
public:
	// 테스트 데이터 만들지
	UFUNCTION(Exec, Category = ExecFunctions)
	void SetTestStarLegacy(bool In_val);
};