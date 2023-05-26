#pragma once
#include "FlatTransform.h"

namespace FlatPhysics {
	
	struct FlatTransform;

	class FlatVector {
	public:
		float x;
		float y;
		FlatVector& operator= (const FlatVector& other) = default;
		FlatVector() : x(0), y(0) {};
		FlatVector(float x, float y) : x(x), y(y) {}
		FlatVector operator+ (const FlatVector& other) const;
		FlatVector operator- (const FlatVector& other) const;
		FlatVector operator* (const float s) const;
		friend FlatVector operator* (const float s, const FlatVector& v);
		FlatVector operator/ (const float s) const;

		FlatVector& operator+= (const FlatVector& other) {
			(*this) = (*this) + other;
			return (*this);
		}
		FlatVector& operator-= (const FlatVector& other) {
			(*this) = (*this) - other;
			return (*this);
		}
		FlatVector& operator*= (const float s) {
			(*this) = (*this) * s;
			return (*this);
		}
		FlatVector& operator/= (const float s) {
			(*this) = (*this) / s;
			return (*this);
		}
		FlatVector operator-() const {
			return FlatVector(-x, -y);
		}
		
		float Length() const {
			return sqrt(x * x + y + y);
		}

		FlatVector Normalize() const {
			float length = Length();
			return { x / length, y / length };
		}

		bool operator== (const FlatVector& other) const;
		
		static FlatVector Transform(FlatVector v, FlatTransform transform);

		static FlatVector Zero() {
			return { 0.0f, 0.0f };
		}
	};

	

}


