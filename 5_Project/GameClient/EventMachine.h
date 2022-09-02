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
	// IEventState ����� �޴� State Class ���� friend�� ����ش�.
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

	// State�� �ѱ�� ���ǵ�
	bool isIntro = false;
	bool isFarming = false;				// �̰��ϳ��� �Ĺ��̺�Ʈ �� �ϱ�
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


	// �÷��̾� ������ ����
	bool isMove = true;

	// ���ű����� ���� �Ⱦҳ� ���Ⱦҳ�
	bool isSellDream = false;

	// yes �� �����߳�
	bool isSelectedYes = false;

	// no �� �����߳�
	bool isSelectedNo = false;

	// infancy���� shaking�ؼ� �װ� ��Ƴ�����..
	// farming���� fadein �Ҷ�� �ִ� bool��
	bool isShakingDeath = false;

	// �ź��� ��忣�� ��
	bool isSellDreamTalk = false;

	// ��忣�� 
	bool isBadEnding = false;

	bool isHappyEnding = false;

private:

	// ���� �������� �̺�Ʈ
	IEventState* _currentState;
	// ��ǳ�� UI
	shared_ptr<GameObject> _mainTalkPanel;
	
	//  ȸ�� �̹���
	shared_ptr<GameObject> _FlisingImage;
	int _preState, _newState;


	// ���� �̺�Ʈ���� ���� ī�޶�
	shared_ptr<GameObject> _camera;

	// ���� �÷��� �ϴ� �÷��̾�(��)
	shared_ptr<GameObject> _boat;

	// ���� ���� fadeinout �� �г� ���� �ٲ𶧸��� ������ ���Ʋ��ش�.
	shared_ptr<GameObject> _fadeInOutPanel;
	
	// ���� �÷��� �ϴ� ����
	shared_ptr<GameObject> _dodo;

	// �ҸӴ�
	shared_ptr<GameObject> _grandMa;

	shared_ptr<GameObject> _wave;
	shared_ptr<GameObject> _yesButton;

	// ���� ���� button
	shared_ptr<GameObject> _noButton;

	// space UI
	shared_ptr<GameObject> _spacePanel;

	// arrow UI
	shared_ptr<GameObject> _arrowPanel;

	// skybox
	shared_ptr<GameObject> _skyBox;

	// �����
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

	// state���� ��Ƴ��� ��
	map<int, IEventState*> _states;

public:
	void Release();

	// TODO : �� ���� ���ν����ϰų� ����ȭ�鰥�� clear�� ������ҰͰ�����..?
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