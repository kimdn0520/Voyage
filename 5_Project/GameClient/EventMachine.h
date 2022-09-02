#pragma once
#include "InfancyText.h"

class IEventState;
class GameObject;

class EventMachine
{
public:
	EventMachine();
	virtual ~EventMachine();

private:
	static EventMachine* eventManager;

public:
	static EventMachine* GetInstance();

public:
	// IEventState 상속을 받는 State Class 들을 friend로 모아준다.
	friend class EVENT_TITLE;
	friend class EVENT_INTRO;
	friend class EVENT_INFANCY_FARMING;
	friend class EVENT_INFANCY_SHAKING;
	friend class EVENT_INFANCY_END;
	friend class EVENT_YOUTH_INTRO;
	friend class EVENT_YOUTH_ROW;
	friend class EVENT_YOUTH_FARMING;
	friend class EVENT_YOUTH_SOLO;
	friend class EVENT_ADULTHOOD_INTRO;
	friend class EVENT_ADULTHOOD_SEAGULL;
	friend class EVENT_ADULTHOOD_COMPETITOR;
	friend class EVENT_ADULTHOOD_SWIRL;
	friend class EVENT_OLDAGE_INTRO;
	friend class EVENT_OLDAGE_FLIASHING;
	friend class EVENT_OLDAGE_END;
	friend class EVENT_ADULTHOOD_THUNDER;
	friend class EVENT_ADULTHOOD_TURTLE;
	friend class EVENT_ADULTHOOD_NOTURTLE;
	friend class EVENT_ADULTHOOD_UPSTAR;
	friend class EVENT_ADULTHOOD_GIRL;
	friend class EVENT_ADULTHOOD_RHYTHM;
	friend class EVENT_ADULTHOOD_WEDDING;

	// State를 넘기는 조건들
	bool isIntro = false;
	bool isFarming = false;				// 이거하나로 파밍이벤트 다 하기
	bool isInfacnyShaking = false;
	bool isInfacnyEnd = false;
	
	bool isYouthIntro = false;
	bool isYouthRow = false;
	bool isYouthIndependent = false;

	bool isAdulthoodIntro = false;
	bool isAdultSeaGull = false;
	bool isAdultCompetitor = false;
	bool isAdultSwirl = false;
	bool isAdultThunder = false;
	bool isAdultUpstar = false;
	bool isAdultGirl = false;
	bool isAdultRhythm = false;
	bool isAdultWedding = false;

	bool isOldAgeIntro = false;
	bool isOldAgeFlishing = false;
	bool isOldAgeEnd = false;


	// 플레이어 움직임 막기
	bool isMove = true;

	// 갈매기한테 꿈을 팔았나 안팔았나
	bool isSellDream = false;

	// yes 를 선택했나
	bool isSelectedYes = false;

	// no 를 선택했나
	bool isSelectedNo = false;

	// infancy에서 shaking해서 죽고 살아났을때..
	// farming에서 fadein 할라고 있는 bool값
	bool isShakingDeath = false;

	// 거북이 배드엔딩 톡
	bool isSellDreamTalk = false;

	// 배드엔딩 
	bool isBadEnding = false;

	bool isHappyEnding = false;

private:

	// 현재 진행중인 이벤트
	IEventState* _currentState;
	// 말풍선 UI
	shared_ptr<GameObject> _mainTalkPanel;
	
	//  회상 이미지
	shared_ptr<GameObject> _FlisingImage;
	int _preState, _newState;


	// 현재 이벤트중인 씬의 카메라
	shared_ptr<GameObject> _camera;

	// 현재 플레이 하는 플레이어(배)
	shared_ptr<GameObject> _boat;

	// 현재 씬의 fadeinout 할 패널 씬이 바뀔때마다 씬에서 갈아껴준다.
	shared_ptr<GameObject> _fadeInOutPanel;
	
	// 현재 플레이 하는 도도
	shared_ptr<GameObject> _dodo;

