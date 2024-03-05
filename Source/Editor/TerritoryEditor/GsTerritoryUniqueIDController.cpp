#include "GsTerritoryUniqueIDController.h"

//UE4
#include "Editor.h"
#include "Misc/MessageDialog.h"
#include "Engine/DataTable.h"
#include "WorldBrowserModule.h"
#include "Engine/WorldComposition.h"
#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"

//T1 Project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "Runtime/Public/GsTable.h"
#include "UTIL/GsText.h"


//Territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "GsTerritoryEditorSetting.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "TerritoryEditor.h"

const int64 MAX_INT64 = TNumericLimits<int64>::Max();
const int64 MIN_INT64 = TNumericLimits<int64>::Min();
const int64 MAX_UINT32 = TNumericLimits<uint32>::Max();

FGsTerritoryObjectInstanceIDController::~FGsTerritoryObjectInstanceIDController()
{
	_usedInstanceIDs.Empty();
	_unusedInstanceIDs.Empty();
}

void FGsTerritoryObjectInstanceIDController::InitUnusedInstanceIDs()
{
	_unusedInstanceIDs.Empty();
	_unusedInstanceIDs.Reserve(TERRITORY_INSTANCE_ID_MAX);
	//사용되지 않는 instance id를 map에 넣는다. 
	for (uint32 i = 1; i <= TERRITORY_INSTANCE_ID_MAX; ++i)
	{
		if (false == _usedInstanceIDs.Contains(i))
		{
			_unusedInstanceIDs.Add(i);
		}
	}
}

void FGsTerritoryObjectInstanceIDController::InvalidMaxInstanceID()
{
	if (false == _isAscending)
	{
		return;
	}

	int64 max = MIN_INT64;
	for (uint32 id : _usedInstanceIDs)
	{
		if (id > max)
		{
			max = id;
		}
	}

	_maxInstanceID = max;
}

bool FGsTerritoryObjectInstanceIDController::TryGenerateAscendingInstanceID(uint32& outInstanceID)
{
	int64 start = _maxInstanceID + 1;
	if (start > MAX_UINT32)
	{
		return false;
	}

	if (start > TERRITORY_INSTANCE_ID_MAX)
	{
		return false;
	}

	for (uint32 i = start; i <= TERRITORY_INSTANCE_ID_MAX; ++i)
	{
		if (IsCanGenerateUniqueID(i))
		{
			_maxInstanceID = i;
			outInstanceID = i;

			return true;
		}
	}

	return false;
}

bool FGsTerritoryObjectInstanceIDController::TryGenerateInstanceID(uint32& outInstanceID)
{
	int64 find = INDEX_NONE;

	//사용되지 않은 첫번째 인덱스의 id값 사용
	for (uint32 iter : _unusedInstanceIDs)
	{
		find = iter;
		break;
	}

	outInstanceID = (uint32)find;

	return (find != -1);
}

bool FGsTerritoryObjectInstanceIDController::TryGenerateUniqueID(EGsTerritoryObjType inObjType, int64& outTerritoryUniqueID, int64& outInstanceID)
{
	//비어 있다면 더 이상 생성 할 수 없다. 
	if (_unusedInstanceIDs.Num() == 0)
	{
		return false;
	}

	uint32 id = 0;

	if (_isAscending)
	{
		if (false == TryGenerateAscendingInstanceID(id))
		{
			return false;
		}
	}
	else
	{
		if (false == TryGenerateInstanceID(id))
		{
			return false;
		}
	}

	return TryGenerateUniqueID(inObjType, id, outTerritoryUniqueID, outInstanceID);
}

bool FGsTerritoryObjectInstanceIDController::TryGenerateUniqueID(EGsTerritoryObjType inObjType, uint32& inInstanceID, int64& outTerritoryUniqueID, int64& outInstanceID)
{
	if (false == Add(inInstanceID))
	{
		return false;
	}

	if (_isAscending)
	{
		InvalidMaxInstanceID();
	}

	outInstanceID = inInstanceID;
	outTerritoryUniqueID = CombineTerritoryUniqueID(inObjType, inInstanceID);

	return true;
}

