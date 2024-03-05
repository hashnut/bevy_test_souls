#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

#include "DataTableConverter/GsDataTableConvertBase.h"
#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"	

#include "Text.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h" 

#include "CoreUObject/Public/UObject/UnrealType.h"
#include "Core/Public/HAL/FileManager.h"
#include "Core/Public/GenericPlatform/GenericPlatformFile.h"

#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "DataSchema/Skill/Upgrade/GsSchemaSkillUpgradeSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInfo.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectStatSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectSkillUpSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeSkillNotifySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeAbnormalitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectInvokeCooldownSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectSkillUpgradeSet.h"
#include "DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectStatParam.h"
#include "DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInvokeSkillNotifyParam.h"
#include "DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInvokeAbnormalityParam.h"
#include "DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInvokeCooldownParam.h"
#include "DataSchema/Skill/HitEffect/GsSchemaHitEffectSetData.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Icon/GsSchemaSkillIconSetData.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"

#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivitySetExcel.h"
#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityEffectInfotExcel.h"
#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityEffectStatGroupExcel.h"
#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityEffectSkillUpSetExcel.h"
#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityEffectSkillUpSetGroupExcel.h"
#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityEffectInvokeSkillNotifySetExcel.h"
#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityInvokeAbnormalitySetExcel.h"
#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityEffectInvokeCooldownSetExcel.h"
#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityEffectSkillUpgradeSetExcel.h"
#include "DataSchema/NeverCook/Skill/Passivity/GsSchemaPassivityEffectStatSetExcel.h"

#include "GameObject/ObjectClass/Data/GsHitEffectData.h"


#define DEF_ERR_POPUP_MESSAGE(Message)\
FNotificationInfo Info(Message);\
Info.ExpireDuration = 10.0f;\
Info.bFireAndForget = true;\
Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));\
FSlateNotificationManager::Get().AddNotification(Info);\
return false


// 2022/05/10 PKT - DataTable 경로
const static FString DataTablePath = FString("/Game/DataCenter/DataTable/");


//==========================================================================================================================================
// 2022/05/10 PKT - 변환 대상의 Data Table
enum class ERealDTIndex
{
	PassivitySet,
	PassivityEffectStatSetData,
	PassivityEffectSkillUpSet,
	PassivityEffectInvokeSkillNotifySet,
	PassivityEffectInvokeAbnormalitySet,
	PassivityEffectInvokeCooldownSet,
	PassivityEffectSkillUpgradeSet,

	Max,
};

const static TMap<ERealDTIndex, FString> realDTPath =
{
	{ERealDTIndex::PassivitySet,							FString("Skill/Passivity/DTGsPassivitySet.DTGsPassivitySet")}
	, {ERealDTIndex::PassivityEffectStatSetData,			FString("Skill/Passivity/DTGsPassivityEffectStatSet.DTGsPassivityEffectStatSet")}
	, {ERealDTIndex::PassivityEffectSkillUpSet,				FString("Skill/Passivity/DTGsPassivityEffectSkillUpSet.DTGsPassivityEffectSkillUpSet")}
	, {ERealDTIndex::PassivityEffectInvokeSkillNotifySet,	FString("Skill/Passivity/DTGsPassivityEffectInvokeSkillNotifySet.DTGsPassivityEffectInvokeSkillNotifySet")}
	, {ERealDTIndex::PassivityEffectInvokeAbnormalitySet,	FString("Skill/Passivity/DTGsPassivityEffectInvokeAbnormalitySet.DTGsPassivityEffectInvokeAbnormalitySet")}
	, {ERealDTIndex::PassivityEffectInvokeCooldownSet,		FString("Skill/Passivity/DTGsPassivityEffectInvokeCooldownSet.DTGsPassivityEffectInvokeCooldownSet")}
	, {ERealDTIndex::PassivityEffectSkillUpgradeSet,		FString("Skill/Passivity/DTGsPassivityEffectSkillUpgradeSet.DTGsPassivityEffectSkillUpgradeSet")}
};
//==========================================================================================================================================


