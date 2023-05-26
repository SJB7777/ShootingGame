#include "FlatBody.h"

namespace FlatPhysics
{
	FlatVector FlatBody::GetPosition() const 
	{
		
		return position;
	}

	float FlatBody::GetAngle() const
	{
		return angle;
	}

	
	FlatBody::FlatBody() {}

	FlatBody::FlatBody(float density, float mass, float inertia, float restitution, float area,
		bool isStatic, float radius, float width, float height,
		const std::vector<FlatVector>& vertices, ShapeType shapeType) :
		position(FlatVector::Zero()), LinearVelocity(FlatVector::Zero()), angle(0.0f), AngularVelocity(0.0f),
		Density(density), Mass(mass), InvMass(mass > 0.0f ? 1.0f / mass : 0.0f),
		Inertia(inertia), InvInertia(inertia > 0.0f ? 1.0f / inertia : 0.0f), Restitution(restitution), Area(area),
		IsStatic(isStatic), Radius(radius), Width(width), Height(height), shapeType(shapeType),
		transformUpdateRequired(true), aabbUpdateRequired(true), StaticFriction(0.6f), DynamicFriction(0.4f)
	{
		

		if (shapeType == ShapeType::Box)
		{
			this->vertices = vertices;
			transformedVertices.resize(vertices.size());
		}
		else
		{
			this->vertices.clear();
			transformedVertices.clear();
		}

		
	}


	


	std::vector<FlatVector> FlatBody::CreateBoxVertices(float width, float height)
	{
		
		
		float left = -width / 2.0f;
		float right = left + width;
		float bottom = -height / 2.0f;
		float top = bottom + height;

		std::vector<FlatVector> vertices = {
			{ left, top },
			{ right, top },
			{ right, bottom },
			{ left, bottom }
		};

		return vertices;
	}


	std::vector<FlatVector> FlatBody::GetTransformedVertices()
	{

		if (transformUpdateRequired)
		{
			FlatTransform transform(position, angle);
			for (int i = 0; i < vertices.size(); i++)
			{
				FlatVector& v = vertices[i];
				transformedVertices[i] = FlatVector::Transform(v, transform);
			}
		}

		transformUpdateRequired = false;
		return transformedVertices;
	}

	FlatAABB FlatBody::GetAABB()
	{
		if(aabbUpdateRequired)
		{
			
			float minX = std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::lowest();
			float maxY = std::numeric_limits<float>::lowest();
			if (shapeType == ShapeType::Box)
			{
				std::vector<FlatVector> vertices = GetTransformedVertices();

				for (auto& v: vertices)
				{
					
					if (v.x < minX) { minX = v.x; }
					if (v.x > maxX) { maxX = v.x; }
					if (v.y < minY) { minY = v.y; }
					if (v.y > maxY) { maxY = v.y; }
				}
			}
			else if (shapeType == ShapeType::Circle)
			{
				minX = position.x - Radius;
				minY = position.y - Radius;
				maxX = position.x + Radius;
				maxY = position.y + Radius;
			}
			else
			{
				throw std::invalid_argument("Unknown ShapeType.");
			}

			aabb = FlatAABB( minX, minY, maxX, maxY );
		}
		aabbUpdateRequired = false;
		return aabb;
	}

	void FlatBody::Step(float time, const FlatVector& gravity, int iterations)
	{
		
		if (IsStatic)
		{
			return;
		}


		time /= (float)iterations;
		FlatVector acceration = gravity + force * InvMass;
		LinearVelocity += acceration * time;
		
		position += LinearVelocity * time;
		

		angle += AngularVelocity * time;
		
		force = FlatVector::Zero();

		transformUpdateRequired = true;
		aabbUpdateRequired = true;
		
	}

