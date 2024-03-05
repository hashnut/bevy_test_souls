#pragma once

#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"

#include "GsUIRegionItem.generated.h"

class UHorizontalBox;
class UPanelWidget;
class UWidgetSwitcher;
class UImage;
class UTextBlock;

class UGsUIVFX;
class UGsButton;
class UGsUIContinentalDungeonMenu;
class FGsRegionInfo;


UENUM()
enum class EGsMapType : uint8
{
	Normal,
	Dungeon,
	InvadeOnAir,
	InvadePeace,
};

UCLASS(BlueprintType)
class UGsUIRegionItem : public UUserWidget
{	
	GENERATED_BODY()
protected:
	// 지역 맵 id
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	int _regionMapId;
	// 던전인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	bool _isDungeon = false;
	// 신기루 사원 2, 3, 4층 등 대륙맵에서 로컬플레이어가 있다고 표시하기 위한 정보
	// https://jira.com2us.com/jira/browse/C2URWQ-7502
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<int> _regionGroup;
	// 아이콘 붙이는곳
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UHorizontalBox* _iconRoot;
	// 맵 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textMapName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _invadeInfoPanel;
	// 침공가능 맵의 레벨
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textInvadeLevel;
	// 현재 침공 인원
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textInvadeCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textMapLevel;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIVFX* _effInvadeRegion;
	// 지역 버튼
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnRegion;
	// 맵 타입
	// 0: 일반
	// 1: 던전
	// 2: 침공전
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherMapType;

	// town image
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgInTown;
	// 침공전일때 마을 표시
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgInTown_1;
	// 침공 가능한 위치 마을 표시(침공전 시간은 아님)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgInTown_2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIContinentalDungeonMenu* _dungeonMenu;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _levelTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _dungeonLevelTextBlock;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textLevel;

private:
	FText _levelFormat;
	EGsMapType _mapType = EGsMapType::Normal;

	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	// 로직 함수
public:
	// 지역 버튼 클릭
	UFUNCTION()
	void OnClickRegion();
	void UpdateInvaderData();


	// get
public:
	// 지역 맵 id 구하기
	int GetRegionMapId()
	{
		return _regionMapId;
	}
	// 지역 맵 그룹 정보 구하기
	TArray<int> GetRegionGroup()
	{
		return _regionGroup;
	}
	// 아이콘 붙일때
	UHorizontalBox* GetIconRoot()
	{
		return _iconRoot;
	}
	void SetIsInTown(bool In_isInTown);
	FGsRegionInfo* FindRegionInfo(int In_regionId);

	//bool GetDungeonSpotInfo(int In_dungeonId, FGsSchemaSpotInfo& Out_spotInfo, int& Out_mapId);

	bool IsShowDungeonMenu();
	void ShowDungeonMenu(bool In_val);
	void InvalidLevelInfo(int32 inLevelId);
	void InvalidDungoenLevelInfo(int32 inLevelId);
	EGsMapType GetMapType();

protected:
	void UpdateMapType();

private:
	void InvalidNormalMap();
	void InvalidDungoen();
};