#include "pch.h"
#include "DialogController.h"
#include "GameObject.h"

DialogController::DialogController()
{
	LoadJson();
}

DialogController::~DialogController()
{

}

bool DialogController::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
	writer->StartObject();

	writer->String("Node");
	writer->Int(_node);

	writer->String("Id");
	writer->Int(_id);

	writer->String("Character");
	writer->String(_name.c_str());

	writer->String("Scipt");
	writer->String(_name.c_str());

	writer->EndObject();
	return true;
}

bool DialogController::Deserialize(const rapidjson::Value& obj)
{
	Node(obj["Node"].GetInt());
	Id(obj["Id"].GetInt());
	Script(obj["Script"].GetString());
	Name(obj["Name"].GetString());

	return true;
}

//bool DialogController::Deserialize(const std::string& string)
//{
//	rapidjson::Document doc;
//	int i = 0;
//
//	while (string[i] != '[')
//	{
//		i++;
//	}
//
//	std::string str = string.substr(i);
//
//	if (InitDocument(string,doc))
//	return false;
//
//	if (!doc.IsArray())
//		return false;
//	
//	for (rapidjson:: Value:: ConstValueIterator itr = doc.Begin(); itr!= doc.End(); ++ itr)
//	{
//		DialogController dia;
//		dia.Deserialize(*itr);
//
//	}
//
//
//	DialogueInfo _scripData;
//	return false;
//}

void DialogController::LoadJson()
{
	DialogController dialogController;
	dialogController.DeserializeFromFile("Data\\json\\DialogTable.json");
}

DialogueNodePtr DialogController::CreateDialogueNode(int num)
{
	DialogueNodePtr spDialogueNode = std::make_shared<DialogueNode>();

	for (auto iter : _dialogueInfo)
	{
		if (iter.Node() == num)
		{
			spDialogueNode->_dialogueInfo.Script().c_str();
			RunDialogue(num);

		}
	}

	return spDialogueNode;
}

//void DialogController::InitDialogueTree(int num)
//{
//
//	for (auto iter : _dialogueInfo)
//	{
//		if (iter.Node() == num)
//		{
//			_dialogueNode = CreateDialogueNode(num);
//
//		}
//		else if (iter._id == num)
//		{
//			ReplaceDialogueContents(num);
//			_dialogueNode->_sequenceDialogueInfo.emplace_back(Script().c_str());
//		}
//
//	}
//
//	_dialogueNode = CreateDialogueNode(num);
//	ReplaceDialogueContents(num);
//
//	
//	_dialogueInfo =
//	/*_dialogueNode->_triggerContents.emplace_back(num);
//	_dialogueNode->_triggerContents.emplace_back(num);
//	_dialogueNode->_triggerContents.emplace_back(num);*/
//
//
//	//	// D0�� �б� ����
//	//	_dialogueNode->_triggerContents.emplace_back("�����Ⱑ ���δ�.");
//	//	_dialogueNode->_triggerContents.emplace_back("���� �Ұ��� ��Ź�Ѵ�.");
//	//	_dialogueNode->_triggerContents.emplace_back("���̸� ���´�.");
//	//
//	//#pragma region D0�� ���� ����
//	//	// D0N0�� ���� ���
//	//	DialogueNodePtr spDepth1DialogueNode = CreateDialogueNode("�÷��̾�", "������ ������!");
//	//
//	//	DialogueInfo dialogueInfo = { "���� ���", "������ ������� �� �� ���� �����." };
//	//	spDepth1DialogueNode->_sequenceDialogueInfo.push_back(dialogueInfo);
//	//	spDepth1DialogueNode->_nextDialogueNode.push_back(spDepth1DialogueNode);
//
//		//#pragma region D0N1�� ���� ��� ����
//		//	// D0N1�� ���� ���
//		//	spDepth1DialogueNode = CreateDialogueNode("�÷��̾�", "�ȳ��ϼ���, ���� �Ұ� �� ���ּ���.");
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "���� �Ұ� ���̱���, �˰ڽ��ϴ�!");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "�츮 ������ ������ ��� ���� ���̿���.");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "���� ����� �νɵ� ����, ��ǵ� ���� �Ͼ�� ����!");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "�׷���... ��... (���� �ӹ��Ÿ���)");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	spDepth1DialogueNode->vecStrTriggerContents.emplace_back("�����ϰ� ������.");
//		//	spDepth1DialogueNode->vecStrTriggerContents.emplace_back("���� ������ �����.");
//		//
//		//#pragma region D0N1->D1�� ���� ����
//		//	// D0N1->D1N0�� ���� ���
//		//	DialogueNodePtr spDepth2DialogueNode = CreateDialogueNode("�÷��̾�", "�׷�����, �׷� �� �̸�...");
//		//	spDepth1DialogueNode->vecNextDialogueNode.push_back(spDepth2DialogueNode);
//		//
//		//#pragma region D0N1->D1N1�� ���� ��� ����
//		//	// D0N1->D1N1�� ���� ���
//		//	spDepth2DialogueNode = CreateDialogueNode("�÷��̾�", "���� ���̶� �ֳ���?");
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "���� �㸸 �Ǹ� ���� �Ű� ������...");
//		//	spDepth2DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "�ν÷���� �Ҹ��� �鸰�ٰ� �ϰ�...");
//		//	spDepth2DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "���� ���� �ôٴ� ����� �־��...");
//		//	spDepth2DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "������ ���ذ� ������, �ٵ� �Ű��� ���̳� ����...");
//		//	spDepth2DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	spDepth2DialogueNode->vecStrTriggerContents.emplace_back("�����ϰ� ������.");
//		//	spDepth2DialogueNode->vecStrTriggerContents.emplace_back("�����غ��ٰ� �Ѵ�.");
//		//
//		//#pragma region D0N1->D1N1->D2�� ���� ����
//		//	// D0N1->D1N1->D2N0�� ���� ���
//		//	DialogueNodePtr spDepth3DialogueNode = CreateDialogueNode("�÷��̾�", "�׷�����, �׷� �� �̸�..."); // �ߺ� ����
//		//	spDepth2DialogueNode->vecNextDialogueNode.push_back(spDepth3DialogueNode);
//		//
//		//#pragma region D0N1->D1N1->D2N1�� ���� ��� ����
//		//	// D0N1->D1N1->D2N1�� ���� ���
//		//	spDepth3DialogueNode = CreateDialogueNode("�÷��̾�", "���� ������ ���� �˾ƺ��Կ�.");
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "��...! �����մϴ�, ��ħ ������ �ʿ��߾��!");
//		//	spDepth3DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "���� ����Բ� �ȳ��ص帮�ڽ��ϴ�!");
//		//	spDepth3DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	spDepth2DialogueNode->vecNextDialogueNode.push_back(spDepth3DialogueNode);
//		//#pragma endregion
//		//
//		//	spDepth2DialogueNode->vecNextDialogueNode.push_back(spDepth3DialogueNode);
//		//#pragma endregion
//		//
//		//	spDepth1DialogueNode->vecNextDialogueNode.push_back(spDepth2DialogueNode);
//		//#pragma endregion
//		//#pragma endregion
//		//
//		//	g_spRootDialogueNode->vecNextDialogueNode.push_back(spDepth1DialogueNode);
//		//#pragma endregion
//		//
//		//#pragma region D0N2�� ���� ��� ����
//		//	// D0N2�� ���� ���
//		//	spDepth1DialogueNode = CreateDialogueNode("�÷��̾�", "�Ƿ�����, ���̴� ��� �ǽó���?");
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "�� ���̿�...?");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "��... ��...");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "�˼�������, �˷��帱 �� �����ϴ�.");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	g_spRootDialogueNode->vecNextDialogueNode.push_back(spDepth1DialogueNode);
//}

