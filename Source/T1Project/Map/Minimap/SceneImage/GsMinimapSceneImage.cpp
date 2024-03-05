#include "GsMinimapSceneImage.h"

#include "Map/Minimap/MapIcon/GsMinimapIconLocal.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"

#include "Level/GsLevelDefine.h"

#include "UI/UILib/Container/GsWidgetPool.h"
#include "UI/UILib/Manager/GsWidgetPoolManager.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Slate/WidgetTransform.h"
#include "UMG/Public/Blueprint/UserWidget.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/MiniMap/GsSchemaMiniMapData.h"

#include "Kismet/KismetMathLibrary.h"

#define USE_SCENE_IMAGE

UGsMinimapSceneImage::~UGsMinimapSceneImage()
{
	
}
void UGsMinimapSceneImage::Initialize()
{
	_delegateSetSceneImagePos = GMessage()->GetUIMinimapSceneImage().AddUObject(
		MessageContentMinimap::SETPOS_IMAGE, this, &UGsMinimapSceneImage::OnSetImagePos);
}
void UGsMinimapSceneImage::Finalize()
{
	GMessage()->GetUIMinimapSceneImage().Remove(_delegateSetSceneImagePos);
}
// scene image root 세팅한다
// ui NativeOnInitialized 호출 시점 -> 이건 최초 한번만함
// 로비 갔다가 오면 scope가 game인 minimap 매니저 데이터는 초기화됨
// NativeConstruct 호출시점으로 변경
// 1. In_sceneImageRoot: 이미지 로드할 캔버스
// 2. In_sizePanel: 사이즈 캔버스
void UGsMinimapSceneImage::SetSceneImageRoot(UCanvasPanel* In_sceneImageRoot, UCanvasPanel* In_sizePanel)
{
	if (In_sceneImageRoot == nullptr ||
		In_sizePanel == nullptr)
	{
		return;
	}

	_sceneImageRoot = In_sceneImageRoot;
	
	// 씬 이미지를 쓴다면
#ifdef USE_SCENE_IMAGE
	LoadSceneImage();
#else
	// 무조건 레이더다
	_isRadarMode = true;
#endif

}

// obj 위치에서 ui 위치 구하기
FVector2D UGsMinimapSceneImage::GetUIPos(const FVector2D& In_objPos)
{	
	// 캔버스 이동이 되면
	// 캔버스 말고 상위 root 가 이동이 되면...
	// 지금은 캔버스(icon root) 이동 시켜버리면
	// 영역 밖이라고 그리지 않음.....

	// 플레이어가 중심이 되게 계산

	if (_iconLocal == nullptr)
	{
		return FVector2D::ZeroVector;
	}
	float uiRate = GetUIRate();
	FVector2D targetIconPos = In_objPos * uiRate;
	FVector2D localIconPos = _iconLocal->GetActorPos2D() * uiRate;

	// 로컬 아이콘 상대적인 위치다
	FVector2D uiPos(_widgetViewHalfDist.X - (localIconPos.X - targetIconPos.X),
		_widgetViewHalfDist.Y - (localIconPos.Y - targetIconPos.Y));
	return uiPos;
}

void UGsMinimapSceneImage::Tick(float In_delta)
{
	InitSize();
	// 레이다 모드면 할필요 없다
	if (_isRadarMode == true ||
		_iconLocal == nullptr ||		
		_loadedSceneImage == nullptr)
	{
		return;
	}

	float uiRate = GetUIRate();
	FVector2D localIconPos = _iconLocal->GetActorPos2D() * uiRate;

	// 로컬 아이콘 위치랑 같다
	FVector2D uiPos(_widgetViewHalfDist.X - localIconPos.X,
		_widgetViewHalfDist.Y - localIconPos.Y);	
	_loadedSceneImage->SetRenderTranslation(uiPos);
}


// 이미지 위치 변경
void UGsMinimapSceneImage::SetImagePos(const FVector2D& In_pos)
{
	if (_loadedSceneImage == nullptr)
	{
		return;
	}
	_loadedSceneImage->SetRenderTranslation(In_pos);
	
}
void UGsMinimapSceneImage::OnSetImagePos(const FVector2D& In_pos)
{
	SetImagePos(In_pos);
}

// 씬이미지 로드
void UGsMinimapSceneImage::LoadSceneImage()
{
	if (GLevel() == nullptr)
	{
		return;
	}
	
	const FGsSchemaMiniMapData* data = UGsTableUtil::FindRow<FGsSchemaMiniMapData>(GLevel()->GetCurrentLevelId());

	// 씬 이미지 경로가 없거나
	// 실제 맵사이즈가 없거나
	// ui 맵 사이즈가 없으면 
	// 레이더 모드다
	if (nullptr == data)
	{
		_isRadarMode = true;

		return;
	}
		
	FString strPath = data->MiniMapImagePath.GetAssetPathString();

	if (strPath.IsEmpty() || data->RealMapSize == FVector2D::ZeroVector ||
		data->MiniMapSize == FVector2D::ZeroVector ||
		data->RealMapSize.X == 0.0f)
	{
		_isRadarMode = true;
		return;
	}

	// 기존께 있다면 해제
	if (_loadedSceneImage != nullptr)
	{
		ReleaseImage(_loadedSceneImage);
		_loadedSceneImage = nullptr;
	}

	_loadedSceneImage = MakeSceneImageWidget(data->MiniMapImagePath);

	if (_sceneImageRoot != nullptr &&
		_loadedSceneImage != nullptr)
	{
		_loadedSceneImageSlot = _sceneImageRoot->AddChildToCanvas(_loadedSceneImage);
		// 실제 데이터로 비율 정함
		_uiSceneImageRate = data->MiniMapSize.X / data->RealMapSize.X;

		_isRadarMode = false;
		return;
	}

	_isRadarMode = true;
}

