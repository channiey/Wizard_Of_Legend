#include "stdafx.h"
#include "BossSpawnTrigger.h"
#include "IceBoss.h"
#include "Transform.h"
#include "Collider.h"
#include "MathLib.h"
#include "BossStatUI.h"

CBossSpawnTrigger::CBossSpawnTrigger(const OBJ_TYPE& _eType)
	: CSpawnTrigger(_eType)
	, pBossUI(nullptr)
{
}


CBossSpawnTrigger::~CBossSpawnTrigger()
{
	
}

const bool CBossSpawnTrigger::Init()
{
	// Set Component
	Create_Transform();
	Create_Collider();

	VEC2 vLT{ 3000, 2200 };
	VEC2 vRB{ 3600, 2900 };
	VEC2 vScale = { vRB.fX - vLT.fX - 200, vRB.fY - vLT.fY - 200 };

	m_pTransform->Set_Pos(VEC2{ vLT.fX + vScale.fX * 0.5f, vLT.fY + vScale.fY * 0.5f });
	m_pTransform->Set_Scale(vScale);

	m_pCollider->Set_Pos(VEC2{ vLT.fX + vScale.fX * 0.5f, vLT.fY + vScale.fY * 0.5f });
	m_pCollider->Set_Scale(vScale);

	// Set Spawn Obj
	m_iMaxLevel = 1;
	CObj* pObj = nullptr;

	// Level_1
	vector<CObj*> vecSpawn_1;

	pObj = new CIceBoss(OBJ_TYPE::ENEMY);
	pObj->Init();
	pObj->Get_Transform()->Set_Pos(VEC2{ 3344, 2563 });
	pObj->Set_Active(false);

	vecSpawn_1.push_back(pObj);

	m_mapSpawn.insert({ 1, vecSpawn_1 });
	
	return true;
}

void CBossSpawnTrigger::Update()
{
}

void CBossSpawnTrigger::LateUpdate()
{
	CSpawnTrigger::Is_EndLevel();
}

void CBossSpawnTrigger::Render(HDC _hDC)
{
}
