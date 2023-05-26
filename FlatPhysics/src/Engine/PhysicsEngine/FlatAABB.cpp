#include "FlatAABB.h"

namespace FlatPhysics
{
	FlatAABB::FlatAABB(const FlatVector& min, const FlatVector& max) :
		Min(min), Max(max)
	{}

	FlatAABB::FlatAABB(float minX, float minY, float maxX, float maxY) :
		Min({minX, minY}), Max({maxX, maxY})
	{}


    FlatAABB FlatAABB::Merge(const FlatAABB& other) const{
        return {
            {std::min(Min.x, other.Min.x), std::min(Min.y, other.Min.y)},
            {std::max(Max.x, other.Max.x), std::max(Max.y, other.Max.y) } 
        };
        
    }

    // Calculate the center of the AABB
    FlatVector FlatAABB::center() const {
        return (Min + Max) * 0.5f;
    }

    // Calculate the extents (half the width and height) of the AABB
    FlatVector FlatAABB::extents() const {
        return (Max - Min) * 0.5f;
    }

    // Check if this AABB overlaps with another AABB
    bool FlatAABB::overlaps(const FlatAABB& other) const {
        if (Max.x < other.Min.x || Min.x > other.Max.x) return false;
        if (Max.y < other.Min.y || Min.y > other.Max.y) return false;
        return true;
    }

    FlatVector FlatAABB::GetSize() const {
        return FlatVector(Max.x - Min.x, Max.y - Min.y);
    }

    FlatAABB& FlatAABB::operator=(const FlatAABB& other) {
        if (this != &other) {
            Min = other.Min;
            Max = other.Max;
        }
        return *this;
    }

}