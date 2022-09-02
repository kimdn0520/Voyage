#include "pch.h"
#include "ParserData.h"
#include "Lexer.h"
#include "ASEToken.h"
#include "ASEParser.h"

unsigned int ASEParser::_materialID = 0;

ASEParser::ASEParser()
	: _lexer(make_shared<Lexer>())
{}

ASEParser::~ASEParser()
{}

shared_ptr<NoobData::ASEModel> ASEParser::Load(string filePath)
{
	FILE* file;
	fopen_s(&file, filePath.c_str(), "r");

	int nowToken = 0;
	_refCheckPoint = _materialID;

	_ASEModel = make_shared<NoobData::ASEModel>();

	// feof(FILE* stream) // ������ ���� �����ϰԵǸ� 0�� �ƴ� ���� ��ȯ
	while (!feof(file))
	{
		fgets(_lineArray, 256, file);

		_lexer->ResetChIndex();

		nowToken = _lexer->GetToken(_lineArray);

		switch (nowToken)
		{
		case TOKEND_BLOCK_START:
			break;
		case TOKEND_BLOCK_END:
			break;

		case TOKENR_HELPER_CLASS:
			break;

		case TOKENR_3DSMAX_ASCIIEXPORT:
		{
			_asciiexport = Parsing_Int();
		}
		break;

		case TOKENR_COMMENT:
			break;

			//--------------------------------------------------
			// SCENE(FrameData)
			//--------------------------------------------------
		case TOKENR_SCENE:
			break;
		case TOKENR_SCENE_FILENAME:
		{
			shared_ptr<NoobData::ASEFrameData> frameData = make_shared<NoobData::ASEFrameData>();
			_ASEModel->frameData = frameData;
			_ASEModel->frameData->fileName = Parsing_String();
		}
		break;
		case TOKENR_SCENE_FIRSTFRAME:
		{
			_ASEModel->frameData->firstFrame = Parsing_Int();
		}
		break;
		case TOKENR_SCENE_LASTFRAME:
		{
			_ASEModel->frameData->lastFrame = Parsing_Int();
		}
		break;
		case TOKENR_SCENE_FRAMESPEED:
		{
			_ASEModel->frameData->frameSpeed = Parsing_Int();
		}
		break;
		case TOKENR_SCENE_TICKSPERFRAME:
		{
			_ASEModel->frameData->ticksPerFrame = Parsing_Int();
		}
		break;
		case TOKENR_SCENE_MESHFRAMESTEP:
			break;
		case TOKENR_SCENE_KEYFRAMESTEP:
			break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
			break;
		case TOKENR_SCENE_AMBIENT_STATIC:
			break;
		case TOKENR_SCENE_ENVMAP:
			break;

			//---------------------------------------------------------
			// MATERIAL_LIST
			// �ϴ�! SubMaterial ������ ���� �ʰ� ������..
			//---------------------------------------------------------
		case TOKENR_MATERIAL_COUNT:
		{
			int materialCnt = Parsing_Int();
			if (materialCnt == 0)
			{
				_ASEModel->materials.resize(1);
				std::shared_ptr<NoobData::ASEMaterialInfo> newMaterial = make_shared<NoobData::ASEMaterialInfo>();
				_ASEModel->materials[_materialIndex] = newMaterial;
				// _materials[_materialIndex] = newMaterial;
			}
			else
			{
				_ASEModel->materials.resize(materialCnt);
			}
		}
		break;
		case TOKENR_MATERIAL:
		{
			_materialIndex = Parsing_Int();

			std::shared_ptr<NoobData::ASEMaterialInfo> newMaterial = make_shared<NoobData::ASEMaterialInfo>();
			_ASEModel->materials[_materialIndex] = newMaterial;
			_materialID++;
		}
		break;
		case TOKENR_MATERIAL_AMBIENT:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			_ASEModel->materials[_materialIndex]->ambient = Vector4(x, y, z, 1.0f);
		}
		break;
		case TOKENR_MATERIAL_DIFFUSE:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			_ASEModel->materials[_materialIndex]->diffuse = Vector4(x, y, z, 1.0f);
		}
		break;
		case TOKENR_MATERIAL_SPECULAR:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			_ASEModel->materials[_materialIndex]->specular = Vector4(x, y, z, 1.0f);
		}
		break;
		case TOKENR_MAP_DIFFUSE:
		{
			curTexture = 0;
			_ASEModel->materials[_materialIndex]->isDiffuse = true;
		}
		break;
		case TOKENR_MAP_BUMP:
		{
			curTexture = 1;
			_ASEModel->materials[_materialIndex]->isNormal = true;
		}
		break;
		case TOKENR_MAP_SPECULAR:
		{
			curTexture = 2;
			_ASEModel->materials[_materialIndex]->isSpecular = true;
		}
		break;
		case TOKENR_BITMAP:
		{
			// �̸��� ��������! �޸��忡�� �ٲ�
			string str = Parsing_String();
			wstring wstr(str.begin(), str.end());

			if (curTexture == 0)
			{
				_ASEModel->materials[_materialIndex]->diffuseTexName = wstr;
			}
			else if (curTexture == 1)
			{
				_ASEModel->materials[_materialIndex]->normalTexName = wstr;
			}
			else if (curTexture == 2)
			{
				_ASEModel->materials[_materialIndex]->specularTexName = wstr;
			}
		}
		break;

		//-----------------------------------------
		// GEOMOBJECT, HELPEROBJECT, SHAPEOBJECT
		//-----------------------------------------
		case TOKENR_GROUP:
			break;

			// vertices�� ���� Control(animation)�� �ִ� �༮
		case TOKENR_HELPEROBJECT:
		{
			PushMesh();
			std::shared_ptr<NoobData::ASEMeshInfo> aseMeshInfo = make_shared<NoobData::ASEMeshInfo>();
			_ASEModel->meshInfos.push_back(aseMeshInfo);
			_parsingMeshInfo = aseMeshInfo;
		}
		break;
		case TOKENR_GEOMOBJECT:
		{
			PushMesh();
			std::shared_ptr<NoobData::ASEMeshInfo> aseMeshInfo = make_shared<NoobData::ASEMeshInfo>();
			_ASEModel->meshInfos.push_back(aseMeshInfo);
			_parsingMeshInfo = aseMeshInfo;
		}
		break;
		case TOKENR_SHAPEOBJECT:		// ��������~
			break;

		case TOKENR_NODE_NAME:
		{
			hash<std::string> hasher;
			_parsingMeshInfo->gameObjName = Parsing_String();
			_parsingMeshInfo->meshName = hasher(_parsingMeshInfo->gameObjName);
		}
		break;

		case TOKENR_NODE_PARENT:
			// �� ����� �θ� ����� ����.
			// ���ο� ���ӿ�����Ʈ�� ���� �ش� �θ����� �̸��� ���� ���� ������Ʈ�� ã�Ƽ�
			// �� �θ��� �ڽ����� �־��� ��..?
			_parsingMeshInfo->parentName = Parsing_String();
			break;

			//-----------------------------------------
			// NODE_TM
			//-----------------------------------------
		case TOKENR_NODE_TM:
			break;

		case TOKENR_INHERIT_POS:
			break;
		case TOKENR_INHERIT_ROT:
			break;
		case TOKENR_INHERIT_SCL:
			break;
		case TOKENR_TM_ROW0:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			_row0 = { x, z, y };
		}
		break;
		case TOKENR_TM_ROW1:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			_row1 = { x, z, y };
		}
		break;
		case TOKENR_TM_ROW2:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			_row2 = { x, z, y };
		}
		break;
		case TOKENR_TM_ROW3:
		{
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			_row3 = { x, z, y };

			// ROW3 ���� ��������� ������ش�
			_parsingMeshInfo->nodeTM =
			{ _row0.x, _row0.y, _row0.z, 0,
			  _row2.x, _row2.y, _row2.z, 0,
			  _row1.x, _row1.y, _row1.z, 0,
			  _row3.x, _row3.y, _row3.z, 1.0f
			};

			// ���⼭ �װ�Ƽ�� ������ �۾��ؾ���. 
			// ��ȯ ������ (detA, detA, detA, detA) �̷��� ���´ٰ� �Ѵ�. �ٶȰ���
			XMVECTOR det = XMMatrixDeterminant(_parsingMeshInfo->nodeTM);

			// ������ �װŸ���~
			if (det.m128_f32[0] < 0)
			{
				// Decompose �ߴٰ� scale -�ְ� �ٽ� ���ľ���..
				XMVECTOR scale;
				XMVECTOR rotQuat;
				XMVECTOR trans;
				XMMatrixDecompose(&scale, &rotQuat, &trans, _parsingMeshInfo->nodeTM);
				scale = -1 * scale;

				// �ٽ� SRT ����
				_parsingMeshInfo->nodeTM = XMMatrixScaling(scale.m128_f32[0], scale.m128_f32[1], scale.m128_f32[2]) *
					XMMatrixRotationQuaternion(rotQuat) *
					XMMatrixTranslation(trans.m128_f32[0], trans.m128_f32[1], trans.m128_f32[2]);

				_isNegativeScale = true;
			}
		}
		break;
		case TOKENR_TM_POS:
			break;
		case TOKENR_TM_ROTAXIS:
			break;
		case TOKENR_TM_ROTANGLE:
			break;
		case TOKENR_TM_SCALE:
			break;
		case TOKENR_TM_SCALEAXIS:
			break;
		case TOKENR_TM_SCALEAXISANG:
			// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
			break;

			//-----------------------------------------
			// Animation Data
			//-----------------------------------------
		case TOKENR_TM_ANIMATION:
		{
			_parsingMeshInfo->isAnimation = true;		// �ִϸ��̼��� �����ִ� ������Ʈ��..

			shared_ptr<NoobData::ASEAnimation> animationClip = make_shared<NoobData::ASEAnimation>();
			_parsingMeshInfo->animationClips.push_back(animationClip);
			_parsingAnimation = animationClip;

			_parsingAnimation->nodename = _parsingMeshInfo->gameObjName;
		}
		break;
		case TOKENR_ANIMATION_NAME:
		{
			_parsingAnimation->animName = hasher(Parsing_String());
		}
		break;
		case TOKENR_CONTROL_POS_SAMPLE:
		{
			// ���� ��ǥ ���ϴ�.
			shared_ptr<NoobData::Animation_pos> aniPos = make_shared<NoobData::Animation_pos>();
			aniPos->tick = Parsing_Int();
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();
			aniPos->pos = { x, z, y, 1.0f };
			_parsingAnimation->position.push_back(aniPos);
		}
		break;
		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			// ���� �̶��ϴ�.
			shared_ptr<NoobData::Animation_rot> aniRot = make_shared<NoobData::Animation_rot>();
			aniRot->tick = Parsing_Int();
			float x = Parsing_Float();
			float z = Parsing_Float();
			float y = Parsing_Float();
			// ���� ������ ȸ�� ������ �޴´�.
			// ������ ȸ�� ���� ���� ������ ���Ͽ� �� �� �ð� �������� �����ȴ�.
			float angle = Parsing_Float();

			XMVECTOR axis = { x, y, z, 0.f };						// axis�� �޴´�.
			XMVECTOR quat = XMQuaternionRotationAxis(axis, angle);	// axis�� ���� ȸ�� ���ʹϾ��� ����Ѵ�.

			// ���ʹϾ� ������ �����̱⶧����.. �������� exporter�� �����ؼ� ���´�
			/*if (!_parsingAnimation->rotation.empty())
			{
				quat = XMQuaternionMultiply(_parsingAnimation->rotation.back()->rotQT, quat);
			}*/

			aniRot->rotQT = quat;
			_parsingAnimation->rotation.push_back(aniRot);
		}
		break;
		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			// ? ���� ����..???
		}
		break;

		//---------------------------------------------------------------------------------------------------------
		//			M        E       S         H
		//---------------------------------------------------------------------------------------------------------
		case TOKENR_MESH:
			break;
		case TOKENR_TIMEVALUE:
			break;
		case TOKENR_MESH_NUMBONE:
			break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;
		case TOKENR_MESH_NUMVERTEX:
		{
			_numVertex = Parsing_Int();
			_tmpVertices.resize(Parsing_Int(), Vector3(0, 0, 0));
		}
		break;
		case TOKENR_MESH_NUMFACES:
		{
			// �ӽ� Index �������, �ӽ� Normal ������, Vertices ����� �����ش�.
			int size = Parsing_Int();
			_tmpIndices.resize(size * 3, 0);
			_tmpNormal.resize(size * 3, Vector3{ 1, 1, 1 });
		}
		break;

		///// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			break;
		case TOKENR_MESH_VERTEX:
		{
			// Max -> DirectX  �� y�� z���� �ٲ��ָ� �ȴ�.
			int index = Parsing_Int();
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			// ��... ����� �ȳ־��༭ �ڽ� �߸��׸����־���.. 2022-05-12
			_ASEModel->minPos.x = min(_ASEModel->minPos.x, x);
			_ASEModel->minPos.z = min(_ASEModel->minPos.z, y);
			_ASEModel->minPos.y = min(_ASEModel->minPos.y, z);
			_ASEModel->maxPos.x = max(_ASEModel->maxPos.x, x);
			_ASEModel->maxPos.z = max(_ASEModel->maxPos.z, y);
			_ASEModel->maxPos.y = max(_ASEModel->maxPos.y, z);

			// �� Vertex�鿡 NODE_TM���� ���� ���� ���, �� �ڽ��� ���������
			// ������� �����־� Vertex���� ���� ��ǥ�� ������ �ְ� �Ѵ�.
			XMMATRIX worldTMInverse = XMMatrixInverse(nullptr, _parsingMeshInfo->nodeTM);
			XMVECTOR vertexVec = { x, z, y };
			XMVECTOR vertexXworldTM = XMVector3Transform(vertexVec, worldTMInverse);
			XMFLOAT3 vertex;
			XMStoreFloat3(&vertex, vertexXworldTM);

			_tmpVertices[index].pos.x = vertex.x;
			_tmpVertices[index].pos.y = vertex.y;
			_tmpVertices[index].pos.z = vertex.z;
		}
		break;

		//---------------------------------------------------------------------------------------------------------
		//			B        O       N         E
		//---------------------------------------------------------------------------------------------------------
		case TOKENR_SKIN_INITTM:
			break;
		case TOKENR_BONE_LIST:
			break;
		case TOKENR_BONE:
		{
			// _ASEModel->boneInfos.resize(Parsing_Int());
		}
		break;
		case TOKENR_BONE_NAME:
		{
			std::shared_ptr<NoobData::ASEBoneInfo> aseBoneInfo = make_shared<NoobData::ASEBoneInfo>();
			_parsingMeshInfo->boneInfos.push_back(aseBoneInfo);
			_parsingMeshInfo->boneInfos[_parsingMeshInfo->boneInfos.size() - 1]->boneName = Parsing_String();
		}
		break;
		case TOKENR_BONE_PROPERTY:
			break;
		case TOKENR_MESH_WVERTEXS:
			break;
		case TOKENR_MESH_WEIGHT:
		{
			_weightNum = Parsing_Int();
			_weightCount = 0;
		}
		break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			_tmpVertices[_weightNum].boneIndices[_weightCount] = Parsing_Int();
			_tmpVertices[_weightNum].weights[_weightCount] = Parsing_Float();
			_weightCount++;
		}
		break;


		/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
			break;
		case TOKENR_MESH_FACE:
		{
			// �Ƹ� ������ A: ( ���� ù ��° �� ) B: ( ���� �� ��° �� ) C: ( ���� �� ��° �� ) �̷����̴�.
			// blender�� �׳� �־��µ�.. max�� ACB ?
			int faceIndex = Parsing_Int() * 3;
			Parsing_Int();						// A: B: C: �̷��༮�鶧����..
			int first = Parsing_Int();
			Parsing_Int();
			int second = Parsing_Int();
			Parsing_Int();
			int third = Parsing_Int();

			_tmpIndices[faceIndex] = first;
			_tmpIndices[faceIndex + 1] = third;
			_tmpIndices[faceIndex + 2] = second;
		}
		break;

		case TOKENR_MESH_NUMTVERTEX:
		{
			// ����ä�� ����..?
			_tmpTVert.resize(Parsing_Int(), { Vector2(0, 0) });
		}
		break;
		case TOKENR_MESH_TVERTLIST:
			break;
		case TOKENR_MESH_TVERT:
		{
			// ���ο� TVertex�� ���� ���Ϳ� �ִ´�, ���� 3�� �ִµ� �����Ŵ� ������.
			int index = Parsing_Int();
			float x = Parsing_Float();
			float y = Parsing_Float();

			_tmpTVert[index].x = x;
			_tmpTVert[index].y = 1.0f - y;
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
		{
			_tmpTFaceIndices.resize(Parsing_Int() * 3);
		}
		break;
		case TOKENR_MESH_TFACE:
		{
			int tFaceIndex = Parsing_Int() * 3;
			int first = Parsing_Int();
			int second = Parsing_Int();
			int third = Parsing_Int();

			_tmpTFaceIndices[tFaceIndex] = first;
			_tmpTFaceIndices[tFaceIndex + 1] = third;
			_tmpTFaceIndices[tFaceIndex + 2] = second;
		}
		break;

		case TOKENR_MESH_FACENORMAL:
			break;
		case TOKENR_MESH_VERTEXNORMAL:
		{
			// �ٵ� VertexNoraml ���� �� ���� �־����... �ٵ� FACE���� �����ԵǸ�
			// �� ���ο� ���ؽ��� �븻���� �ֱⰡ �������.. �ٵ�..
			// �׳� �׷��� tmp�� ��Ƶΰ� optimize���� �ٽ� �����Ұ���
			Parsing_Int();							// 3367 0 7834 ����

			int index = _normalIdx++;
			float x = Parsing_Float();
			float y = Parsing_Float();
			float z = Parsing_Float();

			_tmpNormal[index] = { x, z, y };

			if (_normalIdx % 3 == 0)
				swap(_tmpNormal[_normalIdx - 1], _tmpNormal[_normalIdx - 2]);
		}
		break;

		//---------------------------------------------------------------------------------------------------------
		// OBJECT �� ���̴�. 
		// ���ӿ�����Ʈ�� ����� �־��ְ� ���� �������� �ʱ�ȭ ���ش�.
		// �������� ������ ���Ǵ� ��쵵 �Ǵ��ؼ� �־��ش�
		//---------------------------------------------------------------------------------------------------------
		case TOKENR_MATERIAL_REF:
		{
			_materialNum = Parsing_Int();		// � ���͸����� ��������..
		}
		break;

		// ���� �Ⱦ�
		case TOKEND_END:
			break;

		default:
			break;
		}
	}

	PushMesh();

	//// ���ӿ�����Ʈ�� �ش��ϴ� �� ������Ʈ���� Ǫ���Ѵ�.
	//for (auto& gameObj : _gameObjects)
	//{
	//	shared_ptr<SkinAnimator> skinAnimator = gameObj.second->GetComponent<SkinAnimator>();

	//	// ��Ų�ִϸ����Ͱ� �ִٸ�.. 
	//	if (skinAnimator != nullptr)
	//	{
	//		for (int i = 0; i < skinAnimator->_boneNameList.size(); i++)
	//		{
	//			skinAnimator->PushBoneObj(_gameObjects[skinAnimator->_boneNameList[i]]);

	//			// ���� ���̾�� �ٲ��ٰ��̿���.. ����
	//			//_gameObjects[skinAnimator->_boneNameList[i]]->GetComponent<MeshRenderer>()->GetMesh()->SetRenderState(RESOURCE_MANAGER->GetWireRS());

	//			int materialsize = _gameObjects[skinAnimator->_boneNameList[i]]->GetComponent<MeshRenderer>()->GetMaterial().size();
	//			for (int j = 0; j < materialsize; j++)
	//			{
	//				_gameObjects[skinAnimator->_boneNameList[i]]->GetComponent<MeshRenderer>()->GetMaterial()[j]->SetShader(RESOURCE_MANAGER->GetShader((int)SHADER_TYPE::LIGHT));
	//				_gameObjects[skinAnimator->_boneNameList[i]]->GetComponent<MeshRenderer>()->SetBone(true);	// �޽÷��������� ������� ������ �Ҷ��..
	//			}
	//		}
	//	}
	//}

	return _ASEModel;
}


