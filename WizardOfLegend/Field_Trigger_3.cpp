#include "stdafx.h"
#include "Field_Trigger_3.h"

#include "Cyclops.h"
#include "GhoulLarge.h"
#include "Blob.h"

#include "Transform.h"
#include "Collider.h"
#include "MathLib.h"
CField_Trigger_3::CField_Trigger_3(const OBJ_TYPE& _eType)
	: CSpawnTrigger(_eType)
{
}


CField_Trigger_3::~CField_Trigger_3()
{
}

const bool CField_Trigger_3::Init()
{
	// Set Component
	Create_Transform();
	Create_Collider();

	VEC2 vLT{ 1150, 1560 };
	VEC2 vRB{ 1850, 1920 };
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

	pObj = new CBlob(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);

	pObj = new CBlob(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);

	pObj = new CBlob(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_1.push_back(pObj);


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

	pObj = new CBlob(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_2.push_back(pObj);

	pObj = new CBlob(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(CMathLib::Get_Random_Pos(vLT, vRB));
	pObj->Set_Active(false);
	vecSpawn_2.push_back(pObj);


	m_mapSpawn.insert({ 2, vecSpawn_2 });


	return true;
}

void CField_Trigger_3::Update()
{
}

void CField_Trigger_3::LateUpdate()
{
	CSpawnTrigger::Is_EndLevel();
}

void CField_Trigger_3::Render(HDC _hDC)
{
}
