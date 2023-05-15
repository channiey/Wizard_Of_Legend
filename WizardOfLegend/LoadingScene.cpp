#include "stdafx.h"
#include "LoadingScene.h"
#include "TimeMgr.h"
#include "EventMgr.h"
#include "SelectFont.h"
#include "MainGame.h"

CLoadingScene::CLoadingScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos)
	: CScene(_eType, _vStartPos)
	, m_fLoadingTime(2.f)
	, m_fAcc(0.f)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
{
}


CLoadingScene::~CLoadingScene()
{
}

const bool CLoadingScene::Init()
{
	return true;
}

void CLoadingScene::Update()
{
	CScene::Update();

	m_fAcc += DT;
	if (m_fAcc >= m_fLoadingTime)
	{
		m_fAcc = 0.f;
		CEventMgr::Get_Inst()->Change_Scene(SCENE_TYPE::FIELD);
	}
}

void CLoadingScene::LateUpdate()
{
	CScene::LateUpdate();
}

void CLoadingScene::Render(HDC _hDC)
{
	CScene::Render(_hDC);

	Rect rc{ 0, 0, WINCX, WINCY };
	SolidBrush brush(Color(255, 0, 0, 0));
	m_Graphics.FillRectangle(&brush, rc);


	SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::MEDIUM);

	TCHAR tch[128] = {};
	VEC2 tPos{ WINCX * 0.5f, WINCY*0.5f };

	SetBkMode(_hDC, TRANSPARENT);
	SetTextAlign(_hDC, TA_CENTER);

	swprintf_s(tch, L"Loading...");
	TextOut(_hDC, int(tPos.fX), int(tPos.fY), tch, _tcsclen(tch));
}

void CLoadingScene::Enter()
{
}

void CLoadingScene::Exit()
{
}
