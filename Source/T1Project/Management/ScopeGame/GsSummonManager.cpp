#include "GsSummonManager.h"

#include "Fairy/GsFairyData.h"
#include "Costume/GsCostumeData.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemConsumeDetail.h"
#include "DataSchema/Item/GsSchemaItemEffectSummonFairy.h"
#include "DataSchema/Item/GsSchemaItemEffectSummonCostume.h"

#include "DataSchema/Fairy/GsSchemaFairySummon.h"
#include "DataSchema/Costume/GsSchemaCostumeSummon.h"
#include "DataSchema/Summon/GsSchemaSummonCostumeResPath.h"
#include "DataSchema/Summon/GsSchemaSummonConfig.h"
#include "DataSchema/Summon/GsSchemaSummonCostumeAppearResPath.h"

#include "DataSchema/Costume/GsSchemaCostumeProbability.h"

#include "ActorEx/GsActorSummonSceneObject.h"
#include "ActorEx/GsActorSummonFairy.h"
#include "ActorEx/GsActorSummonCostume.h"
#include "ActorEx/GsActorSummonComposeSceneObject.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "GsFairyManager.h"
#include "GsCostumeManager.h"

#include "Option/GsGameUserSettings.h"

#include "GameMode/GsGameModeTestClient.h"
#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"

#include "Summon/GsSummonCardData.h"
#include "Summon/GsSummonUnveilObjectInterface.h"

#include "Item/GsItem.h"

#include "Currency/GsCurrencyHelper.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "UTIL/GsTableUtil.h"

#include "Classes/Components/SceneComponent.h"

#include "Runtime/Engine/Classes/GameFramework/Character.h"

void UGsSummonManager::Initialize()
{
	const UGsTable* costumeAppearDataTbl = FGsSchemaSummonCostumeAppearResPath::GetStaticTable();

	if (nullptr == costumeAppearDataTbl)
	{
		return;
	}

	TArray<const FGsSchemaSummonCostumeAppearResPath*> rowsAppearData;
	costumeAppearDataTbl->GetAllRows<FGsSchemaSummonCostumeAppearResPath>(rowsAppearData);

	if (rowsAppearData.Num() == 0)
	{
		return;
	}

	for (const FGsSchemaSummonCostumeAppearResPath* iter : rowsAppearData)
	{
		if (nullptr == iter)
		{
			continue;
		}

		for (int iterId : iter->appearSequenceId)
		{
			if (_arrCostumeAppearCinematic.Contains(iterId) == true)
			{
				continue;
			}
			_arrCostumeAppearCinematic.Add(iterId);
		}
	}
}

void UGsSummonManager::Finalize()
{
	if (_arraySummonFairyData.Num() != 0)
	{
		_arraySummonFairyData.Empty();
	}

	if (_arraySummonCostumeData.Num() != 0)
	{
		_arraySummonCostumeData.Empty();
	}

	ClearCardOrderData();

	DespawnUnveilObjects();

	_localData = nullptr;
}

// 소환된 페어리 데이터 추가
void UGsSummonManager::SetSummonFairyData(TArray<TSharedPtr<FGsFairyData>> In_arrData)
{
	_arraySummonFairyData = In_arrData;

	_summonType = EGsSummonType::SUMMON_FAIRY;

	// 현재 마일리지 max 값 세팅
	if (const FGsSchemaSummonConfig* findTbl = FindSummonConfig())
	{
		_maxSummonMileage = findTbl->fairyMileage;
	}
}
// 소환된 페어리 데이터 추가
void UGsSummonManager::SetSummonCostumeData(TArray<TSharedPtr<FGsCostumeData>> In_arrData)
{
	_arraySummonCostumeData = In_arrData;

	_summonType = EGsSummonType::SUMMON_COSTUME;

	// 현재 마일리지 max 값 세팅
	if (const FGsSchemaSummonConfig* findTbl = FindSummonConfig())
	{
		_maxSummonMileage = findTbl->costumeMileage;
	}
}
// 카드 위치 정렬
// y위치로 정렬(큰게 좌측 부터)
void UGsSummonManager::SortCard(EGsSummonType In_summonType, TArray<USceneComponent*> In_arrayPosData)
{	
	ClearCardOrderData();
	
	// 카드 순서 데이터
	TArray<FGsSummonCardData*> arrayCardOrderByGrade;

	if (In_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		for (int i = 0; i < _arraySummonFairyData.Num(); ++i)
		{
			if (i >= In_arrayPosData.Num())
			{
				continue;
			}
			TSharedPtr<FGsFairyData> data = _arraySummonFairyData[i];
			arrayCardOrderByGrade.Add(
				new FGsSummonCardDataFairy(data, data->GetFairyGrade(), i));
		}
	}
	else if(In_summonType == EGsSummonType::SUMMON_COSTUME)
	{
		for (int i = 0; i < _arraySummonCostumeData.Num(); ++i)
		{
			if (i >= In_arrayPosData.Num())
			{
				continue;
			}
			TSharedPtr<FGsCostumeData> data = _arraySummonCostumeData[i];
			arrayCardOrderByGrade.Add(
				new FGsSummonCardDataCostume(data, data->GetCostumeGrade(), i));
		}
	}
	// 고등급의 위로
	arrayCardOrderByGrade.Sort([](FGsSummonCardData& lhs, FGsSummonCardData& rhs)
	{
		return lhs.IsMyGradeBigger(&rhs);
	});
	// 우측(내림차순)
	TArray<FGsSummonCardData*> arrayRight;

	bool isLeft = true;
	for ( int i =0;  arrayCardOrderByGrade.Num() > i; ++i)
	{
		FGsSummonCardData* cardData = arrayCardOrderByGrade[i];
		if (cardData == nullptr)
		{
			continue;
		}

		cardData->_cardOpenOrder = i;

		// 좌측(오름차순)
		if (isLeft == true)
		{
			// 제일 앞으로 넣음
			_arrayCardOrderData.Insert(cardData, 0);
		}
		// 우측(내림차순)
		else
		{
			arrayRight.Add(cardData);
		}
		isLeft = !isLeft;
	}
	// 우측이랑 합친다
	_arrayCardOrderData.Append(arrayRight);

}

