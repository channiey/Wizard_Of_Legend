#include "stdafx.h"
#include "Scene.h"
#include "ObjMgr.h"
#include "Debug_Render.h"
#include "CameraMgr.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "Animator.h"
#include "SoundMgr.h"

CScene::CScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos)
	: m_eType(_eType)
	, m_vStartPos(_vStartPos)
	, m_fAcc(0.f)
	, m_fPlayerAppearTime(0.5f)
	, m_bPlayerAppear(false)
{
}


CScene::~CScene()
{
}

const bool CScene::Init()
{
	return true;
}

void CScene::Update()
{
	//Appear_Player();

	CObjMgr::Get_Inst()->Update();
}

void CScene::LateUpdate()
{
	CObjMgr::Get_Inst()->LateUpdate();
}

void CScene::Render(HDC _hDC)
{
	CObjMgr::Get_Inst()->Render(_hDC);

	CDebug_Render::Render(_hDC);
}

void CScene::Appear_Player()
{
	if (m_bPlayerAppear) return;

	CObj* pPlayer = CObjMgr::Get_Inst()->Get_Player();

	if (nullptr == pPlayer) return;

	m_fAcc += DT;

	if (m_fAcc >= m_fPlayerAppearTime)
	{
		m_fAcc = 0.f;
		pPlayer->Set_Active(true);
		pPlayer->Get_Animator()->Play(L"PORTAL", true);
		m_bPlayerAppear = true;
		CCameraMgr::Get_Inst()->Shake_Camera(0.2f, 10.f);
		CSoundMgr::Get_Inst()->StopSound(SOUND_PORTAL);
		CSoundMgr::Get_Inst()->PlaySound(L"ExitPortal.wav", SOUND_PORTAL, 1.f);
	}
}
