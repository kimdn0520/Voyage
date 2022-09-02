#pragma once
#include "NewbieEngineDLL.h"
#include "AlignedAllocationPolicy.h"
#include "Component.h"
#include "ObjectInfo.h"

class Transform;
class ParticleInfo; 

/// <summary>
/// ��ƼŬ ������Ʈ�� �����
/// ��ƼŬ ������ �����ϰ� �־��ְ� ������ش�.
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

	//// ���� �� �� ��ƼŬ�� ��ġ �� �� �ִ� ������ ������ ����
	//NewbieEngine_DLL void SetParticleDeviation(float deviationX, float deviationY, float deviationZ);
	//// ��ƼŬ�� �ӵ��� �ӵ� ��ȭ�� ����
	//NewbieEngine_DLL void SetParticleVelocity(float velocity, float velocityvariation);
	//// ��ƼŬ �������� ũ�⸦ ����
	//NewbieEngine_DLL void SetParticleSize(float size);
	//// �ʴ� ���� �� ��ƼŬ ���� ����
	//NewbieEngine_DLL void SetParticlePerSecond(float persecond);
	//// ��ƼŬ �ý��ۿ� ���Ǵ� �ִ� ��ƼŬ ���� ����
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

