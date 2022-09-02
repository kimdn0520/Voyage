#pragma once
#include "JsonBase.h"
#include "MonoBehaviour.h"


//class DialogueTree 
//{
//public :
//	void Init();;
//	void DestoryTree();
//
//	int PerformDialogue();
//private:
//	vector<DialogueNode*> _dialogueNodes;
//};
//
//class DialogueNode 
//{
//public:
//	string text;
//	vector<DialogueOption> _dialogueOption;
//};
//
//class DialogueNode;
//
//class DialogueOption
//{
//public :
//	string text;
//	int returnCode;
//	DialogueNode* nextNode; 
//
//};

// 대사 정보입니다. 
//struct DialogueInfo
//{
//	UINT _node;			 // Node 넘버
//	UINT _id;			 // 고유 넘버
//	std::string _name;	 // 화자(말하는 사람)의 이름
//	std::string _script; // 대사 내용
//
//};


struct DialogueNode; // 전방 선언
using DialogueNodePtr = std::shared_ptr<DialogueNode>;

// 대화 트리의 노드입니다.
struct DialogueNode
{
	DialogController              _dialogueInfo;			// 화자의 정보
	std::vector<DialogController> _sequenceDialogueInfo;	// 연속으로 진행되는 대사 모음
	std::vector<std::string>      _triggerContents;			// 분기되는 내용 모음
	std::vector<DialogueNodePtr>  _nextDialogueNode;		// 다음 대사 모음 (하나 이상일 수도 있음!)
};

class DialogController : public JsonBase 
{
public :
	DialogController();
	virtual ~DialogController();

public:
	// JSON vector
	vector<DialogController> _dialogueInfo;
	// 대화당 루트 노드는 필수!
	DialogueNodePtr _dialogueNode; 

public :
	virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer)const;
	virtual bool Deserialize(const rapidjson::Value& obj)override;
	/*virtual bool Deserialize(const std::string& string);*/


public:
	int Node() const { return _node; }
	void Node(int node) { _node = node; }

	int Id() const { return _id; }
	void Id(int id){_id = id; }

	const std::string& Script() const { return _script; }
	void Script(const std::string& script) { _script = script; }

	const std::string& Name() const { return _name; }
	void Name(const std::string& name) { _name = name; }

private:
	std::string _name;
	std::string _script;
	int _id;
	int _node;

public :

	// 대화 트리 함수 선언 모음
	/*void CreateDialogueNode(int num);*/
	DialogueNodePtr CreateDialogueNode(int num);
	void LoadJson();
	void InitDialogueTree(int num);
	void RunDialogue(int num);
	void EndDialogue();
	void ShowDialogueInfo(const DialogController& dialogueInfo);
	void DrawDialogueEdge();
	void ReplaceDialogueContents(int num);
};