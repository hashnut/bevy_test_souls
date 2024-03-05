#include "GsBookmarkManager.h"

#include "Map/Bookmark/GsBookmarkData.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "Net/GsNetSendServiceWorld.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/State/GsGameObjectStateManager.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/GsAutoMoveInfo.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UTIL/GsMove.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "DataSchema/ItemCollection/GsSchemaItemCollection.h"

#include "Shared/Shared/SharedInclude/SharedEnums.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetSendServiceWorld.h"

// 초기화
void FGsBookmarkManager::Initialize()
{
	FGsMessageHolder* msg = GMessage();

	if (msg == nullptr)
	{
		return;
	}
	auto& uiHandler = msg->GetUI();

	_listUiDelegate.Emplace(uiHandler.AddRaw(MessageUI::INIT_BOOK_MARK_ALL_INFO,
		this, &FGsBookmarkManager::OnInitBookMarkAllInfo));

	InitServerData();
}
// 북마크 모든정보 초기화
void FGsBookmarkManager::OnInitBookMarkAllInfo(const IGsMessageParam*)
{
	InitServerData();
}
// 서버 데이터 초기화(가져오기)
void FGsBookmarkManager::InitServerData()
{
	ClearData();

	const FGsNetUserData* userData = GGameData()->GetUserData();

	for (const FGsNetUserData::FBookmarkInfo& iter : userData->listBookmarkInfo)
	{
		FGsBookmarkData* newData = new FGsBookmarkData(
			iter.mid, iter.mName, iter.mPos, iter.mMapId);
		_arrBookmarkData.Add(newData);

		// 맵리스트에 추가
		AddMapList(newData);
	}
	// 추가 되었으니 정렬
	Sort(_currSortType);

	// 모든 맵 리스트 정렬
	for (auto iter : _mapBookmarkList)
	{
		SortMapList(iter.Key);
	}

	_isDirtySortBookMarkList = false;
}
// 북마크 만들기 요청
void FGsBookmarkManager::SendBookmarkCreate(const FString& In_name)
{
	FGsNetSendServiceWorld::SendBookmarkCreate(In_name);
}
// 북마크 수정
void FGsBookmarkManager::SendBookmarkUpdate(int In_itemIndex, const FString& In_name)
{
	// 인덱스가 배열보다 크면 리턴
	if (In_itemIndex >= _arrBookmarkData.Num())
	{
		return;
	}

	FGsBookmarkData* data = _arrBookmarkData[In_itemIndex];
	if (data == nullptr)
	{
		return;
	}


	// 서버 요청
	_reqChangeName = In_name;
	FGsNetSendServiceWorld::SendBookmarkUpdate(data->GetId(), In_name);
}
// 북마크 삭제
void FGsBookmarkManager::SendBookmarkDelete(int In_itemIndex)
{
	// 인덱스가 배열보다 크면 리턴
	if (In_itemIndex >= _arrBookmarkData.Num())
	{
		return;
	}

	FGsBookmarkData* data = _arrBookmarkData[In_itemIndex];
	if (data == nullptr)
	{
		return;
	}
	FGsNetSendServiceWorld::SendBookmarkDelete(data->GetId());
}

// 북마크 만들기 결과
void FGsBookmarkManager::RecvBookmarkCreate(uint64 In_id, const FString& In_name,
	const FVector& In_pos, int In_mapId)
{
	// 이전꺼에서 찾아본다
	FGsBookmarkData* findData = GetBookmarkDataById(In_id);

	// 기존에 없던거면
	if (findData == nullptr)
	{
		FGsBookmarkData* newData = new FGsBookmarkData(
			In_id, In_name, In_pos, In_mapId);
		// 추가
		// 제일 앞에 넣음
		_arrBookmarkData.Insert(newData, 0);

		_isDirtySortBookMarkList = true;

		// 맵리스트에 추가
		AddMapList(newData);
		// 추가후 정렬
		SortMapList(In_mapId);
	}
	// 기존에 있던거면
	else
	{
		findData->SetAllData(In_name, In_pos, In_mapId);
	}

	// 생성시 정렬이 아니라
	// 생성후 ui 다시 열거나
	// 정렬 버튼 누를때 정렬하자


	// ui에 메시지 전송(전체 갱신)
	GMessage()->GetGameObject().SendMessage(MessageGameObject::BOOKMARK_ALL_LIST_CHANGE, nullptr);
}

