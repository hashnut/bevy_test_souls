#include "GsUIRegionSceneImage.h"

#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UILib/Container/GsWidgetPool.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"

#include "Message/GsMessageContents.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

#include "ActorEx/GsCharacterLocalPlayer.h"

#include "ActorComponentEx/GsQueryFloorHeightComponent.h"

#include "ControllerEx/GsPlayerController.h"

#include "DataSchema/Map/WorldMap/GsSchemaRegionMapData.h"

#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGameClientVersion.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsMove.h"
#include "UTIL/GsUIUtil.h"

#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/Image.h"

#include "Engine/Classes/Engine/Texture2D.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "HAL/PlatformApplicationMisc.h"

#include "T1Project.h"

// 씬 이미지 로드
void UGsUIRegionSceneImage::LoadSceneImage()
{
	int currentMapId = GSWorldMap()->GetShowRegionMapId();
	const FGsSchemaRegionMapData* data =
		UGsTableUtil::FindRowById<UGsTableRegionMapData, FGsSchemaRegionMapData>(currentMapId);

	if (nullptr == data)
	{
		return;
	}

	FString strPath = data->regionImagePath.GetAssetPathString();

	if (strPath.IsEmpty() || data->realMapSize == FVector2D::ZeroVector ||
		data->regionMapSize == FVector2D::ZeroVector ||
		data->realMapSize.X == 0.0f)
	{
		return;
	}
	// 기존께 있다면 해제
	if (_loadedSceneImage != nullptr)
	{
		ReleaseImage(_loadedSceneImage);
		_loadedSceneImage = nullptr;
	}

	_loadedSceneImage = MakeSceneImageWidget(data->regionImagePath);

	if (_sceneImageRoot != nullptr &&
		_loadedSceneImage != nullptr)
	{
		UCanvasPanelSlot* loadedSceneImageSlot =
			_sceneImageRoot->AddChildToCanvas(_loadedSceneImage);

		if (nullptr != loadedSceneImageSlot)
		{
			// 센터
			loadedSceneImageSlot->SetAnchors(FAnchors(0.5f, 0.5f, 0.5f, 0.5f));
			// 오프셋, 크기
			loadedSceneImageSlot->SetOffsets(FMargin(
				data->UIShiftMapPos.X, data->UIShiftMapPos.Y,
				data->regionMapSize.X, data->regionMapSize.Y));
			// 정렬
			loadedSceneImageSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		}

		float uiSceneImageRate = data->regionMapSize.X / data->realMapSize.X;
		GSWorldMap()->SetSceneImageRate(uiSceneImageRate);
		GSWorldMap()->SetUIShiftMapPos(data->UIShiftMapPos);


		// check ui color data
		// 해당 함수는 window, mac 플랫폼만 구현 되어있어서
		// and, ios에서는 못구함
		// https://jira.com2us.com/jira/browse/C2URWQ-4949

		// 테스트 값이 켜졌는지
		bool isOnTestTextureMove = GSWorldMap()->GetTestPickMoveTexture();
		// branch 값 켜졌는지
		bool isOnBranchTextureMove = IsBranch(EGsGameClientBranchType::REGION_MAP_PICK_MOVE_TEXTURE);

		// 테스트 값이든 branch 값이든 켜졌으면 처리 시작
		bool isOnTextureMove = (isOnTestTextureMove || isOnBranchTextureMove) ? true : false;

		if(isOnTextureMove == true)
		{ 
			if (UTexture2D* findTextrue = UGsUIUtil::FindTextureByUserWidget(_loadedSceneImage))
			{
				uint32 xSize =0;
				uint32 ySize = 0;
				if (true == UGsUIUtil::FindTextureSize(findTextrue, xSize, ySize))
				{
					float realUIXSize = FMath::Max(1.0f, data->regionMapSize.X);
					float textureRate = xSize / realUIXSize;

					// 실제 텍스쳐 size/ 실제 ui 크기 비율 저장(나중에 ui 위치에서 텍스쳐 위치로 변경하기 위해서)
					GSWorldMap()->SetTextureRate(textureRate);
				}
			}
		}
	}
}

// 이미지 해제
void UGsUIRegionSceneImage::ReleaseImage(UUserWidget* In_image)
{
	if (In_image == nullptr)
	{
		return;
	}

	if (_sceneImagePoolManager.IsValid())
	{
		In_image->RemoveFromParent();
		_sceneImagePoolManager->ReleaseWidget(In_image->GetClass(), In_image);
	}
}