//==========================================================================================================================================
// 2022/05/10 PKT - Excel Data Table
enum class EExcelDTIndex
{
	PassivitySetExcel,
	PassivityEffectInfoExcel,
	PassivityEffectStatSetExcel,
	PassivityEffectStatGroupExcel,
	PassivityEffectSkillUpSetExcel,
	PassivityEffectSkillUpGroupExcel,
	PassivityEffectInvokeSkillNotifySetExcel,
	PassivityEffectInvokeAbnormalitySetExcel,
	PassivityEffectInvokeCooldownSetExcel,
	PassivityEffectSkillUpgradeSetExcel,
	Max,
};

const static TMap<EExcelDTIndex, FString> excelDTPath =
{
	{EExcelDTIndex::PassivitySetExcel,							FString("NeverCook/Skill/Passivity/DTGsPassivitySetExcel.DTGsPassivitySetExcel")}
	, {EExcelDTIndex::PassivityEffectInfoExcel,					FString("NeverCook/Skill/Passivity/DTGsPassivityEffectInfotExcel.DTGsPassivityEffectInfotExcel")}
	, {EExcelDTIndex::PassivityEffectStatSetExcel,				FString("NeverCook/Skill/Passivity/DTGsPassivityEffectStatSetExcel.DTGsPassivityEffectStatSetExcel")}
	, {EExcelDTIndex::PassivityEffectStatGroupExcel,			FString("NeverCook/Skill/Passivity/DTGsPassivityEffectStatGroupExcel.DTGsPassivityEffectStatGroupExcel")}
	, {EExcelDTIndex::PassivityEffectSkillUpSetExcel,			FString("NeverCook/Skill/Passivity/DTGsPassivityEffectSkillUpSetExcel.DTGsPassivityEffectSkillUpSetExcel")}
	, {EExcelDTIndex::PassivityEffectSkillUpGroupExcel,			FString("NeverCook/Skill/Passivity/DTGsPassivityEffectSkillUpSetGroupExcel.DTGsPassivityEffectSkillUpSetGroupExcel")}
	, {EExcelDTIndex::PassivityEffectInvokeSkillNotifySetExcel,	FString("NeverCook/Skill/Passivity/DTGsPassivityEffectInvokeSkillNotifySetExcel.DTGsPassivityEffectInvokeSkillNotifySetExcel")}
	, {EExcelDTIndex::PassivityEffectInvokeAbnormalitySetExcel,	FString("NeverCook/Skill/Passivity/DTGsPassivityInvokeAbnormalitySetExcel.DTGsPassivityInvokeAbnormalitySetExcel")}
	, {EExcelDTIndex::PassivityEffectInvokeCooldownSetExcel,	FString("NeverCook/Skill/Passivity/DTGsPassivityEffectInvokeCooldownSetExcel.DTGsPassivityEffectInvokeCooldownSetExcel")}
	, {EExcelDTIndex::PassivityEffectSkillUpgradeSetExcel,		FString("NeverCook/Skill/Passivity/DTGsPassivityEffectSkillUpgradeSetExcel.DTGsPassivityEffectSkillUpgradeSetExcel")}
};

//==========================================================================================================================================


//==========================================================================================================================================
// 2022/05/10 PKT - Helper Func
auto LoadHitEffectDataByRowName = [](const FName& InFindKey, UGsHitEffectData** OutNewData) -> bool
{
	(*OutNewData) = nullptr;

	const UGsTable* table = FGsSchemaHitEffectSetData::GetStaticTable();
	const FGsSchemaHitEffectSetData* findRow;
	if (table && table->FindRow(InFindKey, findRow))
	{
		const FString& path = findRow->path.GetAssetPathName().ToString();
		(*OutNewData) = LoadObject<UGsHitEffectData>(nullptr, *path);
		if (nullptr == (*OutNewData) || !(*OutNewData)->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Error, TEXT("nullptr == OutNewData || !OutNewData->IsValidLowLevel(), InPath:%s"), *path);
			(*OutNewData) = nullptr;
		}
	}

	return ((*OutNewData) != nullptr);
};


auto GetIconPathById = [](const FName& InFindKey, FSoftObjectPath* OutSoftObjectPath) -> bool
{
	(*OutSoftObjectPath) = nullptr;

	const UGsTable* table = FGsSchemaSkillIconSetData::GetStaticTable();
	const FGsSchemaSkillIconSetData* findRow;
	if (table && table->FindRow(InFindKey, findRow))
	{
		(*OutSoftObjectPath) = findRow->iconPath;
	}

	return ((*OutSoftObjectPath).IsNull() == false);
};

