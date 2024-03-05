// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsPopupSystemEnum.generated.h"

UENUM()
enum class EPopupOpenType : uint8
{
	Serial,						// 직렬
	Parallel,					// 병렬
};

UENUM()
enum class EPopupButtonType : uint8
{
	Yes,
	YesNo,
};

UENUM()
enum class EPopupType : uint8
{
	None,
	BackToLobby,
	BackToTitle,
	BackToLauncher,
};