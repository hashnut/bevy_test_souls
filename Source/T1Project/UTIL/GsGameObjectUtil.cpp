// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectUtil.h"
#include "T1Project.h"
#include "Components/CapsuleComponent.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"

#include "GameObject/GsSchemaShapeData.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectPlayer.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Stat/GsStatInfo.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Customize/GsCustomizeHandler.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCharacterBase.h"

#include "Guild/GsGuildPlayerData.h"

#include "DataSchema/Item/GsSchemaItemMaterialData.h"
#include "DataSchema/GameObject/GsSchemaCreatureRaceData.h"

#include "DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedBossEnum.h"
#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "UI/UILib/Define/GsUIDefine.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Animation/GsLobbyAnimInstanceState.h"
#include "Animation/GsAnimationDefine.h"

#include "UTIL/GsAnimation.h"

#include "Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Animation/AnimInstance.h"
#include "Classes/Animation/AnimMontage.h"
#include "Engine/Classes/Animation/AnimSequence.h"
#include "GsTableUtil.h"
#include "GsDir.h"

#include "Item/Data/GsItemStatData.h"


const FName LOBBY_SPAWN = TEXT("Spawn");

const FName LOBBY_M_IDLE_POSE = TEXT("A_PC_M_Customize_Pose_A");
const FName LOBBY_F_IDLE_POSE = TEXT("A_PC_F_Customize_Pose_A");

#if WITH_EDITOR
const uint32 CLIENT_SPAWN_INSTANCE_ID_MAX = 1048575;
#endif

#if WITH_EDITOR
bool UGsGameObjectUtil::TryGenerateClientSpawnGameId(EGsGameObjectType inType, int32 inIndex, int64& outGameId)
{
	if (inIndex > CLIENT_SPAWN_INSTANCE_ID_MAX)
	{
		return false;
	}

	int64 type = (((int64)inType) << 20);
	int64 index = (int64)inIndex;
	int64 clientGameId = (((int64)1) << 63);
	int64 gameId = ((type | index) | clientGameId);

	outGameId = gameId;

	return true;
}
#endif

EGsGameObjectType UGsGameObjectUtil::GetObjectType(NpcFunctionType inNpcFuncType)
{
	switch (inNpcFuncType)
	{
	case NpcFunctionType::SANDBAG:
		return EGsGameObjectType::Sandbag;
	case NpcFunctionType::CAMP:
		return EGsGameObjectType::Camp;
	case NpcFunctionType::SANCTUM:
	case NpcFunctionType::SANCTUM_NEXUS:
		return EGsGameObjectType::Sanctum;
	}

	return EGsGameObjectType::NonPlayer;
}

EGsGameObjectType UGsGameObjectUtil::GetObjectType(CreatureSpawnType inCreatureSpawnType)
{
	switch (inCreatureSpawnType)
	{
	case CreatureSpawnType::NPC:
		return EGsGameObjectType::NonPlayer;
	case CreatureSpawnType::PROP:
		return EGsGameObjectType::PropObject;	
	}

	return EGsGameObjectType::Base;
}

CreatureSpawnType UGsGameObjectUtil::GetObjectType(EGsGameObjectType inCreatureSpawnType)
{
	switch (inCreatureSpawnType)
	{
	case EGsGameObjectType::NonPlayer:
		return CreatureSpawnType::NPC;
	case EGsGameObjectType::PropObject:
		return CreatureSpawnType::PROP;
	}

	return CreatureSpawnType::NONE;
}

bool UGsGameObjectUtil::IsEnemy(UGsGameObjectBase* Target)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		return (player->CastGameObject<UGsGameObjectLocalPlayer>())->IsEnemy(Target);
	}
	return false;
}

bool UGsGameObjectUtil::CanTarget(UGsGameObjectBase* Target)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		return (player->CastGameObject<UGsGameObjectLocalPlayer>())->CanTarget(Target);
	}
	return false;
}

bool UGsGameObjectUtil::CanInteraction(UGsGameObjectBase* Target)
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		return (player->CastGameObject<UGsGameObjectLocalPlayer>())->CanInteraction(Target);
	}
	return false;
}