//==========================================================================================================================================

template <typename T = UGsDataTable, typename TRow = FGsSchemaRowBase>
void FillRowDataPassivityEffect(UGsDataTable* InTable, const FName& InRowName, TRow* OutRowData)
{
	OutRowData->DataTable = nullptr;
	OutRowData->RowName = NAME_None;

	if (InRowName.IsNone())
	{
		return;
	}

	const TMap<FName, uint8*>& rowMap = InTable->GetRowMap();
	for (const TPair<FName, uint8*>& itPair : rowMap)
	{
		const T* data = reinterpret_cast<const T*>(itPair.Value);
		if (nullptr == data)
		{
			continue;
		}

		if (0 == data->GetRowName().Compare(InRowName))
		{
			OutRowData->DataTable = InTable;
			OutRowData->RowName = data->GetRowName();
		}
	}
}

//==========================================================================================================================================
// 2022/05/10 PKT - Convert Func
bool ConvertPassivitySetData(const TMap<EExcelDTIndex, UGsDataTable*>& InExcelDTMap, TMap<ERealDTIndex, UGsDataTable*>& InRealDTMap)
{
	const UGsDataTable* excelPassivitySetDT = InExcelDTMap[EExcelDTIndex::PassivitySetExcel];
	
	auto BuildEffectInfoList = [&](int32 InGroupId, TArray<FGsSchemaPassivityEffectInfo>* OutEffectInfoList)
	{
		(*OutEffectInfoList).Empty();

		if (0 >= InGroupId)
		{
			return;
		}

		const UGsDataTable* excelPassivityEffectInfoDT = InExcelDTMap[EExcelDTIndex::PassivityEffectInfoExcel];
		const TMap<FName, uint8*>& rowMap = excelPassivityEffectInfoDT->GetRowMap();
		for (const auto& RowKvIt : rowMap)
		{
			const FGsSchemaPassivityEffectInfotExcel* excelPassivityEffectInfoData = reinterpret_cast<const FGsSchemaPassivityEffectInfotExcel*>(RowKvIt.Value);
			if (nullptr == excelPassivityEffectInfoData || InGroupId != excelPassivityEffectInfoData->effectInfoGroupId)
			{
				continue;
			}

			FGsSchemaPassivityEffectInfo effectInfo;
			effectInfo.effectId = excelPassivityEffectInfoData->effectId;
			effectInfo.conditionInfo.type = excelPassivityEffectInfoData->conditionInfoType;
			effectInfo.conditionInfo.method = excelPassivityEffectInfoData->conditionInfoMethod;
			effectInfo.executeConditionId = excelPassivityEffectInfoData->executeConditionId;

			if (effectInfo.conditionInfo.type == PassivityConditionType::ABNORMALITY_EFFECT_TYPE)
			{
				effectInfo.conditionInfo.param1 = 0;
				effectInfo.conditionInfo.param2 = 0;
				effectInfo.conditionInfo.param3 = excelPassivityEffectInfoData->param3;
			}
			else
			{
				effectInfo.conditionInfo.param1 = excelPassivityEffectInfoData->param1;
				effectInfo.conditionInfo.param2 = excelPassivityEffectInfoData->param2;
				effectInfo.conditionInfo.param3 = AbnormalityEffectType::NONE;
			}

			(*OutEffectInfoList).Emplace(effectInfo);
		}
	};

	// hitSoundData 채우기 위해 로드
	UGsDataTable* soundResDT = nullptr;
	FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Sound/DTGsSoundResData.DTGsSoundResData"), soundResDT);

	const TMap<FName, uint8*>& rowMap = excelPassivitySetDT->GetRowMap();
	for (const auto& RowKvIt : rowMap)
	{
		const FGsSchemaPassivitySetExcel* excelPassivitySetData = reinterpret_cast<const FGsSchemaPassivitySetExcel*>(RowKvIt.Value);
		if (nullptr == excelPassivitySetData || 0 >= excelPassivitySetData->id)
		{
			continue;
		}

		FGsSchemaPassivitySet realData;
		realData.id = excelPassivitySetData->id;
		realData.name = excelPassivitySetData->name;
		realData.nameText = excelPassivitySetData->nameText;
		realData.descText = excelPassivitySetData->descText;
		realData.type = excelPassivitySetData->type;
		
		// 2022/05/10 PKT - Build Effect Info List
		BuildEffectInfoList(excelPassivitySetData->effectInfoGroupId, &(realData.effectInfoList));

		realData.isBuff = excelPassivitySetData->isBuff;
		
		// 2022/05/10 PKT - Load Hit Effect Data
		LoadHitEffectDataByRowName(excelPassivitySetData->hitEffectId, &realData.hiteffectData);
		
		//FillRowDataPassivityEffect
		FillRowDataPassivityEffect<FGsSchemaSoundResData, FGsSchemaSoundResDataRow>(
			soundResDT, excelPassivitySetData->hitSoundId, &realData.hitSoundData);

		realData.hideUI = excelPassivitySetData->hideUI;

		// 2022/05/10 PKT - Load Icon Resource
		GetIconPathById(excelPassivitySetData->iconId, &realData.iconPath);

		// 2022/05/10 PKT - Load Small Icon Resource
		GetIconPathById(excelPassivitySetData->smallIconId, &realData.smallIconPath);
		
		realData.effectText = excelPassivitySetData->effectText;

		InRealDTMap[ERealDTIndex::PassivitySet]->AddRow(excelPassivitySetData->GetRowName(), realData);
	}


	return true;
}


