#include "GsSphere.h"

#include "Intersection/IntersectionUtil.h"


bool FGsSphere::RayIntersection(const FVector& rayOrigin, const FVector& rayDirection)
{
	FLinearIntersection ResultOut;
	return IntersectionUtil::LineSphereIntersection<float>(rayOrigin, rayDirection, Center, W, ResultOut);
}


bool FGsSphere::RayIntersectionInFront(const FVector& rayOrigin, const FVector& rayDirection)
{
	return IntersectionUtil::LineSphereTest<float>(rayOrigin, rayDirection, Center, W);
}


#define BIGNUMBER 100000000.0f  		/* hundred million */
void FGsSphere::Compute(const FGsSphereInterface& source)
{
	FVector xmin, xmax, ymin, ymax, zmin, zmax, dia1, dia2;

	/* FIRST PASS: find 6 minima/maxima points */
	xmin.Set(BIGNUMBER, BIGNUMBER, BIGNUMBER);
	xmax.Set(-BIGNUMBER, -BIGNUMBER, -BIGNUMBER);
	ymin.Set(BIGNUMBER, BIGNUMBER, BIGNUMBER);
	ymax.Set(-BIGNUMBER, -BIGNUMBER, -BIGNUMBER);
	zmin.Set(BIGNUMBER, BIGNUMBER, BIGNUMBER);
	zmax.Set(-BIGNUMBER, -BIGNUMBER, -BIGNUMBER);


	// get min , max Vertex;
	FVector vertex;
	int count = source.GetVertexCount();
	for (int i = 0; i < count; i++)
	{
		source.GetVertex(i, vertex);
		if (vertex.X < xmin.X) 	xmin = vertex; /* New xminimum point */
		if (vertex.X > xmax.X)	xmax = vertex;
		if (vertex.Y < ymin.Y)	ymin = vertex;
		if (vertex.Y > ymax.Y)	ymax = vertex;
		if (vertex.Z < zmin.Z)	zmin = vertex;
		if (vertex.Z > zmax.Z)	zmax = vertex;
	}
	//각 축의 길이를 얻는다.
	/* Set xspan = distance between the 2 points xmin & xmax (squared) */
	float dx = xmax.X - xmin.X;
	float dy = xmax.Y - xmin.Y;
	float dz = xmax.Z - xmin.Z;
	float xspan = dx * dx + dy * dy + dz * dz;

	/* Same for y & z spans */
	dx = ymax.X - ymin.X;
	dy = ymax.Y - ymin.Y;
	dz = ymax.Z - ymin.Z;
	float yspan = dx * dx + dy * dy + dz * dz;

	dx = zmax.X - zmin.X;
	dy = zmax.Y - zmin.Y;
	dz = zmax.Z - zmin.Z;
	float zspan = dx * dx + dy * dy + dz * dz;

	/* Set points dia1 & dia2 to the maximally separated pair */
	dia1 = xmin;
	dia2 = xmax; /* assume xspan biggest */
	float maxspan = xspan;

	if (yspan > maxspan)
	{
		maxspan = yspan;
		dia1 = ymin;
		dia2 = ymax;
	}

	if (zspan > maxspan)
	{
		dia1 = zmin;
		dia2 = zmax;
	}


	/* dia1,dia2 is a diameter of initial sphere */
	/* calc initial center */
	Center.X = ((dia1.X + dia2.X) * 0.5f);
	Center.Y = ((dia1.Y + dia2.Y) * 0.5f);
	Center.Z = ((dia1.Z + dia2.Z) * 0.5f);
	/* calculate initial radius**2 and radius */
	dx = dia2.X - Center.X; /* x component of radius vector */
	dy = dia2.Y - Center.Y; /* y component of radius vector */
	dz = dia2.Z - Center.Z; /* z component of radius vector */
	float fRadius2 = dx * dx + dy * dy + dz * dz;
	W = float(sqrt(fRadius2));

	/* SECOND PASS: increment current sphere */

	for (int i = 0; i < count; i++)
	{
		FVector caller_p;
		source.GetVertex(i, caller_p);
		dx = caller_p.X - Center.X;
		dy = caller_p.Y - Center.Y;
		dz = caller_p.Z - Center.Z;
		float old_to_p_sq = dx * dx + dy * dy + dz * dz;
		if (old_to_p_sq > fRadius2) 	/* do r**2 test first */
		{ 	/* this point is outside of current sphere */
			float old_to_p = float(sqrt(old_to_p_sq));
			/* calc radius of new sphere */
			W = (W + old_to_p) * 0.5f;
			fRadius2 = W * W; 	/* for next r**2 compare */
			float old_to_new = old_to_p - W;
			/* calc center of new sphere */
			float recip = 1.0f / old_to_p;

			float cx = (W * Center.X + old_to_new * caller_p.X) * recip;
			float cy = (W * Center.Y + old_to_new * caller_p.Y) * recip;
			float cz = (W * Center.Z + old_to_new * caller_p.Z) * recip;

			Center.Set(cx, cy, cz);
		}
	}
}
