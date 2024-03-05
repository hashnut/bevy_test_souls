#include "GsSphereTree.h"
#include "GsSphereNode.h"
#include "GsSpherePack.h"


FGsSpherePackFactory::~FGsSpherePackFactory()
{
	m_kSpheres.ReleasePool();
	m_pIntegrateFIFO.Reset();
	m_pRecomputeFIFO.Reset();
}

void FGsSpherePackFactory::Init(const FVector& levelCenter, float levelRaius, int maxspheres, float rootsize, float leafsize, float gravy)
{
	m_nMaxSphere = maxspheres * 4;
	m_fMaxRootSize = rootsize;
	m_fMaxLeafSize = leafsize;
	m_fSuperSphereGravy = gravy;

	if (false == m_pIntegrateFIFO.IsValid())
	{
		m_pIntegrateFIFO = MakeShareable(new FGsSpherePack());
	}
	m_pIntegrateFIFO->Init(m_nMaxSphere);

	
	if (false == m_pRecomputeFIFO.IsValid())
	{
		m_pRecomputeFIFO = MakeShareable(new FGsSpherePack());
	}
	m_pRecomputeFIFO->Init(m_nMaxSphere);

	m_kSpheres.SetPoolCount(m_nMaxSphere);

	m_pRoot = m_kSpheres.GetFreeLink();
	m_pRoot->Init(SharedThis(this), FVector::ZeroVector, 6553600, nullptr, nullptr);
	m_pRoot->SetSphereNodeFlag(GsSphereNodeFlag(SPF_SUPERSPHERE | SPF_ROOTNODE | SPF_ROOT_TREE));

	m_pLeaf = m_kSpheres.GetFreeLink();
	m_pLeaf->Init(SharedThis(this), FVector::ZeroVector, 1638400, nullptr, nullptr);
	m_pLeaf->SetSphereNodeFlag(GsSphereNodeFlag(SPF_SUPERSPHERE | SPF_ROOTNODE | SPF_LEAF_TREE));
}

void FGsSpherePackFactory::Process(void)
{
	bool bRecompute = false;
	bool bIntergreate = false;

	FGsSphereNode* pack = nullptr;
	if (m_pRecomputeFIFO.IsValid())
	{
		int maxrecompute = m_pRecomputeFIFO->GetCount();		
		
		bRecompute = maxrecompute != 0;

		for (int i = 0; i < maxrecompute; i++)
		{
			pack = m_pRecomputeFIFO->Pop();

			if (nullptr == pack)
			{
				break;
			}
#ifdef DEBUG_SPHERE
			// no longer on the fifo!!
			pack->SetFifoRecompute(nullptr);
#endif
			if (pack->Recompute(m_fSuperSphereGravy))
			{
				Remove(pack);
			}			
		}
	}
	
	if(m_pIntegrateFIFO.IsValid())
	{
		int maxintegrate = m_pIntegrateFIFO->GetCount();

		bIntergreate = maxintegrate != 0;

		for (int i = 0; i < maxintegrate; i++)
		{
			pack = m_pIntegrateFIFO->Pop();
			if (nullptr == pack)
			{
				break;
			}

#ifdef DEBUG_SPHERE
			pack->SetFifoIntegrate(nullptr);
#endif
			if (pack->HasSphereNodeFlag(SPF_ROOT_TREE))
			{
				Integrate(pack, m_pRoot, m_fMaxRootSize); 
			}
			else
			{
				Integrate(pack, m_pLeaf, m_fMaxLeafSize); 
			}				
		}
	}

#ifdef DEBUG_SPHERE
	if (bRecompute || bIntergreate)
	{
		switch (m_nDebugMode)
		{
		case 1:
			m_pRoot->SpawnDebug(true);
		case 2:
			m_pLeaf->SpawnDebug();
		case 3:
			m_pRoot->SpawnDebug(true);
			m_pLeaf->SpawnDebug();
			break;
		}
	}
#endif
}

void FGsSpherePackFactory::Reset()
{
	m_pRoot->Reset();
	m_pLeaf->Reset();
}

void FGsSpherePackFactory::SpawnDebug(int inMode)
{
	m_nDebugMode = inMode;
}