FVector UGsGameObjectUtil::GetLocalPlayerLocation()
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		return player->GetLocation();
	}

	return FVector::ZeroVector;
}

int16 UGsGameObjectUtil::GetLocalPlayerDirToAngle()
{
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		const FVector& dir = player->GetRotation().Vector();
		return FGsDir::dirToAngle(dir);
	}

	return 0;
}

TArray<TWeakPtr<struct FGsFence>> UGsGameObjectUtil::GetCurrentEnteredFence()
{
	TArray<TWeakPtr<struct FGsFence>> fenceList;
	if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(player))
		{
			if (FGsFenceHandler* handler = localPlayer->GetFenceHandler())
			{
				return handler->GetEnteredFenceArray();	
			}
		}
	}

	return fenceList;
}

FText UGsGameObjectUtil::GetNpcName(int32 inCreatureId)
{
	const FGsSchemaNpcData* npcData = UGsTableUtil::FindRowById<UGsTableNpcData, FGsSchemaNpcData>(inCreatureId);
	if (nullptr == npcData)
	{
		return FText::GetEmpty();
	}

	return npcData->nameText;
}

FText UGsGameObjectUtil::GetRaceText(CreatureRaceType inRace)
{	
	FText outText = FText::GetEmpty();
	
	const UGsTableCreatureRaceData* table = Cast<UGsTableCreatureRaceData>(FGsSchemaCreatureRaceData::GetStaticTable());
	if (nullptr == table)
	{
		return outText;
	}

	const FGsSchemaCreatureRaceData* row = nullptr;
	if (false == table->FindRowById(inRace, row))
	{
		return outText;
	}

	outText = row->name;
	return outText;
}
// 종족의 약점 재질(복수개 일수 있음)
FText UGsGameObjectUtil::GetRaceWeaknessItemMaterialText(CreatureRaceType inRace)
{
	// 없음 텍스트
	FText emptyText;
	FText::FindText(FILE_KEY_ITEM_MATERIAL, TEXT_KEY_ITEM_MATERIAL_NONE, emptyText);;
	const UGsTableCreatureRaceData* table = Cast<UGsTableCreatureRaceData>(FGsSchemaCreatureRaceData::GetStaticTable());
	if (nullptr == table)
	{
		return emptyText;
	}

	const FGsSchemaCreatureRaceData* row = nullptr;
	if (false == table->FindRowById(inRace, row))
	{
		return emptyText;
	}

	// 약점 재질이 한개도 없다면 빈 텍스트
	if (row->weaknessItemMaterial.Num() == 0)
	{
		return emptyText;
	}

	FText addText = FText::FromString(TEXT(", "));
	FText endText = FText::FromString(TEXT(""));
	FText format = FText::FromString("{0}{1}{2}");
	FText resVal;
	for (int i=0; i< row->weaknessItemMaterial.Num(); ++i)
	{
		ItemMaterial material = row->weaknessItemMaterial[i];

		bool isLast = ((row->weaknessItemMaterial.Num() - 1) == i) ? true : false;

		resVal = FText::Format(format, resVal , GetMaterialText(material) , (isLast ? endText : addText));
	}

	return resVal;
}

FText UGsGameObjectUtil::GetRankText(CreatureRankType inRace)
{
	FText gradeText;
	switch (inRace)
	{
	case CreatureRankType::BOSS:
		FText::FindText(TEXT("WorldMapText"), TEXT("MonBoss"), gradeText);
		break;
	case CreatureRankType::ELITE:
		FText::FindText(TEXT("WorldMapText"), TEXT("MonElite"), gradeText);
		break;
	case CreatureRankType::NAMED:
		FText::FindText(TEXT("WorldMapText"), TEXT("MonNamed"), gradeText);
		break;
	case CreatureRankType::RAID:
		FText::FindText(TEXT("WorldMapText"), TEXT("MonRaid"), gradeText);
		break;
	default:
		FText::FindText(TEXT("WorldMapText"), TEXT("MonNormal"), gradeText);
		break;
	}

	return gradeText;
}

