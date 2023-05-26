#pragma once

#include "FlatVector.h"
#include "FlatMath.h"
#include <vector>
#include <string>
#include <algorithm>

namespace FlatPhysics
{
	enum WindingOrder;
	class PolygonHelper
	{
	public:
		static bool Triangulate(const std::vector<FlatVector>& vertices, std::vector<int>& triangles, std::string& errorMessage)
		{
			errorMessage.clear();
			triangles.clear();

			if (vertices.empty())
			{
				errorMessage = "The vertex list is empty.";
				return false;
			}

			if (vertices.size() < 3)
			{
				errorMessage = "The vertex list must have at least 3 vertices.";
				return false;
			}

			if (vertices.size() > 1024)
			{
				errorMessage = "The max vertex list size is 1024.";
				return false;
			}

			/*if (!IsSimplePolygon(vertices))
			{
				errorMessage = "The vertex list does not define a simple polygon.";
				return false;
			}*/

			/*if (ContainsColinearEdges(vertices))
			{
				errorMessage = "The vertex list contains colinear edges.";
				return false;
			}*/
			/*float area;
			WindingOrder windingOrder;
			ComputePolygonArea(vertices, area, windingOrder);

			if (windingOrder == WindingOrder::Invalid)
			{
				errorMessage = "The vertex list does not contain a valid polygon.";
				return false;
			}

			if (windingOrder == WindingOrder::CounterClockwise)
			{
				std::reverse(vertices.begin(), vertices.end());
			}*/

			std::vector<int> indexVector;
			for (int i = 0; i < vertices.size(); i++)
			{
				indexVector.push_back(i);
			}

			int totaltriangleCount = vertices.size() - 2;
			int totaltriangleIndexCount = totaltriangleCount * 3;

			triangles.resize(totaltriangleIndexCount);
			int triangleIndexCount = 0;

			while (indexVector.size() > 3)
			{
				for (int i = 0; i < indexVector.size(); i++)
				{
					int a = indexVector[i];
					int b = FlatMath::GetItem(indexVector, i - 1);
					int c = FlatMath::GetItem(indexVector, i + 1);

					FlatVector va = vertices[a];
					FlatVector vb = vertices[b];
					FlatVector vc = vertices[c];

					FlatVector ab = vb - va;
					FlatVector ac = vc - va;

					// Is ear test vertex convex?
					if (FlatMath::Cross(ab, ac) < 0.0f)
					{
						continue;
					}

					bool isEar = true;

					// Does test ear contain any polygon vertices?
					for (int j = 0; j < vertices.size(); j++)
					{
						if (j == a || j == b || j == c)
						{
							continue;
						}

						FlatVector p = vertices[j];

						if (IsPointInTriangle(p, vb, va, vc))
						{
							isEar = false;
							break;
						}
					}

					if (isEar)
					{
						triangles[triangleIndexCount++] = b;
						triangles[triangleIndexCount++] = a;
						triangles[triangleIndexCount++] = c;

						indexVector.erase(remove(indexVector.begin(), indexVector.end(), i), indexVector.end());
						break;
					}
				}
			}

			triangles[triangleIndexCount++] = indexVector[0];
			triangles[triangleIndexCount++] = indexVector[1];
			triangles[triangleIndexCount++] = indexVector[2];

			return true;
		}

		static bool IsPointInTriangle(FlatVector p, FlatVector a, FlatVector b, FlatVector c)
		{
			FlatVector ab = b - a;
			FlatVector bc = c - b;
			FlatVector ca = a - c;

			FlatVector ap = p - a;
			FlatVector bp = p - b;
			FlatVector cp = p - c;

			float cross1 = FlatMath::Cross(ab, ap);
			float cross2 = FlatMath::Cross(bc, bp);
			float cross3 = FlatMath::Cross(ca, cp);

			if (cross1 > 0.0f || cross2 > 0.0f || cross3 > 0.0f)
			{
				return false;
			}

			return true;
		}

		static bool IsSimplePolygon(const std::vector<FlatVector>& vertices)
		{
			return true;
		}

		static bool ContainsColinearEdges(const std::vector<FlatVector>& vertices)
		{
			return true;
		}

		static void ComputePolygonArea(const std::vector<FlatVector>& vertices, float& area, WindingOrder& windingOrder)
		{

		}
	};
}
