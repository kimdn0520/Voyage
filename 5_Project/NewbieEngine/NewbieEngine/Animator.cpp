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
/// 해당 애니메이션의 tick 장착
/// animClip에 정보가 있어야한다. SetAnimClip 후에 해준다.
/// totalFrame을 pos 랑 rot 중에 가장 갯수가 많은걸로 해준다.
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
		// ASE파서에서 Decompose해서 받았던 original scale,rot,trans 로 다시 갈아껴준다.
		_transform->SetLocalScale(_transform->GetOriScale());
		_transform->SetLocalPosition(_transform->GetOriTrans());
		_transform->SetLocalRotation(_transform->GetOriRot());
		return;
	}

	// firstFrame 일단 scene꺼 받은다음에 이상하면 수동으로 바꿔주면댐
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

	// frameSpeed는 일단 scene꺼 받은다음에 이상하면 수동으로 바꿔주면댐
	if (_updateTime < 1.0f / _currentClip->frameSpeed)
		return;

	_updateTime = 0.f;

	// 현재 틱을 구한당.
	// 증가하는 frame 에다가 ticksperFrame(프레임 마다의 틱) 을 곱해준다.
	int currentPosTick = _currentFrame * _currentClip->ticksPosPerFrame;
	int currentRotTick = _currentFrame * _currentClip->ticksRotPerFrame;
	int preIndex = 0;
	int postIndex = 0;
	float t = 0.f;				// 보간 계수

	// 클립에 Animation_pos가 있다면..
	if (!_currentClip->position.empty())
	{
		for (int i = 0; i < _tickNumberPosList[_clipIndex].size(); i++)
		{
			// 현재 틱이 맨 처음 애니메이션보다 작거나 같은 경우
			// 맨처음 애니메이션을 재생한다.
			if (currentPosTick <= _tickNumberPosList[_clipIndex][0])
			{
				preIndex = 0;
				postIndex = 1;

				// tickPos 가 한개일경우..?!
				if (_tickNumberPosList[_clipIndex].size() == 1)
					postIndex = 0;

				t = 0.f;

				break;
			}

			// 현재 틱이 모든 posTick 보다 큰 경우
			// 마지막 애니메이션을 재생한다.
			if (currentPosTick >= _tickNumberPosList[_clipIndex][_tickNumberPosList[_clipIndex].size() - 1])
			{
				// tickPos 가 한개일 경우?
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

			// 현재 틱이 작은 경우
			// 그 전의 가장 가까운 틱으로 한다.
			preIndex = i - 1;
			postIndex = i;

			t = static_cast<float>(currentPosTick - _tickNumberPosList[_clipIndex][preIndex]) / (_tickNumberPosList[_clipIndex][postIndex] - _tickNumberPosList[_clipIndex][preIndex]);
			break;
		}

		Vector3 pos = XMVectorLerp(_currentClip->position[preIndex]->pos, _currentClip->position[postIndex]->pos, t);

		_transform->SetLocalPosition(pos);
	}

	// 클립에 Animation_rot가 있다면.. 
	if (!_currentClip->rotation.empty())
	{
		for (int i = 0; i < _tickNumberRotList[_clipIndex].size(); i++)
		{
			// 현재 틱이 맨 처음 애니메이션보다 작거나 같은 경우
			// 맨처음 애니메이션을 재생한다.
			if (currentRotTick <= _tickNumberRotList[_clipIndex][0])
			{
				preIndex = 0;
				postIndex = 1;

				// tickRot 가 한개일경우..?!
				if (_tickNumberRotList[_clipIndex].size() == 1)
					postIndex = 0;

				t = 0.f;

				break;
			}

			// 현재 틱이 모든 posTick 보다 큰 경우
			// 마지막 애니메이션을 재생한다.
			if (currentRotTick >= _tickNumberRotList[_clipIndex][_tickNumberRotList[_clipIndex].size() - 1])
			{
				// tickRot 가 한개일 경우?
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

			// 현재 틱이 작은 경우
			// 그 전의 가장 가까운 틱으로 한다.
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
