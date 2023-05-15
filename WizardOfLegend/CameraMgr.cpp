#include "stdafx.h"
#include "CameraMgr.h"
#include "Obj.h"
#include "Transform.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "CameraHurtUI.h"
#include "EventMgr.h"
#include "MainGame.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CCameraMgr* CCameraMgr::m_pInst = nullptr;

CCameraMgr::CCameraMgr()
	: m_pTarget(nullptr)
	, m_bShake(false)
	, m_fAcc(0.f)
	, m_pHurtUI(nullptr)
	, m_fAccHurt(0.f)
	, m_fHurtTime(0.f)
	, m_fMoveTime(0.f)
	, m_fMoveSpeed(0.f)
	, m_fAccMoveTime(0.f)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	, m_bFadeIn(false)
	, m_bFadeOut(false)
	, m_fFadevalue(0.f)
{
	ZeroMemory(&m_vLookAt, sizeof(VEC2));
	ZeroMemory(&m_vCurLookAt, sizeof(VEC2));
	ZeroMemory(&m_vPreLookAt, sizeof(VEC2));
}


CCameraMgr::~CCameraMgr()
{
}

void CCameraMgr::Init()
{
	m_pHurtUI = new CCameraHurtUI(OBJ_TYPE::UI, UI_TYPE::FIXED);
	m_pHurtUI->Init();
	m_pHurtUI->Set_Active(false);
	CEventMgr::Get_Inst()->Add_Obj(m_pHurtUI);
}

void CCameraMgr::Update()
{
	if (m_pTarget)
	{
		if (m_pTarget->Is_Dead())
		{
			m_pTarget = nullptr;
		}
		else
		{
			m_vLookAt = m_pTarget->Get_Transform()->Get_Pos();
			
			Effect();
		}
	}
	else
	{
		Move_WithArrowKey();
	}

	Limit_Pos();

	Calculate_Diff(); // 화면 중앙 좌표와 카메라 lookAt 좌표 간의 차이값 계산
}

void CCameraMgr::Render(HDC _hDC)
{
	if (m_bFadeIn)
	{
		m_fFadevalue += DT* 0.1f;
		if (m_fFadevalue >= 255.f)
		{
			m_fFadevalue = 255.f;
			m_bFadeOut = false;
		}
		

		Rect rc{ 0, 0, WINCX, WINCY };
		SolidBrush brush(Color(int(m_fFadevalue), 0, 0, 0));
		m_Graphics.FillRectangle(&brush, rc);
	}
	else if (m_bFadeOut)
	{
		m_fFadevalue += DT * 100.f;

		if (m_fFadevalue >= 255.f)
		{
			m_fFadevalue = 255.f;
			m_bFadeOut = false;
		}
		


		CSoundMgr::Get_Inst()->SetChannelVolume(SOUND_BGM, (255 - m_fFadevalue) / 255);

		Rect rc{ 0, 0, WINCX, WINCY };
		SolidBrush brush(Color(int(m_fFadevalue), 0, 0, 0));
		m_Graphics.FillRectangle(&brush, rc);
	}


}

void CCameraMgr::Calculate_Diff()
{
	//VEC2 vRes{ WINCX, WINCY };
	//VEC2 vCenter = vRes / 2;
	//
	//m_vDiff = m_vLookAt - vCenter;


	/////////////////////////////

	m_fAccMoveTime += DT;

	if (m_fMoveTime <= m_fAccMoveTime)
	{
		m_vCurLookAt = m_vLookAt;
	}
	else
	{
		VEC2 vLookDir = m_vLookAt - m_vPreLookAt; // Set할 Look으로의 방향을 구한다
		vLookDir.Normalize();

		m_vCurLookAt = m_vPreLookAt + vLookDir * m_fMoveSpeed * DT;
	}

	VEC2 vCenter = VEC2(WINCX * 0.5f, WINCY * 0.5f);

	m_vDiff = m_vCurLookAt - vCenter;

	m_vPreLookAt = m_vCurLookAt;
}

