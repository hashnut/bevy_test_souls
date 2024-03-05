
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

#include "DataTableConverter/GsDataTableConvertBase.h"
#include "GsDataTable.h"
#include "Helper/RSQLEditorUtil.h"	

#include "Text.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h" 

#include "../../../../T1Project/UTIL/GsTableUtil.h"
#include "../../../../T1Project/UTIL/GsEnum.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/NeverCook/Abnormality/GsSchemaAbnormalityEffectSet.h"
#include "DataSchema/NeverCook/Abnormality/GsSchemaAbnormalityEffectParamConvert.h"
#include "DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityStatSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityEffectUtilSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityOvertimeSet.h"
#include "DataSchema/Abnormality/AbnormalityEffect/GsSchemaAbnormalityCrowdControlSet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalityGroupSet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalityEffectGroupSet.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalityRemoveGroupSet.h"

// 공통 PARAM 구조체 데이터 설정
#define DEF_CONVERT_TABLE_INITIALIZE_SET(TSchema, Name)\
TSchema.RowName = Name;\
TSchema.id = effectSet->id;\
TSchema.level = effectSet->level;\
TSchema.type = effectSet->abnormalityEffectType

#define DEF_ERR_POPUP_MESSAGE(Message)\
FNotificationInfo Info(Message);\
Info.ExpireDuration = 10.0f;\
Info.bFireAndForget = true;\
Info.Image = FCoreStyle::Get().GetBrush(TEXT("MessageLog.Warning"));\
FSlateNotificationManager::Get().AddNotification(Info);\
UE_LOG(LogTemp, Error, TEXT("[AbnormalitySet Converter] %s"), *Message.ToString())


