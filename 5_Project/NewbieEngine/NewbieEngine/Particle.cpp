#include "pch.h"
#include "Particle.h"
#include "GraphicsEngineManager.h"
#include "ParticleInfo.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "Resources.h"
#include "Transform.h"

Particle::Particle(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::PARTICLE),
	_particleInfo(make_shared<ParticleInfo>()),
	_transform(gameObject->GetComponent<Transform>())
	//_meshInfo(make_shared<MeshInfo>())
{
}

Particle::~Particle()
{

}

void Particle::PushMaterial()
{
	shared_ptr<MaterialInfo> material = make_shared<MaterialInfo>();
}

void Particle::SetParticleInfo(int materialId/*shared_ptr<ParticleInfo> particleinfo*/)
{
	//_particleInfo = particleinfo;

//	_particleInfo->_particleList.resize(_particleInfo->_maxParticles);

	//for (int i = 0; i < _particleInfo->_maxParticles; i++)
	//{
	//	//std::shared_ptr<ParticleType> particletype=make_shared<ParticleType>();
	//	_particleInfo->_particleList[i]=make_shared<ParticleType>();
	//	_particleInfo->_particleList[i]->isActive = false;
	//}
	_materialID = materialId;
	
	// 일단 여기서 초기화 해준다. 나중에 엔진으로 빼고 되는지 확인을 위해서 그냥 넣어줌.
	// 방출 될 때 파티클이 위치 할 수 있는 임의의 편차를 설정합니다.
	_particleInfo->_particleDeviationX = 0.5f;
	_particleInfo->_particleDeviationY = 2.0f;
	_particleInfo->_particleDeviationZ = 0.1f;

	// 파티클의 속도와 속도 변화를 설정합니다.
	_particleInfo->_particleVelocity = 1.0f;
	_particleInfo->_particleVelocityVariation = 0.2f;

	// 파티클의 물리적 크기를 설정합니다.
	_particleInfo->_particleSize = 0.5f;

	// 초당 방출 할 파티클 수를 설정합니다.
	_particleInfo->_particlesPerSecond = 250.0f;

	// 파티클 시스템에 허용되는 최대 파티클 수를 설정합니다.
	_particleInfo->_maxParticles = 5000;

	// 아직 배출되지 않으므로 현재 파티클 수를 0으로 초기화합니다.
	_particleInfo->_currentParticleCount = 0;

	// 초당 파티클 방출 속도의 초기 누적 시간을 지웁니다.
	_particleInfo->_accumulatedTime = 0.0f;

	_particleInfo->_currentParticleCount++;

	std::shared_ptr<ParticleType> particletype = make_shared<ParticleType>();

	// 이제 임의 화 된 파티클 속성을 생성합니다.
	particletype->positionX = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationX;
	particletype->positionY = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationZ;
	particletype->positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationZ;

	particletype->velocity = _particleInfo->_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleVelocityVariation;

	// 바다니깐 red와 green은 필요 없다. 
	particletype->red =  0.f;
	particletype->green =0.f;
	particletype->blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

	particletype->isActive = true;

	_particleInfo->_particleList.push_back(particletype);

}

void Particle::KillParticles()
{
	// 특정 높이 범위를 벗어난 모든 파티클을 제거합니다.
	//for (int i = 0; i < _particleInfo->_maxParticles; i++)
	//{
	//	if ((_particleInfo->_particleList[i]->isActive == true) && (_particleInfo->_particleList[i]->positionY < -3.0f))
	//	{
	//		_particleInfo->_particleList[i]->isActive = false;
	//		_particleInfo->_currentParticleCount--;

	//		// 이제 모든 살아있는 파티클을 배열위로 이동시켜 파괴된 파티클을 지우고 배열을 올바르게 정렬된 상태로 유지합니다.
	//		for (int j = i; j < _particleInfo->_maxParticles - 1; j++)
	//		{
	//			_particleInfo->_particleList[j]->positionX = _particleInfo->_particleList[j + 1]->positionX;
	//			_particleInfo->_particleList[j]->positionY = _particleInfo->_particleList[j + 1]->positionY;
	//			_particleInfo->_particleList[j]->positionZ = _particleInfo->_particleList[j + 1]->positionZ;
	//			_particleInfo->_particleList[j]->red = _particleInfo->_particleList[j + 1]->red;
	//			_particleInfo->_particleList[j]->green = _particleInfo->_particleList[j + 1]->green;
	//			_particleInfo->_particleList[j]->blue = _particleInfo->_particleList[j + 1]->blue;
	//			_particleInfo->_particleList[j]->velocity = _particleInfo->_particleList[j + 1]->velocity;
	//			_particleInfo->_particleList[j]->isActive = _particleInfo->_particleList[j + 1]->isActive;
	//		}
	//	}
	//}
	for (int i = 0; i < _particleInfo->_particleList.size(); i++)
	{
		if (_particleInfo->_particleList[i]->positionZ < -5.0f)
		{
			// i번째 있는 벡터를 지움
			_particleInfo->_particleList.erase(_particleInfo->_particleList.begin() + i);
			_particleInfo->_currentParticleCount--;
			// 지울 idx를 그래픽스에 보내주기위해서 벡터로 저장한다. 
			_deleteIdx.push_back(i);
		}
	}
}

