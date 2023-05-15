#include "stdafx.h"
#include "ResultScene.h"
#include "GameMgr.h"
#include "SelectFont.h"
#include "TimeMgr.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "MainGame.h"
#include "SoundMgr.h"

CResultScene::CResultScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos)
	: CScene(_eType, _vStartPos)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	, m_fPlayTime(0.f)
	, m_bAllOn(false)
	, m_bFlash(false)

{
	ZeroMemory(&m_bOn, 5);
}


CResultScene::~CResultScene()
{
}

const bool CResultScene::Init()
{
	return false;
}

void CResultScene::Update()
{
	m_fAcc += DT;

	if (m_bAllOn)
	{
		if (m_fAcc > 0.5f)
		{
			m_bFlash = !m_bFlash;
			m_fAcc = 0.f;
		}
	}

	if (!m_bAllOn)
	{
		if (m_fAcc > 1.f)
		{
			m_bOn[0] = true;
		}
		if (m_fAcc > 1.8f)
		{
			m_bOn[1] = true;
		}
		if (m_fAcc > 2.6f)
		{
			m_bOn[2] = true;
		}
		if (m_fAcc > 3.4f)
		{
			m_bOn[3] = true;
		}
		if (m_fAcc > 4.2f)
		{
			m_bOn[4] = true;
			m_bAllOn = true;
			m_fAcc = 0.f;
		}
	}
}

void CResultScene::LateUpdate()
{
}

void CResultScene::Render(HDC _hDC)
{
	

	Rect rc{ 0, 0, WINCX, WINCY };
	SolidBrush brush(Color(255, 0, 0, 0));

	m_Graphics.FillRectangle(&brush, rc);



	SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::SUPER_LARGE);
	TCHAR tch[128] = {};
	VEC2 tPos{ WINCX * 0.5f, WINCY * 0.5f - 330.f };

	SetTextAlign(_hDC, TA_CENTER);
	swprintf_s(tch, L"C L E A R");
	TextOut(_hDC, int(tPos.fX), int(tPos.fY), tch, _tcsclen(tch));


	if (m_bAllOn && m_bFlash) return;

	SetTextAlign(_hDC, TA_LEFT);

	tPos = { WINCX * 0.5f - 500.f, WINCY * 0.5f - 230.f };

	float fYTerm = 80.f;
	int iYCount = 1;

	if (!m_bOn[0]) return;
	SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::LARGE);
	swprintf_s(tch, L"Attack Volume : %d", CGameMgr::Get_Inst()->Get_Player_Damage());
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));

	if (!m_bOn[1]) return;
	swprintf_s(tch, L"Damaged Volume : %d", CGameMgr::Get_Inst()->Get_Player_Damaged());
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));

	if (!m_bOn[2]) return;
	swprintf_s(tch, L"Play Time : %d : %d", int(m_fPlayTime / 60), int(int(m_fPlayTime) % 60));
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));

	if (!m_bOn[3]) return;
	swprintf_s(tch, L"Kill : %d", CGameMgr::Get_Inst()->Get_Kills());
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));

	if (!m_bOn[4]) return;
	int iTotal = CGameMgr::Get_Inst()->Get_Player_Damage() - CGameMgr::Get_Inst()->Get_Player_Damaged() + CGameMgr::Get_Inst()->Get_Kills() + 10000;
	swprintf_s(tch, L"Total Score : %d", iTotal);
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));
}

void CResultScene::Enter()
{
	CObj* pPlayer = CObjMgr::Get_Inst()->Get_Player();
	pPlayer->Set_Active(false);

	m_fPlayTime = (float)CTimeMgr::Get_Inst()->Get_TotalTime();

	CSoundMgr::Get_Inst()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Inst()->SetChannelVolume(SOUND_BGM, 1.f);
	CSoundMgr::Get_Inst()->PlayBGM(L"Ending.wav", 1.f);
}

void CResultScene::Exit()
{
}
