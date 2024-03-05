// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTableUtil.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeData.h"

template <>
int32 UGsTableUtil::ConvertRow(const FGsSchemaBase* Schema)
{
	return nullptr == Schema ? -1 : FCString::Atoi(*Schema->GetRowName().ToString());
}

template <>
uint32 UGsTableUtil::ConvertRow(const FGsSchemaBase* Schema)
{
	return nullptr == Schema ? 0 : (uint32)FCString::Atoi64(*Schema->GetRowName().ToString());
}

template <>
int64 UGsTableUtil::ConvertRow(const FGsSchemaBase* Schema)
{
	return nullptr == Schema ? -1 : FCString::Atoi64(*Schema->GetRowName().ToString());
}