FText UGsGameObjectUtil::GetOffensiveText(bool inAgrressive)
{
	FText offensiveText;
	if (inAgrressive)
	{
		FText::FindText(TEXT("WorldMapText"), TEXT("MonOffensive"), offensiveText);
	}
	else
	{
		FText::FindText(TEXT("WorldMapText"), TEXT("MonNonOffensive"), offensiveText);
	}

	return offensiveText;
}

FText UGsGameObjectUtil::GetMaterialText(ItemMaterial InType)
{
	FText emptyText;
	const UGsTableItemMaterialData* table = Cast<UGsTableItemMaterialData>(FGsSchemaItemMaterialData::GetStaticTable());
	if (nullptr == table)
	{
		return emptyText;
	}

	const FGsSchemaItemMaterialData* row = nullptr;
	if (false == table->FindRowById(static_cast<int32>(InType), row))
	{
		return emptyText;
	}

	return row->name;
}

FText UGsGameObjectUtil::GetMonsterElementalText(MonsterElement InType)
{
	FText findText;
	switch (InType)
	{
	case MonsterElement::FLAME:
		FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, ELEMENT_FLAME_TEXT_KEY, findText);
		break;
	case MonsterElement::FROST:
		FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, ELEMENT_FROST_TEXT_KEY, findText);
		break;
	case MonsterElement::HOLY:
		FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, ELEMENT_HOLY_TEXT_KEY, findText);
		break;
	case MonsterElement::DARK:
		FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, ELEMENT_DARK_TEXT_KEY, findText);
		break;
	// 없음
	case MonsterElement::NONE:
		FText::FindText(FILE_KEY_ELEMENT, TEXT_KEY_ELEMENT_NONE, findText);
		break;
	default:
		break;
	}
	return findText;
}

FText UGsGameObjectUtil::GetChannelText(BossSpawnChannel InType)
{
	FText findText;
	switch (InType)
	{
	case BossSpawnChannel::ALL:
		FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, CHANNEL_ALL_TEXT_KEY, findText);
	break;
	case BossSpawnChannel::EACH:
		FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, CHANNEL_EMPTY_TEXT_KEY, findText);
	break;
	case BossSpawnChannel::FIRST:
		FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, CHANNEL_1ST_TEXT_KEY, findText);
		break;
	default:
		break;
	}

	return findText;
}

EGsStateBase UGsGameObjectUtil::GetStatusType(NpcSpecialStatusType inStatusType)
{
	switch (inStatusType)
	{
	case NpcSpecialStatusType::STOP:
		return EGsStateBase::SpaceCrackIdle;
		break;
	}

	return EGsStateBase::None;
}

EGsAbnormalityEffectMainType UGsGameObjectUtil::GetAbnormalityMainType(AbnormalityEffectType inAbnormalityType)
{
	switch (inAbnormalityType)
	{
	case AbnormalityEffectType::OVERTIME_DAMAGE:
	case AbnormalityEffectType::OVERTIME_HOT:
	case AbnormalityEffectType::OVERTIME_SENSITIVE:
		return EGsAbnormalityEffectMainType::OVERTIME;

	case AbnormalityEffectType::CC_STUN:
	case AbnormalityEffectType::CC_SILENCE:
	case AbnormalityEffectType::CC_BIND:
	case AbnormalityEffectType::CC_CANNOT_WARP:
	case AbnormalityEffectType::CC_BURY:
	case AbnormalityEffectType::CC_KNOCK_BACK:
	case AbnormalityEffectType::CC_FEAR:
	case AbnormalityEffectType::CC_TAUNT:
	case AbnormalityEffectType::CC_MOVE_LOCATION:
	case AbnormalityEffectType::CC_POLYMORP:
	case AbnormalityEffectType::CC_BLOCKADE:
		return EGsAbnormalityEffectMainType::CC;

	case AbnormalityEffectType::STAT:
	case AbnormalityEffectType::STAT_DASH:
	case AbnormalityEffectType::STAT_IMMUNE:
	case AbnormalityEffectType::STAT_VENT:
		return EGsAbnormalityEffectMainType::STAT;
	
	case AbnormalityEffectType::UTIL_ON_REMOVE_EXE_SKILL_NOTIFY:
	case AbnormalityEffectType::UTIL_NOT_AI_CHANGE_PHASE:
	case AbnormalityEffectType::UTIL_IMMORTAL:
	case AbnormalityEffectType::UTIL_HIDING:
	case AbnormalityEffectType::UTIL_BERSERKER_RAGE:
	case AbnormalityEffectType::UTIL_POINTED_ARROW:
	case AbnormalityEffectType::UTIL_WEDGE_ARROW:
	case AbnormalityEffectType::UTIL_JAGGED_ARROW:
	case AbnormalityEffectType::UTIL_FINAL_HOWL:
	case AbnormalityEffectType::UTIL_BONUS_DAMAGE:
	case AbnormalityEffectType::UTIL_BUFF_COUNTER:
	case AbnormalityEffectType::UTIL_BURY_KEEP_TARGET:
	case AbnormalityEffectType::UTIL_CONFUSE:
	case AbnormalityEffectType::UTIL_DAMAGE_SHARE:
	case AbnormalityEffectType::UTIL_CREATE_DEPON_NPC:
	case AbnormalityEffectType::UTIL_HP_ABSORPTION:
	case AbnormalityEffectType::UTIL_MOUNT_VEHICLE:
		return EGsAbnormalityEffectMainType::UTIL;
	}

	return EGsAbnormalityEffectMainType::NONE;
}