void UGsSummonManager::SetCardData(EGsSummonType In_summonType)
{
	ClearCardOrderData();

	// 카드 순서 데이터
	TArray<FGsSummonCardData*> arrayCardOrderByGrade;

	if (In_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		for (int i = 0; i < _arraySummonFairyData.Num(); ++i)
		{
			TSharedPtr<FGsFairyData> data = _arraySummonFairyData[i];
			arrayCardOrderByGrade.Add(
				new FGsSummonCardDataFairy(data, data->GetFairyGrade(), i));
		}
	}
	else if (In_summonType == EGsSummonType::SUMMON_COSTUME)
	{
		for (int i = 0; i < _arraySummonCostumeData.Num(); ++i)
		{

			TSharedPtr<FGsCostumeData> data = _arraySummonCostumeData[i];
			arrayCardOrderByGrade.Add(
				new FGsSummonCardDataCostume(data, data->GetCostumeGrade(), i));
		}
	}

	for (int i = 0; arrayCardOrderByGrade.Num() > i; ++i)
	{
		FGsSummonCardData* cardData = arrayCardOrderByGrade[i];
		if (cardData == nullptr)
		{
			continue;
		}
		cardData->_cardOpenOrder = i;
		_arrayCardOrderData.Add(cardData);
	}
}

// 이동할 카메라 회전, 위치 구하기
void UGsSummonManager::GetCamRotAndPos(USceneComponent* In_targetComponent,
	const FVector& In_targetPos, FVector& Out_pos,
	FRotator& Out_rot, FVector& Out_lookTargetPos, float& Out_distCam)
{
	FVector lookTargetOffset = GData()->GetGlobalData()->_summonLookTargetOffset;

	FVector lookTargetPos = In_targetPos + lookTargetOffset;

	FRotator targetRot = FRotator::ZeroRotator;
	if (In_targetComponent != nullptr)
	{
		targetRot = In_targetComponent->GetRelativeRotation();
	}
	// 타겟 회전을 벡터로
	FVector targetRotVec = targetRot.Vector();

	float armLength = GData()->GetGlobalData()->_summonArmLength;

	FVector movePos = lookTargetPos + targetRotVec * armLength;
	Out_pos = movePos;

	FVector movedCamLookVec = lookTargetPos - movePos;
	movedCamLookVec.Normalize();

	Out_rot = movedCamLookVec.Rotation();
	Out_lookTargetPos = lookTargetPos;

	float distanceCam = FVector::Distance(Out_pos, Out_lookTargetPos);

	Out_distCam = distanceCam;
}

// 카드 정렬 데이터 삭제
void UGsSummonManager::ClearCardOrderData()
{
	if (_arrayCardOrderData.Num() != 0)
	{
		for (auto iter : _arrayCardOrderData)
		{
			if (iter == nullptr)
			{
				continue;
			}

			delete iter;
		}
		_arrayCardOrderData.Empty();
	}
}

// 테스트 클라이언트 모드인가
bool UGsSummonManager::GetIsTestClientMode()
{
	bool isTestClientMode = false;
#if WITH_EDITOR
	if (UWorld* world = GLevel()->GetWorld())
	{
		if (AGameModeBase* gameMode = world->GetAuthGameMode())
		{
			if (gameMode->IsA<AGsGameModeTestClient>())
			{
				isTestClientMode = true;
			}
		}
	}
#endif
	return isTestClientMode;
}

// 소환 핸들러 접근
FGsSummonHandler* UGsSummonManager::GetSummonHandler()
{
	FGsSummonHandler* summonHandler = nullptr;

	bool isTestClientMode = false;
#if WITH_EDITOR

	if (GLevel() == nullptr)
	{
		return summonHandler;
	}

	AGsGameModeTestClient* testClientMode = nullptr;
	if (UWorld* world = GLevel()->GetWorld())
	{
		if(AGameModeBase*  gameMode = world->GetAuthGameMode())
		{
			if (gameMode->IsA<AGsGameModeTestClient>())
			{
				isTestClientMode = true;

				testClientMode =
					Cast<AGsGameModeTestClient>(gameMode);
			}
		}
	}
#endif

	if (isTestClientMode == false)
	{
		if (GMode())
		{
			if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
			{
				if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
				{
					FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
					if (nullptr == hud)
					{
						return nullptr;
					}

					summonHandler = hud->GetSummonHandler();
					return summonHandler;
				}
			}
		}
	}
#if WITH_EDITOR
	else
	{
		if (testClientMode != nullptr)
		{
			summonHandler =
				testClientMode->GetTestClientSummonHandler();
		}
	}
#endif

	return summonHandler;
}

// 연출 스킵인가
bool UGsSummonManager::IsSummonSkip()
{
	bool isTestClientMode = false;
#if WITH_EDITOR
	AGsGameModeTestClient* testClientMode = nullptr;
	if (UWorld* world = GetWorld())
	{
		if (AGameModeBase* gameMode = world->GetAuthGameMode())
		{
			if (gameMode->IsA<AGsGameModeTestClient>())
			{
				isTestClientMode = true;

				testClientMode =
					Cast<AGsGameModeTestClient>(gameMode);
			}
		}
	}
#endif
	bool isSkip = false;
	if (isTestClientMode == false)
	{		
		if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
		{
			bool isComposeSkip = GetIsComposeSkipType();
			EGsOptionOutput optionType =  (isComposeSkip == true)? EGsOptionOutput::SUMMON_SKIP_COMPOSE: EGsOptionOutput::SUMMON_SKIP;
			const int32 intIsSkip =
				gameUserSettings->GetOutputSetting(optionType);

			isSkip = (bool)intIsSkip;
		}
	}
	else
	{
		isSkip = GData()->GetGlobalData()->_summonIsSkip;
	}
	return isSkip;
}

// 현재 소환 타입의 데이터 갯수
int UGsSummonManager::GetCurrentSummonTypeDataCount()
{
	int count = 0;

	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		count = _arraySummonFairyData.Num();
	}
	else
	{
		count = _arraySummonCostumeData.Num();
	}
	return count;
}

