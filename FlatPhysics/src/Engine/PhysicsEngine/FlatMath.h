#pragma once


#include <iostream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include "FlatVector.h"

namespace FlatPhysics {
	class FlatMath
	{
	private:
		
		// Equal to 1/2 of a millimeter.
		
		static float VerySmallAmount() { return 0.0005f; }
	public:
		static int Clamp(int value, int min, int max)
		{
			if (min == max)
			{
				return min;
			}

			if (min > max)
			{
				throw std::out_of_range("min is greater than the max.");
			}
			if (value < min)
			{
				return min;
			}
			if (value > max)
			{
				return max;
			}

			return value;
		}

		static float Clamp(float value, float min, float max)
		{
			if (min == max)
			{
				return min;
			}

			if (min > max)
			{
				throw std::out_of_range("min is greater than the max.");
			}
			if (value < min)
			{
				return min;
			}
			if (value > max)
			{
				return max;
			}

			return value;
		}

		static float LengthSquared(const FlatVector& v) {
			return v.x * v.x + v.y * v.y;
		}

		static float Length(const FlatVector& v) {
			return sqrt(v.x * v.x + v.y * v.y);
		}

		static float DistanceSquared(const FlatVector& a, const FlatVector& b) {
			float dx = a.x - b.x;
			float dy = a.y - b.y;
			return dx * dx + dy * dy;
		}

		static float Distance(const FlatVector& a, const FlatVector& b) {
			float dx = a.x - b.x;
			float dy = a.y - b.y;
			return sqrt(dx * dx + dy * dy);
		}

		static FlatVector Normalize(const FlatVector& v) {
			float len = Length(v);
			
			
			return FlatVector(v.x / len, v.y / len);
		}

		static float Dot(const FlatVector& a, const FlatVector& b) {
			return a.x * b.x + a.y * b.y;
		}

		static float Cross(const FlatVector& a, const FlatVector& b) {
			return a.x * b.y - a.y * b.x;
		}

		static bool NearlyEqual(float a, float b)
		{
			return std::abs(a - b) < VerySmallAmount();
		}

		static bool NearlyEqual(const FlatVector& a, const FlatVector& b)
		{
			return DistanceSquared(a, b) < VerySmallAmount() * VerySmallAmount();
		}
		static FlatVector Perpendicular(const FlatVector& v) {
			
			return FlatVector(-v.y, v.x);
		}

		template <typename T>
		static T GetItem(const std::vector<T>& v, int index)
		{
			if (index >= v.size())
			{
				return v[index % v.size()];
			}
			if (index < 0)
			{
				return v[index % v.size() + v.size()];
			}
			return v[index];
		}
	};
	
}