// 맵 이미지 위젯 만들기
UUserWidget* UGsUIRegionSceneImage::MakeSceneImageWidget(const FSoftObjectPath& In_path)
{
	UClass* widgetClass = nullptr;
	if (UGsPathManager* pathMgr = GPath())
	{
		widgetClass = pathMgr->GetClassByObjectPath(In_path);
	}

	if (nullptr == widgetClass)
	{
		return nullptr;
	}

	UUserWidget* widget = Claim(widgetClass);
	if (widget)
	{
		widget->AddToViewport();
	}

	return widget;
}
UUserWidget* UGsUIRegionSceneImage::Claim(UClass* InClass)
{
	if (false == _sceneImagePoolManager.IsValid())
	{
		_sceneImagePoolManager = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
		if (false == _sceneImagePoolManager.IsValid())
		{
			return nullptr;
		}
	}

	UGsWidgetPool* pool = _sceneImagePoolManager->FindPool(InClass);
	if (nullptr == pool)
	{
		pool = _sceneImagePoolManager->CreatePool(InClass);
		if (nullptr == pool)
		{
			return nullptr;
		}
	}

	return pool->Claim<UUserWidget>();
}

FReply UGsUIRegionSceneImage::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const FVector2D touchPos = InGestureEvent.GetScreenSpacePosition();

	ClickImage(touchPos);

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

void UGsUIRegionSceneImage::ClickImage(const FVector2D In_touchPos)
{
	if (_loadedSceneImage == nullptr)
	{
		return;
	}
	
	//GSLOG(Warning, TEXT("In_touchPos x: %f, y: %f"), In_touchPos.X, In_touchPos.Y);

	FGeometry loadedBPGeometry = _loadedSceneImage->GetCachedGeometry();
	FVector2D localBPPos = loadedBPGeometry.AbsoluteToLocal(In_touchPos);

	//GSLOG(Warning, TEXT("localBPPos x: %f, y: %f"), localBPPos.X, localBPPos.Y);

	FVector2D realWorldPos2D = GSWorldMap()->GetRealPosFromUILocalPos(localBPPos);
	


	// check is move possible 
	int currentRegionMapId = GSWorldMap()->GetShowRegionMapId();
	int currMapId = GLevel()->GetCurrentLevelId();

	bool isMovePossible = false;
	bool isUseWarp = false;

	FVector calcZDestPos = FVector(realWorldPos2D.X, realWorldPos2D.Y, 0.0f);
	// 현재맵 이동 불가인가(그림자 전장일때 사용)
	bool isSameMapMoveImpossible = false;

	// same map, check nav mesh data
	if (currentRegionMapId == currMapId)
	{

		calcZDestPos = Get3DPos(realWorldPos2D);

		UGsGameObjectBase* localBaseObject =
			GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);

		if (localBaseObject == nullptr)
		{
			return;
}

		UGsGameObjectLocalPlayer* localObject = Cast<UGsGameObjectLocalPlayer>(localBaseObject);

		if (localObject == nullptr)
		{
			return;
		}

		AGsCharacterLocalPlayer* localChar = localObject->GetLocalCharacter();
		if (localChar == nullptr)
		{
			return;
		}
		AGsPlayerController* controller = localChar->GetPlayerController();
		if (controller == nullptr)
		{
			return;
		}

		float acceptanceRadius = FGsMove::GetMovePosRange();
		// -값 들어가면 이동 실패 되므로 최소값으로 세팅
		acceptanceRadius = FMath::Max(acceptanceRadius, 0.1f);

		FVector localPos = localChar->GetActorLocation();
		isMovePossible = FGsMove::IsMovePossible(controller,
			localChar,
			calcZDestPos,
			localPos,
			acceptanceRadius);

		if (isMovePossible == false)
		{
			FGsSchemaSpotInfo spotInfo;
			int warpId = 0;
			if (true == GSClientSpawn()->TryGetCurrentMapWarpInfo(currMapId, localPos, calcZDestPos,
				warpId, spotInfo))
			{
				isUseWarp = true;
			}
		}

		// 현재 맵이 그림자 전장일때
		// 못가는 섬 지역이 있을수 있으므로
		// 여기까지 못가는거면 그냥못가는걸로 처리
		if (isMovePossible == false && isUseWarp == false)
		{
			MapType currentMapType = GLevel()->GetCurrentMapType();
			if (currentMapType == MapType::GUILD_WAR)
			{
				isSameMapMoveImpossible = true;
			}
		}
	}
	// nav not perfect(far zone)
	// use check color
	// 그림자 전장내에서 이동 불가가 아닐때 텍스쳐 체크 하기
	if(isMovePossible == false &&
		isUseWarp == false &&
		isSameMapMoveImpossible == false)
	{
		// check ui color data
		// 해당 함수는 window, mac 플랫폼만 구현 되어있어서
		// and, ios에서는 못구함
		// https://jira.com2us.com/jira/browse/C2URWQ-4949

		// 테스트 값이 켜졌는지
		bool isOnTestTextureMove = GSWorldMap()->GetTestPickMoveTexture();
		// branch 값 켜졌는지
		bool isOnBranchTextureMove = IsBranch(EGsGameClientBranchType::REGION_MAP_PICK_MOVE_TEXTURE);

		// 테스트 값이든 branch 값이든 켜졌으면 처리 시작
		bool isOnTextureMove = (isOnTestTextureMove || isOnBranchTextureMove)? true: false;

		FColor findColor = FColor::Transparent;
		FLinearColor nowColor = FLinearColor::Black;
		// 텍스쳐 알파로 처리할지
		if(isOnTextureMove == true)
		{ 			
			UImage* findImage = UGsUIUtil::FindImageByUserWidget(_loadedSceneImage);			
			if (findImage != nullptr)
			{

				FGeometry loadedImageBPGeometry = findImage->GetCachedGeometry();
				FVector2D localImageBPPos = loadedImageBPGeometry.AbsoluteToLocal(In_touchPos);

				if (UObject* resourceObj = findImage->Brush.GetResourceObject())
				{
					if (UTexture2D* textureImage = Cast<UTexture2D>(resourceObj))
					{
						// ui 위치에서 텍스쳐 위치로 변경
						FVector2D texturePos = GSWorldMap()->GetTexturePosFromRealUiPos(localImageBPPos);

						uint32 xPos = (uint32)texturePos.X;
						uint32 yPos = (uint32)texturePos.Y;
				
						// 해당 텍스쳐 색상 구하기
						// 텍스쳐는 색상이 아니라 알파가 있으면 지형이고 알파값 없으면 그외 지역이다
						findColor = UGsUIUtil::FindColorByTexture2D(textureImage, xPos, yPos);
					}				
				}
			}
		}
		// 스크린 좌표 픽셀값(window, mac) 으로 할지
		else
		{
			nowColor = FPlatformApplicationMisc::GetScreenPixelColor(In_touchPos);
		}

		if (true == GSWorldMap()->GetTestShowRegionMapTouchColor())
		{
			GSLOG(Warning, TEXT("nowColor R: %f, G: %f, B:%f"), nowColor.R, nowColor.G, nowColor.B);
			GSLOG(Warning, TEXT("nowAlpha A: %d"), findColor.A);
		}

		// 기존 터치 이동 방식(window, mac만 됨)
		if(isOnTextureMove == false)
		{ 
			if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
			{
				if (nowColor.R >= globalData->_regionMapPickZoneColorMin.R &&
					nowColor.R <= globalData->_regionMapPickZoneColorMax.R &&
					nowColor.G >= globalData->_regionMapPickZoneColorMin.G &&
					nowColor.G <= globalData->_regionMapPickZoneColorMax.G &&
					nowColor.B >= globalData->_regionMapPickZoneColorMin.B &&
					nowColor.B <= globalData->_regionMapPickZoneColorMax.B)
				{
					isMovePossible = true;
				}			
			}
		}
		// 텍스쳐 알파값으로 처리
		else
		{ 
			int checkAlpha = 250;
			if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
			{
				checkAlpha = globalData->_regionMapTouchMoveCheckAlpha;
			}

			// 텍스쳐 알파값이 있다면 이동가능
			// 알파값이 250 이상이면 으로 변경(찌꺼기값(ex:14 정도) 있을때도 있어서)
			// https://jira.com2us.com/jira/browse/C2URWQ-7732
			if (findColor.A > checkAlpha)
			{
				isMovePossible = true;
			}
		}

	}

	if (isMovePossible == true || isUseWarp == true)
	{
		if (nullptr != _callbackClickImage)
		{
			_callbackClickImage(calcZDestPos);
		}
	}
	else
	{
		if (nullptr != _callbackClickCantMoveZone)
		{
			_callbackClickCantMoveZone();
		}
	}
	

}

