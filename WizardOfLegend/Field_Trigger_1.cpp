#include "stdafx.h"
#include "Field_Trigger_1.h"

#include "Cyclops.h"
#include "GhoulLarge.h"
#include "Blob.h"

#include "Transform.h"
#include "Collider.h"
#include "MathLib.h"
// 테스트 적 생성
/*CObj* pEnemy1 = new CCyclops(OBJ_TYPE::ENEMY);
pEnemy1->Init();
CEventMgr::Get_Inst()->Add_Obj(pEnemy1);*/

/*CObj* pEnemy2 = new CGhoulLarge(OBJ_TYPE::ENEMY);
pEnemy2->Init();
CEventMgr::Get_Inst()->Add_Obj(pEnemy2);*/

/*CObj* pEnemy3 = new CBlob(OBJ_TYPE::ENEMY);
pEnemy3->Init();
CEventMgr::Get_Inst()->Add_Obj(pEnemy3);*/
CField_Trigger_1::CField_Trigger_1(const OBJ_TYPE& _eType)
	: CSpawnTrigger(_eType)
{
}


CField_Trigger_1::~CField_Trigger_1()
{
}

const bool CField_Trigger_1::Init()
{// Set Component
	Create_Transform();
	Create_Collider();

	VEC2 vLT{ 4800, 1480 };
	VEC2 vRB{ 5550, 2040 };
	VEC2 vScale = { vRB.fX - vLT.fX, vRB.fY - vLT.fY };

	m_pTransform->Set_Pos(VEC2{ vLT.fX + vScale.fX * 0.5f, vLT.fY + vScale.fY * 0.5f });
	m_pTransform->Set_Scale(vScale);

	m_pCollider->Set_Pos(VEC2{ vLT.fX + vScale.fX * 0.5f, vLT.fY + vScale.fY * 0.5f });
	m_pCollider->Set_Scale(vScale);

	// Set Spawn Obj
	m_iMaxLevel = 1;
	CObj* pObj = nullptr;

	// Level_1
	vector<CObj*> vecSpawn_1;

	pObj = new CCyclops(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);

	pObj = new CCyclops(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);

	pObj = new CCyclops(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);

	m_mapSpawn.insert({ 1, vecSpawn_1 });

	//// Level_2
	//vector<CObj*> vecSpawn_2;

	//pObj = new CGhoulLarge(OBJ_TYPE::ENEMY);
	//pObj->Init();
	//pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	//pObj->Set_Active(false);
	//vecSpawn_2.push_back(pObj);

	//pObj = new CGhoulLarge(OBJ_TYPE::ENEMY);
	//pObj->Init();
	//pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	//pObj->Set_Active(false);
	//vecSpawn_2.push_back(pObj);

	//m_mapSpawn.insert({ 2, vecSpawn_2 });


	return true;
}

void CField_Trigger_1::Update()
{
}

void CField_Trigger_1::LateUpdate()
{
	CSpawnTrigger::Is_EndLevel();
}

void CField_Trigger_1::Render(HDC _hDC)
{
	Component_Render(_hDC);
}
