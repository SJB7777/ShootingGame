#pragma once
#include "Common.h"
#include "RandomHelper.h"
#include "FlatConverter.h"
using namespace FlatPhysics;

class FlatEntity
{
private:
	
	MultiBody* multiBody = new MultiBody();
	Color color;

	std::vector<Vector2> vertexBuffer;
public:
	MultiBody* GetBody();
	Color GetColor();
	FlatEntity() {}
	FlatEntity(MultiBody* body);
	FlatEntity(MultiBody* body, const Color& color);
	FlatEntity(FlatWorld& world, float radius, bool isStatic, const Color& color, const FlatVector& position = FlatVector::Zero());
	FlatEntity(FlatWorld& world, float width, float height, bool isStatic, const Color& color, const FlatVector& position = FlatVector::Zero());
	FlatEntity(FlatWorld& world, std::vector<FlatVector>& vertices, bool isStatic, const Color& color, const FlatVector& position = FlatVector::Zero());
	void Draw();

	FlatVector GetPosition();
	
	void MoveTo(const FlatVector& amount);
	void AddForce(const FlatVector& force);
	void SetVelocity(const FlatVector& amount);
	void ToggleIsStatic();

};