// 씬 오브젝트 스폰
// gc에서 코스튬 액터 바로 삭제해서 
// summon handler(일반클래스)에서 생성하던것을 uobject 매니저로 만든 summon manager에서 관리(uproperty)
AGsActorSummonSceneObject* UGsSummonManager::SpawnSceneObject(FVector& In_localPos)
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return nullptr;
	}


	// scene bp load
	FString summonSceneObjectPath =
		GData()->GetGlobalData()->_summonSceneObjectPath.ToString();
	summonSceneObjectPath += TEXT("_C");
	if (UClass* loadObjectC = LoadObject<UClass>(nullptr, *summonSceneObjectPath))
	{
		FTransform tr = FTransform::Identity;
		auto pos = In_localPos;		
		// 2,000,000 -> value make ray cast error
		// use 100,000 value
		pos.Z += GData()->GetGlobalData()->_summonRoomAddZPos;
		tr.SetLocation(pos);

		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AGsActorSummonSceneObject* actor = Cast<AGsActorSummonSceneObject>(world->SpawnActor(loadObjectC, &tr, spawnParam)))
		{
			_sceneObject = actor;
		}
	}

	return _sceneObject;
}
// 씬 오브젝트 디스폰
void UGsSummonManager::DespawnSceneObject()
{
	// 소환룸 삭제
	if (_sceneObject != nullptr)
	{
		_sceneObject->Finalize();
		_sceneObject->Destroy();
		_sceneObject = nullptr;
	}

	if (_composeSceneObject != nullptr)
	{
		_composeSceneObject->Finalize();
		_composeSceneObject = nullptr;
	}

	if (_actorSceneObject != nullptr)
	{
		_actorSceneObject->Destroy();
		_actorSceneObject = nullptr;
	}
}

// 페어리 최소 최대 등급 입력
void UGsSummonManager::SetFairyMinMaxGrade(FairyGrade In_minGrade, FairyGrade In_maxGrade)
{
	_arrayDragRandomParticleColor.Empty();

	int minGrade = (int)In_minGrade;
	int maxGrade = (int)In_maxGrade;
	int maxCount = (int)FairyGrade::MAX;
	for (int i=0; i<maxCount; ++i )
	{
		// 최소, 최대 조건에 맞을때
		if (i >= minGrade && i <= maxGrade)
		{
			FLinearColor linearColor =
				FGsUIColorHelper::GetColorSummonFairyGradeBG((FairyGrade)i);
			_arrayDragRandomParticleColor.Add(linearColor);
		}
	}
}
// 코스툼 최소 최대 등급 입력
void UGsSummonManager::SetCostumeMinMaxGrade(CostumeGrade In_minGrade, CostumeGrade In_maxGrade)
{
	_arrayDragRandomParticleColor.Empty();

	int minGrade = (int)In_minGrade;
	int maxGrade = (int)In_maxGrade;
	int maxCount = (int)CostumeGrade::MAX;
	for (int i = 0; i < maxCount; ++i)
	{
		// 최소, 최대 조건에 맞을때
		if (i >= minGrade && i <= maxGrade)
		{
			FLinearColor linearColor =
				FGsUIColorHelper::GetColorSummonCostumeGradeBG((CostumeGrade)i);
			_arrayDragRandomParticleColor.Add(linearColor);
		}
	}
}

// 랜덤 등급 컬러 구하기
FLinearColor UGsSummonManager::GetRandomGradeColor()
{
	int colorCount = _arrayDragRandomParticleColor.Num();
	if ( colorCount == 0)
	{
		return FLinearColor::White;
	}

	int randomIndex =
		FMath::RandHelper(colorCount);

	return _arrayDragRandomParticleColor[randomIndex];
}

// 페어리 소환 아이템 파티클 등급 세팅
void UGsSummonManager::SetFairySummonItemParticleGrade(int32 In_itemId)
{
	const FGsSchemaItemCommon* findTblItemCommon =
		UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(In_itemId);

	if (findTblItemCommon == nullptr)
	{
		return;
	}

	const FGsSchemaItemConsumeDetail* findTblItemConsumeDetail = 
		UGsTableUtil::FindRowById<UGsTableItemConsumeDetail, FGsSchemaItemConsumeDetail>(findTblItemCommon->detailId);

	if (findTblItemConsumeDetail == nullptr ||
		findTblItemConsumeDetail->effectIDList.Num() == 0)
	{
		return;
	}

	const FGsSchemaItemEffectSummonFairy* findTblItemEffectSummonFairy = 
		UGsTableUtil::FindRowById<UGsTableItemEffectSummonFairy, FGsSchemaItemEffectSummonFairy>(findTblItemConsumeDetail->effectIDList[0]);
	if (nullptr == findTblItemEffectSummonFairy
	|| nullptr == findTblItemEffectSummonFairy->fairySummonId.GetRow())
	{
		return;
	}

	const FGsSchemaFairySummon* findTblFairySummon = findTblItemEffectSummonFairy->fairySummonId.GetRow();		
	if (findTblFairySummon->probabilityList.Num() == 0)
	{
		return;
	}

	TArray<bool> gradeList = { false, false, false, false, false, false, false };

	for (auto& iter : findTblFairySummon->probabilityList)
	{
		if (const auto gradeProbability = iter.probabilityId.GetRow())
		{
			for (auto& e : gradeProbability->fairyList)
			{
				if (auto fairy = e.fairyId.GetRow())
				{
					auto fairyData = GSFairy()->GetFairyData(fairy->id);
					if (fairyData.IsValid())
					{
						int iGrade = static_cast<int>(fairyData.Pin()->GetFairyGrade());
						if (gradeList.IsValidIndex(iGrade))
						{
							gradeList[iGrade] = true;
						}
					}
				}
			}
		}
	}

	int intMinGrade = 0;
	int intMaxGrade = 0;

	int maxGrade = static_cast<int>(FairyGrade::MAX);
	for (int i = static_cast<int>(FairyGrade::RARE); i < maxGrade; i++)
	{
		if (gradeList[i])
		{
			if (intMinGrade == 0)
			{
				intMinGrade = i;
			}
			intMaxGrade = i;
		}
	}
	// 최고 등급이 레어밑이면 세팅할 필요 없음
	if (intMaxGrade == 0)
	{
		return;
	}

	// 등급 세팅
	SetFairyMinMaxGrade((FairyGrade)intMinGrade, (FairyGrade)intMaxGrade);
}