	// 할머니
	shared_ptr<GameObject> _grandMa;

	shared_ptr<GameObject> _wave;
	shared_ptr<GameObject> _yesButton;

	// 현재 씬의 button
	shared_ptr<GameObject> _noButton;

	// space UI
	shared_ptr<GameObject> _spacePanel;

	// arrow UI
	shared_ptr<GameObject> _arrowPanel;

	// skybox
	shared_ptr<GameObject> _skyBox;

	// 배우자
	shared_ptr<GameObject> _girl;

	shared_ptr<GameObject> _girlBoat;

	shared_ptr<GameObject> _competitionPanel1;

	shared_ptr<GameObject> _competitionPanel2;
public:	
	enum
	{
		TITLE,

		INTRO,
		INFANCY_FARMING,
		INFANCY_SHAKING,
		INFANCY_END,

		YOUTH_INTRO,
		YOUTH_ROW,
		YOUTH_FARMING,
		YOUTH_SOLO,

		ADULTHOOD_INTRO,
		ADULTHOOD_SEAGULL,
		ADULTHOOD_COMPETITOR,
		ADULTHOOD_SWIRL,
		ADULTHOOD_THUNDER,
		ADULTHOOD_TURTLE,
		ADULTHOOD_NOTURTLE,
		ADULTHOOD_UPSTAR,
		ADULTHOOD_GIRL,
		ADULTHOOD_RHYTHM,
		ADULTHOOD_WEDDING,

		OLDAGE_INTRO,
		OLDAGE_FLIASHING,
		OLDAGE_END,
	};

	// state들을 모아놓은 곳
	map<int, IEventState*> _states;

public:
	void Release();

	// TODO : 흠 뭔가 새로시작하거나 메인화면갈때 clear를 해줘야할것같은데..?
	void ClearData();

	void SetCamera(shared_ptr<GameObject> camera)
	{
		_camera = camera;
	}

	void SetFadeInOutPanel(shared_ptr<GameObject> panelObj)
	{ 
		_fadeInOutPanel = panelObj; 
	}

	void SetMainTalkPanel(shared_ptr<GameObject> panelObj)
	{
		_mainTalkPanel = panelObj;
	}

	void SetFlisingImage(shared_ptr<GameObject> ImageObj)
	{
		_FlisingImage = ImageObj;
	}

	void SetYesButton(shared_ptr<GameObject> yesbutton)
	{
		_yesButton = yesbutton;
	}

	void SetNoButton(shared_ptr<GameObject> nobutton)
	{
		_noButton = nobutton;
	}

	void SetSpacePanel(shared_ptr<GameObject> spacepanel)
	{
		_spacePanel = spacepanel;
	}

	void SetArrowPanel(shared_ptr<GameObject> arrowpanel)
	{
		_arrowPanel = arrowpanel;
	}

	void SetSkyBox(shared_ptr<GameObject> skyBox)
	{
		_skyBox = skyBox;
	}

	void SetCompetitionPanel_1(shared_ptr<GameObject> panel)
	{
		_competitionPanel1 = panel;
	}

	void SetCompetitionPanel_2(shared_ptr<GameObject> panel)
	{
		_competitionPanel2 = panel;
	}

	void ChangeState(int changeState)
	{
		_currentState = _states[changeState];
	}

public:
	void Update();
	void SetEventNextFatherText(int next);
	void SetNextFatherText(int next);

	void SetPanelImage(int idnum);


	void SetEventYouthNextText(int next);
	void SetNextYouthtText(int next);

	void SetEventAdultNextText(int next);
	void SetNextAdultText(int next);
	
	void SetEventOldAgeNextText(int next);
	void SetNextOldAgeText(int next);

	bool GetSeaGullEventCheck();

	void SetWave(shared_ptr<GameObject> wave);
	void SetCancelFarming(bool check);

	void ResetMachine();
};