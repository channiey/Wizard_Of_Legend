#include "stdafx.h"
#include "BossStageMgr.h"
#include "Transform.h"
#include "Collider.h"
#include "Animator.h"
#include "Rigidbody.h"
#include "IceBoss.h"
#include "EventMgr.h"
#include "TimeMgr.h"
#include "CameraMgr.h"
#include "WarningUI.h"
#include "ObjMgr.h"
#include "GameMgr.h"
#include "Projectile.h"
#include "SoundMgr.h"

CBossStageMgr* CBossStageMgr::m_pInst = nullptr;

CBossStageMgr::CBossStageMgr()
	: m_pBoss(nullptr)
	, m_pWarningUI(nullptr)
	, m_fAcc(0.f)
	, m_fWarningStartTime(4.f)
	, m_bWarning()
	, m_bSlow(false)
	, m_bOutScene(false)
	, m_bEnd(false)
{
}


CBossStageMgr::~CBossStageMgr()
{
	Release();
}

const bool CBossStageMgr::Init()
{

	// ���� ���� �� ��Ȱ��ȭ (���� ������Ʈ ����Ʈ�� �߰� X)
	m_pBoss = new CIceBoss(OBJ_TYPE::ENEMY);
	m_pBoss->Init();
	m_pBoss->Get_Transform()->Set_Pos(VEC2{ 1100, 670 });
	m_pBoss->Set_Active(false);

	m_pWarningUI = new CWarningUI(OBJ_TYPE::UI, UI_TYPE::FIXED);
	m_pWarningUI->Init();
	m_pWarningUI->Set_Active(false);

	// Warning UI ���� �� ��Ȱ��ȭ

	return false;
}

void CBossStageMgr::Update()
{
	if (m_bEnd && !CCameraMgr::Get_Inst()->Is_End_FadeOut())
	{
		CEventMgr::Get_Inst()->Change_Scene(SCENE_TYPE::RESULT);

	}
	if (m_bEnd) return;

	if (m_bOutScene) // ���� ����� ���� �ð� ������ ���̵� �ƿ�
	{
		m_fAcc += DT;

		if (m_fAcc >= 2.f)
		{
			CCameraMgr::Get_Inst()->Fade_Out();
			m_bOutScene = false;
			m_bEnd = true;
		}

		return;
	}

	// ���� UI ����
	m_fAcc += DT;
	if (m_fAcc > m_fWarningStartTime && !m_bWarning) 
	{
		m_bWarning = true;
		Display_Warning_UI();
	}

	// ���� ����
	if (m_fAcc > 9.f)
	{
		if (!m_pBoss->Is_Active())
		{
			m_pBoss->Set_Active(true);
			CEventMgr::Get_Inst()->Add_Obj(m_pBoss);
		}
	}


	// ī�޶� �÷��̾����׷� ������

	if (m_fAcc > 12.f)
	{
		if (CCameraMgr::Get_Inst()->Get_Target()->Get_ObjType() == OBJ_TYPE::ENEMY)
		{
			Camera_Move_To_Player();		
		}
	}

}

void CBossStageMgr::LateUpdate()
{
	if (m_bEnd) return;
}

void CBossStageMgr::Release()
{
}

const bool CBossStageMgr::Display_Warning_UI()
{
	if (nullptr == m_pWarningUI || m_pWarningUI->Is_Active()) return false;

	m_pWarningUI->Set_Active(true);
	CEventMgr::Get_Inst()->Add_Obj(m_pWarningUI);

	return true;
}

const bool CBossStageMgr::Evt_Active_Boss()
{
	if (nullptr == m_pBoss) return false;

	m_pBoss->Set_Active(true);
	CEventMgr::Get_Inst()->Add_Obj(m_pBoss);

	return true;
}

const bool CBossStageMgr::Camera_Move_To_Boss()
{ 
	if (nullptr == m_pBoss) return false;

	//CCameraMgr::Get_Inst()->Shake_Camera(1.f, 10.f);

	CCameraMgr::Get_Inst()->Set_LookAt(CCameraMgr::Get_Inst()->Get_RealPos(m_pBoss->Get_Transform()->Get_Pos()), 1.f);
	CCameraMgr::Get_Inst()->Set_Target(m_pBoss);

	CSoundMgr::Get_Inst()->StopSound(SOUND_CAMERA);
	CSoundMgr::Get_Inst()->PlaySound(L"CardSpawn.wav", SOUND_CAMERA, 1.f);

	return true;
}

const bool CBossStageMgr::Camera_Move_To_Player()
{
	CObj* pPlayer = CObjMgr::Get_Inst()->Get_Player();
	
	if (nullptr == pPlayer) return false;

	CCameraMgr::Get_Inst()->Set_LookAt(CCameraMgr::Get_Inst()->Get_RealPos(pPlayer->Get_Transform()->Get_Pos()), 1.f);
	CCameraMgr::Get_Inst()->Set_Target(pPlayer);


	CSoundMgr::Get_Inst()->StopSound(SOUND_CAMERA);
	CSoundMgr::Get_Inst()->PlaySound(L"CardSpawn.wav", SOUND_CAMERA, 1.f);

	return true;
}

const bool CBossStageMgr::Out_Scene()
{
	m_bOutScene = true;

	m_fAcc = 0.f;
	return true;
}
