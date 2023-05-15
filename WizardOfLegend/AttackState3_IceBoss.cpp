#include "stdafx.h"
#include "AttackState3_IceBoss.h"
#include "EventMgr.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "MainGame.h"
#include "Transform.h"
#include "Obj.h"
#include "Enemy.h"
#include "IceLance_P.h"
#include "MathLib.h"
#include "CameraMgr.h"
#include "IceSword_P.h"
#include "ObjPool.h"
#include "IceSword_P.h"
#include "SoundMgr.h"

CAttackState3_IceBoss::CAttackState3_IceBoss()
	: CState(L"ATTACK3")
{
}

CAttackState3_IceBoss::~CAttackState3_IceBoss()
{
}

void CAttackState3_IceBoss::Enter()
{
	m_fAimTime = 2.f;

	m_fFireTime = 2.f; 

	m_bFired = false;

	m_bAim = false;

	m_bPlus = false;
	
	float fTerm = 70;

	// 1
	VEC2 v1{ 0, WINCY * 0.25f + fTerm };
	VEC2 v2{ 0, WINCY * 0.25f - fTerm };

	// 2
	VEC2 v3{ WINCX * 0.25f + fTerm, 0 };
	VEC2 v4{ WINCX * 0.25f - fTerm, 0 };

	// 3
	VEC2 v5{WINCX, WINCY * 0.75f + fTerm };
	VEC2 v6{ WINCX, WINCY * 0.75f - fTerm };

	// 4
	VEC2 v7{WINCX * 0.75f + fTerm, WINCY };
	VEC2 v8{ WINCX * 0.75f - fTerm, WINCY};

	m_vStartPos.push_back(v1);
	m_vStartPos.push_back(v2);
	m_vStartPos.push_back(v3);
	m_vStartPos.push_back(v4);
	m_vStartPos.push_back(v5);
	m_vStartPos.push_back(v6);
	m_vStartPos.push_back(v7);
	m_vStartPos.push_back(v8);
}

void CAttackState3_IceBoss::Update()
{
	m_fAcc += DT;

	if (!m_bFired && m_fAcc < m_fAimTime)
	{
		Aim();
		Move();
	}

	if (m_fAcc >= m_fAimTime)
	{
		//Get_Owner()->Get_Animator()->Play(L"SLASH", false);

		m_fAcc = 0.f;
		m_fAimTime = 100.f;

		m_bFired = true;

		Fire();
	}

	/*if (m_bFired && !m_bPlus && m_fAcc > 0.5f)
	{
		m_bPlus = true;
		Fire();
	}*/
	
	if (m_bFired && m_fAcc >= m_fFireTime)
	{
		CEventMgr::Get_Inst()->Change_AIState(Get_AI(), L"IDLE");

		m_fAcc = 0.f;

	}
}

void CAttackState3_IceBoss::Render(HDC _hDC)
{
	if (!m_bAim) return;
	

	float fTerm = 60;

	Graphics g(CMainGame::Get_Inst()->Get_SubDC());
	Pen pen(Color(255, 255, 0, 0), 2.f);

	VEC2 v1{ 0, WINCY * 0.25f - fTerm };
	VEC2 v2{ 0, WINCY * 0.25f + fTerm };

	VEC2 v3{ WINCX * 0.25f - fTerm, 0 };
	VEC2 v4{ WINCX * 0.25f + fTerm, 0 };

	VEC2 v5{ WINCX, WINCY * 0.75f - fTerm };
	VEC2 v6{ WINCX, WINCY * 0.75f + fTerm };
	
	VEC2 v7{ WINCX * 0.75f - fTerm, WINCY };
	VEC2 v8{ WINCX * 0.75f + fTerm, WINCY };
	
	//=====

	VEC2 v11{ WINCX, WINCY * 0.25f - fTerm };
	VEC2 v12{ WINCX, WINCY * 0.25f + fTerm };

	VEC2 v13{ WINCX * 0.25f - fTerm, WINCY };
	VEC2 v14{ WINCX * 0.25f + fTerm, WINCY };

	VEC2 v15{ 0, WINCY * 0.75f - fTerm };
	VEC2 v16{ 0, WINCY * 0.75f + fTerm };

	VEC2 v17{ WINCX * 0.75f - fTerm, 0 };
	VEC2 v18{ WINCX * 0.75f + fTerm, 0 };

	g.DrawLine(&pen, PointF(v1.fX, v1.fY), PointF(v11.fX, v11.fY));
	g.DrawLine(&pen, PointF(v2.fX, v2.fY), PointF(v12.fX, v12.fY));
	g.DrawLine(&pen, PointF(v3.fX, v3.fY), PointF(v13.fX, v13.fY));
	g.DrawLine(&pen, PointF(v4.fX, v4.fY), PointF(v14.fX, v14.fY));
	g.DrawLine(&pen, PointF(v5.fX, v5.fY), PointF(v15.fX, v15.fY));
	g.DrawLine(&pen, PointF(v6.fX, v6.fY), PointF(v16.fX, v16.fY));
	g.DrawLine(&pen, PointF(v7.fX, v7.fY), PointF(v17.fX, v17.fY));
	g.DrawLine(&pen, PointF(v8.fX, v8.fY), PointF(v18.fX, v18.fY));

}

void CAttackState3_IceBoss::Exit()
{

}