// 페어리 소환 합성 파티클 등급 세팅
void UGsSummonManager::SetFairySummonComposeParticleGrade(FairyGrade In_composeGrade)
{
	// 현재 합성등급이 매직 밑이거나(매직 성공시 레어이므로 드래그 연출이 있다)
	// 최대 등급이면 안함
	int intComposeGrade = (int)In_composeGrade;
	if (intComposeGrade < (int)FairyGrade::MAGIC ||
		In_composeGrade == FairyGrade::LEGEND)
	{
		return;
	}

	int nextGrade = intComposeGrade + 1;

	FairyGrade minGrade = In_composeGrade;
	// 최소가 매직이면 레어로 바꾼다
	// 드래그 연출은 레어부터이기때문에
	if (In_composeGrade == FairyGrade::MAGIC)
	{
		minGrade = FairyGrade::RARE;
	}

	SetFairyMinMaxGrade(minGrade, (FairyGrade)nextGrade);
}
// 페어리 소환 교체 파티클 등급 세팅
void UGsSummonManager::SetFairySummonWaitChangeParticleGrade(FairyGrade In_waitChangeGrade)
{
	// 최소 최대 같음
	SetFairyMinMaxGrade(In_waitChangeGrade, In_waitChangeGrade);
}
// 코스튬 소환 아이템 파티클 등급 세팅
void UGsSummonManager::SetCostumeSummonItemParticleGrade(int32 In_itemId)
{
	const FGsSchemaItemCommon* findTblItemCommon =
		UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(In_itemId);

	if (findTblItemCommon == nullptr)
	{
		return;
	}

	const FGsSchemaItemConsumeDetail* findTblItemConsumeDetail =
		UGsTableUtil::FindRowById<UGsTableItemConsumeDetail, FGsSchemaItemConsumeDetail>(findTblItemCommon->detailId);

	if (findTblItemConsumeDetail == nullptr ||
		findTblItemConsumeDetail->effectIDList.Num() == 0)
	{
		return;
	}

	const FGsSchemaItemEffectSummonCostume* findTblItemEffectSummonCostume
	= UGsTableUtil::FindRowById<UGsTableItemEffectSummonCostume, FGsSchemaItemEffectSummonCostume>(findTblItemConsumeDetail->effectIDList[0]);
	if (nullptr == findTblItemEffectSummonCostume
	|| nullptr == findTblItemEffectSummonCostume->costumeSummonId.GetRow())
	{
		return;
	}

	const FGsSchemaCostumeSummon* findTblCostumeSummon = findTblItemEffectSummonCostume->costumeSummonId.GetRow();	
	if (0 == findTblCostumeSummon->probabilityList.Num())
	{
		return;
	}

	TArray<bool> gradeList{ false, false, false, false, false, false, false };
	for (auto& iter : findTblCostumeSummon->probabilityList)
	{
		if (const auto gradeProbability = iter.probabilityId.GetRow())
		{
			for (auto& e : gradeProbability->costumeList)
			{
				if (auto costume = e.costumeId.GetRow())
				{
					auto costumeData = GSCostume()->GetCostumeData(costume->id);
					if (costumeData.IsValid())
					{
						int iGrade = static_cast<int>(costumeData.Pin()->GetCostumeGrade());
						if (gradeList.IsValidIndex(iGrade))
						{
							gradeList[iGrade] = true;
						}
					}
				}
			}
		}
	}

	int intMinGrade = 0;
	int intMaxGrade = 0;

	int maxGrade = static_cast<int>(CostumeGrade::MAX);
	for (int i = static_cast<int>(CostumeGrade::RARE); i < maxGrade; i++) //최소가 Rare
	{
		if (gradeList[i])
		{
			if (intMinGrade == 0)
			{
				intMinGrade = i;
			}
			intMaxGrade = i;
		}
	}

	// 최고 등급이 레어밑이면 세팅할 필요 없음
	if (intMaxGrade == 0)
	{
		return;
	}

	// 등급 세팅
	SetCostumeMinMaxGrade((CostumeGrade)intMinGrade, (CostumeGrade)intMaxGrade);
}
// 코스튬 소환 합성 파티클 등급 세팅
void UGsSummonManager::SetCostumeSummonComposeParticleGrade(CostumeGrade In_composeGrade)
{
	// 현재 합성등급이 매직 밑이거나(매직 성공시 레어이므로 드래그 연출이 있다)
	// 최대 등급이면 안함
	int intComposeGrade = (int)In_composeGrade;
	if (intComposeGrade < (int)CostumeGrade::MAGIC ||
		intComposeGrade == (int)CostumeGrade::MAX - 1) //Legend 가 생겼음
	{
		return;
	}

	int nextGrade = intComposeGrade + 1;

	CostumeGrade minGrade = In_composeGrade;
	// 최소가 매직이면 레어로 바꾼다
	// 드래그 연출은 레어부터이기때문에
	if (In_composeGrade == CostumeGrade::MAGIC)
	{
		minGrade = CostumeGrade::RARE;
	}

	SetCostumeMinMaxGrade(minGrade, (CostumeGrade)nextGrade);
}

// 코스튬 소환 교체 파티클 등급 세팅
void UGsSummonManager::SetCostumeSummonWaitChangeParticleGrade(CostumeGrade In_waitChangeGrade)
{
	// 최소 최대 같음
	SetCostumeMinMaxGrade(In_waitChangeGrade, In_waitChangeGrade);
}

