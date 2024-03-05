#pragma once
#include "CoreMinimal.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsUIIconRegionSelectButton.h"
#include "GsUIIconRegionSelectButtonSanctum.generated.h"

/*
*	성소 아이콘(클릭)
*/

struct FGsSanctumData;

class UImage;


UCLASS()
class UGsUIIconRegionSelectButtonSanctum : public UGsUIIconRegionSelectButton
{
	GENERATED_BODY()
protected:
	// 길드 이름
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textGuildName;
	// 길드 엠블렘 아이콘
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgGuildEmblemIcon;
	// 길드 엠블렘 아이콘1(하나는 선택, 하나는 비선택)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgGuildEmblemIcon1;
	// 선택 이미지
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgSelect;

private:
	// 성소 점령되었는가
	bool _isSanctumOccupied = false;
public:	
	// 선택 처리
	// 성소 선택 표시 별도 처리
	// https://jira.com2us.com/jira/browse/CHR-24218
	virtual void SetSelect(bool In_select) override;

public:
	// 성소 정보 세팅
	void SetSanctumInfo(const FGsSanctumData* In_data, FText In_spotName);
};