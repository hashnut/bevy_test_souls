#include "GsActorSummonSequenceCostume.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"

#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Customize/GsCustomizeFunc.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsGameClientVersion.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "DataCenter/Public/DataSchema/Costume/GsSchemaCostumeCommon.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"
#include "GameMode/GsGameModeTestClient.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "UI/UIContent/Tray/GsUITraySummonDialogue.h"
#include "UI/UIContent/Tray/GsUITraySummonInfo.h"
#include "UI/UIContent/Tray/GsUITraySummonObtain.h"

#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsTableUtil.h"

#include "Summon/GsSummonDefine.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/AssetManager.h"

void AGsActorSummonSequenceCostume::ApplyCurrentLocalPlayerAndSelectCostume(
	UGsGameObjectLocalPlayer* In_GameObjectLocalPlayer,
	int In_costumeId)
{
	CostumeId castCostumeId = static_cast<CostumeId>(In_costumeId);

	if (In_GameObjectLocalPlayer == nullptr)
	{
		return;
	}

	SetCostumeId(castCostumeId);

	// 한번 지우고 시작하자
	Finalize();
	
	_customizeHandler = new FGsCustomizeHandler();
	_customizeHandler->InitializeActor(this);
	

	bool isTestClientMode = false;

	CreatureGenderType genderType = CreatureGenderType::ALL;
#if WITH_EDITOR
	if (UWorld* world = GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::TestClient)
			{
				isTestClientMode = true;

				if (AGsGameModeTestClient* testMode = Cast<AGsGameModeTestClient>(gameMode))
				{
					genderType =  testMode->_genderType;
				}
			}
		}
	}
#endif

	if (isTestClientMode == false)
	{
		if (FGsGameObjectDataLocalPlayer* localData =
			In_GameObjectLocalPlayer->GetCastData<FGsGameObjectDataLocalPlayer>())
		{
			FGsCustomizeFunc::SetCustomizeByLocalData(_customizeHandler, localData, _costumeId, true, true, false, true);
		}
	}
	else
	{
		AGsCharacterPlayer* SourceActor = Cast< AGsCharacterPlayer>(In_GameObjectLocalPlayer->GetCharacter());

		if (SourceActor)
		{
			GetMesh()->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentBODY()->SkeletalMesh);
			_partsSkeletalComponentFACE->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentFACE()->SkeletalMesh);
			_partsSkeletalComponentHAIR->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentHAIR()->SkeletalMesh);
		}
		_customizeHandler->InitializeScaleDisplacementAllBone(genderType);
		_customizeHandler->SetCostume(INVALID_COSTUME_ID, _costumeId);
	}
	// 레전드 뒤에 날개 안보이게 처리
	// 레전드만 분리처리 (유니크나 에픽의 이펙트는 나와야 되어서)
	// https://jira.com2us.com/jira/browse/CHR-22631	
	const FGsSchemaCostumeCommon* costumeTable =
		UGsTableUtil::FindRowById<UGsTableCostumeCommon, FGsSchemaCostumeCommon>(castCostumeId);

	if(costumeTable != nullptr && costumeTable->grade == CostumeGrade::LEGEND)
	{ 
		SetHiddenCostumeEffect(true);
	}
	

	// beginplay에서 스케일시 무기 크기가 2배되어서
	// 여기서 해보자
	InitScale();
}
void AGsActorSummonSequenceCostume::ApplyCurrentLocalPlayerAndSelectCostumeWithVeilCostume(UGsGameObjectLocalPlayer* In_GameObjectLocalPlayer,
	int In_costumeId, bool In_isHigh)
{
	CostumeId castCostumeId = static_cast<CostumeId>(In_costumeId);

	if (In_GameObjectLocalPlayer == nullptr)
	{
		return;
	}

	SetCostumeId(castCostumeId);

	// 한번 지우고 시작하자
	Finalize();

	_customizeHandler = new FGsCustomizeHandler();
	_customizeHandler->InitializeActor(this);


	bool isTestClientMode = false;

	CreatureGenderType genderType = CreatureGenderType::ALL;
#if WITH_EDITOR
	if (UWorld* world = GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::TestClient)
			{
				isTestClientMode = true;

				if (AGsGameModeTestClient* testMode = Cast<AGsGameModeTestClient>(gameMode))
				{
					genderType = testMode->_genderType;
				}
			}
		}
	}
