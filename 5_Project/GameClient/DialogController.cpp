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
//	//	// D0의 분기 대사들
//	//	_dialogueNode->_triggerContents.emplace_back("분위기가 별로다.");
//	//	_dialogueNode->_triggerContents.emplace_back("마을 소개를 부탁한다.");
//	//	_dialogueNode->_triggerContents.emplace_back("나이를 묻는다.");
//	//
//	//#pragma region D0의 반응 대사들
//	//	// D0N0의 반응 대사
//	//	DialogueNodePtr spDepth1DialogueNode = CreateDialogueNode("플레이어", "마을이 후졌네!");
//	//
//	//	DialogueInfo dialogueInfo = { "마을 사람", "무례한 사람에게 더 할 말은 없어요." };
//	//	spDepth1DialogueNode->_sequenceDialogueInfo.push_back(dialogueInfo);
//	//	spDepth1DialogueNode->_nextDialogueNode.push_back(spDepth1DialogueNode);
//
//		//#pragma region D0N1의 반응 대사 묶음
//		//	// D0N1의 반응 대사
//		//	spDepth1DialogueNode = CreateDialogueNode("플레이어", "안녕하세요, 마을 소개 좀 해주세요.");
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "마을 소개 말이군요, 알겠습니다!");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "우리 마을은 작지만 살기 좋은 곳이에요.");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "마을 사람들 인심도 좋고, 사건도 거의 일어나지 않죠!");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "그런데... 음... (말을 머뭇거린다)");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	spDepth1DialogueNode->vecStrTriggerContents.emplace_back("무시하고 떠난다.");
//		//	spDepth1DialogueNode->vecStrTriggerContents.emplace_back("무슨 일인지 물어본다.");
//		//
//		//#pragma region D0N1->D1의 반응 대사들
//		//	// D0N1->D1N0의 반응 대사
//		//	DialogueNodePtr spDepth2DialogueNode = CreateDialogueNode("플레이어", "그렇군요, 그럼 전 이만...");
//		//	spDepth1DialogueNode->vecNextDialogueNode.push_back(spDepth2DialogueNode);
//		//
//		//#pragma region D0N1->D1N1의 반응 대사 묶음
//		//	// D0N1->D1N1의 반응 대사
//		//	spDepth2DialogueNode = CreateDialogueNode("플레이어", "무슨 일이라도 있나요?");
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "요즘 밤만 되면 밭이 신경 쓰여요...");
//		//	spDepth2DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "부시럭대는 소리가 들린다고도 하고...");
//		//	spDepth2DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "빨간 눈을 봤다는 사람도 있어요...");
//		//	spDepth2DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "아직은 피해가 없지만, 다들 신경이 쓰이나 봐요...");
//		//	spDepth2DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	spDepth2DialogueNode->vecStrTriggerContents.emplace_back("무시하고 떠난다.");
//		//	spDepth2DialogueNode->vecStrTriggerContents.emplace_back("조사해본다고 한다.");
//		//
//		//#pragma region D0N1->D1N1->D2의 반응 대사들
//		//	// D0N1->D1N1->D2N0의 반응 대사
//		//	DialogueNodePtr spDepth3DialogueNode = CreateDialogueNode("플레이어", "그렇군요, 그럼 전 이만..."); // 중복 내용
//		//	spDepth2DialogueNode->vecNextDialogueNode.push_back(spDepth3DialogueNode);
//		//
//		//#pragma region D0N1->D1N1->D2N1의 반응 대사 묶음
//		//	// D0N1->D1N1->D2N1의 반응 대사
//		//	spDepth3DialogueNode = CreateDialogueNode("플레이어", "무슨 일인지 제가 알아볼게요.");
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "앗...! 감사합니다, 마침 도움이 필요했어요!");
//		//	spDepth3DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "제가 촌장님께 안내해드리겠습니다!");
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
//		//#pragma region D0N2의 반응 대사 묶음
//		//	// D0N2의 반응 대사
//		//	spDepth1DialogueNode = CreateDialogueNode("플레이어", "실례지만, 나이는 어떻게 되시나요?");
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "제 나이요...?");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "어... 음...");
//		//	spDepth1DialogueNode->vecSequenceDialogueInfo.push_back(dialogueInfo);
//		//
//		//	ReplaceDialogueContents(dialogueInfo._strContents, "죄송하지만, 알려드릴 수 없습니다.");
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
		// 별 의미 없는 화면 갱신용이에요.
		/*ShowDialogueInfo(*(--spDialogueNode->_sequenceDialogueInfo.end()));*/
		// 연속 대사 진행 부분이에요.

		if (iter.Id() == num)
		{
			if (spDialogueNode->_sequenceDialogueInfo.empty() != true)
			{
				// 연속 대사일 경우 다음 대사 노드의 인덱스는 0 고정이에요!
				// 묶음 대사 노드에 선택지가 없을 때만 다음 대사로 이동해야 해요.
				if ((spDialogueNode->_nextDialogueNode.empty() != true) &&
					(spDialogueNode->_triggerContents.empty() == true))
				{
					RunDialogue(num);
				}
			}
		}

		if (iter.Id() == num)
		{
			// 분기 대사 진행 부분이에요.
			if (spDialogueNode->_triggerContents.empty() != true)
			{
				size_t size = spDialogueNode->_triggerContents.size();
				for (size_t i = 0; i < size; ++i)
				{
					 i + 1, spDialogueNode->_triggerContents[i].c_str();
				}
				// 선택된 인덱스에 맞는 다음 대사 노드로 재귀 함수를 호출해야 해요!
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
	/*printf("대화가 종료되었어요!\n\n");*/
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
