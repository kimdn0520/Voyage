#include "pch.h"
#include "Animator.h"
#include "GameObject.h"
#include "Transform.h"
#include "TimeManager.h"
#include "Transform.h"
#include "ObjectInfo.h"

Animator::Animator(shared_ptr<GameObject> gameObject)
	: Component(gameObject, COMPONENT_TYPE::ANIMATOR),
	_transform(gameObject->GetTransform())
{}

Animator::~Animator()
{}

void Animator::SetAnimClip(size_t name, shared_ptr<AnimationClipInfo> animClip)
{
	_animClips.insert(make_pair(name, animClip));
}

/// <summary>
/// �ش� �ִϸ��̼��� tick ����
/// animClip�� ������ �־���Ѵ�. SetAnimClip �Ŀ� ���ش�.
/// totalFrame�� pos �� rot �߿� ���� ������ �����ɷ� ���ش�.
/// </summary>
void Animator::PushTickData(size_t name)
{
	_animClips[name]->totalFrame = 0;

	if (!_animClips[name]->position.empty())
	{
		for (int i = 0; i < _animClips[name]->position.size(); i++)
		{
			_tickNumberPosList[name].push_back(_animClips[name]->position[i]->tick);
		}

		if (_animClips[name]->position.size() == 1)
			_animClips[name]->ticksPosPerFrame = _animClips[name]->position[0]->tick;
		else
			_animClips[name]->ticksPosPerFrame = _animClips[name]->position[1]->tick - _animClips[name]->position[0]->tick;

		if (_animClips[name]->totalFrame < _animClips[name]->position.size())
			_animClips[name]->totalFrame = _animClips[name]->position.size();
	}

	if (!_animClips[name]->rotation.empty())
	{
		for (int i = 0; i < _animClips[name]->rotation.size(); i++)
		{
			_tickNumberRotList[name].push_back(_animClips[name]->rotation[i]->tick);
		}

		if (_animClips[name]->rotation.size() == 1)
			_animClips[name]->ticksRotPerFrame = _animClips[name]->rotation[0]->tick;
		else
			_animClips[name]->ticksRotPerFrame = _animClips[name]->rotation[1]->tick - _animClips[name]->rotation[0]->tick;

		if (_animClips[name]->totalFrame < _animClips[name]->rotation.size())
			_animClips[name]->totalFrame = _animClips[name]->rotation.size();
	}
}

void Animator::Play(size_t hashName, bool isLoop)
{
	_clipIndex = hashName;
	_updateTime = 0.f;
	_isLoop = isLoop;
	_isPlay = true;

	if (_animClips[_clipIndex] == nullptr)
	{
		// ASE�ļ����� Decompose�ؼ� �޾Ҵ� original scale,rot,trans �� �ٽ� ���Ʋ��ش�.
		_transform->SetLocalScale(_transform->GetOriScale());
		_transform->SetLocalPosition(_transform->GetOriTrans());
		_transform->SetLocalRotation(_transform->GetOriRot());
		return;
	}

	// firstFrame �ϴ� scene�� ���������� �̻��ϸ� �������� �ٲ��ָ��
	_currentFrame = _animClips[hashName]->firstFrame;
}

