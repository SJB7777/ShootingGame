#pragma once
#include <vector>
#include <algorithm>
#include "FlatVector.h"
#include "Collisions.h"
#include "FlatManifold.h"
#include "FlatBody.h"

namespace FlatPhysics {
	
	static int MinIterations() { return 1; }
	static int MaxIterations() { return 128; }


	class FlatWorld {

	private:
		
		
		

		FlatVector gravity;
		std::vector<std::pair<int, int>> contactPairs;

		FlatVector contactArray[2];
		FlatVector impulseArray[2];
		FlatVector raArray[2];
		FlatVector rbArray[2];
		FlatVector frictionImpulseArray[2];
		float jArray[2];
	public:
		
		std::vector<FlatBody*> bodyVector;
		std::vector<MultiBody*> multiVector;
		FlatWorld(const FlatVector& gravity);
		~FlatWorld();

		/*int BodyCount();

		void AddBody(FlatBody* body);

		void RemoveBody(FlatBody* body);

		bool GetBody(int index, FlatBody* &body);*/

		int BodyCount();

		void AddBody(MultiBody* body);

		void RemoveBody(MultiBody* body);

		bool GetBody(int index, MultiBody*& body);

		void Step(float time, int iterations);
		void TestStep(float time, int iterations);
	private:
		void BroadPhase();
		void NarrowPhase();
		void StepBodies(float time, int totalIterations);

		void SeperateBodies(MultiBody* bodyA, MultiBody* bodyB, const FlatVector& mtv);
	public:
		void ResolveCollisionBasic(const FlatManifold& contact);

		void ResolveCollisionWithRotation(const FlatManifold& contact);

		void ResolveCollisionWithRotationAndFriction(const FlatManifold& contact);
	};
}