void ASEParser::Optimize()
{
	_indices.resize(_tmpIndices.size());

	// FACE ������ ��ȸ�ϸ鼭 tmp���ؽ����� ��ü�� �־����ݾ�? �ٵ� ����
	// tmpIndices���� �Ȱ��� ���� ��������! if�б��ϰ� �ٵ� �װ� uv, normal�� ���� ����
	// �ٸ��� �־��ְ�(�ɰ���) ������ �� �־��ش�.
	// [��� : vertex ������ tvert ������ �����ָ� �ȴ�. �ƴϴ� �� �þ ���ִ�. �� ������ �ƴϿ���. �� �������� �ø���� ����]
	// �ش� vertex���ٰ� VertexInfo(uv, normal, index)�� �����ְ�����. 
	vector<vector<VertexInfo>> tmpList;
	tmpList.resize(_numVertex);				// numvertex ��ŭ ������ ������ �� ���� 

	_vertices.resize(_tmpTVert.size());

	int cnt = 0;

	for (int i = 0; i < _tmpIndices.size(); i++)
	{
		// ���� ���ؽ��� ������ �ߺ� �Ǵ��ؼ� �־����� ���� ���ش�.
		if (!tmpList[_tmpIndices[i]].empty())
		{
			bool isUV = false;
			bool isNormal = false;

			// �� ���� �ε��� ã�Ƽ� ������ ����༮���� ã������..
			int sameIndex = 0;

			for (int j = 0; j < tmpList[_tmpIndices[i]].size(); j++)
			{
				if (_tmpTVert.size() != 0)
				{
					if (tmpList[_tmpIndices[i]][j].uv == _tmpTVert[_tmpTFaceIndices[i]])
						isUV = true;
				}
				else
				{
					isUV = true;
				}

				if (tmpList[_tmpIndices[i]][j].normal == _tmpNormal[i])
					isNormal = true;

				if (isUV && isNormal)
				{
					sameIndex = j;	// ��༮�̱���..
					break;
				}

				isUV = false; isNormal = false;
			}

			// ������ continue�� ���� �ʴ´�.
			if (isUV && isNormal)
			{
				// ���� �ε��� �־��ֱ�!! �߿�!
				_indices[i] = tmpList[_tmpIndices[i]][sameIndex].index;		// ��༮�� index �ھ��ֱ�~
				continue;
			}
		}

		_indices[i] = cnt;

		// �ɰ��⸦ ���� ���� �־�����.
		VertexInfo vertexInfo;

		if (_tmpTVert.size() != 0 && cnt < _tmpTVert.size())
			_vertices[cnt].pos = _tmpVertices[_tmpIndices[i]].pos;
		else
			_vertices.push_back(_tmpVertices[_tmpIndices[i]]);

		// TFace�� �ش��ϴ� �ε����� TVert�� �����ͼ� �־��ش�. � TVert�� �����ð���..
		if (_tmpTVert.size() != 0)
		{
			_vertices[cnt].uv = _tmpTVert[_tmpTFaceIndices[i]];
			vertexInfo.uv = _tmpTVert[_tmpTFaceIndices[i]];
		}

		// �װ�Ƽ�� �������� ������Ʈ ���ٸ�..! �븻�� -1 ��������Ѵ�..!
		if (_isNegativeScale)
			_tmpNormal[i] = _tmpNormal[i] * -1.f;

		_vertices[cnt].normal = _tmpNormal[i];
		vertexInfo.normal = _tmpNormal[i];
		vertexInfo.index = cnt;
		tmpList[_tmpIndices[i]].push_back(vertexInfo);


		for (int j = 0; j < 8; j++)
		{
			_vertices[cnt].boneIndices[j] = _tmpVertices[_tmpIndices[i]].boneIndices[j];
			_vertices[cnt].weights[j] = _tmpVertices[_tmpIndices[i]].weights[j];
		}

		cnt++;
	}

	// NormalMap�� ���� tangent ���
	// �븻���ؽ��ĸ� reverse �ϱ⿡�� ���� �̻���������? �׷� 1-v�� ������ϳ�? �ٵ� �ȵǴ°Ͱ����� ��
	// �ϴ� �������븻���� ���ϱ� �������ϴµ� ���̴��� �� �� �޴��� Ȯ���ؾ���?
	for (int i = 0; i < _indices.size(); i += 3) // �ﰢ���� ����
	{
		int i0 = _indices[i];
		int i1 = _indices[i + 1];
		int i2 = _indices[i + 2];

		// e1 = p1 - p0, e2 = p2 - p0
		Vector3 e1 = _vertices[i1].pos - _vertices[i0].pos;
		Vector3 e2 = _vertices[i2].pos - _vertices[i0].pos;

		float x1 = _vertices[i1].uv.x - _vertices[i0].uv.x; // u1 - u0
		float y1 = _vertices[i1].uv.y - _vertices[i0].uv.y; // v1 - v0
		float x2 = _vertices[i2].uv.y - _vertices[i0].uv.x; // u2 - u0
		float y2 = _vertices[i2].uv.y - _vertices[i0].uv.y; // v2 - v0

		float det = (x1 * y2) - (x2 * y1); // ad - bc
		if (det == 0.f) { det = 0.00001f; }
		float r = 1.0f / det;

		Vector3 tangent = (e1 * y2 - e2 * y1) * r;

		_vertices[i0].tangent += tangent;
		_vertices[i1].tangent += tangent;
		_vertices[i2].tangent += tangent;
	}
}

