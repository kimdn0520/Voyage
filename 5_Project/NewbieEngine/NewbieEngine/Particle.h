#pragma once
#include "NewbieEngineDLL.h"
#include "AlignedAllocationPolicy.h"
#include "Component.h"
#include "ObjectInfo.h"

class Transform;
class ParticleInfo; 

/// <summary>
/// 파티클 컴포넌트를 만든다
/// 파티클 정보를 저장하고 넣어주고 만들어준다.
/// </summary>
class Particle : public Component, public AlignedAllocationPolicy<16>
{
public:
	NewbieEngine_DLL Particle(shared_ptr<GameObject> gameObject);
	virtual ~Particle();

private:

	shared_ptr<ParticleInfo> _particleInfo;

	shared_ptr<Transform> _transform;

	//vector<shared_ptr<MeshInfo>> _meshInfo;

	int _materialID;

	vector<int> _deleteIdx;
public:
	//shared_ptr<MeshInfo> GetMeshInfo() { return _meshInfo; }

	//NewbieEngine_DLL void SetMesh(size_t mesh) { _meshInfo->name = mesh; }

	NewbieEngine_DLL int GetMaterialID() { return _materialID; }

	NewbieEngine_DLL void PushMaterial();

	//// 방출 될 때 파티클이 위치 할 수 있는 임의의 편차를 설정
	//NewbieEngine_DLL void SetParticleDeviation(float deviationX, float deviationY, float deviationZ);
	//// 파티클의 속도와 속도 변화를 설정
	//NewbieEngine_DLL void SetParticleVelocity(float velocity, float velocityvariation);
	//// 파티클 물리적인 크기를 설정
	//NewbieEngine_DLL void SetParticleSize(float size);
	//// 초당 방출 할 파티클 수를 설정
	//NewbieEngine_DLL void SetParticlePerSecond(float persecond);
	//// 파티클 시스템에 허용되는 최대 파티클 수를 설정
	//NewbieEngine_DLL void SetParticleMaxCount(float maxcount);
	
	NewbieEngine_DLL void SetParticleInfo(int materialId/*shared_ptr<ParticleInfo> particleinfo*/);

	void KillParticles();
	
	void EmitParticles(float frameTime);

	void SortParticles();

	void UpdateParticles(float frameTime);

	void ParticleRender();

	void FixedUpdate() override;

	void Render() override;
};

