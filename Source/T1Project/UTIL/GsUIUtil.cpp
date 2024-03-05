// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIUtil.h"

#include "Runtime/Engine/Classes/Camera/PlayerCameraManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

#include "UMG/Public/Components/ProgressBar.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ListView.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/NamedSlotInterface.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Blueprint/UserWidget.h"

#include "Engine/Classes/Engine/Texture2D.h"
#include "Engine/Classes/Engine/Texture.h"
#include "Engine/Public/TextureResource.h"

#include "SlateCore/Public/Styling/SlateBrush.h"

#include "CoreUObject/Public/Serialization/BulkData.h"

#include "Components/EditableTextBox.h"

#if WITH_EDITOR
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/LevelEditorViewport.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#endif

#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"

#include "UI/UIContent/Popup/Boss/GsBossInfoSpawnListViewEntry.h"

#include "UTIL/GsText.h"
#include "UTIL/GsCronUtil.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"

#include "GameObject/Data/GsGameObjectDataPlayer.h"

#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsInterServerManager.h"

#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedFuncs.h"
#include "DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Guild/GsGuildPlayerData.h"
#include "Guild/GsGuildData.h"


bool UGsUIUtil::TryGetRotationForFacePlayerCamera(UWorld* in_world, const FVector& in_location, FRotator& out_rot)
{
	APlayerController* player = in_world->GetFirstPlayerController();

	if (player)
	{
		APlayerCameraManager* camera = player->PlayerCameraManager;

		if (camera)
		{
			out_rot = UKismetMathLibrary::FindLookAtRotation(in_location, camera->GetCameraLocation());

			return true;
		}
	}

	return false;
}

bool UGsUIUtil::TryGetCameraLocation(UWorld* inWorld, FVector& outLocation)
{
	if (inWorld)
	{
		if (APlayerController* player = inWorld->GetFirstPlayerController())
		{
			if (APlayerCameraManager* camera = player->PlayerCameraManager)
			{
				outLocation = camera->GetCameraLocation();

				return true;
			}
		}
	}

	return false;
}


float UGsUIUtil::GetCurrentInteractionWidgetButtonSize(APlayerCameraManager* inCamera, const FVector& inLocation, float inWidgetSize, float inColliderSizeFactor,
	float In_aspectRatioMulVal, float In_aspectRatioWeight)
{
	float rate =
		GetCurrentInteractionWidgetButtonSizeRate(inCamera, inLocation, inColliderSizeFactor, In_aspectRatioMulVal, In_aspectRatioWeight);

	return rate * inWidgetSize;
}
// 원래크기에서 비율크기 변경된값
float UGsUIUtil::GetCurrentInteractionWidgetButtonSizeRate(APlayerCameraManager* inCamera, const FVector& inLocation, float inColliderSizeFactor,
	float In_aspectRatioMulVal, float In_aspectRatioWeight)
{
	if (nullptr == inCamera)
	{
		return 0;
	}

	const FVector& camLoc = inCamera->GetCameraLocation();
	// 내위치
	const FVector& myPos = inLocation;

	float dist = (myPos - camLoc).Size();

	// 각도 계산 처리 추가
	// 카메라 foward와 카메라에서 액터 방향 사잇각에 따라 크기가 달라서 보정
	// 카메라 회전값(foward  구하기 위한값)
	const FRotator camRot = inCamera->GetCameraRotation();
	// 카메라 forward 값
	FVector camForward = camRot.Vector();
	// 카메라에서 나 방향
	FVector camToMe = myPos - camLoc;

	// 노말라이즈
	// 벡터 크기는 제외하고 각도 값만 가져온다
	camToMe.Normalize();
	camForward.Normalize();
	// 닷 곱셈
	// cosine 값이므로 각도가 클수록 사이즈가 줄어듬
	float dotProduct = FVector::DotProduct(camToMe, camForward);

	// fov 값에 따라 크기가 달라진다...
	// fov 작아질수록 콜리더가 커지므로
	// fov 소수점 자리를 곱하게 한다...(그럴듯하게 가고 있음...)
	// 80, 70 -> 0.8, 0.7
	float fov = inCamera->GetFOVAngle();
	float fovRate = fov * 0.01f;

	// 거리, 사이즈, fov 관련 계산 값
	float calcVal =
		dist * 0.001f *
			inColliderSizeFactor *
			dotProduct *
			fovRate;

	float resultVal =
		// 원래 결과값 보간된값
		((1.0 - In_aspectRatioWeight)* calcVal) +
		// 가로 세로비 적용된 보간된값
		((In_aspectRatioWeight * calcVal)* In_aspectRatioMulVal);

	return resultVal;
}
#if WITH_EDITOR
bool UGsUIUtil::TryGetEditorCameraLocation(FVector& outLocation)
{
	if (FViewportClient* client = GEditor->GetActiveViewport()->GetClient())
	{
		if (FLevelEditorViewportClient* level = ((FLevelEditorViewportClient*)client))
		{
			outLocation = level->GetViewLocation();

			return true;
		}
	}

	return false;
}