FGsSphereNode* FGsSpherePackFactory::AddSphere(const FVector& pos, float radius, int flags /*= SPF_LEAF_TREE*/, FGsSphereNode* linkdata /*= nullptr*/, void* userdata /*= nullptr*/)
{
	FGsSphereNode* pack = m_kSpheres.GetFreeLink();
	check(pack);

	if (flags & SPF_ROOT_TREE)
	{
		pack->Init(SharedThis(this), pos, radius, linkdata, userdata);
		pack->SetSphereNodeFlag(SPF_ROOT_TREE); 
		AddIntegrateFIFO(pack); 
	}
	else
	{
		pack->Init(SharedThis(this), pos, radius, linkdata, userdata);
		pack->SetSphereNodeFlag(SPF_LEAF_TREE); 
		AddIntegrateFIFO(pack); 
	}
	return pack;
}

void FGsSpherePackFactory::AddIntegrateFIFO(FGsSphereNode* pack)
{
	if (pack->HasSphereNodeFlag(SPF_ROOT_TREE))
	{
		m_pRoot->AddChild(pack);
	}
	else
	{
		m_pLeaf->AddChild(pack);
	}

	pack->SetSphereNodeFlag(SPF_INTEGRATE); 

#ifdef DEBUG_SPHERE
	FGsSphereNode** fifo = m_pIntegrateFIFO->Push(pack); 
	// 디버그용 세팅
	pack->SetFifoIntegrate(fifo);
#else
	m_pIntegrateFIFO->Push(pack);
#endif
}

void FGsSpherePackFactory::AddRecomputeFIFO(FGsSphereNode* recompute)
{
	if (false == recompute->HasSphereNodeFlag(SPF_RECOMPUTE))
	{
		if (recompute->GetChildCount())
		{
			recompute->SetSphereNodeFlag(SPF_RECOMPUTE); 

#ifdef DEBUG_SPHERE
			FGsSphereNode** fifo = m_pRecomputeFIFO->Push(recompute);
			// 디버그용 세팅
			recompute->SetFifoRecompute(fifo);
#else
			m_pRecomputeFIFO->Push(recompute);
#endif
		}
		else
		{
			Remove(recompute);
		}
	}
}

// 구 구조간 병합 처리
void FGsSpherePackFactory::Integrate(FGsSphereNode* pack, FGsSphereNode* supersphere, float node_size)
{
	FGsSphereNode* search(supersphere->GetChildren());

	FGsSphereNode* nearestCompletely = nullptr;  
	float neardistComplete = 1e11;		  

	FGsSphereNode* nearest = nullptr; 
	float neardist = 1e11;    

	int scount = 1;

	while (nullptr != search)
	{
		if (search->HasSphereNodeFlag(SPF_SUPERSPHERE) && (false == search->HasSphereNodeFlag(SPF_ROOTNODE)) && search->GetChildCount())
		{
			float distsqr = pack->DistanceSquared(search);
			if (nullptr != nearestCompletely)
			{
				if (distsqr < neardistComplete)
				{
					float d = sqrtf(distsqr) + pack->GetRadius();
					// 찾은 슈퍼구가 입력된 구를 포함하는 상태
					if (d <= search->GetRadius())
					{
						neardistComplete = distsqr;
						nearestCompletely = search;
					}
				}
			}
			else
			{
				float d = (sqrtf(distsqr) + pack->GetRadius()) - search->GetRadius();

				if (d < neardist)
				{
					// 찾은 슈퍼구가 입력된 구를 포함하는 상태
					if (d < 0)
					{
						neardistComplete = distsqr;
						nearestCompletely = search;
					}
					else
					{
						// 포함은 아니지만 현재까지 가장 가까운 슈퍼구
						neardist = d;
						nearest = search;
					}
				}
			}
		}
		search = search->GetNextSibling();
	}

	if (nullptr != nearestCompletely)
	{
		pack->Unlink();

		nearestCompletely->AddChild(pack);
		nearestCompletely->Recompute(m_fSuperSphereGravy);

		pack->ComputeBindingDistance(nearestCompletely);

		if (nearestCompletely->HasSphereNodeFlag(SPF_LEAF_TREE))
		{
			FGsSphereNode* link = nearestCompletely->GetLinkData();
			link->NewPosRadius(nearestCompletely->GetPos(), nearestCompletely->GetRadius());
		}

	}
	else
	{
		bool newsphere = true;

		// 인접한 슈퍼구가 있고
		if (nullptr != nearest)
		{
			// 싸이즈를 슈퍼구 증가폭 까지 늘려서 원하는 반경내에 들어온다면
			float newsize = neardist + nearest->GetRadius() + m_fSuperSphereGravy;

			if (newsize <= node_size)
			{
				pack->Unlink();

				nearest->SetRadius(newsize);
				nearest->AddChild(pack);
				nearest->Recompute(m_fSuperSphereGravy);

				pack->ComputeBindingDistance(nearest);

				if (nearest->HasSphereNodeFlag(SPF_LEAF_TREE))
				{
					// 리프 트리일때는 루트에 복제정보 갱신
					FGsSphereNode* link = nearest->GetLinkData();
					link->NewPosRadius(nearest->GetPos(), nearest->GetRadius());
				}

				newsphere = false;
			}

		}

		// 조건에 맞는 슈퍼구가 없을대는 신규(대상 포지션 + 슈퍼구 반지름)로 생성한다.
		if (newsphere)
		{
			check(supersphere->HasSphereNodeFlag(SPF_ROOTNODE));

			pack->Unlink();

			FGsSphereNode* parent = m_kSpheres.GetFreeLink();
			check(nullptr != parent);
			parent->Init(SharedThis(this), pack->GetPos(), pack->GetRadius() + m_fSuperSphereGravy, nullptr );

			// 부모의 속성을 따라간다.
			if (supersphere->HasSphereNodeFlag(SPF_ROOT_TREE))
			{
				parent->SetSphereNodeFlag(SPF_ROOT_TREE);
			}
			else
			{
				parent->SetSphereNodeFlag(SPF_LEAF_TREE);
			}

			// 종속관계 설정
			parent->SetSphereNodeFlag(SPF_SUPERSPHERE);
			parent->AddChild(pack);
			supersphere->AddChild(parent);

			parent->Recompute(m_fSuperSphereGravy);
			pack->ComputeBindingDistance(parent);

			if (parent->HasSphereNodeFlag(SPF_LEAF_TREE))
			{
				// 리프 트리일때는 루트에 복제정보 생성
				FGsSphereNode* link = AddSphere(parent->GetPos(), parent->GetRadius(), SPF_ROOT_TREE, parent);
				parent->SetLinkData(link); 
			}
		}
	}
	pack->ClearSphereNodeFlag(SPF_INTEGRATE); 
}