// remote 내소속인가(파티, 기사단 소속 되어있는지 체크)
// 스캔, 락온에서 전용 사용
 bool UGsGameObjectUtil::GetIsMyCrewRemote(UGsGameObjectBase* In_remote)
{
	 if (In_remote == nullptr)
	 {
		 return false;
	 }

	 if (In_remote->GetObjectType() != EGsGameObjectType::RemotePlayer)
	 {
		 return false;
	 }

	// 내가 파티 소속인가
	if (GSParty()->GetIsInParty() == true)
	{		
		// 타겟이 파티 소속인가
		if (GSParty()->IsInPartyGameId(In_remote->GetGameId()) == true)
		{
			return true;
		}
		
	}

	// 로컬 오브젝트
	UGsGameObjectBase* localPlayerObjBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localPlayerObjBase == nullptr)
	{
		return false;
	}
	// 로컬 데이터
	FGsGameObjectDataPlayer* localPlayerData = localPlayerObjBase->GetCastData<FGsGameObjectDataPlayer>();
	if (localPlayerData == nullptr)
	{
		return false;
	}
	// 로컬 길드 데이터
	const FGsGuildPlayerData* localGuildData = localPlayerData->GetGuildData();

	// 타겟 데이터
	FGsGameObjectDataPlayer* targetPlayerData = In_remote->GetCastData<FGsGameObjectDataPlayer>();

	// 내가 길드 소속인가
	if (localGuildData != nullptr &&
		INVALID_GUILD_DB_ID != localGuildData->_guildDBId)
	{
		// 대상도 길드 소속인가				
		if (targetPlayerData != nullptr)
		{
			// 타겟 길드 데이터
			const FGsGuildPlayerData* targetGuildData = targetPlayerData->GetGuildData();
			if (targetGuildData != nullptr &&
				INVALID_GUILD_DB_ID != targetGuildData->_guildDBId)
			{
				// 같은 길드인가
				if (localGuildData->_guildDBId ==
					targetGuildData->_guildDBId)
				{
					return true;
				}
			}
		}
	}

	return false;
}

 // remote id가 내소속인가(파티, 기사단 소속 되어있는지 체크)
 // 매너 모드에서 전용 사용
 bool UGsGameObjectUtil::GetIsMyCrewRemoteByGameId(int64 In_gameId)
 {
	 UGsGameObjectBase* findRemote = GSGameObject()->FindObject(EGsGameObjectType::RemotePlayer, In_gameId);
	 if (findRemote == nullptr)
	 {
		 return false;
	 }

	 return GetIsMyCrewRemote(findRemote);
 }

 // 위젯의 이름 show/ hide 옵션 갱신 
 void UGsGameObjectUtil::UpdateWidgetShowNameOption(EGsGameObjectType In_type)
 {
	 TArray<UGsGameObjectBase*> findObjects =
		GSGameObject()->FindObjectArray(In_type);

	 for (auto iter: findObjects)
	 {
		 if (iter == nullptr)
		 {
			 continue;
		 }

		 iter->UpdateWidgetShowNameOption();
	 }
 }

 // widget hp bar show/ hide cheat option update
 void UGsGameObjectUtil::UpdateWidgetShowHPBar()
 {
	 TArray<UGsGameObjectBase*> findObjects =
		 GSGameObject()->FindObjectArray(EGsGameObjectType::Base);

	 for (auto iter : findObjects)
	 {
		 if (iter == nullptr)
		 {
			 continue;
		 }

		 iter->UpdateWidgetShowHPBar();
	 }
 }

 // 인터랙션 버튼 위젯  show/ hide 옵션 갱신 
 void UGsGameObjectUtil::UpdateWidgetShowInteractionButtonOption()
 {
	 TArray<UGsGameObjectBase*> findObjects =
		 GSGameObject()->FindObjectArray(EGsGameObjectType::NonPlayer);

	 for (auto iter : findObjects)
	 {
		 if (iter == nullptr)
		 {
			 continue;
		 }
		 UGsGameObjectNonPlayer* nonPlayer = Cast<UGsGameObjectNonPlayer>(iter);
		 if (nonPlayer == nullptr) 
		 {
			 continue;
		 }
		 nonPlayer->UpdateWidgetShowInteractionButtonOption();
	 }
 }

 // 로컬 타겟인가
 bool UGsGameObjectUtil::IsLocalTarget(UGsGameObjectBase* In_object)
 {
	 if (In_object == nullptr)
	 {
		 return false;
	 }

	 UGsGameObjectBase* localPlayerObjBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	 if (localPlayerObjBase == nullptr)
	 {
		 return false;
	 }

	 FGsTargetHandlerBase* targetHandler = localPlayerObjBase->GetTargetHandler();
	 if (targetHandler == nullptr)
	 {
		 return false;
	 }
	 UGsGameObjectBase* localTargetObj = targetHandler->GetTarget();
	 UGsGameObjectBase* localSecondTargetObj = targetHandler->GetSecondTarget();

	//https://jira.com2us.com/jira/browse/C2URWQ-4918
	// 옵션에서 이름 표시 껏을때
	// 1, 2차타겟 둘다 표시하게 처리
	 return (localTargetObj == In_object || localSecondTargetObj == In_object) ? true : false;
 }
 // 현재 로컬 mp rate 값 구하기
 float UGsGameObjectUtil::GetLocalMPRate()
 {
	 UGsGameObjectBase* localPlayerObjBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

	 if (localPlayerObjBase == nullptr)
	 {
		 return 0.0f;
	 }
	 UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(localPlayerObjBase);
	 if (localPlayer == nullptr)
	 {
		 return 0.0f;
	 }

	 const FGsStatInfo* mpInfo = localPlayer->GetCreatureStatInfoByType(StatType::MP);

	 if (mpInfo == nullptr)
	 {
		 return 0.0f;
	 }

	 float nowMP = mpInfo->GetStatValue();
	 float maxMP = mpInfo->GetMaxStatValue();

	 // 분모가 0이면 큰일이다
	 if (maxMP == 0.0f)
	 {
		 return 0.0f;
	 }

	 return nowMP / maxMP;
 }

 // 특정 bone 위치와 로컬의 차이 위치 구하기(z값, 인게임 카메라에서 사용할 값)
 float UGsGameObjectUtil::GetLocalBonePosDiff(AGsCharacterLocalPlayer* In_localChar, 
	 FName In_stateName,
	 FName In_boneName,
	 CreatureGenderType In_gender,
	 float In_time)
 {
	 if (In_localChar == nullptr)
	 {
		 GSLOG(Error, TEXT("localChar == nullptr"));
		 return 0.0f;
	 }

	 USkeletalMeshComponent* skeletalComponent = In_localChar->GetMesh();
	 if (skeletalComponent == nullptr)
	 {
		 return 0.0f;
	 }

	 UAnimInstance* AnimInstance = skeletalComponent->GetAnimInstance();
	 if (AnimInstance == nullptr)
	 {
		 return 0.0f;
	 }

	 UGsLobbyAnimInstanceState* lobbyAnimInstance = Cast<UGsLobbyAnimInstanceState>(AnimInstance);

	 if (lobbyAnimInstance == nullptr)
	 {
		 return 0.0f;
	 }
	 FName assetName = (In_gender == CreatureGenderType::MALE) ? 
		 FGsAnimationDefine::LOBBY_M_IDLE_POSE : FGsAnimationDefine::LOBBY_F_IDLE_POSE;
	 UAnimSequenceBase* animSequeunceBase =  lobbyAnimInstance->GetAnimSequence(In_stateName, assetName);
	 if (animSequeunceBase == nullptr)
	 {
		 return 0.0f;
	 }

	 UAnimSequence* animSequence = Cast<UAnimSequence>(animSequeunceBase);
	 if (animSequence == nullptr)
	 {
		 return 0.0f;
	 }

	 // 로비에서 선택 되었을때 모션이 있어서
	 // 본스케일 대기후(0.1초) 그시점의 본위치는 무기별로 다르다(맨손이 제일 낮게 나옴)
	 // 애님 시퀀스를 구해서 0초 대의 본위치를 구한다
	 FTransform findTransform = FGsAnimationUtil::GetBoneTransformFromSequence(
		 In_localChar,
		 skeletalComponent, animSequence, 
		 In_boneName, In_time);
	 FVector vecWorldConstant = findTransform.GetLocation();

	 if (FGsGameDataManager* dataManager = GGameData())
	 {
		 if (true == dataManager->GetIsShowTestCamBoneLog())
		 {

			 const FString showLog =
				 FString::Printf(TEXT("vecWorldConstant.Z: %f"),
					 vecWorldConstant.Z);

			 GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *showLog);	

			 GSLOG(Error, TEXT("[ShowTestCamBoneLog] %s"), *showLog);
		 }
	 }

	 

	 // 카메라가 몸통중간에 달려있어서 몸체tr로 해봄
	 FVector relativePosition = In_localChar->GetTransform().InverseTransformPosition(vecWorldConstant);

	 float savedCameraTargetBoneHeight = relativePosition.Z;

	 return savedCameraTargetBoneHeight;
 }

 // a와 b bone 사이 거리 구하기
 float UGsGameObjectUtil::GetLocalTwoBonePosDist(
	 AGsCharacterBase* In_char,
	 FName In_boneNameA, FName In_boneNameB)
 {
	 if (In_char == nullptr)
	 {
		 GSLOG(Error, TEXT("In_char == nullptr"));
		 return 0.0f;
	 }

	 USkeletalMeshComponent* skeletalComponent = In_char->GetMesh();
	 if (skeletalComponent == nullptr)
	 {
		 return 0.0f;
	 }
	 
	// 필요한 본 위치 정보 얻기
	FVector vecSpaceConstantA = skeletalComponent->GetBoneLocation(In_boneNameA);
	FVector vecSpaceConstantB = skeletalComponent->GetBoneLocation(In_boneNameB);

	float dist = FVector::Dist(vecSpaceConstantA, vecSpaceConstantB);

	return dist;
 }

 bool UGsGameObjectUtil::GetStatDiff(const TArray<FGsItemStatData>& inSource, const TArray<FGsItemStatData>& inDest, OUT TMap<StatType, FGsStatDiffInfo>& outStatMap)
 {
	 outStatMap.Empty();
	 StatType statType;
	 int32 value;	 

	for(const FGsItemStatData& levelStatInfo : inSource)
	{
		statType = levelStatInfo.type.GetStatType();
		value = levelStatInfo.value;
		const FGsItemStatData* find = inDest.FindByPredicate([&](const FGsItemStatData& info)->bool{
				return statType == info.type.GetStatType();
			});

		if (find)
		{
			if (value != find->value)
			{
				outStatMap.Emplace(statType, FGsStatDiffInfo(value, find->value));
			}
		}
	}

	return outStatMap.Num() > 0;
 }

 FGsTargetHandlerLocalPlayer* UGsGameObjectUtil::FindTargetHandlerLocalPlayer()
 {
	 FGsTargetHandlerLocalPlayer* targetHandler = nullptr;
	 UGsGameObjectBase* localPalyer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	 if (localPalyer)
	 {
		 return localPalyer->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	 }

	 return nullptr;
 }

 FVector UGsGameObjectUtil::GetParticleEffectScale(UGsGameObjectBase* In_object)
 {
	 if (UGsDataContainManager* dataContainManager = GData())
	 {
		 const FVector2D& defaultSize = dataContainManager->GetGlobalData()->_defaultParticleCapsuleSize;
		 FGsGameObjectData* gameObjectData = In_object->GetData();
		 return FVector(gameObjectData->GetScaledCapsuleRadius() / defaultSize.X,
			 gameObjectData->GetScaledCapsuleHalfHeight() / defaultSize.Y, 1.f);
	 }
	 return FVector::OneVector;
 }

 FVector UGsGameObjectUtil::GetParticleEffectScale(AActor* In_Actor)
 {	 
	 if (UGsDataContainManager* dataContainManager = GData())
	 {
		 if (AGsCharacterBase* castActor = Cast<AGsCharacterBase>(In_Actor))
		 {
			 if (UCapsuleComponent* capsuleComponent = castActor->GetCapsuleComponent())
			 {
				 const FVector2D& defaultSize = dataContainManager->GetGlobalData()->_defaultParticleCapsuleSize;
				 return FVector(capsuleComponent->GetScaledCapsuleRadius() / defaultSize.X,
					 capsuleComponent->GetScaledCapsuleHalfHeight() / defaultSize.Y, 1.f);
			 }
		 }
	 }
	 return FVector::OneVector;
 }

 //---------------------------------------------------------------------------------------------
