#include "stdafx.h"
#include "AttackState2_IceBoss.h"
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
#include "ObjPool.h"
#include "SoundMgr.h"

CAttackState2_IceBoss::CAttackState2_IceBoss()
	: CState(L"ATTACK2")
{
}


CAttackState2_IceBoss::~CAttackState2_IceBoss()
{
}

void CAttackState2_IceBoss::Enter()
{
	m_fAimTime = 2.f;

	m_fFireTime = 2.f; //2

	m_bFired = false;

	m_bAim = false;

	m_bPlus1 = false;
	m_bPlus2 = false;

}

void CAttackState2_IceBoss::Update()
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

	if (m_bFired && !m_bPlus1 && m_fAcc > 0.3f)
	{
		m_bPlus1 = true;
		Fire();
	}
	if (m_bFired && !m_bPlus2 && m_fAcc > 0.6f)
	{
		m_bPlus2 = true;
		Fire();
	}

	if (m_bFired && m_fAcc >= m_fFireTime)
	{
		CEventMgr::Get_Inst()->Change_AIState(Get_AI(), L"ATTACK3");

		m_fAcc = 0.f;

	}
}

void CAttackState2_IceBoss::Render(HDC _hDC)
{
	if (!m_bAim) return;

	VEC2 vOwnerPos = CCameraMgr::Get_Inst()->Get_RenderPos(Get_Owner()->Get_Transform()->Get_Pos());
	VEC2 vPlayerPos = CCameraMgr::Get_Inst()->Get_RenderPos(CObjMgr::Get_Inst()->Get_Player()->Get_Transform()->Get_Pos());

	Graphics g(CMainGame::Get_Inst()->Get_SubDC());
	Pen pen(Color(200, 255, 0, 0), 2.f);

	g.DrawLine(&pen, PointF(vOwnerPos.fX, vOwnerPos.fY), PointF(vPlayerPos.fX, vPlayerPos.fY));
}

void CAttackState2_IceBoss::Exit()
{
}

void CAttackState2_IceBoss::Fire()
{
	CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
	CSoundMgr::Get_Inst()->PlaySound(L"IceSwordSwing.wav", SOUND_BOSS, 1.f);

	if (m_bAim)
		m_bAim = false;

	VEC2 vOwnerPos = Get_Owner()->Get_Transform()->Get_Pos();
	VEC2 vPlayerPos = CObjMgr::Get_Inst()->Get_Player()->Get_Transform()->Get_Pos();

	CIceLance_P* pProj = static_cast<CIceLance_P*>(CObjPool<CIceLance_P>::Get_Obj());

	if (nullptr == pProj) return;

	float fTheta = CMathLib::Get_Angle(vOwnerPos, vPlayerPos);

	pProj->Reset(Get_Owner(), Get_Owner()->Get_Transform()->Get_Pos(), DF_AD, fTheta);

}

void CAttackState2_IceBoss::Aim()
{
	if (!m_bAim)
		m_bAim = true;
}

void CAttackState2_IceBoss::Move()
{
	VEC2 vOwnerPos = Get_Owner()->Get_Transform()->Get_Pos();
	VEC2 vTargetPos = CObjMgr::Get_Inst()->Get_Player()->Get_Transform()->Get_Pos();

	VEC2 vLook = vTargetPos - vOwnerPos;
	vLook.Normalize();


	Get_Owner()->Get_Transform()->Add_Pos(vLook * Get_Owner()->Get_Speed() *DT);
}