bool ConvertPassivityStatSetData(const TMap<EExcelDTIndex, UGsDataTable*>& InExcelDTMap, TMap<ERealDTIndex, UGsDataTable*>& InRealDTMap)
{
	auto BuildEffectStat = [&](int32 InGroupId, TArray<FGsSchemaPassivityEffectStatParam>* OutEffectStatList)
	{
		(*OutEffectStatList).Empty();

		if (0 >= InGroupId)
		{
			return;
		}

		const UGsDataTable* excelDataTable = InExcelDTMap[EExcelDTIndex::PassivityEffectStatGroupExcel];
		const TMap<FName, uint8*>& rowMap = excelDataTable->GetRowMap();
		for (const auto& RowKvIt : rowMap)
		{
			const FGsSchemaPassivityEffectStatGroupExcel* excelData = reinterpret_cast<const FGsSchemaPassivityEffectStatGroupExcel*>(RowKvIt.Value);
			if (nullptr == excelData || InGroupId != excelData->groupId)
			{
				continue;
			}

			FGsSchemaPassivityEffectStatParam realData;
			realData.value = excelData->value;
			realData.type = excelData->type;

			(*OutEffectStatList).Emplace(realData);
		}
	};

	const UGsDataTable* excelEffectStatSetDT = InExcelDTMap[EExcelDTIndex::PassivityEffectStatSetExcel];

	const TMap<FName, uint8*>& rowMap = excelEffectStatSetDT->GetRowMap();
	for (const auto& RowKvIt : rowMap)
	{
		const FGsSchemaPassivityEffectStatSetExcel* excelData = reinterpret_cast<const FGsSchemaPassivityEffectStatSetExcel*>(RowKvIt.Value);
		if (nullptr == excelData)
		{
			continue;
		}


		FGsSchemaPassivityEffectStatSet realData;
		realData.id = excelData->passivityEffectStatId;
		BuildEffectStat(excelData->passivityEffectStatGroupId, &realData.param);

		InRealDTMap[ERealDTIndex::PassivityEffectStatSetData]->AddRow(excelData->GetRowName(), realData);
	}

	return true;
}


