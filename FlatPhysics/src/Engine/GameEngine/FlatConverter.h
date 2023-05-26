#pragma once
#include "../PhysicsEngine.h"
#include "raylib.h"

using namespace FlatPhysics;

class FlatConverter {

public:
	static float scale() { return 1.0f; }

	static Vector2 ToVector2(const FlatVector& v) {
		return Vector2{ v.x, v.y };
	}

	static FlatVector ToFlatVector(const Vector2& v) {
		return FlatVector{ v.x, v.y };
	}

	static void ToVector2Array(const std::vector<FlatVector>& src, std::vector<Vector2>& dst)
	{
		if (dst.empty() || src.size() != dst.size())
		{
			dst.resize(src.size());
		}

		for (int i = 0; i < src.size(); i++)
		{
			
			dst[i] = ToVector2(src[i]);
		}
	}

	
};


class GameDraw {
public:
	static void DrawPolygonLines(const std::vector<Vector2>& points, float thick, const Color& color)
	{

		for (int i = 0; i < points.size(); i++)
		{
			DrawLineEx(points[i], points[(i + 1) % points.size()], thick, color);
		}

	}
	static void DrawPolygonFill(const std::vector<Vector2>& points, const std::vector<int>& triangleIndices, const Color& color)
	{
		if (points.size() < 3)
		{
			throw "Need 3 or more vertices.";
		}

		for (int i = 0; i < triangleIndices.size(); i += 3)
		{
			DrawTriangle(points[triangleIndices[i]], points[triangleIndices[i + 1]], points[triangleIndices[i + 2]], color);
		}
	}
	static void DrawLine(const FlatVector& startPos, const FlatVector& endPos, float thick, const Color& color)
	{
		DrawLineEx({ startPos.x, startPos.y }, { endPos.x, endPos.y }, thick, color);
	}
};