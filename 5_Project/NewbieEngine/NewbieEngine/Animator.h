#pragma once
#include "NewbieEngineDLL.h"
#include "Component.h"

struct AnimationClipInfo;
class Transform;

class Animator : public Component
{
public:
	NewbieEngine_DLL Animator(shared_ptr<GameObject> gameObject);
	NewbieEngine_DLL virtual ~Animator();

private:
	shared_ptr<Transform> _transform;

	// vector<shared_ptr<AnimationClipInfo>> _animClips;
	unordered_map<size_t, shared_ptr<AnimationClipInfo>> _animClips;

	unordered_map<size_t, vector<int>> _tickNumberPosList;

	unordered_map<size_t, vector<int>> _tickNumberRotList;
	
	float	_updateTime = 0.f;
	int		_currentFrame = 0;
	size_t		_clipIndex = 0;		

	bool _isPlay = true;
	
	bool _isLoop = true;

	// 애니메이션에 해당하는 tick 리스트를 돈다.

public:
	NewbieEngine_DLL void SetAnimClip(size_t name, shared_ptr<AnimationClipInfo> animClip);

	void SetIsPlay(bool val) { _isPlay = val; }

	int GetAnimCount() { return static_cast<int>(_animClips.size()); }
	
	int GetCurrentClipIndex() { return _clipIndex; }

	NewbieEngine_DLL void PushTickData(size_t name);

	NewbieEngine_DLL void Play(size_t hashName, bool loop);

public:
	virtual void Update() override;
};