bool FGsTerritoryObjectInstanceIDController::Add(uint32 inInstanceID)
{
	//Check range
	if (inInstanceID > TERRITORY_INSTANCE_ID_MAX)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("FGsTerritoryObjectInstanceIDController::Add - wrong range - id : %lld"), (int64)inInstanceID);

		return false;
	}

	//Check id already used?
	if (_usedInstanceIDs.Contains(inInstanceID))
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("FGsTerritoryObjectInstanceIDController::Add - id already used - id : %lld"), (int64)inInstanceID);

		return false;
	}

	//Check id is invalid?
	if (false == _unusedInstanceIDs.Contains(inInstanceID))
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("FGsTerritoryObjectInstanceIDController::Add - id is invalid - id : %lld"), (int64)inInstanceID);

		return false;
	}

	_usedInstanceIDs.Add(inInstanceID);
	_unusedInstanceIDs.Remove(inInstanceID);

#if TERRITORY_EDITRO_UNIQUEID_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryObjectInstanceIDController::Add - id : %lld"), (int64)inInstanceID);
#endif

	return true;
}

bool FGsTerritoryObjectInstanceIDController::AddForce(uint32 inInstanceID)
{
	if (inInstanceID > TERRITORY_INSTANCE_ID_MAX)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("FGsTerritoryObjectInstanceIDController::AddForce - wrong range - id : %lld"), (int64)inInstanceID);

		return false;
	}

	if (false == _usedInstanceIDs.Contains(inInstanceID))
	{
		_usedInstanceIDs.Add(inInstanceID);
	}

	if (_unusedInstanceIDs.Contains(inInstanceID))
	{
		_unusedInstanceIDs.Remove(inInstanceID);
	}	

	if (_isAscending)
	{
		InvalidMaxInstanceID();
	}

	return true;
}

bool FGsTerritoryObjectInstanceIDController::Remove(uint32 inID)
{
	//Check range
	if (inID > TERRITORY_INSTANCE_ID_MAX)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("FGsTerritoryObjectInstanceIDController::Remove - wrong range - id : %lld"), (int64)inID);

		return false;
	}

	//이미 사용되지 않은 것이면 에러 
	if (_unusedInstanceIDs.Contains(inID))
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("FGsTerritoryObjectInstanceIDController::Remove - id already removed - id : %lld"), (int64)inID);

		return false;
	}

	//사용되어있는 아이디에 없으면 에러 
	if (false == _usedInstanceIDs.Contains(inID))
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("FGsTerritoryObjectInstanceIDController::Remove - id is invalid - id : %lld"), (int64)inID);

		return false;
	}

	_usedInstanceIDs.Remove(inID);
	_unusedInstanceIDs.Add(inID);

#if TERRITORY_EDITRO_UNIQUEID_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryObjectInstanceIDController::Remove - level : %lld"), inID);
#endif

	if (_isAscending)
	{
		InvalidMaxInstanceID();
	}

	return true;
}


int64 FGsTerritoryObjectInstanceIDController::CombineTerritoryUniqueID(EGsTerritoryObjType inObjType, uint32 inInstanceID)
{
	uint32 objType = (uint32)inObjType;
	objType = objType << 20;
	uint32 territoryUniqueID = (objType | inInstanceID);
	return territoryUniqueID;
}

int64 FGsTerritoryObjectInstanceIDController::GetInstanceIDFromUniqueID(EGsTerritoryObjType inObjType, uint32 inInstanceID)
{
	uint32 objType = (uint32)inObjType;
	objType = objType << 20;
	uint32 territoryUniqueID = (objType ^ inInstanceID);
	return territoryUniqueID;
}