bool ConvertPassivityEffectSkillUpSet(const TMap<EExcelDTIndex, UGsDataTable*>& InExcelDTMap, TMap<ERealDTIndex, UGsDataTable*>& InRealDTMap)
{
	auto BuildSkillUpParam = [&](int32 InGroupId, TArray<FGsSchemaPassivityEffectSkillUpParam>* OutSkillUpParam)
	{
		(*OutSkillUpParam).Empty();

		if (0 >= InGroupId)
		{
			return;
		}

		const UGsDataTable* excelDataTable = InExcelDTMap[EExcelDTIndex::PassivityEffectSkillUpGroupExcel];
		const TMap<FName, uint8*>& rowMap = excelDataTable->GetRowMap();
		for (const auto& RowKvIt : rowMap)
		{
			const FGsSchemaPassivityEffectSkillUpSetGroupExcel* excelData = reinterpret_cast<const FGsSchemaPassivityEffectSkillUpSetGroupExcel*>(RowKvIt.Value);
			if (nullptr == excelData || InGroupId != excelData->passivityEffectSkillUpSetGroupId)
			{
				continue;
			}

			FGsSchemaPassivityEffectSkillUpParam realData;
			realData.userSkillSetId = excelData->userSkillSetId;
			realData.value = excelData->value;

			(*OutSkillUpParam).Emplace(realData);
		}
	};

	const UGsDataTable* excelDT = InExcelDTMap[EExcelDTIndex::PassivityEffectSkillUpSetExcel];

	const TMap<FName, uint8*>& rowMap = excelDT->GetRowMap();
	for (const auto& RowKvIt : rowMap)
	{
		const FGsSchemaPassivityEffectSkillUpSetExcel* excelData = reinterpret_cast<const FGsSchemaPassivityEffectSkillUpSetExcel*>(RowKvIt.Value);
		if (nullptr == excelData)
		{
			continue;
		}

		FGsSchemaPassivityEffectSkillUpSet realData;
		realData.id = excelData->id;
		BuildSkillUpParam(excelData->passivityEffectSkillUpSetGroupId, &realData.param);

		InRealDTMap[ERealDTIndex::PassivityEffectSkillUpSet]->AddRow(excelData->GetRowName(), realData);
	}

	return true;
}

bool ConvertPassivityEffectInvokeSkillNotifySet(const TMap<EExcelDTIndex, UGsDataTable*>& InExcelDTMap, TMap<ERealDTIndex, UGsDataTable*>& InRealDTMap)
{
	const UGsDataTable* orginalDT = InExcelDTMap[EExcelDTIndex::PassivityEffectInvokeSkillNotifySetExcel];

	// 2022/05/11 PKT - 에러 처리는 느슨하게..
	UGsDataTable* skillNotiftSetDT = nullptr;
	const FString skillNotifyDTPath = DataTablePath + "Skill/DTGsSkillNotifySet.DTGsSkillNotifySet";
	FGsDataTableConvertBase::LoadDataTable(skillNotifyDTPath, skillNotiftSetDT);

	auto FindSkillNotifySet = [&skillNotiftSetDT](const FName& InRowName, int InKey, FGsSchemaSkillNotifySetRow* OutRowData)
	{
		(*OutRowData).DataTable = nullptr;
		(*OutRowData).RowName = NAME_None;

		if (const UGsTableSkillNotifySet* gsTable = Cast<UGsTableSkillNotifySet>(FGsSchemaSkillNotifySet::GetStaticTable()))
		{
			const FGsSchemaSkillNotifySet* outSchema;
			if (gsTable->FindRowById(InKey, outSchema))
			{
				(*OutRowData).DataTable = skillNotiftSetDT;
				(*OutRowData).RowName = outSchema->GetRowName();
			}
			else
			{
				FText message = FText::FromString(FString::Printf(
					TEXT("PassivityEffectInvokeSkillNotifySet에서 SkillNotifySet 정보를 찾을수 없습니다. ( %s )"),
					*InRowName.ToString()));
				DEF_ERR_POPUP_MESSAGE(message);
			}
		}
		return true;
	};


	const TMap<FName, uint8*>& rowMap = orginalDT->GetRowMap();
	for (const auto& RowKvIt : rowMap)
	{
		const FGsSchemaPassivityEffectInvokeSkillNotifySetExcel* excelData = reinterpret_cast<const FGsSchemaPassivityEffectInvokeSkillNotifySetExcel*>(RowKvIt.Value);
		if (nullptr == excelData)
		{
			continue;
		}

		FGsSchemaPassivityEffectInvokeSkillNotifySet realData;
		realData.id = excelData->id;
		realData.param.ratePct = excelData->ratePct;
		realData.param.rateCondition = excelData->rateCondition;
		realData.param.cause = excelData->cause;
		realData.param.causeInfo.type = excelData->cause;
		realData.param.causeInfo.id = excelData->causeParamId;
		realData.param.causeInfo.abnormalityEffectType = excelData->causeParam1;
		realData.param.causeInfo.itemCategorySub = excelData->causeParam2;
		realData.param.causeInfo.itemGrade = excelData->causeParam3;
		realData.param.cooldownTime = excelData->cooldownTime;
		realData.param.target = excelData->target;
		// 2022/05/11 PKT - Find SkilNotifySet
		if (false == FindSkillNotifySet(excelData->RowName, excelData->skillNotifyId, &realData.param.skillNotifyId))
		{
			return false;
		}

		InRealDTMap[ERealDTIndex::PassivityEffectInvokeSkillNotifySet]->AddRow(excelData->GetRowName(), realData);
	}

	return true;
}