	void FlatBody::Move(const FlatVector& amount)
	{
		if (IsStatic)
		{
			return;
		}
		position += amount;
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void FlatBody::MoveTo(const FlatVector& position)
	{
		this->position = position;
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void FlatBody::Rotate(float amount)
	{
		if (IsStatic)
		{
			return;
		}
		angle += amount;
		if (angle > 2 * PI)
		{
			angle -= 2 * PI;
		}
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void FlatBody::RotateTo(float amount)
	{
		angle = amount;
		
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void FlatBody::AddForce(const FlatVector& amount)
	{
		force = amount;
	}

	bool FlatBody::CreateCircleBody(float radius, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage) {

		errorMessage = { 0 };
		
		float area = radius * radius * PI;

		if (area < FlatConstants::MinBodySize) {
			errorMessage = "Circle radius is too small. Min circle area is " + std::to_string(FlatConstants::MinBodySize) + ".";
			return false;
		}

		if (area > FlatConstants::MaxBodySize) {
			errorMessage = "Circle radius is too large. Max circle area is " + std::to_string(FlatConstants::MaxBodySize) + ".";
			return false;
		}

		if (density < FlatConstants::MinDensity) {
			errorMessage = "Density radius is too small. Min density area is " + std::to_string(FlatConstants::MinDensity) + ".";
			return false;
		}

		if (density > FlatConstants::MaxDensity) {
			errorMessage = "Density radius is too large. Min density area is " + std::to_string(FlatConstants::MaxDensity) + ".";
			return false;
		}

		restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);

		float mass = 0.0f;
		float inertia = 0.0f;

		if (!isStatic)
		{
			mass = area * density;
			inertia = (1.0f / 2.0f) * mass * (radius * radius);
		}
		
		body = FlatBody(density, mass, inertia, restitution, area, isStatic, radius, 0.0f, 0.0f, std::vector<FlatVector>(), ShapeType::Circle);
		return true;
	}

	bool FlatBody::CreateBoxBody(float width, float height, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage) {

		errorMessage = { 0 };

		float area = width * height;

		if (area < FlatConstants::MinBodySize) {
			errorMessage = "Area is too small. Min area is " + std::to_string(FlatConstants::MinBodySize) + ".";
			return false;
		}

		if (area > FlatConstants::MaxBodySize) {
			errorMessage = "Area is too large. Max area area is " + std::to_string(FlatConstants::MaxBodySize) + ".";
			return false;
		}

		if (density < FlatConstants::MinDensity) {
			errorMessage = "Density radius is too small. Min density area is " + std::to_string(FlatConstants::MinDensity) + ".";
			return false;
		}

		if (density > FlatConstants::MaxDensity) {
			errorMessage = "Density radius is too large. Min density area is " + std::to_string(FlatConstants::MaxDensity) + ".";
			return false;
		}

		restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);

		float mass = 0.0f;
		float inertia = 0.0f;

		if (!isStatic)
		{
			mass = 0.001f * area * density;
			inertia = (1.0f / 12.0f) * mass * (width * width + height * height);
		}
		
		std::vector<FlatVector> vertices = CreateBoxVertices(width, height);
		body = FlatBody(density, mass, inertia, restitution, area, isStatic, 0.0f, width, height, vertices, ShapeType::Box);
		return true;
	}

	bool FlatBody::CreatePolygonBody(std::vector<FlatVector>& vertices, float density, bool isStatic, float restitution, FlatBody& body, std::string& errorMessage)
	{
		errorMessage = { 0 };

		restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);

		float area = 0.0f;
		
		FlatVector c = FlatVector::Zero(); // centroid
			
		float inertia = 0.0f;
		const float inv3 = 1.0f / 3.0f;

		for (int i = 0; i < vertices.size(); ++i)
		{
			FlatVector p1 = vertices[i];
			FlatVector p2 = vertices[(i + 1) % vertices.size()];

			float D = FlatMath::Cross(p2, p1);
			float triangleArea = 0.5f * D;

			area += triangleArea;

			// Use area to weight the centroid average, not just vertex position
			c += triangleArea * inv3 * (p1 + p2);

			float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
			float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;

			inertia += (0.25f * inv3 * D) * (intx2 + inty2);
		}

		if (area < 0.0f)
		{
			area = -area;
			inertia = -inertia;
		}
		

		if (area < FlatConstants::MinBodySize) {
			errorMessage = "Area is too small. Min area is " + std::to_string(FlatConstants::MinBodySize) + ".";
			return false;
		}

		if (area > FlatConstants::MaxBodySize) {
			errorMessage = "Area is too large. Max area area is " + std::to_string(FlatConstants::MaxBodySize) + ".";
			return false;
		}

		if (density < FlatConstants::MinDensity) {
			errorMessage = "Density radius is too small. Min density area is " + std::to_string(FlatConstants::MinDensity) + ".";
			return false;
		}

		if (density > FlatConstants::MaxDensity) {
			errorMessage = "Density radius is too large. Min density area is " + std::to_string(FlatConstants::MaxDensity) + ".";
			return false;
		}

		c *= 1.0f / area;

		//// Translate vertices to centroid (make the centroid (0, 0))
		//for (int i = 0; i < vertices.size(); ++i)
		//{
		//	vertices[i] -= c;
		//}

		float mass = density * area;
		
		if (isStatic)
		{
			mass = 0.0f;
			inertia = 0.0f;
		}
		

		
		body = FlatBody(density, mass, inertia, restitution, area, isStatic, 0.0f, 0.0f, 0.0f, vertices, ShapeType::Box);
		return true;
	}

	
	
	bool MultiBody::CreateConcaveBody(std::vector<FlatVector>& vertices, float density, bool isStatic, float restitution, MultiBody& multiBody, std::string& errorMessage)
	{
		errorMessage = { 0 };

		restitution = FlatMath::Clamp(restitution, 0.0f, 1.0f);

		float area = 0.0f;

		FlatVector c = FlatVector::Zero(); // centroid

		float inertia = 0.0f;
		const float inv3 = 1.0f / 3.0f;

		for (int i = 0; i < vertices.size(); ++i)
		{
			FlatVector p1 = vertices[i];
			FlatVector p2 = vertices[(i + 1) % vertices.size()];

			float D = FlatMath::Cross(p2, p1);
			float triangleArea = 0.5f * D;

			area += triangleArea;

			// Use area to weight the centroid average, not just vertex position
			c += triangleArea * inv3 * (p1 + p2);

			float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
			float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;

			inertia += (0.25f * inv3 * D) * (intx2 + inty2);
		}

		if (area < 0.0f)
		{
			area = -area;
			inertia = -inertia;
		}


		if (area < FlatConstants::MinBodySize) {
			errorMessage = "Area is too small. Min area is " + std::to_string(FlatConstants::MinBodySize) + ".";
			return false;
		}

		if (area > FlatConstants::MaxBodySize) {
			errorMessage = "Area is too large. Max area area is " + std::to_string(FlatConstants::MaxBodySize) + ".";
			return false;
		}

		if (density < FlatConstants::MinDensity) {
			errorMessage = "Density radius is too small. Min density area is " + std::to_string(FlatConstants::MinDensity) + ".";
			return false;
		}

		if (density > FlatConstants::MaxDensity) {
			errorMessage = "Density radius is too large. Min density area is " + std::to_string(FlatConstants::MaxDensity) + ".";
			return false;
		}

		c *= 1.0f / area;

		// Translate vertices to centroid (make the centroid (0, 0))
		for (int i = 0; i < vertices.size(); ++i)
		{
			vertices[i] -= c;
		}

		float mass = density * area;

		if (isStatic)
		{
			mass = 0.0f;
			inertia = 0.0f;
		}

		std::vector<int> triangles;
		
		PolygonHelper::Triangulate(vertices, triangles, errorMessage);
		std::vector<FlatBody*> subBodies;
		for (int i = 0; i < triangles.size(); i += 3)
		{
			std::vector<FlatVector> points;
			points.emplace_back(vertices[triangles[i]]);
			points.emplace_back(vertices[triangles[i + 1]]);
			points.emplace_back(vertices[triangles[i + 2]]);
			FlatBody* body = new FlatBody();
			FlatBody::CreatePolygonBody(points, density, isStatic, restitution, *body, errorMessage);
			subBodies.push_back(body);
		}

		multiBody = MultiBody(subBodies, density, mass, inertia, restitution, area, isStatic, vertices);
		return true;
	}

	void MultiBody::CreateSingleBody(FlatBody& body, MultiBody& multibody)
	{
		std::vector<FlatBody*> subBodies = { &body };
		multibody = MultiBody(subBodies, body.Density, body.Mass, body.Inertia, body.Restitution, body.Area, body.IsStatic, body.vertices);
	}

	FlatAABB MultiBody::GetAABB()
	{
		if (aabbUpdateRequired)
		{

			float minX = std::numeric_limits<float>::max();
			float minY = std::numeric_limits<float>::max();
			float maxX = std::numeric_limits<float>::lowest();
			float maxY = std::numeric_limits<float>::lowest();
			
			

			for (auto& body : subBodies)
			{
				FlatAABB aabb = body->GetAABB();
				if (aabb.Min.x < minX) { minX = aabb.Min.x; }
				if (aabb.Max.x > maxX) { maxX = aabb.Max.x; }
				if (aabb.Min.y < minY) { minY = aabb.Min.y; }
				if (aabb.Max.y > maxY) { maxY = aabb.Max.y; }
			}
			
			
			AABB = FlatAABB(minX, minY, maxX, maxY);
		}
		aabbUpdateRequired = false;
		return AABB;
	}
	void MultiBody::Step(float time, const FlatVector& gravity, int iterations)
	{
		if (IsStatic)
		{
			return;
		}


		time /= (float)iterations;
		FlatVector acceration = gravity + force * InvMass;
		
		LinearVelocity += acceration * time;
		
		position += LinearVelocity * time;


		angle += AngularVelocity * time;

		force = FlatVector::Zero();

		transformUpdateRequired = true;
		aabbUpdateRequired = true;

		for (auto& body : subBodies)
		{
			body->LinearVelocity = LinearVelocity;
			body->MoveTo(position);
			body->AngularVelocity = AngularVelocity;
			body->RotateTo(angle);
		}
	}

	void MultiBody::Move(const FlatVector& amount)
	{
		if (IsStatic)
		{
			return;
		}
		position += amount;
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void MultiBody::MoveTo(const FlatVector& position)
	{
		for (auto& body : subBodies)
		{
			body->MoveTo(position);
		}
		this->position = position;
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void MultiBody::Rotate(float amount)
	{
		if (IsStatic)
		{
			return;
		}
		angle += amount;
		if (angle > 2 * PI)
		{
			angle -= 2 * PI;
		}
		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void MultiBody::RotateTo(float amount)
	{
		for (auto& body : subBodies)
		{
			body->RotateTo(amount);
		}
		angle = amount;

		transformUpdateRequired = true;
		aabbUpdateRequired = true;
	}

	void MultiBody::AddForce(const FlatVector& amount)
	{
		force = amount;
	}
	

}