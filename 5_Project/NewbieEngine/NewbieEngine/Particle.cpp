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
	
	// �ϴ� ���⼭ �ʱ�ȭ ���ش�. ���߿� �������� ���� �Ǵ��� Ȯ���� ���ؼ� �׳� �־���.
	// ���� �� �� ��ƼŬ�� ��ġ �� �� �ִ� ������ ������ �����մϴ�.
	_particleInfo->_particleDeviationX = 0.5f;
	_particleInfo->_particleDeviationY = 2.0f;
	_particleInfo->_particleDeviationZ = 0.1f;

	// ��ƼŬ�� �ӵ��� �ӵ� ��ȭ�� �����մϴ�.
	_particleInfo->_particleVelocity = 1.0f;
	_particleInfo->_particleVelocityVariation = 0.2f;

	// ��ƼŬ�� ������ ũ�⸦ �����մϴ�.
	_particleInfo->_particleSize = 0.5f;

	// �ʴ� ���� �� ��ƼŬ ���� �����մϴ�.
	_particleInfo->_particlesPerSecond = 250.0f;

	// ��ƼŬ �ý��ۿ� ���Ǵ� �ִ� ��ƼŬ ���� �����մϴ�.
	_particleInfo->_maxParticles = 5000;

	// ���� ������� �����Ƿ� ���� ��ƼŬ ���� 0���� �ʱ�ȭ�մϴ�.
	_particleInfo->_currentParticleCount = 0;

	// �ʴ� ��ƼŬ ���� �ӵ��� �ʱ� ���� �ð��� ����ϴ�.
	_particleInfo->_accumulatedTime = 0.0f;

	_particleInfo->_currentParticleCount++;

	std::shared_ptr<ParticleType> particletype = make_shared<ParticleType>();

	// ���� ���� ȭ �� ��ƼŬ �Ӽ��� �����մϴ�.
	particletype->positionX = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationX;
	particletype->positionY = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationZ;
	particletype->positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationZ;

	particletype->velocity = _particleInfo->_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleVelocityVariation;

	// �ٴٴϱ� red�� green�� �ʿ� ����. 
	particletype->red =  0.f;
	particletype->green =0.f;
	particletype->blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

	particletype->isActive = true;

	_particleInfo->_particleList.push_back(particletype);

}

void Particle::KillParticles()
{
	// Ư�� ���� ������ ��� ��� ��ƼŬ�� �����մϴ�.
	//for (int i = 0; i < _particleInfo->_maxParticles; i++)
	//{
	//	if ((_particleInfo->_particleList[i]->isActive == true) && (_particleInfo->_particleList[i]->positionY < -3.0f))
	//	{
	//		_particleInfo->_particleList[i]->isActive = false;
	//		_particleInfo->_currentParticleCount--;

	//		// ���� ��� ����ִ� ��ƼŬ�� �迭���� �̵����� �ı��� ��ƼŬ�� ����� �迭�� �ùٸ��� ���ĵ� ���·� �����մϴ�.
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
			// i��° �ִ� ���͸� ����
			_particleInfo->_particleList.erase(_particleInfo->_particleList.begin() + i);
			_particleInfo->_currentParticleCount--;
			// ���� idx�� �׷��Ƚ��� �����ֱ����ؼ� ���ͷ� �����Ѵ�. 
			_deleteIdx.push_back(i);
		}
	}
}

void Particle::EmitParticles(float frameTime)
{
	_particleInfo->_accumulatedTime += frameTime;

	bool emitParticle = false;

	std::shared_ptr<ParticleType> particletype = make_shared<ParticleType>();

	// �� ��ƼŬ�� ������ �ð����� Ȯ��
	if (_particleInfo->_accumulatedTime > (100.0f / _particleInfo->_particlesPerSecond))
	{
		_particleInfo->_accumulatedTime = 0.0f;
		emitParticle = true;
	}

	// �� ��ƼŬ�� ������ �ð��̰� �ִ밪���� ���� ��ƼŬ���� ���ٸ� �����Ѵ�. 
	// ���� ��ƼŬ�� �ִ밪�� ������ �ȵǱ� ����
	if (emitParticle && _particleInfo->_currentParticleCount < (_particleInfo->_maxParticles - 1))
	{
		_particleInfo->_currentParticleCount++;

		// ���� ���� ȭ �� ��ƼŬ �Ӽ��� �����մϴ�.
		particletype->positionX = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationX;
		particletype->positionY = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationZ;
		particletype->positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleDeviationZ;

		particletype->velocity = _particleInfo->_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * _particleInfo->_particleVelocityVariation;

		// �ٴٴϱ� red�� green�� �ʿ� ����. 
		particletype->red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		particletype->green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
		particletype->blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;

		particletype->isActive = true;

		_particleInfo->_particleList.push_back(particletype);

		SortParticles();


		//	// ���� �� ��ġ�� �˾����Ƿ� �ε������� �� ��ġ �� �迭�� �����Ͽ� �� ��ƼŬ�� ���� ������ �������մϴ�.
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

		//	// ���� ��Ȯ�� ���� ������ ��ƼŬ �迭�� �����Ͻʽÿ�.
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

	// ���� ������ ���� ��ƼŬ�� �ڿ��� ������ �������ؾ��ϹǷ� ��ƼŬ �迭�� �����ؾ� �մϴ�.
	// Z ���̸� ����Ͽ� �����ϹǷ� ��Ͽ��� ��ƼŬ�� �����ؾ� �ϴ� ��ġ�� ã�ƾ��մϴ�.
	// ������ �츰 ž���̹Ƿ� Y�� �������� ������ �ݴϴ�. 
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
	// �� �������� ��ġ, �ӵ� �� ������ �ð��� ����Ͽ� �Ʒ������� �̵��Ͽ� ��� ��ƼŬ�� ������Ʈ�մϴ�.
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

	// ������Ʈ Ÿ���� ��ƼŬ�� �־��ش�. 
	meshInfo->type = OBJECT_TYPE::PARTICLE;
	meshInfo->name = (int)MeshName::ParticleMesh;
	meshInfo->particleIdx = 0;

	GraphicsEngineManager::GetInstance()->SetObjInfo(meshInfo);

}

