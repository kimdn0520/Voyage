#pragma once

class ColliderBase;
	
class PhysicsManager
{
public:
	PhysicsManager() {};
	~PhysicsManager() {};

private:
	static shared_ptr<PhysicsManager> physicsManager;

public:
	static shared_ptr<PhysicsManager> GetInstance();

private:

	
public:
	void ColStateUpdate(std::vector<std::shared_ptr<ColliderBase>>& colliders);

	void CheckCollision(std::vector<std::shared_ptr<ColliderBase>>& colliders);

	void CollisionEvent(std::vector<std::shared_ptr<ColliderBase>>& colliders);
	
	void Update(std::vector<std::shared_ptr<ColliderBase>>& colliders);
};