AActor* UGsSummonManager::SpawnSceneObjectByType(EGsSummonSceneType In_sceneType,const FVector& In_localPos)
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return nullptr;
	}

	// same type scene exist
	// return exist scene object
	if (_sceneType == In_sceneType &&
	_actorSceneObject != nullptr)
	{
		return _actorSceneObject;
	}

	// 이전씬과 다르면 무조건 지우자
	// remove scene
	if (_actorSceneObject != nullptr &&
		In_sceneType != _sceneType)
	{
		DespawnSceneObject();
	}

	_sceneType = In_sceneType;


	// scene bp load
	FString sceneObjectPath;
	switch (In_sceneType)
	{
	case EGsSummonSceneType::SummonFairy:
		sceneObjectPath = GData()->GetGlobalData()->_summonFairySceneObjectPath.ToString();
		break;
	case EGsSummonSceneType::SummonCostume:
		sceneObjectPath = GData()->GetGlobalData()->_summonCostumeSceneObjectPath.ToString();
		break;
	case EGsSummonSceneType::Compose:
		sceneObjectPath = GData()->GetGlobalData()->_composeSceneObjectPath.ToString();
		break;
	case EGsSummonSceneType::HighGet:
		sceneObjectPath = GData()->GetGlobalData()->_highGetSceneObjectPath.ToString();
		break;
	case EGsSummonSceneType::HighPre:
		sceneObjectPath = GData()->GetGlobalData()->_highPreSceneObjectPath.ToString();
		break;
	}
		
	sceneObjectPath += TEXT("_C");
	AActor* actor = nullptr;
	if (UClass* loadObjectC = LoadObject<UClass>(nullptr, *sceneObjectPath))
	{
		FTransform tr = FTransform::Identity;
		auto pos = In_localPos;
		// 2,000,000 -> value make ray cast error
		// use 100,000 value
		pos.Z += GData()->GetGlobalData()->_summonRoomAddZPos;
		tr.SetLocation(pos);

		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		actor = Cast<AActor>(world->SpawnActor(loadObjectC, &tr, spawnParam));
	}

	if (In_sceneType == EGsSummonSceneType::Compose)
	{
		_composeSceneObject = Cast<AGsActorSummonComposeSceneObject>(actor);
	}


	_actorSceneObject = actor;	

	return actor;
}

// sequence reference
void UGsSummonManager::SummonAllObjects()
{
	DespawnUnveilObjects();

	int dataCount = GetCurrentSummonTypeDataCount();
	if (dataCount == 0)
	{
		return;
	}

	for (int i = 0; i < dataCount; ++i)
	{
		// 이미스폰된거면 안함
		if (_mapSummonUnveilObject.Contains(i) == true)
		{
			continue;
		}

		if (_summonType == EGsSummonType::SUMMON_FAIRY)
		{
			SummonFairy(i);
		}
		else
		{
			SummonCostume(i);
		}
	}

	MakeVeilObject();
}



void UGsSummonManager::SummonFairy(int In_dataIndex)
{
	TArray<TSharedPtr<FGsFairyData>> arrayData = GetSummonFairyData();

	if (arrayData.Num() == 0 ||
		arrayData.Num() <= In_dataIndex)
	{
		return;
	}

	TSharedPtr<FGsFairyData> fairyData = arrayData[In_dataIndex];

	if (fairyData.IsValid() == false)
	{
		return;
	}
	FairyGrade fairyGrade = fairyData->GetFairyGrade();
	int fairyId = fairyData->_fairyCommon->id;

	FString path =
		fairyData->GetSummonFairyPath().ToString();

	if (IGsSummonUnveilObjectInterface* unveilObject = SpawnUnveilObject(path, In_dataIndex))
	{
		if (AGsActorSummonFairy* fairy = static_cast<AGsActorSummonFairy*>(unveilObject))
		{
			fairy->SetGrade(fairyGrade);
			fairy->AttachGradeParticle();
		}
	}
}

void UGsSummonManager::SummonCostume(int In_dataIndex)
{
	TArray<TSharedPtr<FGsCostumeData>> arrayData = GetSummonCostumeData();

	if (arrayData.Num() == 0 ||
		arrayData.Num() <= In_dataIndex)
	{
		return;
	}

	TSharedPtr<FGsCostumeData> costumeData = arrayData[In_dataIndex];

	if (costumeData.IsValid() == false)
	{
		return;
	}
	const CostumeGrade costumeGrade = costumeData->GetCostumeGrade();
	int costumeId = costumeData->_costumeData->id;

	if (_localData == nullptr)
	{
		return;
	}

	CreatureGenderType genderType = _localData->GetCreatureGenderType();
#if WITH_EDITOR
	if (UWorld* world = GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::TestClient)
			{
				if (AGsGameModeTestClient* testMode = Cast<AGsGameModeTestClient>(gameMode))
				{
					genderType = testMode->_genderType;
				}
			}
		}
	}
#endif

	FString path;
	if (const FGsSchemaSummonCostumeResPath* summonCostume =
		UGsTableUtil::FindRowById<UGsTableSummonCostumeResPath, FGsSchemaSummonCostumeResPath>(
			genderType))
	{
		path = summonCostume->objectBPPath.ToString();
	}


	if (IGsSummonUnveilObjectInterface* unveilObject = SpawnUnveilObject(path, In_dataIndex))
	{
		if (AGsActorSummonCostume* costume = static_cast<AGsActorSummonCostume*>(unveilObject))
		{
			costume->Initialize(_localData, costumeData->_id, costumeGrade);
			costume->AttachGradeParticle();
		}

	}
}

void UGsSummonManager::MakeVeilObject()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	FString summonVeilPath = 
		(_summonType == EGsSummonType::SUMMON_FAIRY)?
		GData()->GetGlobalData()->_summonVeilFairy.ToString() :
		GData()->GetGlobalData()->_summonVeilCostume.ToString();
	// veil object spawn
	FString summonVeilLoadPath = summonVeilPath + TEXT("_C");


	if (UClass* unveilObjectC = LoadObject<UClass>(nullptr, *summonVeilLoadPath))
	{
		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* makeActor =
			world->SpawnActor(unveilObjectC, &FTransform::Identity, spawnParam);

		if (makeActor == nullptr)
		{
			return;
		}

		if (_summonType == EGsSummonType::SUMMON_FAIRY)
		{
			_veilObject =
				Cast<AGsActorSummonFairy>(makeActor);
		}
		else
		{
			_veilObject =
				Cast<AGsActorSummonCostume>(makeActor);
		}

		if (_veilObject == nullptr)
		{
			return;
		}


		// 처음 스폰하자마자 hide
		_veilObject->SetActorHiddenInGame(true);
	}
}