FRotator UGsUIUtil::GetFaceRotationForEditorCamera(const FVector& in_actor)
{
	FLevelEditorViewportClient* Client = (FLevelEditorViewportClient*)(GEditor->GetActiveViewport()->GetClient());

	if (Client)
	{
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(in_actor, Client->GetViewLocation());

		return rot;
	}

	return FRotator::ZeroRotator;
}


#endif


// 카오 등급 색상
FLinearColor UGsUIUtil::GetPVPAlignmentGradeColor(PVPAlignmentGrade inPvpAlignmentGrade)
{
	FLinearColor color;
	switch (inPvpAlignmentGrade)
	{
	case PVPAlignmentGrade::NETURAL:
		color = FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY);
		break;
	case PVPAlignmentGrade::THREAT:
		color = FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_THREAT);
		break;
	case PVPAlignmentGrade::CHAOTIC:
		color = FGsUIColorHelper::GetColor(EGsUIColorType::PVP_ALIGNMENT_CHAOS);
		break;
	}

	return color;
}

FLinearColor UGsUIUtil::GetPVPOrGuildColor(PVPAlignmentGrade In_pvpAlignmentGrade, FGsGameObjectDataPlayer* In_playerData)
{
	EGsUIColorType colorType = EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY;
	switch (In_pvpAlignmentGrade)
	{
	case PVPAlignmentGrade::NETURAL:
	{
		bool isInGuild = false;
		if (In_playerData != nullptr)
		{			
			const FGsGuildPlayerData* guildData = In_playerData->GetGuildData();
			if (guildData != nullptr &&
				INVALID_GUILD_DB_ID != guildData->_guildDBId)
			{
				isInGuild = true;
			}
		}

		if (false == isInGuild)
		{
			colorType = EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY;
		}
		else
		{
			if (true == In_playerData->IsAllyGuild())
			{
				colorType = EGsUIColorType::GUILD_DEFAULT_COLOR;
			}
			else if (true == In_playerData->IsEnemyGuild())
			{
				colorType = EGsUIColorType::GUILD_ENEMY_COLOR;
			}
			else
			{
				colorType = EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY;
			}
		}
	}
		break;
	case PVPAlignmentGrade::THREAT:
		colorType = EGsUIColorType::PVP_ALIGNMENT_THREAT;
		break;
	case PVPAlignmentGrade::CHAOTIC:
		colorType = EGsUIColorType::PVP_ALIGNMENT_CHAOS;
		break;
	}
	FLinearColor color = FGsUIColorHelper::GetColor(colorType);
	return color;
}

