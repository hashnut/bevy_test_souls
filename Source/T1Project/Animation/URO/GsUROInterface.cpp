#include "GsUROInterface.h"

#include "Engine/Classes/Engine/EngineTypes.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"


void IGsUROInterface::InitSkeletalMeshURO()
{
	TArray<USkeletalMeshComponent*> charMeshs;
	GetSkeletalMeshs(charMeshs);
	if (charMeshs.Num() == 0)
	{
		return;
	}

	UGsDataContainManager* globalDataManager = GData();

	if (nullptr == globalDataManager)
	{
		return;
	}
	const UGsGlobalConstant* globalConstData = globalDataManager->GetGlobalData();

	if (nullptr == globalConstData)
	{
		return;
	}

	for (USkeletalMeshComponent* iter : charMeshs)
	{
		if (iter == nullptr)
		{
			continue;
		}
		iter->bEnableUpdateRateOptimizations = globalConstData->bEnableUpdateRateOptimizations;
		iter->bDisplayDebugUpdateRateOptimizations = globalConstData->bDisplayDebugUpdateRateOptimizations;

		if (nullptr == iter->AnimUpdateRateParams)
		{
			continue;
		}

		iter->AnimUpdateRateParams->BaseNonRenderedUpdateRate = globalConstData->BaseNonRenderedAnimUpdateRate;
		iter->AnimUpdateRateParams->BaseVisibleDistanceFactorThesholds = globalConstData->AnimUpdateRateVisibleMaxDistanceFactor;
		iter->AnimUpdateRateParams->MaxEvalRateForInterpolation = globalConstData->MaxEvalRateForInterpolation;
		iter->AnimUpdateRateParams->ShiftBucket = globalConstData->UroShiftBucket;
		iter->AnimUpdateRateParams->bShouldUseMinLod = false;

		iter->AnimUpdateRateParams->bShouldUseLodMap = globalConstData->bUseLODMapForURO;
		iter->AnimUpdateRateParams->LODToFrameSkipMap = globalConstData->LODToFrameSkipMap;
	}
}