#include "GsCustomizeFunc.h"

#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Customize/GsCustomizeHandler.h"


#include "Item/GsItemManager.h"

#include "UTIL/GsTAbleUtil.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "DataSchema/Item/GSSchemaItemEquipDetail.h"
#include "DataSchema/User/GsSchemaUserData.h"

#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"

#include "T1Project.h"



// FGsCustomizeHandler* In_customizeHandler : 작업 진행용 Handler
// UserLookInfo* lookInfo : 무기외형 참고용
// int32 Tid : DTGsUserData (소켓 정보를 얻어오기 위함)
// bool inBattle = false : 전투(손), 평화(등) 소켓에 붙여준다.
bool FGsCustomizeFunc::AttachWeapon(FGsCustomizeHandler* InCustomizeHandler, const UserLookInfo* lookInfo, int32 Tid, bool inBattle)
{	
	if (nullptr == InCustomizeHandler)
		return false;

	if (nullptr == lookInfo)
		return false;

	const FGsSchemaItemEquipDetail* equipItem = UGsItemManager::GetEquipItemTableDataByTID(lookInfo->mWeapon);

	if (nullptr == equipItem)
		return false;

	int32 weaponId					= equipItem->modelPath;	
	Level weaponLevel				= lookInfo->mWeaponLevel;
	CreatureWeaponType weaponType = equipItem->weaponType;

	// 무기 교체	
	InCustomizeHandler->SetWeaponChange(weaponId, weaponType, weaponLevel);

	// 등(Peace) 소켓 붙이기
	const FGsSchemaUserData* userDataTable = UGsTableUtil::FindRowById<UGsTableUserData, FGsSchemaUserData>(Tid);
	InCustomizeHandler->ChangeWeaponSocket(userDataTable, weaponType, inBattle);

	if (const FGsSchemaWeaponSocketInfo* findInfo = userDataTable->weaponSocketList.Find(weaponType))
	{
		if (auto player = InCustomizeHandler->GetPlayer())
		{
			player->SetWeaponTransformByCurve(player->GetWeaponChildActorComponent(), &findInfo->peace, false);
			player->SetWeaponTransformByCurve(player->GetWeaponSubChildActorComponent(), &findInfo->peace, false);
		}
	}

	return true;
}

// 로컬 데이터를 넣는다

void FGsCustomizeFunc::SetCustomizeByLocalData(FGsCustomizeHandler* customizeHandler,
	FGsGameObjectDataLocalPlayer* localData, CostumeId costumeId,
	bool showCustomHair, bool applyBoneScale, bool useWeapon, bool applyFace )
{
	if (customizeHandler == nullptr ||
		localData == nullptr)
	{
		return;
	}

	customizeHandler->SetGenderType(localData->GetCreatureGenderType());

	const CustomizeFaceData* faceData = localData->GetCustomizeFaceData();
	const CustomizeBodyData* bodyData = localData->GetCustomizeBodyData();

	if (nullptr != faceData && nullptr != bodyData)
	{		
		if (applyBoneScale)
		{
			customizeHandler->SetCustomizeDataBonScale(*faceData, *bodyData);
		}
		else
		{
			customizeHandler->SetCustomizeData(*faceData, *bodyData);
		}
	}
	else
	{
		GSLOG(Error, TEXT("face or body no data"));
	}

	if (useWeapon)
	{
		AttachWeapon(customizeHandler, localData->GetUserLookInfo(), localData->GetTableId());
	}

	customizeHandler->SetCostume(localData->GetUserLookInfo()->mTransformCostumeId, costumeId, showCustomHair, true, true, applyBoneScale, applyFace);
}