void Particle::EmitParticles(float frameTime)
{
	_particleInfo->_accumulatedTime += frameTime;

	bool emitParticle = false;

	std::shared_ptr<ParticleType> particletype = make_shared<ParticleType>();

	// 새 파티클을 방출할 시간인지 확인
	if (_particleInfo->_accumulatedTime > (100.0f / _particleInfo->_particlesPerSecond))
	{
		_particleInfo->_accumulatedTime = 0.0f;
		emitParticle = true;
	}

	// 새 파티클을 방출할 시간이고 최대값보다 현재 파티클수가 적다면 방출한다. 
	// 현재 파티클이 최대값을 넘으면 안되기 때문
	if (emitParticle && _particleInfo->_currentParticleCount < (_particleInfo->_maxParticles - 1))
	{
		_particleInfo->_currentParticleCount++;

		// 이제 임의 화 된 파티클 속성을 생성합니다.
		particletype->positionX = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationX;
		particletype->positionY = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationZ;
		particletype->positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationZ;

		particletype->velocity = _particleInfo->_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleVelocityVariation;

		// 바다니깐 red와 green은 필요 없다. 
		particletype->red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		particletype->green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		particletype->blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

		particletype->isActive = true;

		_particleInfo->_particleList.push_back(particletype);

		SortParticles();


		//	// 삽입 할 위치를 알았으므로 인덱스에서 한 위치 씩 배열을 복사하여 새 파티클을 위한 공간을 만들어야합니다.
		//	int i = _particleInfo->_currentParticleCount;
		//	int j = i - 1;

		//	while (i != index)
		//	{
		//		_particleInfo->_particleList[i]->positionX = _particleInfo->_particleList[j]->positionX;
		//		_particleInfo->_particleList[i]->positionY = _particleInfo->_particleList[j]->positionY;
		//		_particleInfo->_particleList[i]->positionZ = _particleInfo->_particleList[j]->positionZ;
		//		_particleInfo->_particleList[i]->red = _particleInfo->_particleList[j]->red;
		//		_particleInfo->_particleList[i]->green = _particleInfo->_particleList[j]->green;
		//		_particleInfo->_particleList[i]->blue = _particleInfo->_particleList[j]->blue;
		//		_particleInfo->_particleList[i]->velocity = _particleInfo->_particleList[j]->velocity;
		//		_particleInfo->_particleList[i]->isActive = _particleInfo->_particleList[j]->isActive;
		//		i--;
		//		j--;
		//	}

		//	// 이제 정확한 깊이 순서로 파티클 배열에 삽입하십시오.
		//	_particleInfo->_particleList[index]->positionX = positionX;
		//	_particleInfo->_particleList[index]->positionY = positionY;
		//	_particleInfo->_particleList[index]->positionZ = positionZ;
		//	_particleInfo->_particleList[index]->red = red;
		//	_particleInfo->_particleList[index]->green = green;
		//	_particleInfo->_particleList[index]->blue = blue;
		//	_particleInfo->_particleList[index]->velocity = velocity;
		//	_particleInfo->_particleList[index]->isActive = true;
	}
}

void Particle::SortParticles()
{

	// 이제 블렌딩을 위해 파티클을 뒤에서 앞으로 렌더링해야하므로 파티클 배열을 정렬해야 합니다.
	// Z 깊이를 사용하여 정렬하므로 목록에서 파티클을 삽입해야 하는 위치를 찾아야합니다.
	// 하지만 우린 탑뷰이므로 Y를 기준으로 정렬해 줍니다. 
	for (int i = 0; i < _particleInfo->_particleList.size(); i++)
	{
		for (int j = 0; j < _particleInfo->_particleList.size(); j++)
		{
			if (_particleInfo->_particleList[i]->positionY < _particleInfo->_particleList[j]->positionY)
			{
				std::shared_ptr<ParticleType> particletype = make_shared<ParticleType>();
				particletype = _particleInfo->_particleList[i];
				_particleInfo->_particleList[i] = _particleInfo->_particleList[j];
				_particleInfo->_particleList[j] = particletype;
			}
		}
	}
}

void Particle::UpdateParticles(float frameTime)
{
	// 각 프레임은 위치, 속도 및 프레임 시간을 사용하여 아래쪽으로 이동하여 모든 파티클을 업데이트합니다.
	for (int i = 0; i < _particleInfo->_currentParticleCount; i++)
	{
		_particleInfo->_particleList[i]->positionZ = _particleInfo->_particleList[i]->positionZ - (_particleInfo->_particleList[i]->velocity * frameTime * 0.001f);
	}
}

void Particle::ParticleRender()
{
	KillParticles();

	EmitParticles(TimeManager::GetInstance()->GetDeltaTime());

	UpdateParticles(TimeManager::GetInstance()->GetDeltaTime());

	Resources::GetInstance()->SetParticleInfo(_particleInfo);

	GraphicsEngineManager::GetInstance()->SetParticleDeletIndex(_deleteIdx);

	Resources::GetInstance()->LoadParticleMesh((int)TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)RasterState::SOLID);
}

void Particle::FixedUpdate()
{

}

void Particle::Render()
{
	ParticleRender();

	shared_ptr<MeshInfo> meshInfo = make_shared<MeshInfo>();

	meshInfo->worldTM = _transform->GetWorldMatrix();
	meshInfo->worldPos = _transform->GetWorldPosition();

	meshInfo->isParticle = true;
	meshInfo->materialID = _materialID;

	// 오브젝트 타입을 파티클로 넣어준다. 
	meshInfo->type = OBJECT_TYPE::PARTICLE;
	meshInfo->name = (int)MeshName::ParticleMesh;
	meshInfo->particleIdx = 0;

	GraphicsEngineManager::GetInstance()->SetObjInfo(meshInfo);

}

