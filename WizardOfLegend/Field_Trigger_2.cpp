#include "stdafx.h"
#include "Field_Trigger_2.h"

#include "Cyclops.h"
#include "GhoulLarge.h"
#include "Blob.h"

#include "Transform.h"
#include "Collider.h"
#include "MathLib.h"

#include "SkillItem.h"

#include "EventMgr.h"

CField_Trigger_2::CField_Trigger_2(const OBJ_TYPE& _eType)
	: CSpawnTrigger(_eType)
{
}


CField_Trigger_2::~CField_Trigger_2()
{
}

const bool CField_Trigger_2::Init()
{
	// Set Component
	Create_Transform();
	Create_Collider();

	VEC2 vLT{ 4020, 2800 };
	VEC2 vRB{ 4400, 3200 };
	VEC2 vScale = { vRB.fX - vLT.fX, vRB.fY - vLT.fY };

	m_pTransform->Set_Pos(VEC2{ vLT.fX + vScale.fX * 0.5f, vLT.fY + vScale.fY * 0.5f });
	m_pTransform->Set_Scale(vScale);

	m_pCollider->Set_Pos(VEC2{ vLT.fX + vScale.fX * 0.5f, vLT.fY + vScale.fY * 0.5f });
	m_pCollider->Set_Scale(vScale);

	// Set Spawn Obj
	m_iMaxLevel = 2;
	CObj* pObj = nullptr;

	// Level_1
	vector<CObj*> vecSpawn_1;

	pObj = new CGhoulLarge(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);

	pObj = new CGhoulLarge(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);

	pObj = new CGhoulLarge(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);


	/*pObj = new CBlob(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);


	pObj = new CBlob(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);*/

	m_mapSpawn.insert({ 1, vecSpawn_1 });

	// Level_2
	vector<CObj*> vecSpawn_2;
	pObj = new CCyclops(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_2.push_back(pObj);

	pObj = new CCyclops(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_2.push_back(pObj);

	pObj = new CGhoulLarge(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_2.push_back(pObj);

	pObj = new CGhoulLarge(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_2.push_back(pObj);

	pObj = new CGhoulLarge(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_2.push_back(pObj);


	m_mapSpawn.insert({ 2, vecSpawn_2 });


	m_pDropItem = new CSkillItem(OBJ_TYPE::ITEM, ITEM_TYPE::SKILL);
	m_pDropItem->Init();

	return true;
}

void CField_Trigger_2::Update()
{
}

void CField_Trigger_2::LateUpdate()
{
	Is_EndLevel();
}

void CField_Trigger_2::Render(HDC _hDC)
{
}

const bool CField_Trigger_2::Is_EndLevel()
{
	map<UINT, vector<CObj*>>::iterator mapIter = m_mapSpawn.find(m_iCurLevel);

	if (mapIter == m_mapSpawn.end())
		return false;
	else
	{
		for (vector<CObj*>::iterator iter = mapIter->second.begin(); iter != mapIter->second.end();)
		{
			if ((*iter)->Is_Dead())
				iter = mapIter->second.erase(iter); // 몬스터의 메모리 해제는 몬스터 자기 자신이 호출한다.
			else
				++iter;
		}

		if (mapIter->second.empty()) // 현재 레벨의 모든 오브젝트가 사망
		{
			++m_iCurLevel; // 현재 단계를 1 증가

			if (m_iCurLevel > m_iMaxLevel) // 마지막 단계였다면
			{
				m_pDropItem->Set_Active(true);
				m_pDropItem->Get_Transform()->Set_Pos(VEC2{ m_pTransform->Get_Pos().fX, m_pTransform->Get_Pos().fY - 10 });
				CEventMgr::Get_Inst()->Add_Obj(m_pDropItem);

				CEventMgr::Get_Inst()->Delete_Obj(this); // 자기 자신을 삭제한다.
				return true;
			}
			else
			{
				Spawn_CurLevel();
			}
		}
	}
}