bool ConvertPassivityEffectInvokeAbnormalitySet(const TMap<EExcelDTIndex, UGsDataTable*>& InExcelDTMap, TMap<ERealDTIndex, UGsDataTable*>& InRealDTMap)
{
	const UGsDataTable* orginalDT = InExcelDTMap[EExcelDTIndex::PassivityEffectInvokeAbnormalitySetExcel];

	// 2022/05/11 PKT - 에러 처리는 느슨하게..
	UGsDataTable* abnormalitySetDT = nullptr;
	const FString abnormalitySetDTPath = DataTablePath + "Abnormality/DTGsAbnormalitySet.DTGsAbnormalitySet";
	FGsDataTableConvertBase::LoadDataTable(abnormalitySetDTPath, abnormalitySetDT);

	auto FindAbnormalitySet = [&abnormalitySetDT](const FName& InRowName, int InKey, FGsSchemaAbnormalitySetRow* OutRowData)
	{
		(*OutRowData).DataTable = nullptr;
		(*OutRowData).RowName = NAME_None;

		if (const UGsTableAbnormalitySet* gsTable = Cast<UGsTableAbnormalitySet>(FGsSchemaAbnormalitySet::GetStaticTable()))
		{
			const FGsSchemaAbnormalitySet* outSchema;
			if (gsTable->FindRowById(InKey, outSchema))
			{
				(*OutRowData).DataTable = abnormalitySetDT;
				(*OutRowData).RowName = outSchema->GetRowName();
			}
			else
			{
				FText message = FText::FromString(FString::Printf(
					TEXT("PassivityInvokeAbnormalitySet에서 AbnormalitySet 정보를 찾을수 없습니다. ( %s )"),
					*InRowName.ToString()));
				DEF_ERR_POPUP_MESSAGE(message);
			}
		}
		return true;
	};

	const TMap<FName, uint8*>&rowMap = orginalDT->GetRowMap();
	for (const auto& RowKvIt : rowMap)
	{
		const FGsSchemaPassivityInvokeAbnormalitySetExcel* excelData = reinterpret_cast<const FGsSchemaPassivityInvokeAbnormalitySetExcel*>(RowKvIt.Value);
		if (nullptr == excelData)
		{
			continue;
		}

		FGsSchemaPassivityEffectInvokeAbnormalitySet realData;
		realData.id = excelData->id;
		realData.param.ratePct = excelData->ratePct;
		realData.param.cause = excelData->cause;
		realData.param.causeInfo.type = excelData->cause;
		realData.param.causeInfo.id = excelData->causeParamId;
		realData.param.causeInfo.abnormalityEffectType = excelData->causeParam1;
		realData.param.causeInfo.itemCategorySub = excelData->causeParam2;
		realData.param.causeInfo.itemGrade = excelData->causeParam3;
		realData.param.cooldownTime = excelData->cooldownTime;
		realData.param.target = excelData->target;
		// 2022/05/11 PKT - Find AbnormalitySet
		if (false == FindAbnormalitySet(excelData->RowName, excelData->abnormalityId, &realData.param.abnormalityId))
		{
			return false;
		}

		InRealDTMap[ERealDTIndex::PassivityEffectInvokeAbnormalitySet]->AddRow(excelData->GetRowName(), realData);
	}

	return true;
}

