#pragma once

class FGsStarLegacySlotMagnitudeInfo;

struct FGsSchemaStarLegacyCategory;

// 별의 유산 카테고리 정보(ex: 1성계)
// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=396019674

class FGsStarLegacyCategoryInfo
{
public:
	// 카테고리 id
	int _categoryId;
	// 배열: n등성 정보들
	TArray<FGsStarLegacySlotMagnitudeInfo*> _arraySlotMagnitudeInfo;
	// 테이블 정보
	const FGsSchemaStarLegacyCategory* _tblInfo;

	// 로직
public:
	// 정리
	void Finalize();

};