void ASEParser::ResetData()
{
	_vertices.clear();
	_indices.clear();
	_tmpVertices.clear();
	_tmpIndices.clear();
	_tmpTVert.clear();
	_tmpTFaceIndices.clear();
	_tmpNormal.clear();
	vector<NoobData::Vertex>().swap(_vertices);
	vector<unsigned int>().swap(_indices);
	vector<NoobData::Vertex>().swap(_tmpVertices);
	vector<unsigned int>().swap(_tmpIndices);
	vector<Vector2>().swap(_tmpTVert);
	vector<unsigned int>().swap(_tmpTFaceIndices);
	vector<Vector3>().swap(_tmpNormal);

	_materialIndex = 0;
	_materialNum = 0;
	curTexture = 0;
	_numVertex = 0;
	_normalIdx = 0;
	_weightNum = 0;
	_weightCount = 0;
	_normalIdx = 0;

	_isNegativeScale = false;
}

void ASEParser::PushMesh()
{
	if (_parsingMeshInfo != nullptr)
	{
		//_parsingMeshInfo->materials.push_back(materials[_materialNum]);	// ���͸��� �־��ְ�..
		// �ؿ��ɷ� �ϸ�ȴ�.
		// material ID�� REF�� ��Ī�����ֱ����� ����
		_parsingMeshInfo->materialID = _materialNum + _refCheckPoint;
		
		//_materialID = _materialNum + _refCheckPoint;
		// material ref�� �����Ŷ�� isutil ���� �ٲܱ�..?

		Optimize();
		_parsingMeshInfo->vertices = _vertices;
		_parsingMeshInfo->indices = _indices;
		ResetData();
	}
}