//void DialogController::RunDialogue(const DialogueNodePtr& spDialogueNode)
void DialogController::RunDialogue(int num)
{
	DialogueNodePtr spDialogueNode = std::make_shared<DialogueNode>();

	for (auto iter : _dialogueInfo)
	{
		/*ShowDialogueInfo(spDialogueNode->_dialogueInfo);*/
		/*for (const auto& iter : spDialogueNode->_sequenceDialogueInfo)
		{
				ShowDialogueInfo(iter);
		}*/
		// �� �ǹ� ���� ȭ�� ���ſ��̿���.
		/*ShowDialogueInfo(*(--spDialogueNode->_sequenceDialogueInfo.end()));*/
		// ���� ��� ���� �κ��̿���.

		if (iter.Id() == num)
		{
			if (spDialogueNode->_sequenceDialogueInfo.empty() != true)
			{
				// ���� ����� ��� ���� ��� ����� �ε����� 0 �����̿���!
				// ���� ��� ��忡 �������� ���� ���� ���� ���� �̵��ؾ� �ؿ�.
				if ((spDialogueNode->_nextDialogueNode.empty() != true) &&
					(spDialogueNode->_triggerContents.empty() == true))
				{
					RunDialogue(num);
				}
			}
		}

		if (iter.Id() == num)
		{
			// �б� ��� ���� �κ��̿���.
			if (spDialogueNode->_triggerContents.empty() != true)
			{
				size_t size = spDialogueNode->_triggerContents.size();
				for (size_t i = 0; i < size; ++i)
				{
					 i + 1, spDialogueNode->_triggerContents[i].c_str();
				}
				// ���õ� �ε����� �´� ���� ��� ���� ��� �Լ��� ȣ���ؾ� �ؿ�!
				if (spDialogueNode->_nextDialogueNode.empty() != true)
				{
					spDialogueNode->_nextDialogueNode.at(num);
					RunDialogue(num);
					return;
				}
			}
		}
	}
}

void DialogController::EndDialogue()
{
	/*printf("��ȭ�� ����Ǿ����!\n\n");*/
}

void DialogController::ReplaceDialogueContents(int num)
{
	DialogueNodePtr spDialogueNode = std::make_shared<DialogueNode>();

	for (auto iter : _dialogueInfo)
	{
		if (iter.Id() == num)
		{
			spDialogueNode->_dialogueInfo.Script().c_str();
		}
	}
}

//void DialogController::ShowDialogueInfo(const DialogueInfo& dialogueInfo)
//{
//
//}
//
//void DialogController::DrawDialogueEdge()
//{
//
//}