// 쉴드 프로그래스 바 계산 처리
void UGsUIUtil::SetHPShieldProgressBar(UProgressBar* In_progressBarHp,
	UProgressBar* In_progressBarShieldFront,
	UProgressBar* In_progressBarShieldBack,
	int In_currHp, int In_maxHp, int In_currShield)
{
	if (In_progressBarHp == nullptr ||
		In_progressBarShieldFront == nullptr ||
		In_progressBarShieldBack == nullptr)
	{
		return;
	}

	float hpRatio = FMath::Clamp(static_cast<float>(In_currHp) / static_cast<float>(In_maxHp), 0.0f, 1.0f);
	In_progressBarHp->SetPercent(hpRatio);

	// 실드가 없을 경우 
	if (0 >= In_currShield)
	{
		// 실드 모두 가림
		In_progressBarShieldFront->SetVisibility(ESlateVisibility::Collapsed);
		In_progressBarShieldBack->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	// maxHp를 초과할 경우
	if (In_currShield + In_currHp > In_maxHp)
	{
		In_progressBarShieldFront->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		In_progressBarShieldBack->SetVisibility(ESlateVisibility::Collapsed);

		In_progressBarShieldFront->SetPercent(FMath::Clamp(
			static_cast<float>(In_currShield) / static_cast<float>(In_maxHp), 0.0f, 1.0f));
	}
	else
	{
		In_progressBarShieldFront->SetVisibility(ESlateVisibility::Collapsed);
		In_progressBarShieldBack->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		// HP바 뒤로 나와보이도록 더해서 계산
		In_progressBarShieldBack->SetPercent(FMath::Clamp(
			static_cast<float>(In_currHp + In_currShield) / static_cast<float>(In_maxHp), 0.0f, 1.0f));
	}
}

void UGsUIUtil::SetTextBlock(UTextBlock* inTextBlock, FTextKey inNamespace, FTextKey inKey)
{
	FText findText;
	if (FText::FindText(inNamespace, inKey, findText))
	{
		inTextBlock->SetText(findText);
	}
}

void UGsUIUtil::SetTextBlockOneParam(UTextBlock* inTextBlock, FTextKey inNamespace, FTextKey inKey, const FText& InParamText)
{
	FText findText;
	if (FText::FindText(inNamespace, inKey, findText))
	{
		FText formatText = FText::Format(findText, InParamText);
		inTextBlock->SetText(formatText);
	}
}

// 에디터블 텍스트 박스 이름 체크 함수
// 특정 글자수 넘으면 자른다
void UGsUIUtil::CheckEditableTextNameLength(UEditableTextBox* In_editableTextBox, int In_maxTextLen, bool In_IsNameType)
{
	if (nullptr == In_editableTextBox)
	{
		return;
	}

	FString strText = In_editableTextBox->GetText().ToString();
	int convertLen = (In_IsNameType) ? 
		FGsTextUtil::GetStringLengthNameType(strText) : FGsTextUtil::GetStringLengthRegex(strText);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (In_maxTextLen < convertLen)
	{
		strText.RemoveAt(strText.Len() - 1);
		In_editableTextBox->SetText(FText::FromString(strText));
	}
}

// get dungeon map name
FString UGsUIUtil::GetDungeonMapName(const FGsSchemaMapData* In_mapData, EGsDungeonGroupType In_dungeonType, int In_dungeonLevel)
{
	FString makeString = "";
	if (In_mapData == nullptr)
	{
		return makeString;
	}

	makeString = In_mapData->MapStringId.ToString();

	return makeString;
}

void UGsUIUtil::SetFieldBossCompetitionSpawnInfo(const FGsSchemaFieldBossData* In_bossTableData, UTextBlock* In_textBlock)
{
	if (In_bossTableData == nullptr ||
		In_textBlock == nullptr)
	{
		return;
	}
	// 고정 시간일때는 표시 다르게 처리
	// https://jira.com2us.com/jira/browse/CHR-23655

	// makeCron이 비어있으면 기존처럼 등장 간격
	if(In_bossTableData->makeCron.IsEmpty() == true)
	{ 
		FTimespan spawnIntervalMinut = FTimespan::FromMinutes(In_bossTableData->spawnIntervalM);
		FString spawnIntervalString;

		FGsTimeStringHelper::GetTimeString(spawnIntervalMinut, spawnIntervalString);

		FText ui_text_nPercentInBrackets;
		FText::FindText(TEXT("FieldBossText"), TEXT("UI_Text_NPercentInBrackets"), ui_text_nPercentInBrackets);

		spawnIntervalString += FText::Format(ui_text_nPercentInBrackets, 
			(float)In_bossTableData->spawnProbability / 100.0f).ToString();

		In_textBlock->SetText(FText::FromString(spawnIntervalString));
	}
	// 값이 있으면 크론형식으로 출력
	else
	{
		FString makeText;
		FGsCronUtil::GetCronString(In_bossTableData->makeCron, makeText, false);

		In_textBlock->SetText(FText::FromString(makeText));
	}
}

// list size calc
float UGsUIUtil::GetCaledListSize(UVerticalBox* In_verticalBox, int In_offset)
{
	if (In_verticalBox == nullptr)
	{
		return 0.0f;
	}
	float totalSize = 0.0f;
	TArray<UWidget*> childList = In_verticalBox->GetAllChildren();
	for (int32 i = 0; i < In_offset; ++i)
	{
		UWidget* entry = childList[i];
		if (nullptr == entry)
		{
			continue;
		}

		totalSize += entry->GetDesiredSize().Y;
	}

	return totalSize;
}

bool UGsUIUtil::GetIsReadyVercitalBoxChild(UVerticalBox* In_verticalBox)
{
	if (In_verticalBox == nullptr)
	{
		return true;
	}
	TArray<UWidget*> childList = In_verticalBox->GetAllChildren();
	if (childList.Num() == 0 )
	{
		return true;
	}
	UWidget* entry = childList[0];
	if (entry == nullptr)
	{
		return true;
	}

	float size = entry->GetDesiredSize().Y;

	return (size == 0.0f) ? false : true;
}


FLinearColor UGsUIUtil::GetCampGuildColor(uint64 In_campGuildDbId)
{
	EGsUIColorType colorType = EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY;
	
	FGsGuildPlayerData::EGsGuildIffType guildIff = FGsGuildPlayerData::GetGuildIff(In_campGuildDbId);
	switch (guildIff)
	{
	case FGsGuildPlayerData::EGsGuildIffType::NONE:
	{
		colorType = EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY;
	}
	break;
	case FGsGuildPlayerData::EGsGuildIffType::ALLY:
	{
		colorType = EGsUIColorType::GUILD_ALLY_COLOR;
	}
	break;
	case FGsGuildPlayerData::EGsGuildIffType::ENEMY:
	{
		colorType = EGsUIColorType::GUILD_ENEMY_COLOR;
	}
	break;
	}

	FLinearColor color = FGsUIColorHelper::GetColor(colorType);
	return color;
}


FLinearColor UGsUIUtil::GetPlayerGuildColor(FGsGameObjectDataPlayer* In_playerData)
{
	EGsUIColorType colorType = EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY;

	if (true == In_playerData->IsAllyGuild())
	{
		colorType = EGsUIColorType::GUILD_ALLY_COLOR;
	}
	else if (true == In_playerData->IsEnemyGuild())
	{
		colorType = EGsUIColorType::GUILD_ENEMY_COLOR;
	}
	else
	{
		colorType = EGsUIColorType::PVP_ALIGNMENT_NEUTRALITY;
	}
			
	FLinearColor color = FGsUIColorHelper::GetColor(colorType);
	return color;
}

bool UGsUIUtil::IsInvasionEnemy(FGsGameObjectDataPlayer* In_targetData)
{
	if (In_targetData == nullptr)
	{
		return false;
	}

	FGsGameDataManager* gameDataMgr = GGameData();
	if (nullptr == gameDataMgr)
	{
		return false;
	}

	// 침공 적을 확인하는 것이므로 인터서버는 배제한다
	if (gameDataMgr->IsInInterWorld())
	{
		return false;
	}

	WorldId myHomeWorldId = gameDataMgr->GetUserData()->mHomeWorldId;
	WorldId nowMapWorldId = gameDataMgr->GetLoadWorldId();

	bool isDiffrentWorldUser =  
		SharedFunc::HasOtherUserNamePrefix(gameDataMgr->GetLoadWorldType(), nowMapWorldId, myHomeWorldId, In_targetData->GetHomeWorldId());

	return isDiffrentWorldUser;	
}
// https://jira.com2us.com/jira/browse/CHR-17595
// NPC가 Mapdata.maptype = Instance Single, Single Filed 내부에 있는 경우
// NPCdata.grade = NAMED, BOSS, RAID인 경우 HUD 상 HP를 표기
// NPCdata.grade = NORMAL, ELITE인 경우 HUD 상 HP 미표기
bool UGsUIUtil::IsShowMonsterHP(CreatureRankType In_rank, NpcFunctionType InNpcFunctionType)
{
// 	MapType currentMapType = GLevel()->GetMapType();
// 	if (currentMapType != MapType::INSTANCE_SINGLE &&
// 		currentMapType != MapType::SINGLE_FIELD)
// 	{
// 		return false;
// 	}

	if (In_rank == CreatureRankType::NORMAL ||
		In_rank == CreatureRankType::ELITE)
	{
		return false;
	}

	if (NpcFunctionType::BATTLE_ARENA_BOSS == InNpcFunctionType
		|| NpcFunctionType::BATTLE_ARENA_GIMMICKMONSTER == InNpcFunctionType
		|| NpcFunctionType::BATTLE_ARENA_TOMBSTONE == InNpcFunctionType)
	{
		// 2023/12/18 PKT - Battle Arena에서 사용되는 NPC에서 HP 표기 안되어야 하는 애들
		return false;
	}

	return true;
}

// 자식 위젯들 찾기
void UGsUIUtil::ForWidgetAndChildren(
	UWidget* In_widget, TFunctionRef<void(UWidget*)> In_predicate,
	bool In_recursive)
{
	// Search for any named slot with content that we need to dive into.
	if (INamedSlotInterface* NamedSlotHost = Cast<INamedSlotInterface>(In_widget))
	{
		TArray<FName> SlotNames;
		NamedSlotHost->GetSlotNames(SlotNames);

		for (FName SlotName : SlotNames)
		{
			if (UWidget* SlotContent = NamedSlotHost->GetContentForSlot(SlotName))
			{
				In_predicate(SlotContent);

				if (In_recursive) {
					ForWidgetAndChildren(SlotContent, In_predicate);
				}
			}
		}
	}

	// Search standard children.
	if (UPanelWidget* PanelParent = Cast<UPanelWidget>(In_widget))
	{
		for (int32 ChildIndex = 0; ChildIndex < PanelParent->GetChildrenCount(); ChildIndex++)
		{
			if (UWidget* ChildWidget = PanelParent->GetChildAt(ChildIndex))
			{
				In_predicate(ChildWidget);

				if (In_recursive) {
					ForWidgetAndChildren(ChildWidget, In_predicate);
				}
			}
		}
	}
}

// 텍스쳐2d 에서 fixel color 구하기
// https://forums.unrealengine.com/t/accessing-pixel-values-of-texture2d/279798/2
FColor UGsUIUtil::FindColorByTexture2D(UTexture2D* In_texture2D, uint32 In_xPos, uint32 In_yPos)
{
	FColor findColor = FColor::Transparent;
	if (In_texture2D == nullptr)
	{
		return findColor;
	}
	FTexturePlatformData* platformData = In_texture2D->PlatformData;
	if (platformData == nullptr)
	{
		return findColor;
	}
	/*First you need to understand that a texture is normally, 
	a sum of multiple images called MipMaps.MipMaps are down - 
	scaled versions of your images, always in steps of power of 2, 
	so the original image, is, say, 512x512 - this would be the MipMap “0”, 
	then the engine generates the MipMap “1” which is 256x256 and then MipMap “2” which is 128x128, 
	this continues on down to 16x16 I think.
	The farther away the texture is rendered, 
	the smaller version of the image is used.
	This means that you need to access the mipmap 0 of your texture. */

	if (platformData->Mips.Num() == 0)
	{
		return findColor; 
	}

	/*First we need to access the mipmap 0 of the texture, 
	it is important to only access the data we need through refrences or , 
	like I did here, pointers, either will do.
	We do this to directly access the original data in the memory instead of copying 
	that data into a local variable.This saves memoryand speed : */

	FTexture2DMipMap* myMipMap = &platformData->Mips[0];
	if (myMipMap == nullptr)
	{
		return findColor;
	}
	/*With this, we can access the BulkData which contains the raw image data : */
	FByteBulkData* rawImageData = &myMipMap->BulkData;
	if (rawImageData == nullptr)
	{
		return findColor;
	}
	/*Considering this texture data is used in the rendering thread, 
	we need to lock it to make sure nothing else can access it while we are using it, 
	if we didn’t, we could cause memory corruptions or crashes.
	So now we lock the image data which will also return a pointer to it - 
	which we can then go ahead and cast the unformated data into an array of FColor’s : */
	// 빌드에러 때문에 임시로 막기
	//https://forums.unrealengine.com/t/static-cast-error/469803
	// static_cast -> reinterpret_cast
	FColor* formatedImageData = reinterpret_cast<FColor*>(rawImageData->Lock(LOCK_READ_ONLY));
	if (formatedImageData == nullptr)
	{
		rawImageData->Unlock();
		return findColor;
	}

	/*The FColor array is ofc 1 dimensional.
	So to access certain parts of the image, 
	we need to use the widthand height of the texture to calculate 
	the position of the pixel we are wanting to lookup.
	Here’s a small statement that does just that : */

	uint32 TextureWidth = myMipMap->SizeX, TextureHeight = myMipMap->SizeY;
	if (In_xPos >= 0 && In_xPos < TextureWidth && In_yPos >= 0 && In_yPos < TextureHeight)
	{
		findColor = formatedImageData[In_yPos * TextureWidth + In_xPos];
	}

	/*And now that we are done with everything, we need to unlock the memory again:*/
	rawImageData->Unlock();
	
	return findColor;
}

// 위젯에서 Image 구하기(bp 안의 이미지)
UImage* UGsUIUtil::FindImageByUserWidget(UUserWidget* In_widget)
{
	UImage* findImage = nullptr;
	if (In_widget == nullptr)
	{
		return findImage;
	}
	// bp의 root widget 구하기
	UWidget* rootWidget = In_widget->GetRootWidget();
	// 모든 자식 위젯 검색
	TArray<UWidget*> childWidgets;
	ForWidgetAndChildren(rootWidget, [&childWidgets](UWidget* ChildWidget) {
		childWidgets.Add(ChildWidget);
		}, false);
	
	// 그중 image 구함
	for (UWidget* subWidget : childWidgets)
	{
		if (subWidget == nullptr)
		{
			continue;
		}

		if (true == subWidget->IsA<UImage>())
		{
			findImage = Cast<UImage>(subWidget);
			if (findImage != nullptr)
			{
				break;
			}			
		}
	}

	return findImage;
}

// 위젯에서 texture 구하기(bp 안의 이미지안의 texture)
UTexture2D* UGsUIUtil::FindTextureByUserWidget(UUserWidget* In_widget)
{
	UTexture2D* findTexture = nullptr;
	if (In_widget == nullptr)
	{
		return findTexture;
	}

	UImage* findImage = FindImageByUserWidget(In_widget);
	if (findImage == nullptr)
	{
		return findTexture;
	}

	UObject* resourceObj = findImage->Brush.GetResourceObject();

	if (resourceObj == nullptr)
	{
		return findTexture;
	}

	findTexture = Cast<UTexture2D>(resourceObj);
	return findTexture;
}

// 텍스쳐 크기 구하기
bool UGsUIUtil::FindTextureSize(UTexture2D* In_texture2D, uint32& Out_xSize, uint32& Out_ySize)
{
	if (In_texture2D == nullptr)
	{
		return false;
	}
	FTexturePlatformData* platformData = In_texture2D->PlatformData;
	if (platformData == nullptr)
	{
		return false;
	}
	/*First you need to understand that a texture is normally,
	a sum of multiple images called MipMaps.MipMaps are down -
	scaled versions of your images, always in steps of power of 2,
	so the original image, is, say, 512x512 - this would be the MipMap “0”,
	then the engine generates the MipMap “1” which is 256x256 and then MipMap “2” which is 128x128,
	this continues on down to 16x16 I think.
	The farther away the texture is rendered,
	the smaller version of the image is used.
	This means that you need to access the mipmap 0 of your texture. */

	if (platformData->Mips.Num() == 0)
	{
		return false;
	}

	/*First we need to access the mipmap 0 of the texture,
	it is important to only access the data we need through refrences or ,
	like I did here, pointers, either will do.
	We do this to directly access the original data in the memory instead of copying
	that data into a local variable.This saves memoryand speed : */

	FTexture2DMipMap* myMipMap = &platformData->Mips[0];
	if (myMipMap == nullptr)
	{
		return false;
	}

	Out_xSize = myMipMap->SizeX;
	Out_ySize = myMipMap->SizeY;
	return true;
}

// 천배율에서 100분율(%)얻기
float UGsUIUtil::GetPercentageByThousandMagnification(int In_thousandVal)
{
	float percentage = In_thousandVal * 0.001f;
	return percentage;
}