//재귀호출 가능
void FGsSpherePackFactory::Remove(FGsSphereNode* pack)
{
	// CAN NEVER REMOVE THE ROOT NODE EVER!!!
	if (pack->HasSphereNodeFlag(SPF_ROOTNODE)) return; 

	
	if (pack->HasSphereNodeFlag(SPF_SUPERSPHERE) && pack->HasSphereNodeFlag(SPF_LEAF_TREE))
	{
		FGsSphereNode* link = pack->GetLinkData();
		Remove(link);
	}

	// 부모 관계를 끊는다.
	pack->Unlink();

	pack->RemoveDebug();
	// 메모리풀 반납.
	m_kSpheres.Release(pack);
}

void FGsSpherePackFactory::RayTrace(const FVector& source, const FVector& dest, TWeakPtr<FGsSphereNodeCallback> callback)
{
	FVector dir = dest - source;
	dir.Normalize();
	float dist = FVector::Distance(dest, source);
	m_pCallback = callback;
	m_pRoot->RayTrace(source, dir, dist, SharedThis(this));
}

// 최초는 리프노드를 들고 있지않은 트리로 검사후에.
bool FGsSpherePackFactory::RangeTest(const FVector& center, float radius, TWeakPtr<FGsSphereNodeCallback> callback)
{
	if (false == callback.IsValid())
	{
		return false;
	}
	m_pCallback = callback;
	// 검사는 루트에서 시행한다.
	m_pRoot->RangeTest(center, radius, SharedThis(this), VS_PARTIAL);
	return true;
}

void FGsSpherePackFactory::RayTraceCallback(const FVector& rayPos, const FVector& rayDir, float rayDistance,  const FGsSphereNode* InterSectsphere)
{
	FGsSphereNode* link = InterSectsphere->GetLinkData();
	if (nullptr != link)
	{
		link->RayTrace(rayPos, rayDir, rayDistance, m_pCallback);
	}
}

// 검색된 노드의 리프노드(링크)노드를 검색하여 검색비용을 줄임
void FGsSpherePackFactory::RangeTestCallback(const FVector& pos, float distance, const FGsSphereNode* sphere, GsViewState state)
{
	FGsSphereNode* link = sphere->GetLinkData();
	if (nullptr != link)
	{
		link->RangeTest(pos, distance, m_pCallback, state);
	}
}
