#pragma once
#include "GsSphere.h"

// 
enum GsViewState
{
	VS_INSIDE,   // 구 범위내 완전히 포함
	VS_PARTIAL,  // 구 경계면 충돌
	VS_OUTSIDE   // 구 범위 밖
};

enum GsSphereNodeFlag
{
	SPF_SUPERSPHERE = (1 << 0), // 자동으로 생성/삭제(로직컬)
	SPF_ROOT_TREE	= (1 << 1),	// 데이터 관리용 트리
	SPF_LEAF_TREE	= (1 << 2),	// 데이터 입력용 트리
	SPF_ROOTNODE	= (1 << 3),	// Recompute(X) Remove(X) Intergrate(X) : 설정용 구 (모든계산 예외)
	SPF_RECOMPUTE	= (1 << 4),	// 구 범위 재개산용 플레그
	SPF_INTEGRATE	= (1 << 5),	// 다른트리와 합산용 플레그	
};


class FGsSphereNodeCallback;
class FGsSpherePackFactory;

//#define DEBUG_SPHERE


// 구트리 노드
// 부모 자식, 이웃 표현

class T1PROJECT_API FGsSphereNode : public FGsSphere
{
private:
	FGsSphereNode*			m_pNext;
	FGsSphereNode*			m_pPrevious;	

	FGsSphereNode*			m_pParent;
	FGsSphereNode*			m_pChildren;	

	FGsSphereNode*			m_pNextSibling; 
	FGsSphereNode*			m_pPrevSibling; 

	int						m_nFlags;		
	int						m_nChildCount;	

	float					m_fBindingDistanceSqr;

	FGsSphereNode*			m_pLinkData;

	void*					m_pUserData;

	TWeakPtr<FGsSpherePackFactory>	m_pFactory;		


#ifdef DEBUG_SPHERE
	FGsSphereNode**			m_ppRecompute;		
	FGsSphereNode**			m_ppIntegrate;		

	TWeakObjectPtr<AActor>  m_pDebugActor;
	bool					m_bDirty = true;
#endif

public:
	FGsSphereNode(void)
	{
		m_pUserData = nullptr;				
		m_pLinkData = nullptr;
		m_pFactory = nullptr;				
		m_pNext = nullptr;					
		m_pPrevious = nullptr;
		m_pParent = nullptr;
		m_pNextSibling = nullptr;			
		m_pPrevSibling = nullptr;
		m_pChildren = nullptr;				
		m_nChildCount = 0;					
		W = 0.0f;							
		Center.Set(0, 0, 0);				

#ifdef DEBUG_SPHERE
		m_ppRecompute = nullptr;			
		m_ppIntegrate = nullptr;			
#endif
	};
	virtual ~FGsSphereNode() {}


	void Init(TWeakPtr<FGsSpherePackFactory> factory,
		const FVector& center,
		float radius,
		FGsSphereNode* linkdata,
		void* userdata = nullptr);
	
	void SpawnDebug(bool isRoot = false);
	void RemoveDebug();

public:
	void SetSphereNodeFlag(GsSphereNodeFlag flag)	{ m_nFlags |= flag; };
	void ClearSphereNodeFlag(GsSphereNodeFlag flag) { m_nFlags &= ~flag; };
	bool HasSphereNodeFlag(GsSphereNodeFlag flag) const
	{
		if (m_nFlags & flag) return true;
		return false;
	};
public:
	void SetParent(FGsSphereNode* pack) { m_pParent = pack; };
	FGsSphereNode* GetParent(void) const { return m_pParent; };

public:
	void NewPos(const FVector& pos);
	void NewPosRadius(const FVector& pos, float radius);

public:
	void Unlink();
	bool Recompute(float gravy);

	// 현재 설정된 상태 플레그를 지운다 (자식포함)
	void Reset();
public:
	void SetNextSibling(FGsSphereNode* child) { m_pNextSibling = child; };
	void SetPrevSibling(FGsSphereNode* child) { m_pPrevSibling = child; };

	FGsSphereNode* GetNextSibling(void) const { return m_pNextSibling; };
	FGsSphereNode* GetPrevSibling(void) const { return m_pPrevSibling; };

public:	
	void SetNext(FGsSphereNode* pack)			{ m_pNext = pack; };
	void SetPrevious(FGsSphereNode* pack)		{ m_pPrevious = pack; };

	FGsSphereNode* GetNext(void)     const		{ return m_pNext; };
	FGsSphereNode* GetPrevious(void) const		{ return m_pPrevious; };

public:
	void AddChild(FGsSphereNode* pack);
	void LostChild(FGsSphereNode* pack);

	FGsSphereNode* GetChildren(void)    const { return m_pChildren; };

public:
	void*	GetUserDataPtr(void) const { return m_pUserData; };
	void	SetUserDataPtr(void* data) { m_pUserData = data; };

	FGsSphereNode*	GetLinkData(void) const { return m_pLinkData; };
	void	SetLinkData(FGsSphereNode* data) { m_pLinkData = data; };

#ifdef DEBUG_SPHERE
public:
	void	SetFifoRecompute(FGsSphereNode** fifo)				{ m_ppRecompute = fifo;	};
	void	SetFifoIntegrate(FGsSphereNode** fifo)				{ m_ppIntegrate = fifo;	};
#endif

public:
	virtual void SetRadius(float r) override;

public:
	void ComputeBindingDistance(FGsSphereNode* parent);	

public:
	float DistanceSquared(const FGsSphereNode* pack) const;
	const FVector& GetPos(void) const { return Center; };
	int GetChildCount(void) const { return m_nChildCount; };

public:
	void RayTrace(const FVector& originRay, const FVector& dirRay, float lengthRay, TWeakPtr<FGsSphereNodeCallback> callback);
	void RangeTest(const FVector& center, float radius, TWeakPtr<FGsSphereNodeCallback> callback, GsViewState state);

};