string ASEParser::Parsing_String()
{
	_lexer->GetToken(_lineArray);

	return _lexer->GetWord();
}

int ASEParser::Parsing_Int()
{
	_lexer->GetToken(_lineArray);
	int tmpInt = stoi(_lexer->GetWord());

	return tmpInt;
}

float ASEParser::Parsing_Float()
{
	_lexer->GetToken(_lineArray);
	float tmpFloat = stof(_lexer->GetWord());

	return tmpFloat;
}

//void ASEParser::JudgeObjects()
//{
//	if (_isGeomObject == true)
//	{
//		Optimize();									// ����ȭ �ϰ�
//		PushGeomObject();						// ���ӿ�����Ʈ�� ���� �־��ش�.
//	}
//	else if (_isHelperObject == true)
//	{
//		PushHelperObject();
//	}
//	else if (_isShapeObject == true)
//	{
//		PushShapeObject();
//	}
//
//	ResetData();
//}

void ASEParser::DecomposeMatrix(XMMATRIX worldTM, XMFLOAT3& scale, XMFLOAT3& rotation, XMFLOAT3& trans)
{
	XMVECTOR localScaleVec;
	XMVECTOR localRotQuatVec;
	XMVECTOR localTransVec;
	XMVECTOR localRotationVec;
	XMMatrixDecompose(&localScaleVec, &localRotQuatVec, &localTransVec, worldTM);

	// ���ʹϾ��� ���Ϸ������� �ٲٴ� ����..
	double x = localRotQuatVec.m128_f32[0];
	double y = localRotQuatVec.m128_f32[1];
	double z = localRotQuatVec.m128_f32[2];
	double w = localRotQuatVec.m128_f32[3];
	double sqx = x * x;
	double sqy = y * y;
	double sqz = z * z;
	double sqw = w * w;
	float eulerX = asin(2.0f * (w * x - y * z));
	float eulerY = atan2(2.0f * (x * z + w * y), (-sqx - sqy + sqz + sqw));
	float eulerZ = atan2(2.0f * (x * y + z * w), (-sqx + sqy - sqz + sqw));
	rotation = { eulerX, eulerY, eulerZ };

	XMStoreFloat3(&scale, localScaleVec);
	XMStoreFloat3(&trans, localTransVec);
	rotation = { XMConvertToDegrees(rotation.x),
					  XMConvertToDegrees(rotation.y),
					  XMConvertToDegrees(rotation.z) };
}