void FGsTerritoryObjectInstanceIDController::Initailize()
{
	_usedInstanceIDs.Reserve(TERRITORY_INSTANCE_ID_MAX);
	_usedInstanceIDs.Reset();
	_unusedInstanceIDs.Reserve(TERRITORY_INSTANCE_ID_MAX);
	_unusedInstanceIDs.Reset();
}
bool FGsTerritoryObjectInstanceIDController::IsFull()
{
	return _usedInstanceIDs.Num() == TERRITORY_INSTANCE_ID_MAX;
}

bool FGsTerritoryObjectInstanceIDController::IsCanGenerateUniqueID(uint32& inInstanceID, bool inIsForceCheck)
{
	if (inInstanceID > TERRITORY_INSTANCE_ID_MAX)
	{
		return false;
	}

	if (false == inIsForceCheck)
	{
		if (_usedInstanceIDs.Contains(inInstanceID))
		{
			return false;
		}

		if (false == _unusedInstanceIDs.Contains(inInstanceID))
		{
			return false;
		}
	}

	return true;
}

FGsTerritoryServerLevelInstanceIDController::~FGsTerritoryServerLevelInstanceIDController()
{
	TArray<EGsTerritoryObjType> keyArray;
	_instanceIDController.GenerateKeyArray(keyArray);
	for (const EGsTerritoryObjType& key : keyArray)
	{
		if (_instanceIDController.Contains(key))
		{
			if (_instanceIDController[key].IsValid())
			{
				_instanceIDController.Reset();
			}
		}
	}

	_instanceIDController.Empty();
}

//Territory server level instanc id controller---------------------------------------------------------------------------------------------------------

void FGsTerritoryServerLevelInstanceIDController::Initialize(ULevel* inLevel)
{
	if (nullptr == inLevel)
	{
		return;
	}

	_instanceIDController.Empty();
	_instanceIDController.Reserve((int32)EGsTerritoryObjType::Max);

	//Get all territory actors in server level
	for (uint32 i = 0; i < (uint32)EGsTerritoryObjType::Max; ++i)
	{
		TSharedPtr<FGsTerritoryObjectInstanceIDController> temp = MakeShareable(new FGsTerritoryObjectInstanceIDController());
		if (false == temp.IsValid())
		{
			continue;
		}

		if (IsAscendingGenerateUniqueID((EGsTerritoryObjType)i))
		{
			temp->SetAscending(true);
		}

		_instanceIDController.Add((EGsTerritoryObjType)i, temp);
		_instanceIDController[(EGsTerritoryObjType)i]->Initailize();
	}

	//Get all territory actors from server level
	InitAllTerritoryActorsInServerLevel(inLevel);

	//Initialize unused territory unique id
	InitAllTerritoryActorsUnusedInstanceIDs();
}

bool FGsTerritoryServerLevelInstanceIDController::InitAllTerritoryActorsInServerLevel(ULevel* inLevel)
{
	for (int32 i = 0; i < (int32)EGsTerritoryObjType::Max; ++i)
	{
		TArray<AGsTerritoryActor*> territoryActors;
		EGsTerritoryObjType objType = (EGsTerritoryObjType)i;

		if (UGsTerritoryLevelUtil::TryGetAllTerritoryActor(inLevel, objType, territoryActors))
		{
			if (false == _instanceIDController.Contains(objType))
			{
				continue;
			}

			for (AGsTerritoryActor* temp : territoryActors)
			{
				if (nullptr == temp)
				{
					continue;
				}

				if (false == _instanceIDController[objType]->AddForce(temp->_instanceID))
				{
					FString type = FGsTextUtil::GetEnumValueAsString(TEXT("EGsTerritoryObjType"), objType);

					UE_LOG(LogTerritoryEditor, Error, TEXT("instance id is multi - object type : %s\tid : %lld"), *type, (int64)temp->_instanceID);
				}
			}
		}

		if (_instanceIDController[objType]->IsAscending())
		{
			_instanceIDController[objType]->InvalidMaxInstanceID();
		}
	}

	return true;
}