// 페어리 스폰
IGsSummonUnveilObjectInterface* UGsSummonManager::SpawnUnveilObject(
	FString In_path, int In_index)
{
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return nullptr;
	}

	IGsSummonUnveilObjectInterface* summonUnveilObject = nullptr;

	FString summonUnveilObjectPath = In_path += TEXT("_C");
	if (UClass* unveilObjectC = LoadObject<UClass>(nullptr, *summonUnveilObjectPath))
	{
		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* makeActor =
			world->SpawnActor(unveilObjectC, &FTransform::Identity, spawnParam);

		if (makeActor == nullptr)
		{
			return nullptr;
		}

		if (_summonType == EGsSummonType::SUMMON_FAIRY)
		{
			summonUnveilObject =
				Cast<AGsActorSummonFairy>(makeActor);
		}
		else
		{
			summonUnveilObject =
				Cast<AGsActorSummonCostume>(makeActor);
		}

		if (summonUnveilObject == nullptr)
		{
			return nullptr;
		}

		summonUnveilObject->SetDataIndex(In_index);

		_arraySummonUnveilObject.Add(makeActor);
		_mapSummonUnveilObject.Add(In_index, summonUnveilObject);


		// 처음 스폰하자마자 hide
		summonUnveilObject->SetHide(true);
	}

	return summonUnveilObject;
}
// 모든 페어리 디스폰
void UGsSummonManager::DespawnUnveilObjects()
{
	if (_mapSummonUnveilObject.Num() != 0)
	{
		for (auto iterUnveilObject : _mapSummonUnveilObject)
		{
			if (iterUnveilObject.Value == nullptr)
			{
				continue;
			}

			iterUnveilObject.Value->Finalize();
		}
		_mapSummonUnveilObject.Empty();
	}

	if (_arraySummonUnveilObject.Num() != 0)
	{
		for (auto iterUnveilActor : _arraySummonUnveilObject)
		{
			// 의심부분 수정
			//https://com2us-raon.sentry.io/issues/4160777455/?alert_rule_id=12817173&alert_timestamp=1683375144553&alert_type=email&environment=Development&project=4504115468566528&referrer=alert_email
			if (iterUnveilActor == nullptr)
			{
				continue;
			}
			iterUnveilActor->Destroy();
		}
		_arraySummonUnveilObject.Empty();		
	}

	if (_veilObject != nullptr)
	{
		_veilObject->Destroy();
		_veilObject = nullptr;
	}
}
AGsActorSummonFairy* UGsSummonManager::GetSummonFairyByIndex(int In_index, bool In_isUseVeilObj)
{
	IGsSummonUnveilObjectInterface* unveilObject =
		_mapSummonUnveilObject.FindRef(In_index);

	if (unveilObject == nullptr)
	{
		return nullptr;
	}
	AGsActorSummonFairy* fairy = static_cast<AGsActorSummonFairy*>(unveilObject);

	if (fairy == nullptr)
	{
		return nullptr;
	}

	if (true == fairy->IsHighGrade() &&
		true == In_isUseVeilObj)
	{
		AGsActorSummonFairy* higherFairy = static_cast<AGsActorSummonFairy*>(_veilObject);
		return higherFairy;
	}
	return fairy;
}

int UGsSummonManager::GetCostumeIdByIndex(int In_index)
{
	IGsSummonUnveilObjectInterface* unveilObject =
		_mapSummonUnveilObject.FindRef(In_index);

	if (unveilObject == nullptr)
	{
		return -1;
	}

	AGsActorSummonCostume* costume = static_cast<AGsActorSummonCostume*>(unveilObject);

	if (costume == nullptr)
	{
		return -1;
	}

	return costume->GetCostumeId();
}

bool UGsSummonManager::GetCostumeHighValByIndex(int In_index)
{
	IGsSummonUnveilObjectInterface* unveilObject =
		_mapSummonUnveilObject.FindRef(In_index);

	if (unveilObject == nullptr)
	{
		return false;
	}

	AGsActorSummonCostume* costume = static_cast<AGsActorSummonCostume*>(unveilObject);

	if (costume == nullptr)
	{
		return false;
	}

	return costume->IsHighGrade();
}

void UGsSummonManager::SetIsComposeSuccess(bool In_val)
{
	_isComposeSuccess = In_val;

	if (_isComposeSuccess == true)
	{
		float success1thRate = GData()->GetGlobalData()->_summonComposeSuccess_1thRate;
		bool isFirstResult = success1thRate >= FMath::FRandRange(0.0f, 1.0f);

		if (GData()->GetGlobalData()->_summonComposeResultSuccessSequenceId.Num() != 2)
		{
			return;
		}

		_composeResultSequenceId = (isFirstResult == true)?  
			GData()->GetGlobalData()->_summonComposeResultSuccessSequenceId[0] : 
			GData()->GetGlobalData()->_summonComposeResultSuccessSequenceId[1];
	}
	else
	{
		float success1thRate = GData()->GetGlobalData()->_summonComposeFail_1thRate;
		bool isFirstResult = success1thRate >= FMath::FRandRange(0.0f, 1.0f);

		if (GData()->GetGlobalData()->_summonComposeResultFailSequenceId.Num() != 2)
		{
			return;
		}

		_composeResultSequenceId = (isFirstResult == true) ?
			GData()->GetGlobalData()->_summonComposeResultFailSequenceId[0] :
			GData()->GetGlobalData()->_summonComposeResultFailSequenceId[1];
	}
}

// test result type setting:  0~3
void UGsSummonManager::SetTestComposeResult(int In_resultType)
{
	if (In_resultType >= 2)
	{
		_isComposeSuccess = false;
	}
	else
	{
		_isComposeSuccess = true;
	}

	if (_isComposeSuccess == true)
	{
		if (GData()->GetGlobalData()->_summonComposeResultSuccessSequenceId.Num() != 2)
		{
			return;
		}

		_composeResultSequenceId = (In_resultType == 0) ?
			GData()->GetGlobalData()->_summonComposeResultSuccessSequenceId[0] :
			GData()->GetGlobalData()->_summonComposeResultSuccessSequenceId[1];
	}
	else
	{
		if (GData()->GetGlobalData()->_summonComposeResultFailSequenceId.Num() != 2)
		{
			return;
		}

		_composeResultSequenceId = (In_resultType == 2) ?
			GData()->GetGlobalData()->_summonComposeResultFailSequenceId[0] :
			GData()->GetGlobalData()->_summonComposeResultFailSequenceId[1];
	}
}