void Animator::Update()
{
	if (!_isPlay)
		return;

	_updateTime += TimeManager::GetInstance()->GetDeltaTime();

	const shared_ptr<AnimationClipInfo>& _currentClip = _animClips[_clipIndex];

	if (_currentClip == nullptr)
		return;

	// frameSpeed�� �ϴ� scene�� ���������� �̻��ϸ� �������� �ٲ��ָ��
	if (_updateTime < 1.0f / _currentClip->frameSpeed)
		return;

	_updateTime = 0.f;

	// ���� ƽ�� ���Ѵ�.
	// �����ϴ� frame ���ٰ� ticksperFrame(������ ������ ƽ) �� �����ش�.
	int currentPosTick = _currentFrame * _currentClip->ticksPosPerFrame;
	int currentRotTick = _currentFrame * _currentClip->ticksRotPerFrame;
	int preIndex = 0;
	int postIndex = 0;
	float t = 0.f;				// ���� ���

	// Ŭ���� Animation_pos�� �ִٸ�..
	if (!_currentClip->position.empty())
	{
		for (int i = 0; i < _tickNumberPosList[_clipIndex].size(); i++)
		{
			// ���� ƽ�� �� ó�� �ִϸ��̼Ǻ��� �۰ų� ���� ���
			// ��ó�� �ִϸ��̼��� ����Ѵ�.
			if (currentPosTick <= _tickNumberPosList[_clipIndex][0])
			{
				preIndex = 0;
				postIndex = 1;

				// tickPos �� �Ѱ��ϰ��..?!
				if (_tickNumberPosList[_clipIndex].size() == 1)
					postIndex = 0;

				t = 0.f;

				break;
			}

			// ���� ƽ�� ��� posTick ���� ū ���
			// ������ �ִϸ��̼��� ����Ѵ�.
			if (currentPosTick >= _tickNumberPosList[_clipIndex][_tickNumberPosList[_clipIndex].size() - 1])
			{
				// tickPos �� �Ѱ��� ���?
				if (_tickNumberPosList[_clipIndex].size() == 1)
				{
					preIndex = 0;
					postIndex = 0;
				}
				else
				{
					preIndex = _tickNumberPosList[_clipIndex].size() - 2;
					postIndex = _tickNumberPosList[_clipIndex].size() - 1;
				}

				t = 1.0f;

				break;
			}

			if (currentPosTick > _tickNumberPosList[_clipIndex][i])
				continue;

			// ���� ƽ�� ���� ���
			// �� ���� ���� ����� ƽ���� �Ѵ�.
			preIndex = i - 1;
			postIndex = i;

			t = static_cast<float>(currentPosTick - _tickNumberPosList[_clipIndex][preIndex]) / (_tickNumberPosList[_clipIndex][postIndex] - _tickNumberPosList[_clipIndex][preIndex]);
			break;
		}

		Vector3 pos = XMVectorLerp(_currentClip->position[preIndex]->pos, _currentClip->position[postIndex]->pos, t);

		_transform->SetLocalPosition(pos);
	}

	// Ŭ���� Animation_rot�� �ִٸ�.. 
	if (!_currentClip->rotation.empty())
	{
		for (int i = 0; i < _tickNumberRotList[_clipIndex].size(); i++)
		{
			// ���� ƽ�� �� ó�� �ִϸ��̼Ǻ��� �۰ų� ���� ���
			// ��ó�� �ִϸ��̼��� ����Ѵ�.
			if (currentRotTick <= _tickNumberRotList[_clipIndex][0])
			{
				preIndex = 0;
				postIndex = 1;

				// tickRot �� �Ѱ��ϰ��..?!
				if (_tickNumberRotList[_clipIndex].size() == 1)
					postIndex = 0;

				t = 0.f;

				break;
			}

			// ���� ƽ�� ��� posTick ���� ū ���
			// ������ �ִϸ��̼��� ����Ѵ�.
			if (currentRotTick >= _tickNumberRotList[_clipIndex][_tickNumberRotList[_clipIndex].size() - 1])
			{
				// tickRot �� �Ѱ��� ���?
				if (_tickNumberRotList[_clipIndex].size() == 1)
				{
					preIndex = 0;
					postIndex = 0;
				}
				else
				{
					preIndex = _tickNumberRotList[_clipIndex].size() - 2;
					postIndex = _tickNumberRotList[_clipIndex].size() - 1;
				}

				t = 1.0f;

				break;
			}

			if (currentRotTick > _tickNumberRotList[_clipIndex][i])
				continue;

			// ���� ƽ�� ���� ���
			// �� ���� ���� ����� ƽ���� �Ѵ�.
			preIndex = i - 1;
			postIndex = i;

			t = static_cast<float>(currentRotTick - _tickNumberRotList[_clipIndex][preIndex]) / (_tickNumberRotList[_clipIndex][postIndex] - _tickNumberRotList[_clipIndex][preIndex]);
			break;
		}

		Vector4 rotQT = XMQuaternionSlerp(_currentClip->rotation[preIndex]->rotQT, _currentClip->rotation[postIndex]->rotQT, t);

		Vector3 rotation = _transform->QuatToEuler(rotQT);

		_transform->SetLocalRotation(Vector3(rotation.x, rotation.y, rotation.z));
	}

	_currentFrame++;

	if (_isLoop)
	{
		if (_currentFrame > _currentClip->totalFrame)
			_currentFrame = _currentClip->firstFrame;
	}
	else
	{
		if (_currentFrame > _currentClip->totalFrame)
			_isPlay = false;
	}
}