IMPLEMENT_DATATABLE_CONVERTER(FDataTableConverterAbnormalityEffectSet)
bool FDataTableConverterAbnormalityEffectSet::Convert()
{
	//------------------------------------------------------------------------------------------------------------------
	// AbnormalitySet 검증 구문
	//------------------------------------------------------------------------------------------------------------------
	UGsDataTable* DTGsAbnormalitySet = nullptr;	
	UGsDataTable* DTGsAbnormalityEffectSet = nullptr;	

	// 테이블 로드
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Abnormality/DTGsAbnormalitySet.DTGsAbnormalitySet"),
		DTGsAbnormalitySet))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsAbnormalitySet)"));
		DEF_ERR_POPUP_MESSAGE(message);		
	}	

	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/NeverCook/Abnormality/DTGsAbnormalityEffectSet.DTGsAbnormalityEffectSet"),
		DTGsAbnormalityEffectSet))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsAbnormalityEffectSet)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	const UGsTableAbnormalityEffectSet* abnoramlityEffectSetTable = Cast<UGsTableAbnormalityEffectSet>(FGsSchemaAbnormalityEffectSet::GetStaticTable());
	if (nullptr == abnoramlityEffectSetTable)
	{
		return false;
	}

	// AbnormalityEffectSet 인덱스 버퍼
	TMap<int, const FGsSchemaAbnormalityEffectSet*> mapAbnormalityEffect;
	const TArray<FName> effectSetRowName(DTGsAbnormalityEffectSet->GetRowNames());
	for (const FName& rowName : effectSetRowName)
	{
		if (const FGsSchemaAbnormalityEffectSet* abnormalityEffectSet =
			DTGsAbnormalityEffectSet->FindRow<FGsSchemaAbnormalityEffectSet>(rowName, FString()))
		{
			if (false == mapAbnormalityEffect.Contains(abnormalityEffectSet->id))
			{
				mapAbnormalityEffect.Emplace(abnormalityEffectSet->id, abnormalityEffectSet);
			}
		}
	}

	// 그룹 정보 인덱스 테이블 처리
	TMap<int32, TArray<int32>> mapGroupSet;
	TMap<int32, TArray<int32>> mapEffectGroupSet;
	TMap<int32, TArray<int32>> mapRemoveGroupSet;

 	const TArray<FName> abnormalitySetRowName(DTGsAbnormalitySet->GetRowNames());
 	for (const FName& rowName : abnormalitySetRowName)
 	{
 		if (FGsSchemaAbnormalitySet* abnormalitySet =
 			DTGsAbnormalitySet->FindRow<FGsSchemaAbnormalitySet>(rowName, FString()))
 		{
 			// effectSet Id valid 검사
 			for (int32 effectsetId : abnormalitySet->effectIdList)
 			{
 				if (const FGsSchemaAbnormalityEffectSet** findEffectSet = mapAbnormalityEffect.Find(effectsetId))
 				{
					const UGsTableAbnormalityEffectParamConvert* effectParamConvert = Cast<UGsTableAbnormalityEffectParamConvert>(
						FGsSchemaAbnormalityEffectParamConvert::GetStaticTable());
					const FGsSchemaAbnormalityEffectParamConvert* effectParamConvertTable;
					effectParamConvert->FindRowById((*findEffectSet)->abnormalityEffectType, effectParamConvertTable);
					if (nullptr == effectParamConvert)
					{
						continue;
					}

					bool isOverTime = effectParamConvertTable->convertType == EGsAbnormalityEffectConvertParamType::OVERTIME;
 					// OverTime 타입류 Ticktime 검사
 					if (isOverTime && abnormalitySet->tickTime <= 0)
 					{
 						FString setId = FString::FromInt(abnormalitySet->id);
 						FString errectId = FString::FromInt(effectsetId);
 						FText failedMessage = FText::Format(
 							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet", "OVERTIME 타입이지만 tickTime 설정이 없습니다.. abnormalitySetId : {0} abnormalityEffectSetId {1}"),
 							FText::FromString(setId), FText::FromString(errectId));
 
 						DEF_ERR_POPUP_MESSAGE(failedMessage);
 					}
 
 					// EnumType Valid 검사
 					if (false == IsValidAbnormalityEffectType((*findEffectSet)->abnormalityEffectType))
 					{
 						FString setId = FString::FromInt(abnormalitySet->id);
 						FString errectId = FString::FromInt(effectsetId);
 						FText failedMessage = FText::Format(
 							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet", "AbnormalityEffectMainType이 유효하지않은 값입니다. abnormalitySetId : {0} abnormalityEffectSetId {1}"),
 							FText::FromString(setId), FText::FromString(errectId));
 
 						DEF_ERR_POPUP_MESSAGE(failedMessage);
 					}
 
 					if (false == IsValidAbnormalityEffectType((*findEffectSet)->abnormalityEffectType))
 					{
 						FString setId = FString::FromInt(abnormalitySet->id);
 						FString errectId = FString::FromInt(effectsetId);
 						FText failedMessage = FText::Format(
 							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet", "AbnormalityEffectSubType이 유효하지않은 값입니다. abnormalitySetId : {0} abnormalityEffectSetId {1}"),
 							FText::FromString(setId), FText::FromString(errectId));
 
 						DEF_ERR_POPUP_MESSAGE(failedMessage);
 					}
 				}
 				else
 				{
 					FString setId = FString::FromInt(abnormalitySet->id);
 					FString errectId = FString::FromInt(effectsetId);
 					FText failedMessage = FText::Format(
 						NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet", "abnormalitySet에 해당 정보가 없습니다. abnormalitySetId : {0} abnormalityEffectSetId {1}"),
 						FText::FromString(setId), FText::FromString(errectId));
 						
 					DEF_ERR_POPUP_MESSAGE(failedMessage);
 				}
 			}

			// 레거시 컬럼 컨버팅
			// groupId : 완료후 제거 예정
 			if (abnormalitySet->groupId != 0)
 			{
 				abnormalitySet->stackGroupId = abnormalitySet->groupId;
 			}

			// 그룹 캐싱 데이터 추가
 			if (abnormalitySet->stackGroupId != 0)
 			{
 				TArray<int32>* findList = mapGroupSet.Find(abnormalitySet->stackGroupId);
 				if (nullptr == findList)
 				{
 					TArray<int32> newList;
 					findList = &mapGroupSet.Emplace(abnormalitySet->stackGroupId, newList);
 				}
 				findList->Emplace(abnormalitySet->id);
 			}
			for (int32 groupId : abnormalitySet->effectGroupIdList)
			{
// 				TArray<int32>* findList = mapEffectGroupSet.Find(groupId);
// 				if (nullptr == findList)
// 				{
// 					findList = &mapEffectGroupSet.Emplace(groupId, TArray<int32>());
// 				}
// 				findList->Emplace(abnormalitySet->id);
				TArray<int32>* findList = mapGroupSet.Find(groupId);
				if (nullptr == findList)
				{
					TArray<int32> newList;
					findList = &mapGroupSet.Emplace(groupId, newList);
				}
				findList->Emplace(abnormalitySet->id);
			}
			for (int32 groupId : abnormalitySet->removeGroupIdList)
			{
// 				TArray<int32>* findList = mapRemoveGroupSet.Find(groupId);
// 				if (nullptr == findList)
// 				{
// 					findList = &mapRemoveGroupSet.Emplace(groupId, TArray<int32>());
// 				}
// 				findList->Emplace(abnormalitySet->id);
				TArray<int32>* findList = mapGroupSet.Find(groupId);
				if (nullptr == findList)
				{
					TArray<int32> newList;
					findList = &mapGroupSet.Emplace(groupId, newList);
				}
				findList->Emplace(abnormalitySet->id);
			}
 		}
 	}
	
	UGsDataTable* DTGsAbnormalityGroupSet = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Abnormality/DTGsAbnormalityGroupSet.DTGsAbnormalityGroupSet"),
		DTGsAbnormalityGroupSet))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsAbnormalitySet)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	// 이 그룹 테이블은 엑셀로 빠져 있으므로 순차 검사해서 abnormalityId만 채워 넣는다.
	TArray<int32> removeList;
	for (const FName& rowName : DTGsAbnormalityGroupSet->GetRowNames())
	{
		if (FGsSchemaAbnormalityGroupSet* groupSet =
			DTGsAbnormalityGroupSet->FindRow<FGsSchemaAbnormalityGroupSet>(rowName, FString()))
		{
			if (false == mapGroupSet.Contains(groupSet->groupId))
			{
				FString groupId = FString::FromInt(groupSet->groupId);
				FText message = FText::Format(NSLOCTEXT("not found", "FDataTableConverterAbnormalityEffectSet",
					"GroupSet 테이블에 있는 groupId({ 0 })를 AbnormalitySet에서 찾을 수 없습니다."),
					FText::FromString(groupId));
				DEF_ERR_POPUP_MESSAGE(message);
			}
			else
			{
				groupSet->abnormalityId = mapGroupSet.FindAndRemoveChecked(groupSet->groupId);
			}
		}
	}

	for (TPair<int32, TArray<int32>>& pair : mapGroupSet)
	{
		FString groupId = FString::FromInt(pair.Key);
		FText message = FText::Format(NSLOCTEXT("not found", "FDataTableConverterAbnormalityEffectSet",
			"AbnormalitySet 테이블에 있는 GroupId({ 0 })가 GroupSet 테이블에서 찾을 수 없습니다."),
			FText::FromString(groupId));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	DTGsAbnormalityGroupSet->SavePackage();

	UGsDataTable* DTGsAbnormalityEffectGroupSet = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Abnormality/DTGsAbnormalityEffectGroupSet.DTGsAbnormalityEffectGroupSet"),
		DTGsAbnormalityEffectGroupSet))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsAbnormalitySet)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	DTGsAbnormalityEffectGroupSet->EmptyTable();
	for (TPair<int32, TArray<int32>>& pair : mapEffectGroupSet)
	{
		FGsSchemaAbnormalityEffectGroupSet newInfo;
		newInfo.groupId = pair.Key;
		newInfo.abnormalityId = pair.Value;
		DTGsAbnormalityEffectGroupSet->AddRow(FName(FString::FromInt(pair.Key)), newInfo);
	}
	DTGsAbnormalityEffectGroupSet->SavePackage();

	UGsDataTable* DTGsAbnormalityRemoveGroupSet = nullptr;
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Abnormality/DTGsAbnormalityRemoveGroupSet.DTGsAbnormalityRemoveGroupSet"),
		DTGsAbnormalityRemoveGroupSet))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsAbnormalitySet)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	DTGsAbnormalityRemoveGroupSet->EmptyTable();
	for (TPair<int32, TArray<int32>>& pair : mapRemoveGroupSet)
	{
		FGsSchemaAbnormalityRemoveGroupSet newInfo;
		newInfo.groupId = pair.Key;
		newInfo.abnormalityId = pair.Value;
		DTGsAbnormalityRemoveGroupSet->AddRow(FName(FString::FromInt(pair.Key)), newInfo);
	}
	DTGsAbnormalityRemoveGroupSet->SavePackage();

	
	//------------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------------
	// 통합 테이블(AbnormalityEffectSet) -> 중간 데이블 변환 (AbnormalityIndexSet)
	//------------------------------------------------------------------------------------------------------------------
	static const FString effectSetPath(TEXT("/Game/DataCenter/DataTable/Abnormality/AbnormalityEffect/"));
	UGsDataTable* DTGsAbnormalityEffectParamConvert = nullptr;
	UGsDataTable* DTGsAbnormalityEffectIndexSet = nullptr;
	UGsDataTable* DTGsSkillNotifySet = nullptr;

	// 테이블 로드
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Abnormality/AbnormalityConvert/DTGsAbnormalityIndexSet.DTGsAbnormalityIndexSet"),
		DTGsAbnormalityEffectIndexSet))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsAbnormalityIndexSet)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/Skill/DTGsSkillNotifySet.DTGsSkillNotifySet"),
		DTGsSkillNotifySet))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsSkillNotifySet)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}
	
	if (!FGsDataTableConvertBase::LoadDataTable(
		TEXT("/Game/DataCenter/DataTable/NeverCook/Abnormality/DTGsAbnormalityEffectParamConvert.DTGsAbnormalityEffectParamConvert"),
		DTGsAbnormalityEffectParamConvert))
	{
		FText message = FText::FromString(TEXT("not found TableAsset!!(DTGsAbnormalityEffectParamConvert)"));
		DEF_ERR_POPUP_MESSAGE(message);
	}

	// AbnormalityEffectType에 따른 하위 테이블 정보
	TMap<AbnormalityEffectType, TPair<EGsAbnormalityEffectConvertParamType, UGsDataTable*>> mapEffectTable;
	// AbnormalityEffect 로드 테이블 포인터
	TMap<FString, UGsDataTable*> uniqueEffectTableList;

	const TArray<FName> abnormalityEffectParamRowName(DTGsAbnormalityEffectParamConvert->GetRowNames());
	for (const FName& rowName : abnormalityEffectParamRowName)
	{
		if (const FGsSchemaAbnormalityEffectParamConvert* paramConver =
			DTGsAbnormalityEffectParamConvert->FindRow<FGsSchemaAbnormalityEffectParamConvert>(rowName, FString()))
		{
			UGsDataTable** findEffectTable = uniqueEffectTableList.Find(paramConver->convertTableName);
			if (nullptr == findEffectTable)
			{
				FString path(TEXT("/Game/DataCenter/DataTable/Abnormality/AbnormalityEffect/"));
				path = path + paramConver->convertTableName + TEXT(".") + paramConver->convertTableName;
				UGsDataTable* abnormalityEffectTable = nullptr;
				FGsDataTableConvertBase::LoadDataTable(path, abnormalityEffectTable);
				if (nullptr == abnormalityEffectTable)
				{
					FString strEnum;
					const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("AbnormalityEffectType"), true);
					if (!enumPtr)
					{
						strEnum = TEXT("Invalid");
					}
					strEnum = enumPtr->GetNameStringByIndex((int)paramConver->abnormalityEffectType);
					FText message = FText::Format(
						NSLOCTEXT("not found", "FDataTableConverterAbnormalityEffectSet", "not found TableAsset!! abnormalityType : {0}"),
						FText::FromString(strEnum));

					DEF_ERR_POPUP_MESSAGE(message);
				}
				else
				{
					findEffectTable = &uniqueEffectTableList.Emplace(paramConver->convertTableName, abnormalityEffectTable);
				}
			}

			if (findEffectTable)
			{
 				TPair<EGsAbnormalityEffectConvertParamType, UGsDataTable*> newPair(
 					paramConver->convertType, *findEffectTable);
 				mapEffectTable.Emplace(paramConver->abnormalityEffectType, newPair);
			}
		}
	}

	// Effect 테이블 비우기
	for (TPair<FString, UGsDataTable*>& el : uniqueEffectTableList)
	{
		el.Value->EmptyTable();
	}

	// Index 테이블 비우기
	DTGsAbnormalityEffectIndexSet->EmptyTable();
	
	// 통합 EffectSet
	const TArray<FName> abnormalityEffectSetRowName(DTGsAbnormalityEffectSet->GetRowNames());
	for (const FName& rowName : abnormalityEffectSetRowName)
	{
		if (const FGsSchemaAbnormalityEffectSet* effectSet = 
			DTGsAbnormalityEffectSet->FindRow<FGsSchemaAbnormalityEffectSet>(rowName, FString()))
		{
			// RowName 설정
			FName keyName = FName(*FString::Printf(TEXT("%d.%d"), effectSet->id, effectSet->level));
			// 인덱스 테이블 찾기 및 생성
			const FGsSchemaAbnormalityIndexSet* findIndexSet = nullptr;
			const UGsTableAbnormalityIndexSet* gsTable = Cast<UGsTableAbnormalityIndexSet>(FGsSchemaAbnormalityIndexSet::GetStaticTable());
			gsTable->FindRowById(effectSet->id, findIndexSet);

			// 없다면 신규 생성
			if (nullptr == findIndexSet)
			{
				FGsSchemaAbnormalityIndexSet newSet;
				newSet.id = effectSet->id;
				newSet.effectType = effectSet->abnormalityEffectType;
				// newSet.animationTypeInfo = effectSet->animationTypeInfo;

				// Level과 Index 정보를 일치 시키기위해 0번째 빈원소 생성
				FGsSchemaAbnormalityIndexLevelInfo emptyLevelInfo;
				newSet.levelList.Add(emptyLevelInfo);
				DTGsAbnormalityEffectIndexSet->AddRow(keyName, newSet);
				gsTable->FindRowById(effectSet->id, findIndexSet);
			}

			// Level 채우기
			// const 속성 제거
			FGsSchemaAbnormalityIndexSet* constCastIndexSet = const_cast<FGsSchemaAbnormalityIndexSet*>(findIndexSet);
			// 현재 해당하는 레벨 보다 적게 생성되어있으면 빈원소로 맞춤
			if (constCastIndexSet->levelList.Num() < effectSet->level)
			{
				TArray<FGsSchemaAbnormalityIndexLevelInfo> emptyInfos;
				emptyInfos.AddZeroed((effectSet->level - constCastIndexSet->levelList.Num()));
				constCastIndexSet->levelList.Append(emptyInfos);
			}

			// AbnormalityEffectMainType 으로 하위 테이블들을 생성하고 값을 채운다.
			TPair<EGsAbnormalityEffectConvertParamType, UGsDataTable*>* findInfo = mapEffectTable.Find(effectSet->abnormalityEffectType);
			if (nullptr == findInfo)
			{
				continue;
			}

			UGsDataTable* currTable = findInfo->Value;
			if (nullptr == currTable)
			{
				continue;
			}
			// AbnormalityEffectType에 존재하는 하위 테이블들 설정
			// param 설정의 변경 이슈로 인해 EGsAbnormalityEffectConvertParamType이 추가 된다면 작업이 필요..
			switch (findInfo->Key)
			{
			case EGsAbnormalityEffectConvertParamType::OVERTIME:
				{
					FGsSchemaAbnormalityOvertimeSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					// Param 정보들 설정
					info.param.targetType = SkillTargetTypeStringToEnum(effectSet->param1);
					info.param.targetStat = StatTypeStringToEnum(effectSet->param2);
					info.param.effectMethodType = AbnormalityEffectAddSubMethodTypeStringToEnum(effectSet->param3);
					if (false == IsValidAbnormalityEffectAddSubMethodType(info.param.effectMethodType))
					{
						FString setId = FString::FromInt(effectSet->id);
						FText failedMessage = FText::Format(
							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet",
								"AbnormalityEffectAddSubMethodType이 유효하지 않습니다. abnormalitySetId : {0}"),
							FText::FromString(setId));
						DEF_ERR_POPUP_MESSAGE(failedMessage);
					}
					info.param.value = FCString::Atoi(*effectSet->param4);
					info.param.damageCategory = DamageStatTypeStringToEnum(effectSet->param5);
					if (false == IsValidDamageStatType(info.param.damageCategory))
					{
						FString setId = FString::FromInt(effectSet->id);
						FText failedMessage = FText::Format(
							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet",
								"DamageStatType이 유효하지 않습니다. abnormalitySetId : {0}"),
							FText::FromString(setId));
						DEF_ERR_POPUP_MESSAGE(failedMessage);
					}
					info.param.maxTickDamage = FCString::Atoi(*effectSet->param6);
					info.param.maxTickDamagePC = FCString::Atoi(*effectSet->param7);

					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::CC:
				{
					auto FindEnumValue = [](IN const FString enumName, IN const FString enumValueName) {

						FString realValName = enumName + "::" + enumValueName;
						if (const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *enumName, true))
						{
							if (enumPtr->IsValidEnumName(*realValName))
							{
								return enumPtr->GetValueByName(*realValName, EGetByNameFlags::None);
							}
						}
						return (int64)-1;
					};


					FGsSchemaAbnormalityCrowdControlSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);

					if (effectSet->abnormalityEffectType == AbnormalityEffectType::CC_MOVE_LOCATION)
					{
						info.param.skillNotifyId = FCString::Atoi(*effectSet->param2);
					}
					else
					{
						info.param.knockBackDirection = AbnormalityKnockBackDirectionTypeStringToEnum(effectSet->param1);
						if (false == IsValidAbnormalityKnockBackDirectionType(info.param.knockBackDirection))
						{
							FString setId = FString::FromInt(effectSet->id);
							FText failedMessage = FText::Format(
								NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet",
									"AbnormalityKnockBackDirectionType이 유효하지 않습니다. abnormalitySetId : {0}"),
								FText::FromString(setId));
							DEF_ERR_POPUP_MESSAGE(failedMessage);
						}
					}

					auto animType = FindEnumValue(TEXT("EGsAbnormalityAnimProcessType"), effectSet->param3);
					if (animType >= 0)
					{
						info.param.animationTypeInfo = static_cast<EGsAbnormalityAnimProcessType>(animType);
					}
					else
					{
						info.param.animationTypeInfo = EGsAbnormalityAnimProcessType::NONE;
					}

					//info.param.animationTypeInfo = EGsAbnormalityAnimProcessType(FCString::Atoi(*effectSet->param3));
					// 일단 여기에 추가해두고 나중에 문의해보자..
					//constCastIndexSet->animationTypeInfo = info.param.animationTypeInfo;
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::STAT:
				{
					FGsSchemaAbnormalityStatSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					// Param 정보들 설정
					// 이제 STAT타입이 List로 들어갈일이 없다.
					// 무조건 1개만 존재
					FGsSchemaAbnormalityStat newStat;
					newStat.methodType = AbnormalityEffectStatMethodTypeStringToEnum(effectSet->param1);
					if (false == IsValidAbnormalityEffectStatMethodType(newStat.methodType))
					{
						FString setId = FString::FromInt(effectSet->id);
						FText failedMessage = FText::Format(
							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet",
								"AbnormalityEffectStatMethodType이 유효하지 않습니다. abnormalitySetId : {0}"),
							FText::FromString(setId));
						DEF_ERR_POPUP_MESSAGE(failedMessage);
					}
					newStat.proportionTargetType = SkillTargetTypeStringToEnum(effectSet->param2);
					if (false == IsValidSkillTargetType(newStat.proportionTargetType))
					{
						FString setId = FString::FromInt(effectSet->id);
						FText failedMessage = FText::Format(
							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet",
								"SkillTargetType이 유효하지 않습니다. abnormalitySetId : {0}"),
							FText::FromString(setId));
						DEF_ERR_POPUP_MESSAGE(failedMessage);
					}
					newStat.baseStat = StatTypeStringToEnum(effectSet->param3);
					if (false == IsValidStatType(newStat.baseStat))
					{
						FString setId = FString::FromInt(effectSet->id);
						FText failedMessage = FText::Format(
							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet",
								"StatType이 유효하지 않습니다. abnormalitySetId : {0}"),
							FText::FromString(setId));
						DEF_ERR_POPUP_MESSAGE(failedMessage);
					}
					newStat.conditionValue = FCString::Atoi(*effectSet->param4);
					newStat.affectedStat = StatTypeStringToEnum(effectSet->param5);
					if (false == IsValidStatType(newStat.affectedStat))
					{
						FString setId = FString::FromInt(effectSet->id);
						FText failedMessage = FText::Format(
							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet",
								"StatType이 유효하지 않습니다. abnormalitySetId : {0}"),
							FText::FromString(setId));
						DEF_ERR_POPUP_MESSAGE(failedMessage);
					}
					newStat.affectedValue = FCString::Atoi(*effectSet->param6);
					info.param.Add(newStat);

					currTable->AddRow(info.RowName, info);
				}
					break;
				case EGsAbnormalityEffectConvertParamType::UTIL_ON_REMOVE_EXE_SKILL_NOTIFY:
				{
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					const UGsTableSkillNotifySet* notifyDataTable = Cast<UGsTableSkillNotifySet>(FGsSchemaSkillNotifySet::GetStaticTable());
					const FGsSchemaSkillNotifySet* notifySet = nullptr;
					notifyDataTable->FindRowById(FCString::Atoi(*effectSet->param1), notifySet);
					if (notifySet)
					{
						info.param.SkillNoityId.DataTable = DTGsSkillNotifySet;
						info.param.SkillNoityId.RowName = notifySet->RowName;
					}
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::UTIL_IMMORTAL:
				{
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					// Param 정보들 설정
					info.param.minHp = FCString::Atoi(*effectSet->param1);
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::UTIL_BERSERKER_RAGE:
				{
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					info.param.minHp = FCString::Atoi(*effectSet->param1); //"remainHpRate": 3000,
					info.param.damageRate = FCString::Atoi(*effectSet->param2);
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::UTIL_POINTED_ARROW:
				{
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					const UGsTableAbnormalitySet* abnormalityDataTable = Cast<UGsTableAbnormalitySet>(FGsSchemaAbnormalitySet::GetStaticTable());
					const FGsSchemaAbnormalitySet* abnormalitySet = nullptr;
					abnormalityDataTable->FindRowById(FCString::Atoi(*effectSet->param1), abnormalitySet);
					if (abnormalitySet)
					{
						info.param.abnormalityId.DataTable = DTGsAbnormalitySet;
						info.param.abnormalityId.RowName = abnormalitySet->RowName;
					}
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::UTIL_FINAL_HOWL:
				{					
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					info.param.minHp = FCString::Atoi(*effectSet->param1);
					info.param.damageRate = FCString::Atoi(*effectSet->param2);
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::UTIL_BONUS_DAMAGE:
				{
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					info.param.damageRate = FCString::Atoi(*effectSet->param1);
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::UTIL_COUNTER:
				{
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					info.param.hitType = AbnormalityEffectBuffCountHitTypeStringToEnum(effectSet->param1);
					if (false == IsValidAbnormalityEffectBuffCountHitType(info.param.hitType))
					{
						FString setId = FString::FromInt(effectSet->id);
						FText failedMessage = FText::Format(
							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet",
								"AbnormalityEffectBuffCountHitType이 유효하지 않습니다. abnormalitySetId : {0}"),
							FText::FromString(setId));
						DEF_ERR_POPUP_MESSAGE(failedMessage);
					}
					info.param.skillGrade = AbnormalityEffectBuffCountSkillGradeStringToEnum(effectSet->param2);
					if (false == IsValidAbnormalityEffectBuffCountSkillGrade(info.param.skillGrade))
					{
						FString setId = FString::FromInt(effectSet->id);
						FText failedMessage = FText::Format(
							NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet",
								"AbnormalityEffectBuffCountSkillGrade이 유효하지 않습니다. abnormalitySetId : {0}"),
							FText::FromString(setId));
						DEF_ERR_POPUP_MESSAGE(failedMessage);
					}
					info.param.rate = FCString::Atoi(*effectSet->param3);
					info.param.cooldown = FCString::Atof(*effectSet->param4);
					info.param.damage = FCString::Atoi(*effectSet->param5);
					info.param.damage_reduction_rate = FCString::Atoi(*effectSet->param6);
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::UTIL_HP_ABSORPTION:
				{
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					info.param.hpAbsorptionRate = FCString::Atoi(*effectSet->param1);
					info.param.hpLimitAbsorptionRate = FCString::Atoi(*effectSet->param2);
					info.param.hpAbsorption = FCString::Atoi(*effectSet->param3);
					info.param.hpAbsorptionAffectedByBonusRate = FCString::ToBool(*effectSet->param4);
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::UTIL_DAMAGE_SHARE:
				{
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					info.param.damageShareRate = FCString::Atoi(*effectSet->param1);
					info.param.damageShareApplicationRate = FCString::Atoi(*effectSet->param2);
					info.param.damageShareLimitHpRate = FCString::Atoi(*effectSet->param3);
					currTable->AddRow(info.RowName, info);
				}
				break;
				case EGsAbnormalityEffectConvertParamType::UTIL:
				{
					FGsSchemaAbnormalityEffectUtilSet info;
					DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
					currTable->AddRow(info.RowName, info);
				}
				break;
			default:
				FGsSchemaAbnormalityEffectUtilSet info;
				DEF_CONVERT_TABLE_INITIALIZE_SET(info, keyName);
				currTable->AddRow(info.RowName, info);
				break;
			}
			
			FGsSchemaAbnormalityIndexLevelInfo newLevelInfo;
			newLevelInfo.effectId = effectSet->id;
			newLevelInfo.level = effectSet->level;			
			if (nullptr != currTable)
			{
				newLevelInfo.effectRow.DataTable = currTable;
				newLevelInfo.effectRow.RowName = keyName;
			}
			constCastIndexSet->levelList.Add(newLevelInfo);
		}
		else
		{
		FText failedMessage = FText::Format(
			NSLOCTEXT("not vaild", "FDataTableConverterAbnormalityEffectSet", "AbnormalityEffectSet에서 다음 테이블을 찾을수 없음 {0}"), 
			FText::FromName(rowName));
		DEF_ERR_POPUP_MESSAGE(failedMessage);
		}
	}

	// 테이블 저장
	DTGsAbnormalityEffectIndexSet->SavePackage();
	for (TPair<FString, UGsDataTable*>& el : uniqueEffectTableList)
	{
		el.Value->SavePackage();
	}
	//------------------------------------------------------------------------------------------------------------------

	return true;
}