bool UGsSummonManager::GetFairyHighValByIndex(int In_index)
{
	IGsSummonUnveilObjectInterface* unveilObject =
		_mapSummonUnveilObject.FindRef(In_index);

	if (unveilObject == nullptr)
	{
		return false;
	}
	AGsActorSummonFairy* fairy = static_cast<AGsActorSummonFairy*>(unveilObject);

	if (fairy == nullptr)
	{
		return false;
	}

	return fairy->IsHighGrade();
}

bool UGsSummonManager::GetFairysHighCardExist()
{
	bool isHighCardExist = false;
	int highGrade = (int)FairyGrade::RARE;
	for (int i = 0; i < _arraySummonFairyData.Num(); ++i)
	{		
		TSharedPtr<FGsFairyData> data = _arraySummonFairyData[i];

		FairyGrade fairyGrade = data->GetFairyGrade();

		if ((int)fairyGrade >= highGrade)
		{
			isHighCardExist = true;
			break;
		}
	}

	return isHighCardExist;
}
bool UGsSummonManager::GetCostumesHighCardExist()
{
	bool isHighCardExist = false;
	int highGrade = (int)CostumeGrade::RARE;
	for (int i = 0; i < _arraySummonCostumeData.Num(); ++i)
	{		
		TSharedPtr<FGsCostumeData> data = _arraySummonCostumeData[i];

		CostumeGrade costumeGrade = data->GetCostumeGrade();

		if ((int)costumeGrade >= highGrade)
		{
			isHighCardExist = true;
			break;
		}		
	}
	return isHighCardExist;
}

// bm product id로 group id 구하기
uint64 UGsSummonManager::GetGroupIdByProductId(BMProductId In_productId)
{
	uint64 findId = 0;
	if (true == _mapProductCachingData.Contains(In_productId))
	{
		findId = _mapProductCachingData.FindRef(In_productId);
	}
	else
	{
		FGsBMShopProductData* productData = GBMShop()->FindProduct(In_productId);
		if (productData == nullptr)
		{
			return findId;
		}

		int32 itemId =  productData->GetItemTID();

		const FGsSchemaItemCommon* itemTbl= FGsItem::GetTableDataByID(itemId);
		if (itemTbl == nullptr)
		{
			return findId;
		}

		if (itemTbl->categorySub != ItemCategorySub::SUMMON_COSTUME &&
			itemTbl->categorySub != ItemCategorySub::SUMMON_FAIRY)
		{
			return findId;
		}

		const FGsSchemaItemConsumeDetail* consumeDetail = 
			UGsTableUtil::FindRowById<UGsTableItemConsumeDetail, FGsSchemaItemConsumeDetail>(itemTbl->detailId);
		
		if (consumeDetail == nullptr)
		{
			return findId;
		}

		findId = consumeDetail->cooldownGroupId;

		_mapProductCachingData.Add(In_productId, findId);
		
	}
	return findId;
}

// 합성 스킵 타입인지
bool UGsSummonManager::GetIsComposeSkipType()
{
	bool isComposeType = false;
	switch (_originContent)
	{
		case EGsSummonOriginContentType::SUMMON_COMPOSE:
		case EGsSummonOriginContentType::SUMMON_RE_COMPOSE:
		case EGsSummonOriginContentType::SUMMON_CEILING:
		case EGsSummonOriginContentType::SUMMON_EXCHANGE:
		isComposeType = true;
		break;
	}
	return isComposeType;
}

// 합성: 재료가 레어 이상인가
bool UGsSummonManager::IsComposeByOverRare()
{
	bool isOverRare = false;
	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		if ((int)_fairyComposeTryGrade >= (int)FairyGrade::RARE)
		{
			isOverRare = true;
		}
	}
	else
	{
		if ((int)_costumeComposeTryGrade >= (int)CostumeGrade::RARE)
		{
			isOverRare = true;
		}
	}

	return isOverRare;
}

// config 값 찾기
const FGsSchemaSummonConfig* UGsSummonManager::FindSummonConfig()
{
	const UGsTable* summonConfigDataTbl = FGsSchemaSummonConfig::GetStaticTable();

	if (nullptr == summonConfigDataTbl)
	{
		return nullptr;
	}

	TArray<const FGsSchemaSummonConfig*> rowsSummonConfigData;
	summonConfigDataTbl->GetAllRows<FGsSchemaSummonConfig>(rowsSummonConfigData);

	if (rowsSummonConfigData.Num() == 0)
	{
		return nullptr;
	}

	return rowsSummonConfigData[0];
}

// 뽑기 마일리지 보상 아이템 테이블 접근
const FGsSchemaItemCommon* UGsSummonManager::FindSummonMileageRewardItemTable()
{
	const FGsSchemaItemCommon* rewardItemTbl = FindSummonMileageRewardItemTableBySummonType(false,_summonType);
	return rewardItemTbl;
}
// 뽑기 마일리지 보상 아이템 테이블 접근(타입에 따라 찾기)
const FGsSchemaItemCommon* UGsSummonManager::FindSummonMileageRewardItemTableBySummonType(bool In_isUniqueReward, EGsSummonType In_summonType)
{
	const FGsSchemaSummonConfig* summonConfigTbl = FindSummonConfig();
	if (summonConfigTbl == nullptr)
	{
		return nullptr;
	}
	const FGsSchemaItemCommon* rewardItemTbl = nullptr;
	if (In_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		if (In_isUniqueReward == false)
		{
			rewardItemTbl = summonConfigTbl->fairyMileageRewardItem.GetRow();
		}
		else
		{
			rewardItemTbl = summonConfigTbl->fairyMileageRewardItemUnique.GetRow();
		}
	}
	else
	{
		if (In_isUniqueReward == false)
		{
			rewardItemTbl = summonConfigTbl->costumeMileageRewardItem.GetRow();
		}
		else
		{
			rewardItemTbl = summonConfigTbl->costumeMileageRewardItemUnique.GetRow();
		}
	}
	return rewardItemTbl;
}