bool ConvertPassivityEffectInvokeCooldownSet(const TMap<EExcelDTIndex, UGsDataTable*>& InExcelDTMap, TMap<ERealDTIndex, UGsDataTable*>& InRealDTMap)
{
	const UGsDataTable* orginalDT = InExcelDTMap[EExcelDTIndex::PassivityEffectInvokeCooldownSetExcel];

	UGsDataTable* userSkillSetDT = nullptr;
	const FString userSkillSetDTPath = DataTablePath + "User/DTGsUserSkillSet.DTGsUserSkillSet";
	FGsDataTableConvertBase::LoadDataTable(userSkillSetDTPath, userSkillSetDT);

	auto FindSkillSet = [&userSkillSetDT](const FName& InRowName, int InKey, FGsSchemaUserSkillSetRow* OutRowData)
	{
		(*OutRowData).DataTable = nullptr;
		(*OutRowData).RowName = NAME_None;

		if (const UGsTableUserSkillSet* gsTable = Cast<UGsTableUserSkillSet>(FGsSchemaUserSkillSet::GetStaticTable()))
		{
			const FGsSchemaUserSkillSet* outSchema;
			if (gsTable->FindRowById(InKey, outSchema))
			{
				(*OutRowData).DataTable = userSkillSetDT;
				(*OutRowData).RowName = outSchema->GetRowName();
			}
			else
			{
				FText message = FText::FromString(FString::Printf(
					TEXT("PassivityEffectInvokeCooldownSet에서 UserSkillSet 정보를 찾을수 없습니다. : ( %s )"),
					*InRowName.ToString()));
				DEF_ERR_POPUP_MESSAGE(message);
			}
		}
		return true;
	};

	TMap<int64, TArray<const FGsSchemaPassivityEffectInvokeCooldownSetExcel*>> templeteList;
	const TMap<FName, uint8*>& rowMap = orginalDT->GetRowMap();
	for (const auto& e : rowMap)
	{
		if (const auto excelData = reinterpret_cast<const FGsSchemaPassivityEffectInvokeCooldownSetExcel*>(e.Value))
		{
			auto& tempData = templeteList.FindOrAdd(excelData->id);
			tempData.Add(excelData);
		}
	}

	for (auto& pair : templeteList)
	{
		if (auto iter = pair.Value.FindByPredicate([](auto e) { return e != nullptr; }))
		{
			const auto excelData = *iter;
			FGsSchemaPassivityEffectInvokeCooldownSet realData;
			realData.id = excelData->id;
			realData.param.ratePct = excelData->ratePct;
			realData.param.cause = excelData->cause;
			realData.param.causeInfo.type = excelData->cause;
			realData.param.causeInfo.id = excelData->causeParamId;
			realData.param.causeInfo.abnormalityEffectType = excelData->causeParam1;
			realData.param.causeInfo.itemCategorySub = excelData->causeParam2;
			realData.param.causeInfo.itemGrade = excelData->causeParam3;
			realData.param.cooldownTime = excelData->cooldownTime;
			realData.param.target = excelData->target;
			realData.param.targetCooldownRate = excelData->targetCooldownRate;
			realData.param.targetAllUserSkillSetId = excelData->targetAllUserSkillSetId;

			for (const auto targetExcelData : pair.Value)
			{
				for (int skillId : targetExcelData->targetUserSkillSetIdList)
				{
					FGsSchemaUserSkillSetRow targetSkillSetRow;
 					if (FindSkillSet(excelData->RowName, skillId, &targetSkillSetRow))
 					{
 						realData.param.targetUserSkillSetIdList.Add(targetSkillSetRow);
 					}
				}
			}

			//if(realData.param.targetUserSkillSetId.IsValidIndex(0))
			InRealDTMap[ERealDTIndex::PassivityEffectInvokeCooldownSet]->AddRow(excelData->GetRowName(), realData);
		}
	}

	return true;
}