// 모드에 따라 참조 비율이 다름
float UGsMinimapSceneImage::GetUIRate()
{
	return (_isRadarMode == true) ?_uiRadarRate : _uiSceneImageRate;
}

// 맵 이미지 위젯 만들기
UUserWidget* UGsMinimapSceneImage::MakeSceneImageWidget(const FSoftObjectPath& In_path)
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
UUserWidget* UGsMinimapSceneImage::Claim(UClass* InClass)
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

// 이미지 해제
void UGsMinimapSceneImage::ReleaseImage(UUserWidget* In_image)
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

// 사이즈 초기화
void UGsMinimapSceneImage::InitSize()
{
	if (nullptr == _sceneImageRoot ||
		true == _isInitSize)
	{
		return;
	}

	const FGeometry panelGeometry = _sceneImageRoot->GetCachedGeometry();
	FVector2D getSize = panelGeometry.GetLocalSize();
	// 세팅 전
	if (true == getSize.IsZero())
	{
		return;
	}

	_isInitSize = true;

	_widgetViewHalfDist.X = getSize.X * 0.5f;
	_widgetViewHalfDist.Y = getSize.Y * 0.5f;
	
	// 분모에 0이 나오는거 막기
	float viewGameObjDistanceRadius =
		FMath::Max(GData()->GetGlobalData()->_viewGameObjectRadius, 1.0f);

	_uiRadarRate =
		_widgetViewHalfDist.Y / viewGameObjDistanceRadius;
}

// 보여지는 틀밖에 있는지 체크
bool UGsMinimapSceneImage::IsOutSideFromFrame(const FVector2D& In_objPos)
{
	if (_iconLocal == nullptr)
	{
		return true;
	}
	float uiRate = GetUIRate();
	FVector2D targetIconPos = In_objPos * uiRate;
	FVector2D localIconPos = _iconLocal->GetActorPos2D() * uiRate;

	// 로컬 아이콘과의 차이
	float diffX = FMath::Abs(targetIconPos.X - localIconPos.X);
	float diffY = FMath::Abs(targetIconPos.Y - localIconPos.Y);

	bool isOverPos = false;
	// 뷰 반사이즈 보다 크면 넘어간거임
	if (diffX > _widgetViewHalfDist.X)
	{
		isOverPos = true;
	}
	if(diffY > _widgetViewHalfDist.Y)
	{
		isOverPos = true;
	}
	return isOverPos;
}


// 안쪽으로 위치 계산
FVector2D UGsMinimapSceneImage::CalcInsideViewPos(const FVector2D& In_objPos)
{
	if (_iconLocal == nullptr)
	{
		return FVector2D::ZeroVector;
	}
	float uiRate = GetUIRate();
	FVector2D targetIconPos = In_objPos * uiRate;
	FVector2D localIconPos = _iconLocal->GetActorPos2D() * uiRate;

	// 로컬 아이콘과의 차이
	float diffX = FMath::Abs(targetIconPos.X - localIconPos.X);
	float diffY = FMath::Abs(targetIconPos.Y - localIconPos.Y);

	float shiftSize =
		GData()->GetGlobalData()->_partyIconShiftSize;

	// 로컬 아이콘 상대적인 위치다
	FVector2D outPos(_widgetViewHalfDist.X - (localIconPos.X - targetIconPos.X),
		_widgetViewHalfDist.Y - (localIconPos.Y - targetIconPos.Y));

	// 뷰 반사이즈 보다 크면 넘어간거임
	if (diffX > _widgetViewHalfDist.X)
	{
		// 타겟이 로컬보다 오른쪽이면 +, 아니면 -
		float sign = ((targetIconPos.X - localIconPos.X) > 0.f) ? 1.0f : -1.0f;

		// 안으로 들어오게 하는 값은 반대 부호 씀
		outPos.X = _widgetViewHalfDist.X + (sign * _widgetViewHalfDist.X) + (shiftSize * sign * -1.0f);
	}
	if (diffY > _widgetViewHalfDist.Y)
	{
		// 타겟이 로컬보다 위쪽이면 +, 아니면 -
		float sign = ((targetIconPos.Y - localIconPos.Y) > 0.f) ? 1.0f : -1.0f;

		// 안으로 들어오게 하는 값은 반대 부호 씀
		outPos.Y = _widgetViewHalfDist.Y + (sign * _widgetViewHalfDist.Y) + (shiftSize * sign * -1.0f);
	}

	return outPos;
}
// 로컬로부터 각도
float UGsMinimapSceneImage::GetTargetAngleFromLocal(const FVector2D& In_objPos)
{
	if (_iconLocal == nullptr)
	{
		return 0.0f;
	}
	float uiRate = GetUIRate();
	FVector2D targetIconPos = In_objPos * uiRate;
	FVector2D localIconPos = _iconLocal->GetActorPos2D() * uiRate;

	float angle =
		UKismetMathLibrary::FindLookAtRotation(FVector(localIconPos, 0.0f), FVector(targetIconPos, 0.0f)).Yaw
		+ 90.0f;

	return angle;
}