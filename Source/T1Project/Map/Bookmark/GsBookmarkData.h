#pragma once
#include "CoreMinimal.h"

/*
	위치 즐겨 찾기 데이터
*/

class FGsBookmarkData
{
	// 멤버 변수
private:
	// 시간정렬용, 작을수록 예전에 등록한거다
	uint64 _id;
	// 이름
	FString _name;	
	// 위치
	FVector _pos;
	// 맵 id
	int _mapId;

	// 생성자
public:
	FGsBookmarkData(uint64 In_id, const FString& In_name,
		const FVector& In_pos, int In_mapId) :_id(In_id), _name(In_name),
		_pos(In_pos), _mapId(In_mapId)
	{
	}
	 // get, set
public:
	// 데이터 세팅
	void SetAllData(const FString& In_name,
		const FVector& In_pos, int In_mapId);
	// 이름 변경
	void SetName(const FString& In_name);
	// id 접근
	uint64 GetId() { return _id; }
	// 이름 접근
	const FString& GetName() { return _name; }
	// 맵 id 접근
	int GetMapId() { return _mapId; }
	// 위치 접근
	const FVector& GetPos() { return _pos; }
};