// 북마크 수정 결과
void FGsBookmarkManager::RecvBookmarkUpdate(uint64 In_id)
{
	FGsBookmarkData* findData = GetBookmarkDataById(In_id);

	if (findData == nullptr)
	{
		return;
	}
	// 저장해놓은 이름으로 세팅
	findData->SetName(_reqChangeName);

	// 이름 정렬이면 정렬 한번 때림
	if (_currSortType == EGsBookmarkSortType::Name)
	{
		// 추가 되었으니 정렬
		Sort(_currSortType);		
		// ui에 메시지 전송(전체 갱신)
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BOOKMARK_ALL_LIST_CHANGE, nullptr);
	}
	else if (_currSortType == EGsBookmarkSortType::EnrollTime)
	{
		// 등록 시간순이면 한개의 아이템만 갱신
		GMessage()->GetGameObject().SendMessage(MessageGameObject::BOOKMARK_SEL_LIST_CHANGE, nullptr);
	}
	

}
// 북마크 삭제 결과
void FGsBookmarkManager::RecvBookmarkDelete(uint64 In_id)
{
	FGsBookmarkData* findData = GetBookmarkDataById(In_id);

	// 기존에 없는거면 할거 없음
	if (findData == nullptr)
	{
		return;
	}
	// 맵리스트에서 삭제
	DelMapList(findData);

	delete findData;
	// 삭제
	_arrBookmarkData.Remove(findData);

	// ui에 메시지 전송(전체 갱신)
	GMessage()->GetGameObject().SendMessage(MessageGameObject::BOOKMARK_ALL_LIST_CHANGE, nullptr);
}
// id로 데이터 찾기
FGsBookmarkData* FGsBookmarkManager::GetBookmarkDataById(uint64 In_id)
{
	FGsBookmarkData** findData =
		_arrBookmarkData.FindByPredicate(
			[In_id](FGsBookmarkData* el)
	{
		return el->GetId() == In_id;
	});

	if (findData == nullptr)
	{
		return nullptr;
	}

	return *findData;
}
// 배열 인덱스로 데이터 찾기
FGsBookmarkData* FGsBookmarkManager::GetBookmarkDataByIndex(int In_idex)
{
	// 인덱스가 배열보다 크면 리턴
	// 0보다 작아도 리턴
	if (In_idex >= _arrBookmarkData.Num() || 
		In_idex < 0)
	{
		return nullptr;
	}

	return _arrBookmarkData[In_idex];
}
// 정렬
void FGsBookmarkManager::Sort(EGsBookmarkSortType In_sortType)
{
	_arrBookmarkData.Sort([In_sortType] ( FGsBookmarkData& lhs, FGsBookmarkData& rhs)
		{
			// 등록 시간순은 최근이 앞으로 오게
			if (In_sortType == EGsBookmarkSortType::EnrollTime)
			{
				return lhs.GetId() > rhs.GetId();
			}
			else if(In_sortType == EGsBookmarkSortType::Name)
			{
				return lhs.GetName() < rhs.GetName();
			}
			return false;
		});
}
// 맵 검색용 리스트 정렬
void FGsBookmarkManager::SortMapList(int In_mapId, EGsBookmarkSortType In_sortType)
{
	TArray<FGsBookmarkData*>* listData =
		_mapBookmarkList.Find(In_mapId);

	if (listData == nullptr)
	{
		return;
	}

	listData->Sort([In_sortType](FGsBookmarkData& lhs, FGsBookmarkData& rhs)
	{
		// 등록 시간순은 최근이 앞으로 오게
		if (In_sortType == EGsBookmarkSortType::EnrollTime)
		{
			return lhs.GetId() > rhs.GetId();
		}
		else if (In_sortType == EGsBookmarkSortType::Name)
		{
			return lhs.GetName() < rhs.GetName();
		}
		return false;
	});
}
// 정렬 변경
void FGsBookmarkManager::OnSortChange(EGsBookmarkSortType In_sortType)
{
	_currSortType = In_sortType;
	Sort(_currSortType);
}

// 정리
void FGsBookmarkManager::Finalize()
{
	ClearData();


	FGsMessageHolder* msg = GMessage();
	for (auto iter : _listUiDelegate)
	{
		msg->GetUI().Remove(iter);
	}
	_listUiDelegate.Empty();
}
// 데이터 클리어
void FGsBookmarkManager::ClearData()
{
	if (_arrBookmarkData.Num() == 0)
	{
		return;
	}

	for (FGsBookmarkData* info : _arrBookmarkData)
	{
		if (info == nullptr)
		{
			continue;
		}
		delete info;
	}
	_arrBookmarkData.Empty();

	_mapBookmarkList.Empty();

	_shareInfo.ClearShareInfo();
	_isShareCoolTimeStart = false;
}