#endif

	if (In_isHigh == true)
	{
		UGsSummonManager* summonManager = GSSummon();
		if (summonManager == nullptr)
		{
			return;
		}

		AActor* veilObject = summonManager->GetVeilObject();
		if (veilObject == nullptr)
		{
			return;
		}

		ACharacter* veilCharacter = Cast<ACharacter>(veilObject);
		if (veilCharacter == nullptr)
		{
			return;
		}
		

		GetMesh()->SetSkeletalMesh(veilCharacter->GetMesh()->SkeletalMesh);

		_partsSkeletalComponentFACE->SetSkeletalMesh(nullptr);
		_partsSkeletalComponentHAIR->SetSkeletalMesh(nullptr);

	}
	else
	{ 
		if (isTestClientMode == false)
		{
			if (FGsGameObjectDataLocalPlayer* localData =
				In_GameObjectLocalPlayer->GetCastData<FGsGameObjectDataLocalPlayer>())
			{
				FGsCustomizeFunc::SetCustomizeByLocalData(_customizeHandler, localData, _costumeId, true, true, false, true);

				// https://jira.com2us.com/jira/browse/C2URWQ-4436
				// 코스튬 뽑기 시네마틱에서 발이 파묻히는 현상 수정
				
				UCapsuleComponent* capsule = GetCapsuleComponent();
				const CustomizeBodyData* bodyData = localData->GetCustomizeBodyData();

				if (capsule)
				{
					// 캡슐 사이즈를 GetScaledCapsuleHalfHeight 를 쓰면
					// 바닥을 뚫고 밑으로 내려감
					UGsGameObjectUtil::MeshRelativeLocation(localData->GetCreatureGenderType()
						, bodyData->mHeight
						, capsule->GetUnscaledCapsuleHalfHeight()
						, this);
				}

			}
		}
		else
		{
			AGsCharacterPlayer* SourceActor = Cast< AGsCharacterPlayer>(In_GameObjectLocalPlayer->GetCharacter());

			if (SourceActor)
			{
				GetMesh()->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentBODY()->SkeletalMesh);
				_partsSkeletalComponentFACE->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentFACE()->SkeletalMesh);
				_partsSkeletalComponentHAIR->SetSkeletalMesh(SourceActor->GetPartsSkeletalComponentHAIR()->SkeletalMesh);
			}
			_customizeHandler->InitializeScaleDisplacementAllBone(genderType);
			_customizeHandler->SetCostume(INVALID_COSTUME_ID, _costumeId);
		}

	}

	// beginplay에서 스케일시 무기 크기가 2배되어서
	// 여기서 해보자
	InitScale();
}

void AGsActorSummonSequenceCostume::BeginDestroy()
{
	Finalize();
	Super::BeginDestroy();
}

// 대사 ui 출력
void AGsActorSummonSequenceCostume::ShowTraySummonDialogue(int In_costumeId)
{
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	//  UI 출력
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TraySummonDialogue"));
	if (widget.IsValid())
	{
		TWeakObjectPtr<UGsUITraySummonDialogue> traySummonDialogue = Cast<UGsUITraySummonDialogue>(widget);
		if (traySummonDialogue.IsValid())
		{
			traySummonDialogue->SetData(EGsSummonType::SUMMON_COSTUME, In_costumeId);
		}
	}
}

// 정보 ui 출력
void AGsActorSummonSequenceCostume::ShowTraySummonInfo(int In_costumeId)
{
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	//  UI 출력
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TraySummonInfo"));
	if (widget.IsValid())
	{
		TWeakObjectPtr<UGsUITraySummonInfo> traySummonInfo = Cast<UGsUITraySummonInfo>(widget);
		if (traySummonInfo.IsValid())
		{
			traySummonInfo->SetData(EGsSummonType::SUMMON_COSTUME, In_costumeId);
		}
	}
}

// 획득 ui 출력
void AGsActorSummonSequenceCostume::ShowTraySummonObtain(int In_costumeId)
{
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	//  UI 출력
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TraySummonObtain"));
	if (widget.IsValid())
	{
		TWeakObjectPtr<UGsUITraySummonObtain> traySummonObtain = Cast<UGsUITraySummonObtain>(widget);
		if (traySummonObtain.IsValid())
		{
			traySummonObtain->SetData(EGsSummonType::SUMMON_COSTUME, In_costumeId);
		}
	}
}

void AGsActorSummonSequenceCostume::MakeSpawnEffectCostume(bool In_isHighGrade)
{
	const FSoftObjectPath path =
		In_isHighGrade ?
		GData()->GetGlobalData()->_effectHighGradeSummonSpawnCostume :
		GData()->GetGlobalData()->_effectNormalGradeSummonSpawnCostume;

	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(path);
	if (load == nullptr)
	{
		return;
	}

	UParticleSystem* particle = Cast<UParticleSystem>(load);

	if (particle == nullptr)
	{
		return;
	}
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, GetActorTransform(), true, EPSCPoolMethod::AutoRelease, true);

	USceneComponent* targetcomponent = GetMesh();

	FVector scale = GetRootComponent()->GetComponentScale();

	float reverserVal = (scale.X == 0.0f) ? 1.0 : 1.0f /scale.X;
	FVector reverseScale = FVector(reverserVal, reverserVal, reverserVal);

	UGameplayStatics::SpawnEmitterAttached(
		particle,
		targetcomponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		reverseScale,
		EAttachLocation::Type::KeepRelativeOffset,
		true,
		EPSCPoolMethod::AutoRelease,
		true);
}
