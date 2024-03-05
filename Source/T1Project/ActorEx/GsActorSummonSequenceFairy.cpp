#include "GsActorSummonSequenceFairy.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "Fairy/GsFairyData.h"

#include "Summon/GsSummonDefine.h"

#include "UI/UIContent/Tray/GsUITraySummonObtain.h"
#include "UI/UIContent/Tray/GsUITraySummonDialogue.h"
#include "UI/UIContent/Tray/GsUITraySummonInfo.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Animation/AnimInstance.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/AssetManager.h"

// 선택된 페어리로 세팅(시네마틱으로 넘김)
void AGsActorSummonSequenceFairy::ApplySelectFairy(AGsActorSummonFairy* In_fairy)
{
	// 들어온게 null이면 끝
	if (In_fairy == nullptr)
	{
		return;
	}
	USkeletalMeshComponent* targetBodyMesh = In_fairy->GetPartsSkeletalMeshComponentBody();
	USkeletalMeshComponent* targetFaceMesh = In_fairy->GetPartsSkeletalMeshComponentFace();
	if (targetBodyMesh == nullptr ||
		targetFaceMesh == nullptr)
	{
		return;
	}
	USkeletalMeshComponent* myBodyMesh = GetPartsSkeletalMeshComponentBody();
	if (myBodyMesh == nullptr)
	{
		return;
	}
	myBodyMesh->SetSkeletalMesh(targetBodyMesh->SkeletalMesh);

	USkeletalMeshComponent* myFaceMesh = GetPartsSkeletalMeshComponentFace();

	if (myFaceMesh == nullptr)
	{
		return;
	}
	myFaceMesh->SetSkeletalMesh(targetFaceMesh->SkeletalMesh);

	myBodyMesh->SetAnimInstanceClass(targetBodyMesh->GetAnimInstance()->GetClass());
}

// 획득 ui 출력
void AGsActorSummonSequenceFairy::ShowTraySummonObtain(AGsActorSummonFairy* In_fairy)
{
	// 들어온게 null이면 끝
	if (In_fairy == nullptr)
	{
		return;
	}
	int dataIndex = In_fairy->GetDataIndex();


	TArray<TSharedPtr<FGsFairyData>> arrayData = GSSummon()->GetSummonFairyData();

	if (arrayData.Num() == 0 ||
		arrayData.Num() <= dataIndex)
	{
		return;
	}

	TSharedPtr<FGsFairyData> fairyData = arrayData[dataIndex];

	if (fairyData.IsValid() == false)
	{
		return;
	}

	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	//  UI 출력
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TraySummonObtain"));
	if (widget.IsValid())
	{
		TWeakObjectPtr<UGsUITraySummonObtain> traySummonObtain  = Cast<UGsUITraySummonObtain>(widget);
		if (traySummonObtain.IsValid())
		{
			traySummonObtain->SetData(EGsSummonType::SUMMON_FAIRY, fairyData->_id);
		}
	}
}

// 대사 ui 출력
void AGsActorSummonSequenceFairy::ShowTraySummonDialogue(AGsActorSummonFairy* In_fairy)
{
	// 들어온게 null이면 끝
	if (In_fairy == nullptr)
	{
		return;
	}
	int dataIndex = In_fairy->GetDataIndex();


	TArray<TSharedPtr<FGsFairyData>> arrayData = GSSummon()->GetSummonFairyData();

	if (arrayData.Num() == 0 ||
		arrayData.Num() <= dataIndex)
	{
		return;
	}

	TSharedPtr<FGsFairyData> fairyData = arrayData[dataIndex];

	if (fairyData.IsValid() == false)
	{
		return;
	}

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
			traySummonDialogue->SetData(EGsSummonType::SUMMON_FAIRY, fairyData->_id);
		}
	}
}

// 정보 ui 출력
void AGsActorSummonSequenceFairy::ShowTraySummonInfo(AGsActorSummonFairy* In_fairy)
{
	// 들어온게 null이면 끝
	if (In_fairy == nullptr)
	{
		return;
	}
	int dataIndex = In_fairy->GetDataIndex();


	TArray<TSharedPtr<FGsFairyData>> arrayData = GSSummon()->GetSummonFairyData();

	if (arrayData.Num() == 0 ||
		arrayData.Num() <= dataIndex)
	{
		return;
	}

	TSharedPtr<FGsFairyData> fairyData = arrayData[dataIndex];

	if (fairyData.IsValid() == false)
	{
		return;
	}

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
			traySummonInfo->SetData(EGsSummonType::SUMMON_FAIRY, fairyData->_id);
		}
	}
}

void AGsActorSummonSequenceFairy::MakeSpawnEffectFairy(bool In_isHighGrade)
{
	const FSoftObjectPath path =
		In_isHighGrade ?
		GData()->GetGlobalData()->_effectHighGradeSummonSpawnFairy :
		GData()->GetGlobalData()->_effectNormalGradeSummonSpawnFairy;

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

	USceneComponent* targetcomponent = GetPartsSkeletalMeshComponentBody();

	UGameplayStatics::SpawnEmitterAttached(
		particle,
		targetcomponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		(FVector)(1.0f),
		EAttachLocation::Type::KeepRelativeOffset,
		true,
		EPSCPoolMethod::AutoRelease,
		true);
}
