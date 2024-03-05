// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GsSequenceEnum.generated.h"

UENUM(BlueprintType)
enum class EGsSequenceTextType : uint8
{
	SEQUENCE_TEXT_TYPE_NONE UMETA(DisplayName = "None"),			// None
	SEQUENCE_TEXT_TYPE_AREA UMETA(DisplayName = "Area"),			// 지역명연출
	SEQUENCE_TEXT_TYPE_BOSS UMETA(DisplayName = "Boss"),			// Boss명연출
};
