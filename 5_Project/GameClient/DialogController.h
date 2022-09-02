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

// ��� �����Դϴ�. 
//struct DialogueInfo
//{
//	UINT _node;			 // Node �ѹ�
//	UINT _id;			 // ���� �ѹ�
//	std::string _name;	 // ȭ��(���ϴ� ���)�� �̸�
//	std::string _script; // ��� ����
//
//};


struct DialogueNode; // ���� ����
using DialogueNodePtr = std::shared_ptr<DialogueNode>;

// ��ȭ Ʈ���� ����Դϴ�.
struct DialogueNode
{
	DialogController              _dialogueInfo;			// ȭ���� ����
	std::vector<DialogController> _sequenceDialogueInfo;	// �������� ����Ǵ� ��� ����
	std::vector<std::string>      _triggerContents;			// �б�Ǵ� ���� ����
	std::vector<DialogueNodePtr>  _nextDialogueNode;		// ���� ��� ���� (�ϳ� �̻��� ���� ����!)
};

class DialogController : public JsonBase 
{
public :
	DialogController();
	virtual ~DialogController();

public:
	// JSON vector
	vector<DialogController> _dialogueInfo;
	// ��ȭ�� ��Ʈ ���� �ʼ�!
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

	// ��ȭ Ʈ�� �Լ� ���� ����
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