void CAttackState3_IceBoss::Fire()
{
	// IceBeam
	CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
	CSoundMgr::Get_Inst()->PlaySound(L"IceBeam.wav", SOUND_BOSS, 1.f);

	if (m_bAim)
		m_bAim = false;

	CIceSword_P* pProj;

	pProj = static_cast<CIceSword_P*>(CObjPool<CIceSword_P>::Get_Obj());
	if (nullptr == pProj) return;
	pProj->Reset(Get_Owner(), CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[0]), DF_AD, 0.f);

	pProj = static_cast<CIceSword_P*>(CObjPool<CIceSword_P>::Get_Obj());
	if (nullptr == pProj) return;
	pProj->Reset(Get_Owner(), CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[1]), DF_AD, 0.f);

	pProj = static_cast<CIceSword_P*>(CObjPool<CIceSword_P>::Get_Obj());
	if (nullptr == pProj) return;
	pProj->Reset(Get_Owner(), CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[2]), DF_AD, 270 * PI / 180.f);

	pProj = static_cast<CIceSword_P*>(CObjPool<CIceSword_P>::Get_Obj());
	if (nullptr == pProj) return;
	pProj->Reset(Get_Owner(), CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[3]), DF_AD, 270 * PI / 180.f);

	pProj = static_cast<CIceSword_P*>(CObjPool<CIceSword_P>::Get_Obj());
	if (nullptr == pProj) return;
	pProj->Reset(Get_Owner(), CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[4]), DF_AD, 180 * PI / 180.f);

	pProj = static_cast<CIceSword_P*>(CObjPool<CIceSword_P>::Get_Obj());
	if (nullptr == pProj) return;
	pProj->Reset(Get_Owner(), CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[5]), DF_AD, 180 * PI / 180.f);

	pProj = static_cast<CIceSword_P*>(CObjPool<CIceSword_P>::Get_Obj());
	if (nullptr == pProj) return;
	pProj->Reset(Get_Owner(), CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[6]), DF_AD, 90 * PI / 180.f);

	pProj = static_cast<CIceSword_P*>(CObjPool<CIceSword_P>::Get_Obj());
	if (nullptr == pProj) return;
	pProj->Reset(Get_Owner(), CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[7]), DF_AD, 90 * PI / 180.f);

	/*if (!m_bPlus)
	{
		pProj = new CIceSword_P(OBJ_TYPE::PROJECTILE, 20.f, Get_Owner());
		pProj->Init();
		pProj->Get_Transform()->Set_Theta(0);
		pProj->Get_Transform()->Set_Pos(CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[0]));
		CEventMgr::Get_Inst()->Add_Obj(pProj);

		pProj = new CIceSword_P(OBJ_TYPE::PROJECTILE, 20.f, Get_Owner());
		pProj->Init();
		pProj->Get_Transform()->Set_Theta(0);
		pProj->Get_Transform()->Set_Pos(CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[1]));
		CEventMgr::Get_Inst()->Add_Obj(pProj);


		pProj = new CIceSword_P(OBJ_TYPE::PROJECTILE, 20.f, Get_Owner());
		pProj->Init();
		pProj->Get_Transform()->Set_Theta(270 * PI / 180.f);
		pProj->Get_Transform()->Set_Pos(CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[2]));
		CEventMgr::Get_Inst()->Add_Obj(pProj);

		pProj = new CIceSword_P(OBJ_TYPE::PROJECTILE, 20.f, Get_Owner());
		pProj->Init();
		pProj->Get_Transform()->Set_Theta(270 * PI / 180.f);
		pProj->Get_Transform()->Set_Pos(CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[3]));
		CEventMgr::Get_Inst()->Add_Obj(pProj);
	}
	else
	{
		pProj = new CIceSword_P(OBJ_TYPE::PROJECTILE, 20.f, Get_Owner());
		pProj->Init();
		pProj->Get_Transform()->Set_Theta(180 * PI / 180.f);
		pProj->Get_Transform()->Set_Pos(CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[4]));
		CEventMgr::Get_Inst()->Add_Obj(pProj);

		pProj = new CIceSword_P(OBJ_TYPE::PROJECTILE, 20.f, Get_Owner());
		pProj->Init();
		pProj->Get_Transform()->Set_Theta(180 * PI / 180.f);
		pProj->Get_Transform()->Set_Pos(CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[5]));
		CEventMgr::Get_Inst()->Add_Obj(pProj);

		pProj = new CIceSword_P(OBJ_TYPE::PROJECTILE, 20.f, Get_Owner());
		pProj->Init();
		pProj->Get_Transform()->Set_Theta(90 * PI / 180.f);
		pProj->Get_Transform()->Set_Pos(CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[6]));
		CEventMgr::Get_Inst()->Add_Obj(pProj);

		pProj = new CIceSword_P(OBJ_TYPE::PROJECTILE, 20.f, Get_Owner());
		pProj->Init();
		pProj->Get_Transform()->Set_Theta(90 * PI / 180.f);
		pProj->Get_Transform()->Set_Pos(CCameraMgr::Get_Inst()->Get_RealPos(m_vStartPos[7]));
		CEventMgr::Get_Inst()->Add_Obj(pProj);
	}*/
}

void CAttackState3_IceBoss::Aim()
{
	if (!m_bAim)
		m_bAim = true;
}

void CAttackState3_IceBoss::Move()
{
	VEC2 vOwnerPos = Get_Owner()->Get_Transform()->Get_Pos();
	VEC2 vTargetPos = CObjMgr::Get_Inst()->Get_Player()->Get_Transform()->Get_Pos();

	VEC2 vLook = vTargetPos - vOwnerPos;
	vLook.Normalize();


	Get_Owner()->Get_Transform()->Add_Pos(vLook * Get_Owner()->Get_Speed() *DT);
}