// 뽑기 마일리지 보상 아이템 갯수 계산
int UGsSummonManager::GetSummonMileageRewardItemCountCaculated()
{
	CurrencyType currencyType =
		(_summonType == EGsSummonType::SUMMON_FAIRY) ?
		CurrencyType::SUMMON_FAIRY_MILEAGE : CurrencyType::SUMMON_COSTUME_MILEAGE;
	Currency currentCurrency = FGsCurrencyHelper::GetCurrencyAmount(currencyType);

	int itemCount = 0;
	int maxSummonMileage = GetMaxSummonMileage();
	if (maxSummonMileage != 0)
	{
		itemCount =
			(float)currentCurrency / maxSummonMileage;
	}
	return itemCount;
}

// 뽑기 마일리지 보상 아이템 이전 갯수 계산(이번 획득 마일리지 뺀값)
int UGsSummonManager::GetSummonMileageRewardItemPreCountCaculated()
{
	CurrencyType currencyType =
		(_summonType == EGsSummonType::SUMMON_FAIRY) ?
		CurrencyType::SUMMON_FAIRY_MILEAGE : CurrencyType::SUMMON_COSTUME_MILEAGE;
	Currency currentCurrency = FGsCurrencyHelper::GetCurrencyAmount(currencyType) - (Currency)_addedSummonMileage;

	int itemCount = 0;
	int maxSummonMileage = GetMaxSummonMileage();
	if (maxSummonMileage != 0)
	{
		itemCount =
			(float)currentCurrency / maxSummonMileage;
	}
	return itemCount;
}

// 마일리지 최대 값(유니크인가, 소환 타입)
int UGsSummonManager::GetMaxSummonMileageByType(bool In_isUniqueReward, EGsSummonType In_summonType)
{
	// 현재 마일리지 max 값 세팅
	const FGsSchemaSummonConfig* findTbl = FindSummonConfig();
	if (findTbl == nullptr)
	{
		return 0;
	}
	int maxMileage = 0;
	if (In_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		if (In_isUniqueReward == false)
		{
			maxMileage = findTbl->fairyMileage;
		}
		else
		{
			maxMileage = findTbl->fairyMileageUnique;
		}
	}
	else
	{
		if (In_isUniqueReward == false)
		{
			maxMileage = findTbl->costumeMileage;
		}
		else
		{
			maxMileage = findTbl->costumeMileageUnique;
		}
	}
		 
	return maxMileage;
}

// 타입(유니크 타입, 뽑기 타입 )으로 나중에 뜰 티커 아이템 갯수 저장해놓기
// 서버 send 시점에 저장하기
void UGsSummonManager::SaveSummonMileageRewardItemCountByType(bool In_isUniqueReward, EGsSummonType In_summonType)
{	
	_summonMileageRewardItemCount = GetSummonMileageRewardItemCountByType(In_isUniqueReward, In_summonType);
}
// 타입(유니크 타입, 뽑기 타입 )으로 나중에 뜰 티커 아이템 갯수 구하기
int UGsSummonManager::GetSummonMileageRewardItemCountByType(bool In_isUniqueReward, EGsSummonType In_summonType)
{
	CurrencyType currencyType = CurrencyType::MAX;

	if (In_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		if (In_isUniqueReward == false)
		{
			currencyType = CurrencyType::SUMMON_FAIRY_MILEAGE;
		}
		else
		{
			currencyType = CurrencyType::SUMMON_UNIQUE_FAIRY_MILEAGE;
		}
	}
	else
	{
		if (In_isUniqueReward == false)
		{
			currencyType = CurrencyType::SUMMON_COSTUME_MILEAGE;
		}
		else
		{
			currencyType = CurrencyType::SUMMON_UNIQUE_COSTUME_MILEAGE;
		}
	}


	Currency currentCurrency = FGsCurrencyHelper::GetCurrencyAmount(currencyType);

	int itemCount = 0;
	int maxSummonMileage = GetMaxSummonMileageByType(In_isUniqueReward, In_summonType);
	if (maxSummonMileage != 0)
	{
		itemCount =
			(float)currentCurrency / maxSummonMileage;
	}
	return itemCount;
}
// 소환 보상이 있는가(타입별로, 유니크, 에픽 포함)
bool UGsSummonManager::IsExistSummonMileageRewardByType(EGsSummonType In_summonType)
{
	int epicRewardCount = GetSummonMileageRewardItemCountByType(false, In_summonType);
	bool epicRewardExist = (epicRewardCount == 0)? false: true;

	// 유니크 보상 받을수 있는가
	// 1: 마일리지 체크
	// 2: 기존에 안받았는지 체크
	bool isPossibleUniqueMileageReward = IsPossibleUniqueMileageRewardByType(In_summonType);

	// 에픽 보상이 있거나
	// 유니크 보상이 있으면서(마일리지 상) 이미 받지 않았으면
	return (epicRewardExist == true  || isPossibleUniqueMileageReward == true)? true : false;
}
// 유니크 마일리지 보상 받았는가
bool UGsSummonManager::IsUniqueMileageObtained(EGsSummonType In_summonType)
{
	int obtainCount = 0;
	if (In_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		obtainCount = _uniqueFairyMileageRewardObtainCount;
	}
	else
	{
		obtainCount = _uniqueCostumeMileageRewardObtainCount;
	}

	return (obtainCount == 0)? false: true;
}

// 유니크 보상 받을수 있는가
// 1: 마일리지 체크
// 2: 기존 받았는가 체크
bool UGsSummonManager::IsPossibleUniqueMileageRewardByType(EGsSummonType In_summonType)
{
	bool isUniqueRewardObtained = IsUniqueMileageObtained(In_summonType);

	int uniqueRewardCount = GetSummonMileageRewardItemCountByType(true, In_summonType);
	bool uniqueRewardExist = (uniqueRewardCount == 0) ? false : true;

	return (uniqueRewardExist == true && isUniqueRewardObtained == false) ? true : false;
}