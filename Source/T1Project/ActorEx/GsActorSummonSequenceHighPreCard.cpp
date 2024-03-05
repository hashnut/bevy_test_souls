#include "GsActorSummonSequenceHighPreCard.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsResourceManager.h"

#include "Summon/GsSummonDefine.h"

#include "Classes/GsSpawn.h"

#include "Engine/Classes/Materials/MaterialInstance.h"

#include "Engine/AssetManager.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"

const FName NameSummonSequenceHighPreCardRootComponent = TEXT("Root");
const FName NameSummonSequenceHighPreCardMeshComponent = TEXT("Mesh");


AGsActorSummonSequenceHighPreCard::AGsActorSummonSequenceHighPreCard()
	: Super()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(NameSummonSequenceHighPreCardRootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(NameSummonSequenceHighPreCardMeshComponent);
	Mesh->SetupAttachment(RootComponent);
}

// 카드 세팅(현재)
void AGsActorSummonSequenceHighPreCard::ApplyCurrentTypeCard()
{
	EGsSummonType summonType = GSSummon()->GetSummonType();

	FSoftObjectPath path =
		summonType == EGsSummonType::SUMMON_COSTUME ?
			GData()->GetGlobalData()->_highPreCardCostume :
			GData()->GetGlobalData()->_highPreCardFairy;

	AActor* loadActor = nullptr;
	if (UClass* uClass = GResource()->LoadSync<UClass>(path, true))
	{		
		loadActor = GsSpawn::BPClass(GetWorld(), uClass, FVector::ZeroVector , FRotator::ZeroRotator);
		
	}

	if (loadActor == nullptr)
	{
		return;
	}

	AGsActorSummonSequenceHighPreCard* card = Cast<AGsActorSummonSequenceHighPreCard>(loadActor);
	if (card == nullptr)
	{
		return;
	}

	USkeletalMeshComponent* cardMesh = card->GetSkeletalMeshComponent();
	if (cardMesh == nullptr)
	{
		return;
	}

	Mesh->SetSkeletalMesh(cardMesh->SkeletalMesh);

	TArray<UMaterialInterface*> arrMaterial = cardMesh->GetMaterials();

	if (arrMaterial.Num() == 0)
	{
		return;
	}

	for (int i=0; i< arrMaterial.Num() ; ++i)
	{
		Mesh->SetMaterial(i, arrMaterial[i]);
	}
	
}