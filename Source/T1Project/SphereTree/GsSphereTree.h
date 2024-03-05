#pragma once

#include "classes/GsPool.h"
#include "GsSphereNode.h"

class FGsSphereNodeCallback 
{
public:
	virtual void RayTraceCallback(const FVector& p1,			// source pos of ray
									const FVector& dir,			// direction of ray
									float distance,				// distance of ray
									const FGsSphereNode* sphere)		// sphere ray hit
	{};

	virtual void RangeTestCallback(const FVector& searchpos,	// position we are performing range test against.
									float distance,             // squared distance we are range searching against.
									const FGsSphereNode* sphere,
									GsViewState state)			// sphere within range, VS_PARTIAL if sphere straddles range test
	{};
};

// FIFO Queue;
class FGsSpherePack;

class FGsSpherePackFactory : public FGsSphereNodeCallback, public TSharedFromThis<FGsSpherePackFactory>
{
public:
	virtual ~FGsSpherePackFactory();
private:
	FGsSphereNode* m_pRoot;									
	FGsSphereNode* m_pLeaf;									
	TWeakPtr<FGsSphereNodeCallback> m_pCallback;

	TGsPoolPreSizeAlloc<FGsSphereNode>	m_kSpheres;			

	TSharedPtr<FGsSpherePack> m_pIntegrateFIFO;						
	TSharedPtr<FGsSpherePack> m_pRecomputeFIFO;

	int						m_nMaxSphere;
	float					m_fMaxRootSize;					
	float					m_fMaxLeafSize;					
	float					m_fSuperSphereGravy;			

	int						m_nDebugMode;

public:
	void Init(const FVector& levelCenter,float levelRaius, int maxspheres, float rootsize, float leafsize, float gravy);
	void Process();
	void Reset();

public:
	void SpawnDebug(int inMode);

public:
	FGsSphereNode* AddSphere(const FVector& pos, float radius, int flags = SPF_LEAF_TREE,
		FGsSphereNode* linkdata = nullptr, void* userdata = nullptr);

public:
	void AddIntegrateFIFO(FGsSphereNode* pack);          
	void AddRecomputeFIFO(FGsSphereNode* recompute);     

	void Integrate(FGsSphereNode* pack, FGsSphereNode* supersphere, float node_size);
	void Remove(FGsSphereNode* pack);

public:
	void RayTrace(const FVector& source, const FVector& dest, TWeakPtr<FGsSphereNodeCallback> callback);
	bool RangeTest(const FVector& center, float radius, TWeakPtr<FGsSphereNodeCallback> callback);

protected:
	virtual void RayTraceCallback(const FVector& rayPos, const FVector& rayDir, float rayDistance, const FGsSphereNode* InterSectsphere);
	virtual void RangeTestCallback(const FVector& pos, float distance, const FGsSphereNode* sphere, GsViewState state);
};