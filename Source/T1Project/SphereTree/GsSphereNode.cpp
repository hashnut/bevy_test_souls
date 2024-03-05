
#include "GsSphereNode.h"
#include "GsSphereTree.h"
#ifdef DEBUG_SPHERE
#include "Management/ScopeGame/GsGameObjectManager.h"
#endif

static AActor* BPClass(UWorld* World, UClass* Class, const FVector& SpawnLocation, const FRotator& SpawnRotation, FName Name = NAME_None)
{
	if (nullptr == World)
		return nullptr;

	if (nullptr == Class)
		return nullptr;

	FActorSpawnParameters spawnParam = FActorSpawnParameters();
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (Name != NAME_None)
	{
		spawnParam.Name = Name;
	}

	return World->SpawnActor(Class, &SpawnLocation, &SpawnRotation, spawnParam);
}


void FGsSphereNode::Init(TWeakPtr<FGsSpherePackFactory> factory, const FVector& center, float radius, FGsSphereNode* linkdata, void* userdata /*= nullptr*/)
{
	m_pChildren = nullptr;
	m_nChildCount = 0;

	m_pUserData = userdata;
	m_pLinkData = linkdata;
	m_pParent = nullptr;
	m_pNextSibling = nullptr;
	m_pPrevSibling = nullptr;
	m_nFlags = 0;
	m_pFactory = factory;
	Center = center;
	W = radius;

#ifdef DEBUG_SPHERE
	m_ppRecompute = nullptr;
	m_ppIntegrate = nullptr;	

	m_bDirty = true;
#endif
}

const float ScaleRatio = 0.02f;

void FGsSphereNode::SpawnDebug(bool isRoot)
{
#ifdef DEBUG_SPHERE
	UClass* bpClass = nullptr;
	if (false == m_pDebugActor.IsValid())
	{
		bpClass = LoadObject<UClass>(nullptr, isRoot ? TEXT("/Game/DesignTest/SuperSphereNodeBP.SuperSphereNodeBP_C") : TEXT("/Game/DesignTest/SphereNodeBP.SphereNodeBP_C"));
		m_pDebugActor = BPClass(GSGameObject()->GetWorld(), bpClass, Center, FRotator() /*, name*/);
	}
	else
	{
		m_pDebugActor->SetActorLocation(Center);
	}
	float modelR = W * ScaleRatio;
	m_pDebugActor->SetActorScale3D(FVector(modelR, modelR, modelR));
#endif

	FGsSphereNode* pack = m_pChildren;
	bool found = false;
	while (nullptr != pack)
	{
		pack->SpawnDebug(isRoot);
		pack = pack->GetNextSibling();
	}
}

void FGsSphereNode::RemoveDebug()
{
#ifdef DEBUG_SPHERE
	if (m_pDebugActor.IsValid())
	{
		m_pDebugActor->Destroy();
		m_pDebugActor = nullptr;
	}
#endif
}

void FGsSphereNode::NewPos(const FVector& pos)
{
	Center = pos;    

#ifdef DEBUG_SPHERE
	if (m_pDebugActor.IsValid())
	{
		m_pDebugActor->SetActorLocation(pos);
	}
#endif

	if (m_pParent && (false == HasSphereNodeFlag(SPF_INTEGRATE)))
	{
		float dist = DistanceSquared(m_pParent);

		if (dist >= m_fBindingDistanceSqr)
		{
			if (false == m_pParent->HasSphereNodeFlag(SPF_RECOMPUTE))
			{
				m_pFactory.Pin()->AddRecomputeFIFO(m_pParent);
			}
			Unlink();

			m_pFactory.Pin()->AddIntegrateFIFO(this);
		}
	}

}

