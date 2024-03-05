// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FTableRowBase;
class UDataTable;
/**
 * 임포트시 필요한 후처리에 대한 코드작성
 */
class FPostExcelImportScripter
{
public:
	FPostExcelImportScripter() = default;
	~FPostExcelImportScripter() = default;

public:
	void Bind();
	void UnBind();

private:
	void StartExcelImportStatConvert(UDataTable* inTable);
private:
	//// bak1210
	//// 로직용 데이터(List로 변환)처리한다.
	//bool FillStatConvertDataList(UDataTable* inTable, const FName& inTableRowName, FTableRowBase* inTableRow);
};