// 스켈레탈 메쉬 발의 높이를 캡슐컴포넌트 바닦높이와 일치하도록 상대좌표를 조절한다. (커스터마이즈 때문에  가변Size 임으로 계산을 통해 산출)
// 참고 : UGsCheatManager::InternalCustomizeHeightData 
// CreatureGenderType genderType : 성별정보(성별에 따라 메쉬 다리길이가 다름)
// float customHeight : 커스터마이즈 한 높이정보 (0~100)
// float capsuleHalfHeight : 캡슐컴포넌트의 반높이
// USkeletalMeshComponent* meshComponent : 조절할 대상 메쉬
//---------------------------------------------------------------------------------------------
 void UGsGameObjectUtil::MeshRelativeLocation(CreatureGenderType genderType, float customHeight,
	 float capsuleHalfHeight, AGsCharacterBase* actor)
 {
	 if (nullptr == actor)
	 {
		 return;
	 }

	 if ((genderType < CreatureGenderType::MALE) || (genderType > CreatureGenderType::FEMALE))
	 {
		 return;
	 }

	 float fOffSet = 0.0f;
	 float calcRatio = 0.0f;
	 bool bSmaller = true;

	 if (customHeight > 50)	// 크게 스케일
	 {
		 calcRatio = (customHeight - 50) / 50;
		 bSmaller = false;
	 }
	 else if (customHeight < 50) // 작게 스케일
	 {
		 calcRatio = (50 - customHeight) / 50;
	 }

	 // 비율계산이 있다면
	 if (customHeight != 50)
	 {
		 if (bSmaller)
		 {
			 if (genderType == CreatureGenderType::FEMALE)
			 {
				 fOffSet = GData()->GetGlobalData()->_FEMAILMinOffset * calcRatio;
			 }
			 else
			 {
				 fOffSet = GData()->GetGlobalData()->_MAILMinOffset * calcRatio;
			 }
		 }
		 else
		 {
			 if (genderType == CreatureGenderType::FEMALE)
			 {
				 fOffSet = GData()->GetGlobalData()->_FEMAILMaxOffset * calcRatio;
			 }
			 else
			 {
				 fOffSet = GData()->GetGlobalData()->_MAILMaxOffset * calcRatio;
			 }
		 }

		 fOffSet += capsuleHalfHeight;
		 actor->GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -fOffSet));
		 GSLOG(Warning, TEXT("AdjustCustomizeHeightAtMeshRelativeLocation Apply Offset [%.2f]"), -fOffSet);
	 }
 }

 void UGsGameObjectUtil::SaveCameraBoneHeight(CreatureGenderType genderType, float customHeight)
 {
	 float boneHeight =  GetCameraBoneHeight(genderType, customHeight);
	 if (FGsGameDataManager* dataManager = GGameData())
	 {
		 if (true == dataManager->GetIsShowTestCamBoneLog())
		 {
			 const FString showLog2 =
				 FString::Printf(TEXT("boneDist: %f"), boneHeight);

			 GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, *showLog2);

			 GSLOG(Error, TEXT("[ShowTestCamBoneLog] %s"), *showLog2);
		 }

		 dataManager->SetSavedCameraTargetBoneHeight(boneHeight);
	 }	
 }

 float UGsGameObjectUtil::GetCameraBoneHeight(CreatureGenderType genderType, float customHeight)
 {
	 float boneHeight = 0.0f;
	 float minVal = 0.0f;
	 float maxVal = 0.0f;
	 {
		 if (genderType == CreatureGenderType::FEMALE)
		 {
			 minVal = GData()->GetGlobalData()->_FEMAILMinBip001Spine;
			 maxVal = GData()->GetGlobalData()->_FEMAILMaxBip001Spine;
		 }
		 else
		 {
			 minVal = GData()->GetGlobalData()->_MAILMinBip001Spine;
			 maxVal = GData()->GetGlobalData()->_MAILMaxBip001Spine;
		 }
		 // make rate
		 float heightRate = customHeight * 0.01f;
		 boneHeight = FMath::Lerp(minVal, maxVal, heightRate);
	 }

	 return boneHeight;
 }

 float UGsGameObjectUtil::GetCameraBoneHeightByGameObjectPlayer(UGsGameObjectBase* In_gameObject)
 {
	 float boneHeight = 0.0f;
	 if (In_gameObject == nullptr)
	 {
		 return boneHeight;
	 }
	 UGsGameObjectPlayer* objectPlayer = Cast<UGsGameObjectPlayer>(In_gameObject);
	 if (objectPlayer == nullptr)
	 {
		 return boneHeight;
	 }

	 FGsGameObjectDataPlayer* playerData = objectPlayer->GetCastData<FGsGameObjectDataPlayer>();
	 if (playerData == nullptr)
	 {
		 return boneHeight;
	 }
	 CreatureGenderType gender = playerData->GetCreatureGenderType();

	 FGsCustomizeHandler* customizeHandler = objectPlayer->GetCustomizeHandler();
	 if (customizeHandler == nullptr)
	 {
		 return boneHeight;
	 }

	 const CustomizeBodyData bodyData =  customizeHandler->GetCustomizeBodyData();
	 return GetCameraBoneHeight(gender, bodyData.mHeight);
 }

 bool UGsGameObjectUtil::IsInputBlock(EGsKeyMappingType In_type)
 {
	 if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	 {
		 if (UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>())
		 {
			 if (UGsInputBindingLocalPlayer* inputBinder = local->GetInputBinder())
			 {
				 // 인풋이 막혀있으면 누르지 않음
				 if (inputBinder->IsInputBlock(In_type))
				 {
					 return true;;
				 }
			 }
		 }
	 }
	 return false;
 }

 bool UGsGameObjectUtil::IsInputFlagBlock(EGsKeyMappingInputFlags In_type)
 {
	 if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	 {
		 if (UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>())
		 {
			 if (UGsInputBindingLocalPlayer* inputBinder = local->GetInputBinder())
			 {
				 // 인풋이 막혀있으면 누르지 않음
				 if (inputBinder->IsInputFlagBlock(In_type))
				 {
					 return true;;
				 }
			 }
		 }
	 }
	 return false;
 }

 bool UGsGameObjectUtil::GetSpawnMeCompleteFinFlag()
 {
	 if (nullptr == GSGameObject())
	 {
		 return false;
	 }

	 if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	 {
		 if (UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>())
		 {
			 return local->GetSpawnMeCompleteFinFlag();
		 }
	 }
	 return false;
 }


 void UGsGameObjectUtil::SetSpawnMeCompleteFinFlag(bool InFlag)
 {
	 if (UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	 {
		 if (UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>())
		 {
			 local->SetSpawnMeCompleteFinFlag(InFlag);
		 }
	 }
 }