FVector UGsUIRegionSceneImage::Get3DPos(FVector2D In_2dPos)
{
	FVector realWorldPos3D = FVector(In_2dPos.X, In_2dPos.Y, 0.0f);

	UGsGameObjectBase* localBase = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (localBase == nullptr)
	{
		return realWorldPos3D;;
	}

	UGsGameObjectLocalPlayer* local = localBase->CastGameObject<UGsGameObjectLocalPlayer>();
	if (nullptr == local)
	{
		return realWorldPos3D;
	}

	AGsCharacterLocalPlayer* localChar = local->GetLocalCharacter();

	// use local zpos(check start pos)
	FVector localPos = localChar->GetActorLocation();

	realWorldPos3D.Z = localPos.Z;

	if (localChar == nullptr)
	{
		return realWorldPos3D;
	}
	FVector calcZDestPos = realWorldPos3D;
	if (false == localChar->GetQueryFloorHeightComponent()->TryGetFloor(realWorldPos3D, calcZDestPos))
	{
		// get local z pos
		//FVector localPos = localBase->GetLocation();

		//realWorldPos3D.Z = localPos.Z;

		//// no navi or level pos
		//// find near move posiible pos
		////FGsMove::GetNearMovePosiblePos(realWorldPos3D, calcZDestPos);
		//FGsMove::GetNearMovePossiblePos(realWorldPos3D, calcZDestPos);
	}

	return calcZDestPos;
}