// 맵 리스트에 추가
void FGsBookmarkManager::AddMapList(FGsBookmarkData* In_data)
{
	if (In_data == nullptr)
	{
		return;
	}

	int mapId = In_data->GetMapId();
	TArray<FGsBookmarkData*>* listData =
		_mapBookmarkList.Find(mapId);

	// 기존에 맵id가 없다면
	if (listData == nullptr)
	{
		// 추가
		TArray<FGsBookmarkData*> newList;
		newList.Add(In_data);
		_mapBookmarkList.Add(mapId, newList);
	}
	// 있다면
	else
	{
		uint64 gameId = In_data->GetId();
		FGsBookmarkData** findData =
			listData->FindByPredicate(
				[gameId](FGsBookmarkData* el)
		{
			return el->GetId() == gameId;
		});
		// 없다면
		if (findData == nullptr)
		{
			// 추가
			listData->Add(In_data);
		}		
	}
}
// 맵 리스트에서 삭제
void FGsBookmarkManager::DelMapList(FGsBookmarkData* In_data)
{
	if (In_data == nullptr)
	{
		return;
	}

	int mapId = In_data->GetMapId();
	TArray<FGsBookmarkData*>* listData =
		_mapBookmarkList.Find(mapId);

	// 기존에 맵id가 없다면
	if (listData == nullptr)
	{
		return;
	}
	else
	{
		uint64 gameId = In_data->GetId();
		FGsBookmarkData** findData =
			listData->FindByPredicate(
				[gameId](FGsBookmarkData* el)
		{
			return el->GetId() == gameId;
		});
		// 있다면
		if (findData != nullptr)
		{
			// 삭제
			listData->Remove(In_data);
		}
	}

}

// 해당 맵의 북마크 정보 가져오기
TArray<FGsBookmarkData*>* FGsBookmarkManager::GetBookmarkDataByMapId(int In_mapId)
{
	TArray<FGsBookmarkData*>* listData =_mapBookmarkList.Find(In_mapId);
	return listData;
}
// ui 열기전 호출(정리할껀 정리해라 이말이야)
void FGsBookmarkManager::OnPreOpenUI()
{
	if (_isDirtySortBookMarkList == false)
	{
		return;
	}

	_isDirtySortBookMarkList = false;
	Sort(_currSortType);
}
// 갱신
void FGsBookmarkManager::Update(float In_delta)
{
	if (_isShareCoolTimeStart == true)
	{
		int64 nowTick = FDateTime::UtcNow().GetTicks();
		int64 diffTick = nowTick - _shareCoolTimeStartTime;

		static float maxCoolTime = 
			GData()->GetGlobalData()->_bookmarkShareCoolTime * ETimespan::TicksPerSecond;

		if (diffTick >= maxCoolTime)
		{
			_isShareCoolTimeStart = false;
		}
	}

#ifndef TEST_COLLECTION_CHEAT
	UpdateAllCollectionComplete();
#endif
}
void FGsBookmarkManager::StartShareCoolTime()
{
	_isShareCoolTimeStart = true;
	_shareCoolTimeStartTime = FDateTime::UtcNow().GetTicks();
}

void FGsBookmarkManager::TestExp(bool In_on)
{
	UWorld* world = GSGameObject()->GetWorld();
	if (world == nullptr)
	{
		return;
	}

	if (true == In_on)
	{
		FTimerDelegate timerCallback;
		timerCallback.BindLambda([] {
				FString sendCMD(TEXT("increaseExp 9999999999999999999"));
				FGsNetSendService::SendWorldAdminCommandQA(sendCMD);			
			});
		
		world->GetTimerManager().SetTimer(_testExpTimeHandle,
			timerCallback, 0.1f, true);
	}
	else
	{
		world->GetTimerManager().ClearTimer(_testExpTimeHandle);
		_testExpTimeHandle.Invalidate();
	}
}

void FGsBookmarkManager::TestAllCollectionComplete()
{
	_isStartTestCollectionCheat = true;
	_testCurrentCollectionIndex = 0;
}
void FGsBookmarkManager::UpdateAllCollectionComplete()
{
	if (_isStartTestCollectionCheat == false)
	{
		return;
	}


	const UGsTable* itemCollectionDataTbl = FGsSchemaItemCollection::GetStaticTable();

	if (nullptr == itemCollectionDataTbl)
	{
		_isStartTestCollectionCheat = false;
		return;
	}

	TArray<const FGsSchemaItemCollection*> rowsItemCollectionData;
	itemCollectionDataTbl->GetAllRows<FGsSchemaItemCollection>(rowsItemCollectionData);

	if (rowsItemCollectionData.Num() == 0 ||
		rowsItemCollectionData.Num() <= _testCurrentCollectionIndex)
	{
		_isStartTestCollectionCheat = false;
		return;
	}
	
	const FGsSchemaItemCollection*  collectionTbl = rowsItemCollectionData[_testCurrentCollectionIndex++];

	if (collectionTbl == nullptr)
	{
		return;
	}

	const FString rowNameString =  collectionTbl->GetRowName().ToString();


	const CollectionId filedId = FCString::Atoi(*rowNameString);

	FString command = FString::Printf(TEXT("CompleteItemCollectionOne   %d"), filedId);
	FGsNetSendService::SendWorldAdminCommandQA(command);

}