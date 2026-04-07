#ifndef PHYSICS_HDR
#define PHYSICS_HDR

#include "Foundation/Platform.hpp"

#include <Jolt/Jolt.h>

// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

// Disable common warnings triggered by Jolt, you can use JPH_SUPPRESS_WARNING_PUSH / JPH_SUPPRESS_WARNING_POP to store and restore the warning state
JPH_SUPPRESS_WARNINGS

struct Physics
{
    // The main way to interact with the bodies in the physics system is through the body interface. There is a locking and a non-locking
    // variant of this. We're going to use the locking version (even though we're not planning to access bodies from multiple threads)
    JPH::PhysicsSystem physicsSystem;
    JPH::BodyID sphereID;
    // We need a job system that will execute physics jobs on multiple threads. Typically
    // you would implement the JobSystem interface yourself and let Jolt Physics run on top
    // of your own job scheduler. JobSystemThreadPool is an example implementation.
    JPH::JobSystemThreadPool jobSystem;
    JPH::Body* floor;
    JPH::BodyInterface* bodyInterface;

    // We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
    static constexpr float cDeltaTime = 1.0f / 60.0f;

	void initPhysics();
	void updatePhysics();
	void shutdownPhysics();
};

#endif // !PHYSICS_HDR
