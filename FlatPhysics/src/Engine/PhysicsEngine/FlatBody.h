#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "FlatTransform.h"
#include "FlatVector.h"
#include "FlatMath.h"
#include "FlatConstants.h"
#include "FlatAABB.h"
#include "PolygonHelper.h"


namespace FlatPhysics {
	
	

	enum class ShapeType {
		Circle = 0,
		Box = 1,
		
	};


	class FlatBody {
	
	protected:
		
		FlatVector position;
		float angle;
		FlatVector force;
		
		
		
		std::vector<FlatVector> transformedVertices;
		FlatAABB aabb;

		bool transformUpdateRequired;
		bool aabbUpdateRequired;

	public:
		std::vector<FlatVector> vertices;

		ShapeType shapeType;
		float Density;
		float Mass;
		float InvMass;
		float Restitution;
		float Area;
		float Inertia;
		float InvInertia;
		bool IsStatic;
		float Radius;
		float Width;
		float Height;
		float StaticFriction;
		float DynamicFriction;

		FlatVector LinearVelocity;
		float AngularVelocity;

		FlatVector GetPosition() const;
		float GetAngle() const;
		
		FlatBody();

		FlatBody(float density, float mass, float inertia, float restitution, float area,
			bool isStatic, float radius, float width, float height,
			const std::vector<FlatVector>& vertices, ShapeType shapeType);
	protected:

		
		static std::vector<FlatVector> CreateBoxVertices(float width, float height);

	public:
		std::vector<FlatVector> GetTransformedVertices();

		FlatAABB GetAABB();

		void Step(float time, const FlatVector& gravity, int iterations);

		void Move(const FlatVector& amount);

		void MoveTo(const FlatVector& position);
		
		void Rotate(float amount);

		void RotateTo(float amount);

		void AddForce(const FlatVector& amount);

		static bool CreateCircleBody(float radius, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage);

		static bool CreateBoxBody(float width, float height, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage);

		static bool CreatePolygonBody(std::vector<FlatVector>& vertices, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage);

	};

	class MultiBody
	{
	public:
		FlatVector GetPosition() const { return position; }
		FlatVector position;
		float angle;

		std::vector<FlatBody*> subBodies;
		
		MultiBody() {};
		MultiBody(const std::vector<FlatBody*>& subBodies, float density,
			float mass, float inertia, float restitution, float area,
			bool isStatic, const std::vector<FlatVector>& vertices) : 
			subBodies(subBodies), position(FlatVector::Zero()), LinearVelocity(FlatVector::Zero()), angle(0.0f), AngularVelocity(0.0f),
			Density(density), Mass(mass), InvMass(Mass > 0.0f ? 1.0f / Mass : 0.0f),
			Inertia(inertia), InvInertia(Inertia > 0.0f ? 1.0f / Inertia : 0.0f), Restitution(restitution), Area(area),
			IsStatic(isStatic), transformUpdateRequired(true), aabbUpdateRequired(true), StaticFriction(0.6f), DynamicFriction(0.4f)
		{}

		static bool CreateConcaveBody(std::vector<FlatVector>& vertices, float density, bool isStatic, float restitution, MultiBody& multiBody, std::string& errorMessage);
		static void CreateSingleBody(FlatBody& body, MultiBody& multiBody);
	public:

		
		
		FlatVector force;

		std::vector<FlatVector> vertices;

		std::vector<FlatVector> transformedVertices;
		FlatAABB AABB;

		bool transformUpdateRequired;
		bool aabbUpdateRequired;

	


		
		float Density;
		float Mass;
		float InvMass;
		float Restitution;
		float Area;
		float Inertia;
		float InvInertia;
		bool IsStatic;
		
		float StaticFriction;
		float DynamicFriction;

		FlatVector LinearVelocity;
		float AngularVelocity;


	public:
		
		
		FlatAABB GetAABB();

		void Step(float time, const FlatVector& gravity, int iterations);

		void Move(const FlatVector& amount);

		void MoveTo(const FlatVector& position);

		void Rotate(float amount);

		void RotateTo(float amount);

		void AddForce(const FlatVector& amount);
	};
}