void FGsSphereNode::NewPosRadius(const FVector& pos, float radius)
{
	Center = pos;

#ifdef DEBUG_SPHERE
	if (m_pDebugActor.IsValid())
	{
		m_pDebugActor->SetActorLocation(pos);
		float modelR = radius * ScaleRatio;
		m_pDebugActor->SetActorScale3D(FVector(modelR, modelR, modelR));
	}
#endif

	if (m_pParent && (false == HasSphereNodeFlag(SPF_INTEGRATE)))
	{
		if (radius != GetRadius())
		{
			SetRadius(radius);
			ComputeBindingDistance(m_pParent);
		}
				
		float distsqr = DistanceSquared(m_pParent);

		if (false == m_pParent->HasSphereNodeFlag(SPF_RECOMPUTE))
		{
			m_pFactory.Pin()->AddRecomputeFIFO(m_pParent);
		}
		// 바인딩된 거리보다 크다
		if (distsqr >= m_fBindingDistanceSqr)
		{
			Unlink();
			m_pFactory.Pin()->AddIntegrateFIFO(this);
		}
	}
}

void FGsSphereNode::Unlink(void)
{
#ifdef DEBUG_SPHERE
	if (nullptr != m_ppRecompute) 
	{
		*m_ppRecompute = nullptr;
		m_ppRecompute = nullptr;
	}

	if (nullptr != m_ppIntegrate) 
	{
		*m_ppIntegrate = nullptr;
		m_ppIntegrate = nullptr;
	}
#endif

	if (nullptr != m_pParent)
	{
		m_pParent->LostChild(this);
	}

	m_pParent = nullptr;

	check(nullptr == m_pChildren); 
}

// 자식들의 평균점으로 중점을 이동한후 반지름 검사
// 나자신의 범주에 포함되지 않는 자식이 있다면 반지름 증가, 자식들 신규 중점에 맞게 재계산
// 아니라면 유지
bool FGsSphereNode::Recompute(float gravy)
{
	if (nullptr == m_pChildren)
	{
		return true; 
	}

	if (HasSphereNodeFlag(SPF_ROOTNODE))
	{
		return false; 
	}


	FVector total = FVector::ZeroVector;
	int count = 0;

	FGsSphereNode* pack(m_pChildren);

	while (nullptr != pack)
	{
		total += pack->GetCenter();
		count++;
		pack = pack->GetNextSibling();
	}

	if (0 != count)
	{
		float recip = 1.0f / float(count);
		total *= recip;
		
		FVector oldCenter = Center;
		// 모든 자식포지션의 평균점을 중점으로 재설정
		Center = total; 

		float maxradius = 0;
		pack = m_pChildren;
		while (nullptr != pack)
		{
			float dist = DistanceSquared(pack);
			float radius = sqrtf(dist) + pack->GetRadius();
			if (radius > maxradius)
			{
				maxradius = radius;
				if ((maxradius + gravy) >= GetRadius())
				{
					Center = oldCenter;
					ClearSphereNodeFlag(SPF_RECOMPUTE);
					return false;
				}
			}
			pack = pack->GetNextSibling();
		}

		maxradius += gravy;

		SetRadius(maxradius);
		
		pack = m_pChildren;
		while (nullptr != pack)
		{
			pack->ComputeBindingDistance(this);
			pack = pack->GetNextSibling();
		}
	}

	ClearSphereNodeFlag(SPF_RECOMPUTE);

	return false;
}

void FGsSphereNode::Reset()
{
	FGsSphereNode* pack = m_pChildren;
	while (nullptr != pack)
	{
		pack->Reset();
		pack = pack->GetNextSibling();
	}
}

void FGsSphereNode::AddChild(FGsSphereNode* pack)
{
	FGsSphereNode* my_child = m_pChildren;
	m_pChildren = pack;

	pack->SetNextSibling(my_child);
	pack->SetPrevSibling(nullptr); 
	pack->SetParent(this);

	if (nullptr != my_child)
	{
		my_child->SetPrevSibling(pack);
	}

	m_nChildCount++;

	float distsqr = DistanceSquared(pack);
	float radius = sqrtf(distsqr) + pack->GetRadius();

	check(radius <= GetRadius());
}

