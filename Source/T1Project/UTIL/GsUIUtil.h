// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NoExportTypes.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/WorldBoss/GsSchemaWorldBossData.h"
#include "UMG/Public/Components/ListView.h"
#include "Public/UObject/NoExportTypes.h"
#include "GsUIUtil.generated.h"

/**
 *
 */
 class APlayerCameraManager;

 class UTextBlock;
 class UProgressBar;
 class UEditableTextBox;
 class UTextBlock;
 class UListView;
 class UVerticalBox;
 class UWidget;
 class UUserWidget;
 class UImage;
 class UTexture2D;

 class FGsGameObjectDataPlayer;

 struct FGsSchemaMapData;
 struct FGsSchemaFieldBossData;

 
UCLASS()
class T1PROJECT_API UGsUIUtil : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		static bool TryGetRotationForFacePlayerCamera(UWorld* in_world, const FVector& in_location, FRotator& out_rot);
	UFUNCTION(BlueprintCallable)
		static bool TryGetCameraLocation(UWorld* inWorld, FVector& outLocation);
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable)
		static bool TryGetEditorCameraLocation(FVector& location);
	UFUNCTION(BlueprintCallable)
		static FRotator GetFaceRotationForEditorCamera(const FVector& in_actor);
#endif
	static float GetCurrentInteractionWidgetButtonSize(APlayerCameraManager* inCamera, const FVector& inLocation, float inWidgetSize, float inColliderSizeFactor,
		float In_aspectRatioMulVal, float In_aspectRatioWeight);
	// 원래크기에서 비율크기 변경된값
	static float GetCurrentInteractionWidgetButtonSizeRate(APlayerCameraManager* inCamera, const FVector& inLocation, float inColliderSizeFactor,
		float In_aspectRatioMulVal, float In_aspectRatioWeight);

	// 카오 등급 색상
	static FLinearColor GetPVPAlignmentGradeColor(PVPAlignmentGrade inPvpAlignmentGrade);

	static FLinearColor GetPVPOrGuildColor(PVPAlignmentGrade In_pvpAlignmentGrade, FGsGameObjectDataPlayer* In_playerData);

	// 쉴드 프로그래스 바 계산 처리
	static void SetHPShieldProgressBar(UProgressBar* In_progressBarHp, 
		UProgressBar* In_progressBarShieldFront, 
		UProgressBar* In_progressBarShieldBack, 
		int In_currHp, int In_maxHp, int In_currShield);
	
	static void SetTextBlock(UTextBlock* inTextBlock, FTextKey inNamespace, FTextKey inKey);
	static void SetTextBlockOneParam(UTextBlock* inTextBlock, FTextKey inNamespace, FTextKey inKey, const FText& InParamText);

	// 에디터블 텍스트 박스 이름 체크 함수
	// 특정 글자수 넘으면 자른다
	static void CheckEditableTextNameLength(UEditableTextBox* In_editableTextBox, int In_maxTextLen, bool In_IsNameType);

	// get dungeon map name
	static FString GetDungeonMapName(const FGsSchemaMapData* In_mapData,  EGsDungeonGroupType In_dungeonType, int In_dungeonLevel = 0);

	static void SetFieldBossCompetitionSpawnInfo(const FGsSchemaFieldBossData* In_bossTableData, UTextBlock* In_textBlock);	

	// list size calc
	static float GetCaledListSize(UVerticalBox* In_verticalBox, int In_offset);

	static bool GetIsReadyVercitalBoxChild(UVerticalBox* In_verticalBox);

	static FLinearColor GetCampGuildColor(uint64 In_campGuildDbId);

	static FLinearColor GetPlayerGuildColor(FGsGameObjectDataPlayer* In_playerData);

	static bool IsInvasionEnemy(FGsGameObjectDataPlayer* In_targetData);

	// https://jira.com2us.com/jira/browse/CHR-17595
	static bool IsShowMonsterHP(CreatureRankType In_rank, NpcFunctionType InNpcFunctionType);

	// 자식 위젯들 찾기
	static void ForWidgetAndChildren(
		UWidget* In_widget, TFunctionRef<void(UWidget*)> In_predicate,
		bool In_recursive = true);
	// 텍스쳐2d 에서 fixel color 구하기
	// https://forums.unrealengine.com/t/accessing-pixel-values-of-texture2d/279798/2
	static FColor FindColorByTexture2D(UTexture2D* In_texture2D, uint32 In_xPos, uint32 In_yPos);

	// 위젯에서 Image 구하기(bp 안의 이미지)
	static UImage* FindImageByUserWidget(UUserWidget* In_widget);

	// 위젯에서 texture 구하기(bp 안의 이미지안의 texture)
	static UTexture2D* FindTextureByUserWidget(UUserWidget* In_widget);
	// 텍스쳐 크기 구하기
	static bool FindTextureSize(UTexture2D* In_texture2D, uint32& Out_xSize, uint32& Out_ySize );
	// 천배율에서 100분율(%)얻기
	static float GetPercentageByThousandMagnification(int In_thousandVal);
};