void FGsTerritoryServerLevelInstanceIDController::InitAllTerritoryActorsUnusedInstanceIDs()
{
	TArray<EGsTerritoryObjType> keyArray;
	_instanceIDController.GenerateKeyArray(keyArray);
	for (EGsTerritoryObjType key : keyArray)
	{
		if (_instanceIDController.Contains(key))
		{
			if (_instanceIDController[key].IsValid())
			{
				_instanceIDController[key].Get()->InitUnusedInstanceIDs();
			}
		}
	}
}

bool FGsTerritoryServerLevelInstanceIDController::TryGenerateUniqueID(EGsTerritoryObjType inObjType, int64& outTerritoryUniqueID, int64& outInstanceID)
{
	if (_instanceIDController.Contains(inObjType))
	{
		return _instanceIDController[inObjType]->TryGenerateUniqueID(inObjType, outTerritoryUniqueID, outInstanceID);
	}

	return false;
}

bool FGsTerritoryServerLevelInstanceIDController::TryGenerateUniqueID(EGsTerritoryObjType inObjType, uint32& inInstanceID, int64& outTerritoryUniqueID, int64& outInstanceID)
{
	if (_instanceIDController.Contains(inObjType))
	{
		return _instanceIDController[inObjType]->TryGenerateUniqueID(inObjType, inInstanceID, outTerritoryUniqueID, outInstanceID);
	}

	return false;
}

bool FGsTerritoryServerLevelInstanceIDController::Add(EGsTerritoryObjType inType, uint32 inID)
{
	if (_instanceIDController.Contains(inType))
	{
		if (_instanceIDController[inType].IsValid())
		{
			return _instanceIDController[inType]->Add(inID);
		}
	}

	return false;
}

bool FGsTerritoryServerLevelInstanceIDController::AddForce(EGsTerritoryObjType inType, uint32 inInstanceID)
{
	if (_instanceIDController.Contains(inType))
	{
		if (_instanceIDController[inType].IsValid())
		{
			return _instanceIDController[inType]->AddForce(inInstanceID);
		}
	}

	return false;
}

bool FGsTerritoryServerLevelInstanceIDController::Remove(EGsTerritoryObjType inType, uint32 inInstanceID)
{
	if (_instanceIDController.Contains(inType))
	{
		if (_instanceIDController[inType].IsValid())
		{
			return _instanceIDController[inType]->Remove(inInstanceID);
		}
	}

	return false;
}

bool FGsTerritoryServerLevelInstanceIDController::IsCanGenerateUniqueID(EGsTerritoryObjType inType)
{
	FGsTerritoryObjectInstanceIDController* controller = GetObjectUniqueIDController(inType);
	if (nullptr == controller)
	{
		return false;
	}

	if (controller->IsAscending())
	{
		if (controller->GetMaxInstanceID() == (int64)TERRITORY_INSTANCE_ID_MAX)
		{
			return false;
		}
	}

	if (controller->IsFull())
	{
		return false;
	}

	return true;
}

bool FGsTerritoryServerLevelInstanceIDController::IsCanGenerateUniqueID(EGsTerritoryObjType inType, uint32& inInstanceID, bool inIsForceCheck)
{
	FGsTerritoryObjectInstanceIDController* controller = GetObjectUniqueIDController(inType);
	if (nullptr == controller)
	{
		return false;
	}

	if (controller->IsFull())
	{
		return false;
	}

	return controller->IsCanGenerateUniqueID(inInstanceID, inIsForceCheck);
}

FGsTerritoryObjectInstanceIDController* FGsTerritoryServerLevelInstanceIDController::GetObjectUniqueIDController(EGsTerritoryObjType inType)
{
	if (_instanceIDController.Contains(inType))
	{
		if (_instanceIDController[inType].IsValid())
		{
			return _instanceIDController[inType].Get();
		}
	}

	return nullptr;
}

bool FGsTerritoryServerLevelInstanceIDController::IsAscendingGenerateUniqueID(EGsTerritoryObjType inType)
{
	bool isAscending = false;

	switch (inType)
	{
	case EGsTerritoryObjType::Spot:
		isAscending = true;
		break;
	}

	return isAscending;
}