//부모에서 호출(부모기준에서 판단)
void FGsSphereNode::LostChild(FGsSphereNode* child)
{
	check(m_nChildCount);
	check(nullptr != m_pChildren);

#ifdef DEBUG_SPHERE  
	FGsSphereNode* pack( m_pChildren);
	bool found = false;
	while (nullptr != pack)
	{
		if (pack == child)
		{
			check(!found);
			found = true;
		}
		pack = pack->GetNextSibling();
	}
	check(found);

#endif
	// 이전 이웃노드에서 자식제거
	FGsSphereNode* prev = child->GetPrevSibling();

	if (nullptr != prev)
	{
		FGsSphereNode* next = child->GetNextSibling();
		prev->SetNextSibling(next); 
		if (nullptr != next)
		{
			next->SetPrevSibling(prev);
		}
	}
	else
	{
		// 이후 이웃노드에서 자식제거
		FGsSphereNode* next = child->GetNextSibling();
		m_pChildren = next;
		if (nullptr != m_pChildren)
		{
			m_pChildren->SetPrevSibling(nullptr);
		}
	}

	m_nChildCount--;

	if ((0 == m_nChildCount) && HasSphereNodeFlag(SPF_SUPERSPHERE))
	{
		m_pFactory.Pin()->Remove(this);
	}
}

void FGsSphereNode::SetRadius(float r)
{
	FGsSphere::SetRadius(r);

	if (HasSphereNodeFlag(SPF_LEAF_TREE))
	{
		FGsSphereNode* link = GetLinkData();
		if (nullptr != link && link->GetRadius() != r)
		{
			link->SetRadius(r);

#ifdef DEBUG_SPHERE
			if (m_pDebugActor.IsValid())
			{
				float modelR = r * r;
				m_pDebugActor->SetActorScale3D(FVector(modelR, modelR, modelR));
			}
#endif
		}
	}

}

void FGsSphereNode::ComputeBindingDistance(FGsSphereNode* parent)
{	
	float fBindingDistance = parent->GetRadius() - GetRadius();
	m_fBindingDistanceSqr = (fBindingDistance <= 0.0f) ? 0.0f : fBindingDistance * fBindingDistance;	
}

float FGsSphereNode::DistanceSquared(const FGsSphereNode* pack) const
{
	return FVector::DistSquared(GetCenter(), pack->Center);
}

void FGsSphereNode::RayTrace(const FVector& originRay, const FVector& dirRay, float lengthRay, TWeakPtr<FGsSphereNodeCallback> callback)
{
	bool hit = false;
	if (HasSphereNodeFlag(SPF_SUPERSPHERE))
	{
		if (RayIntersectionInFront(originRay, dirRay))
		{
			FGsSphereNode* pack = m_pChildren;
			while (nullptr != pack)
			{
				pack->RayTrace(originRay, dirRay, lengthRay, callback);
				pack = pack->GetNextSibling();
			}
		}
	}
	else
	{		
		hit = RayIntersection(originRay, dirRay);
		if (hit)
		{
			callback.Pin()->RayTraceCallback(originRay, dirRay, lengthRay, this);
		}
	}
}

void FGsSphereNode::RangeTest(const FVector& inCenter, float inRadius, TWeakPtr<FGsSphereNodeCallback> callback, GsViewState state)
{
	if (state == VS_PARTIAL)
	{
		float d = FVector::Distance(inCenter, GetPos());
		// 구의 교차 판정 (거리에서 반지름을 제거하여 구 인접거리를 구한후 반지름과 교차검사)
		if ((d - inRadius) > GetRadius())
		{
			return;
		}

		// 현재구가 체크하는 영역내에 있음
		if ((GetRadius() + d) < inRadius)
		{
			state = VS_INSIDE;
		}
	}

	// 슈퍼노드는 자식검사용임으로 콜백하지 않음
	if (HasSphereNodeFlag(SPF_SUPERSPHERE))
	{
		FGsSphereNode* pack(m_pChildren);
		while (nullptr != pack)
		{
			pack->RangeTest(inCenter, inRadius, callback, state);
			pack = pack->GetNextSibling();
		}
	}
	else
	{
		callback.Pin()->RangeTestCallback(inCenter, inRadius, this, state);
	}
}