bool ConvertPassivityEffectSkillUpgradeSet(const TMap<EExcelDTIndex, UGsDataTable*>& InExcelDTMap, TMap<ERealDTIndex, UGsDataTable*>& InRealDTMap)
{
	UGsDataTable* skillUpgradeDT = nullptr;
	FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Skill/Upgrade/DTGsSkillUpgradeSet.DTGsSkillUpgradeSet"), skillUpgradeDT);

	const UGsDataTable* excelDT = InExcelDTMap[EExcelDTIndex::PassivityEffectSkillUpgradeSetExcel];

	const TMap<FName, uint8*>& rowMap = excelDT->GetRowMap();
	for (const auto& RowKvIt : rowMap)
	{
		const FGsSchemaPassivityEffectSkillUpgradeSetExcel* excelData = reinterpret_cast<const FGsSchemaPassivityEffectSkillUpgradeSetExcel*>(RowKvIt.Value);
		if (nullptr == excelData)
		{
			continue;
		}

		FGsSchemaPassivityEffectSkillUpgradeSet realData;
		realData.id = excelData->id;

		realData.param.currentStep = excelData->currentStep;
		realData.param.essentialPassivityId = excelData->essentialPassivityId;
		FillRowDataPassivityEffect<FGsSchemaSkillUpgradeSet, FGsSchemaSkillUpgradeSetRow>(
			skillUpgradeDT, excelData->skillUpgradeId, &realData.param.skillUpgradeId);		

		InRealDTMap[ERealDTIndex::PassivityEffectSkillUpgradeSet]->AddRow(excelData->GetRowName(), realData);
	}

	return true;
}

//==========================================================================================================================================


// 2022/05/11 PKT - SkillPassivitySet
IMPLEMENT_DATATABLE_CONVERTER(DataTableConverterSkillPassivitySet)
bool DataTableConverterSkillPassivitySet::Convert()
{
	// 2022/05/10 PKT - Load Real Data Table
	TMap<ERealDTIndex, UGsDataTable*> reallDTSet;
	for (int32 idx = 0; idx < StaticCast<int32>(ERealDTIndex::Max); ++idx)
	{
		ERealDTIndex realIndex = StaticCast<ERealDTIndex>(idx);
		if (false == realDTPath.Contains(realIndex))
		{
			FText message = FText::FromString(FString::Printf(TEXT("not found Target DT File : [%d]"), idx));
			DEF_ERR_POPUP_MESSAGE(message);
		}

		FString filePath = DataTablePath + realDTPath[realIndex];

		if (false == FGsDataTableConvertBase::LoadDataTable(filePath, reallDTSet.FindOrAdd(realIndex)))
		{
			FText message = FText::FromString(FString::Printf(TEXT("not found DT TableAsset : [%s]"), *filePath));
			DEF_ERR_POPUP_MESSAGE(message);
		}
	}

	// 2022/05/10 PKT - Load Excel Data Table
	TMap<EExcelDTIndex, UGsDataTable*> excelDTSet;
	for (int32 idx = 0; idx < StaticCast<int32>(EExcelDTIndex::Max); ++idx)
	{
		EExcelDTIndex excelIndex = StaticCast<EExcelDTIndex>(idx);
		if (false == excelDTPath.Contains(excelIndex))
		{
			FText message = FText::FromString(FString::Printf(TEXT("not found Target DT File : [%d]"), idx));
			DEF_ERR_POPUP_MESSAGE(message);
		}

		FString filePath = DataTablePath + excelDTPath[excelIndex];

		if (false == FGsDataTableConvertBase::LoadDataTable(filePath, excelDTSet.FindOrAdd(excelIndex)))
		{
			FText message = FText::FromString(FString::Printf(TEXT("not found DT TableAsset : [%s]"), *filePath));
			DEF_ERR_POPUP_MESSAGE(message);
		}
	}


	// 2022/05/03 PKT - Real DT Empty
	for (auto& pair : reallDTSet)
	{
		pair.Value->EmptyTable();
	}

	ConvertPassivitySetData(excelDTSet, reallDTSet);
	ConvertPassivityStatSetData(excelDTSet, reallDTSet);
	ConvertPassivityEffectSkillUpSet(excelDTSet, reallDTSet);
	ConvertPassivityEffectInvokeSkillNotifySet(excelDTSet, reallDTSet);
	ConvertPassivityEffectInvokeAbnormalitySet(excelDTSet, reallDTSet);
	ConvertPassivityEffectInvokeCooldownSet(excelDTSet, reallDTSet);
	ConvertPassivityEffectSkillUpgradeSet(excelDTSet, reallDTSet);

	// 2022/05/10 PKT - Real DT Save
	for (auto& pair : reallDTSet)
	{
		pair.Value->SavePackage();
	}

	return true;
}