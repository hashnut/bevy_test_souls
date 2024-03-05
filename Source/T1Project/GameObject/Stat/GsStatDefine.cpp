// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStatDefine.h"


StatType UGsStatUtil::ConvertPlusStatToStat(EPlusStatType inType)
{
	switch (inType)
	{
	case EPlusStatType::STR:
		return StatType::STR;
		break;
	case EPlusStatType::DEX:
		return StatType::DEX;
		break;
	case EPlusStatType::INT:
		return StatType::INT;
		break;
	case EPlusStatType::CON:
		return StatType::CON;
		break;
	case EPlusStatType::AGI:
		return StatType::AGI;
		break;
	case EPlusStatType::WIS:
		return StatType::WIS;
		break;
	}

	return StatType::NONE;
}

EPlusStatType UGsStatUtil::ConvertStatToPlusStat(StatType inType)
{
	switch (inType)
	{
	case StatType::STR:
		return EPlusStatType::STR;
		break;
	case StatType::DEX:
		return EPlusStatType::DEX;
		break;
	case StatType::INT:
		return EPlusStatType::INT;
		break;
	case StatType::CON:
		return EPlusStatType::CON;
		break;
	case StatType::AGI:
		return EPlusStatType::AGI;
		break;
	case StatType::WIS:
		return EPlusStatType::WIS;
		break;
	}

	return EPlusStatType::MAX;
}