void CCameraMgr::Move_WithArrowKey()
{
	float fSpeed = 1000.f;

	if (CInputMgr::Get_Inst()->Key_Hold('W'))
		m_vLookAt.fY -= fSpeed * DT;
	else if (CInputMgr::Get_Inst()->Key_Hold('S'))
		m_vLookAt.fY += fSpeed * DT;
	else if (CInputMgr::Get_Inst()->Key_Hold('A'))
		m_vLookAt.fX -= fSpeed * DT;
	else if (CInputMgr::Get_Inst()->Key_Hold('D'))
		m_vLookAt.fX += fSpeed * DT;
}

const bool CCameraMgr::Set_Target(CObj* _pObj)
{
	if (nullptr == _pObj)
		return false;

	m_pTarget = _pObj;

	return true;
}

void CCameraMgr::Set_LookAt(const VEC2 & _vPos, const float& _fMoveTime)
{
	m_vLookAt = _vPos;
	
	m_fMoveTime = _fMoveTime;

	float fMoveDis = (m_vLookAt - m_vPreLookAt).Length(); // 이동 거리 구함

	m_fMoveSpeed = fMoveDis / m_fMoveTime; // 초당 이동 스피드 구함

	m_fAccMoveTime = 0.f;
}

void CCameraMgr::Apply_Hurt()
{
	m_fAccHurt += DT;
	if (m_fAccHurt >= m_fHurtTime)
	{
		m_bHurt = false;
		m_fAccHurt = 0.f;

		m_pHurtUI->Set_Active(false);
		return;
	}

	if(!m_pHurtUI->Is_Active())
		m_pHurtUI->Set_Active(true);
}

void CCameraMgr::Effect()
{
	if (m_bShake) Apply_Shake();
	if (m_bHurt) Apply_Hurt();
}

void CCameraMgr::Limit_Pos()
{
	int iMapCX, iMapCY = 0;
	CScene* pCurScene = CSceneMgr::Get_Inst()->Get_CurScene();

	if (nullptr == pCurScene) return;

	if (SCENE_TYPE::VILLAGE == pCurScene->Get_SceneType())
	{

	}
	else if (SCENE_TYPE::FIELD == pCurScene->Get_SceneType())
	{
		iMapCX = 6515;
		iMapCY = 4748;
	}
	else if (SCENE_TYPE::BOSS == pCurScene->Get_SceneType())
	{
		iMapCX = 2197;
		iMapCY = 1737;
	}
	else
	{
		return;
	}


	// Limit X
	if (m_vLookAt.fX - WINCX * 0.5f < 0)
		m_vLookAt.fX = WINCX * 0.5f;
	else if(m_vLookAt.fX + WINCX * 0.5f > iMapCX)
		m_vLookAt.fX = iMapCX - (WINCX * 0.5f);
	
	// Limit Y
	if(m_vLookAt.fY - WINCY * 0.5f < 0)
		m_vLookAt.fY = WINCY * 0.5f;
	else if(m_vLookAt.fY + WINCY * 0.5f > iMapCY)
		m_vLookAt.fY = iMapCY - (WINCY * 0.5f);
}

void CCameraMgr::Fade_In(const float & _fTime)
{
	m_fFadevalue = 255.f;
	m_bFadeIn = true;
}

void CCameraMgr::Fade_Out(const float & _fTime)
{
	m_fFadevalue = 0.f;
	m_bFadeOut = true;
}

void CCameraMgr::Apply_Shake()
{
	m_fAcc += DT;
	if (m_fAcc >= m_fShakeTime)
	{
		m_bShake = false;
		m_fAcc = 0.f;
	}

	VEC2 vShakeOffset = VEC2(rand() % (int)m_fIntensity, rand() % (int)m_fIntensity);
	m_vLookAt += vShakeOffset;
}
