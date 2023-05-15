#include "stdafx.h"
#include "MainGame.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "EventMgr.h"
#include "ResMgr.h"
#include "ObjMgr.h"
#include "CameraMgr.h"
#include "SceneMgr.h"
#include "MapMgr.h"
#include "CollisionMgr.h"
#include "Debug_Render.h"
#include "BmpMgr.h"
#include "SoundMgr.h"
#include "UIMgr.h"
#include "ObjPool.h"
#include "DamageUI.h"
#include "ExplodingFireball_P.h"
#include "DragonArc_P.h"
#include "IceKatana_P.h"
#include "IceLance_P.h"
#include "IceSword_P.h"
#include "BossStageMgr.h"
#include "WaterBomb_P.h"
#include "WindSlash_P.h"
#include "FireParticle.h"
#include "SnowParticle.h"

// Life Cycle
// Init -> Update -> LateUpdate(Obj, Component) -> 충돌 검사 및 처리 (OnCollision) -> Render -> Event Update

CMainGame* CMainGame::m_pInst = nullptr;

CMainGame::CMainGame()
{

}

CMainGame::~CMainGame()
{
	Release();
}

const bool CMainGame::Init()
{
	// Window
	m_DC = GetDC(g_hWnd);
	m_RC = { 0, 0, WINCX, WINCY };
	m_bitMap = CreateCompatibleBitmap(m_DC, m_RC.right, m_RC.bottom);
	AdjustWindowRect(&m_RC, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(g_hWnd, nullptr, 100, 100, (m_RC.right - m_RC.left), (m_RC.bottom - m_RC.top), 0);
	m_subDC = CreateCompatibleDC(m_DC);
	HBITMAP hOldbit = (HBITMAP)SelectObject(m_subDC, m_bitMap);
	DeleteObject(hOldbit);

	//Create Font
	Create_Font();

	// Init Manager
	CSoundMgr::Get_Inst()->Init();
	CTimeMgr::Get_Inst()->Init();
	CInputMgr::Get_Inst()->Init();
	CSceneMgr::Get_Inst()->Init();
	CCollisionMgr::Get_Inst()->Init();
	CCameraMgr::Get_Inst()->Init();
	CBossStageMgr::Get_Inst()->Init();

	// Pool Init
	Init_Pool();

	return true;
}

void CMainGame::Progress()
{
	// Update
	CTimeMgr::Get_Inst()->Update();
	CCameraMgr::Get_Inst()->Update();
	CSceneMgr::Get_Inst()->Update();

	// Late 
	CSceneMgr::Get_Inst()->LateUpdate();
	CCollisionMgr::Get_Inst()->Update();
	//CUIMgr::Get_Inst()->Update(); // UI 이벤트 체크

	//Render
	Rectangle(m_subDC, -1, -1, m_RC.right + 1, m_RC.bottom + 1);
	CSceneMgr::Get_Inst()->Render(m_subDC);
	CCameraMgr::Get_Inst()->Render(m_subDC);
	BitBlt(m_DC, 0, 0, m_RC.right, m_RC.bottom, m_subDC, 0, 0, SRCCOPY);

	// Event
	CEventMgr::Get_Inst()->Update();
}

void CMainGame::Release()
{
	Delete_Font();

	CObjMgr::Get_Inst()->Destroy_Inst();
	CEventMgr::Get_Inst()->Destroy_Inst();
	CTimeMgr::Get_Inst()->Destroy_Inst();
	CInputMgr::Get_Inst()->Destroy_Inst();
	CSceneMgr::Get_Inst()->Destroy_Inst();
	CResMgr::Get_Inst()->Destroy_Inst();
	CCameraMgr::Get_Inst()->Destroy_Inst();
	CMapMgr::Get_Inst()->Destroy_Inst();
	CCollisionMgr::Get_Inst()->Destroy_Inst();
	CBmpMgr::Get_Inst()->Destroy_Inst();
	CSoundMgr::Get_Inst()->Destroy_Inst();
	CUIMgr::Get_Inst()->Destroy_Inst();
	CBossStageMgr::Get_Inst()->Destroy_Inst();
}

void CMainGame::Create_Font()
{
	// 내 컴퓨터에 설치되었더라도 다른 컴퓨터에서는 Data 폴더에서 불러와야한다.
	AddFontResource(L"slkscr.ttf");
	AddFontResource(L"slkLato-Regularscr.ttf");

	int iS = 15;
	int iM = 20; // TITLE_SCENE_PRESSKEY
	int iL = 23; // TITLE_SCENE_MENU
	int iXL = 30; // TITLE_SCENE_MENU
	int iXXL = 65; // RESULT
	float fMag = 1.f;

	m_vecFont[(UINT)FONT_TYPE::DEFAULT].push_back(CreateFont(int(iS + (iS * fMag)), iS, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial"));
	m_vecFont[(UINT)FONT_TYPE::DEFAULT].push_back(CreateFont(int(iM + (iM * fMag)), iM, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial"));
	m_vecFont[(UINT)FONT_TYPE::DEFAULT].push_back(CreateFont(int(iL + (iL * fMag)), iL, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial"));
	m_vecFont[(UINT)FONT_TYPE::DEFAULT].push_back(CreateFont(int(iXL + (iXL * fMag)), iXL, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial"));
	m_vecFont[(UINT)FONT_TYPE::DEFAULT].push_back(CreateFont(int(iXXL + (iXXL * fMag)), iXXL, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Arial"));

	m_vecFont[(UINT)FONT_TYPE::GOTHIC].push_back(CreateFont(int(iS + (iS * fMag)), iS, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Lato"));
	m_vecFont[(UINT)FONT_TYPE::GOTHIC].push_back(CreateFont(int(iM + (iM * fMag)), iM, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Lato"));
	m_vecFont[(UINT)FONT_TYPE::GOTHIC].push_back(CreateFont(int(iL + (iL * fMag)), iL, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Lato"));
	m_vecFont[(UINT)FONT_TYPE::GOTHIC].push_back(CreateFont(int(iXL + (iXL * fMag)), iXL, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Lato"));
	m_vecFont[(UINT)FONT_TYPE::GOTHIC].push_back(CreateFont(int(iXXL + (iXXL * fMag)), iXXL, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Lato"));

	m_vecFont[(UINT)FONT_TYPE::RETRO].push_back(CreateFont(int(iS + (iS * fMag)), iS, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Silkscreen"));
	m_vecFont[(UINT)FONT_TYPE::RETRO].push_back(CreateFont(int(iM + (iM * fMag)), iM, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Silkscreen"));
	m_vecFont[(UINT)FONT_TYPE::RETRO].push_back(CreateFont(int(iL + (iL * fMag)), iL, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Silkscreen"));
	m_vecFont[(UINT)FONT_TYPE::RETRO].push_back(CreateFont(int(iXL + (iXL * fMag)), iXL, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Silkscreen"));
	m_vecFont[(UINT)FONT_TYPE::RETRO].push_back(CreateFont(int(iXXL + (iXXL * fMag)), iXXL, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, L"Silkscreen"));

	// http://www.soen.kr/lecture/win32api/reference/Function/CreateFont.htm
}

void CMainGame::Delete_Font()
{
	RemoveFontResource(L"slkscr.ttf");
	RemoveFontResource(L"slkLato-Regularscr.ttf");
}

void CMainGame::Init_Pool()
{
	// UI
	{
		// Obj
		CObjPool<CDamageUI>::Init(OBJ_TYPE::UI, 200);
	}
	
	// Proj
	{
		// Player Proj
		{
			CObjPool<ExplodingFireball_P>::Init(OBJ_TYPE::PROJECTILE, 20);	
			CObjPool<CDragonArc_P>::Init(OBJ_TYPE::PROJECTILE, 20);
			CObjPool<CWaterBomb_P>::Init(OBJ_TYPE::PROJECTILE, 20);
			CObjPool<CWindSlash_P>::Init(OBJ_TYPE::PROJECTILE, 700); // 미안하다...... 
			// AirBurst와 WindSlash는 플레이어 Init시 호출되고 매번 할당이 아닌 자체 OnOff 방식으로 작동하기 때문에
			// 굳이 풀을 사용하지 않는다.
		}
		
		// Enemy Proj
		{
			// Ice Boss
			{
				CObjPool<CIceKatana_P>::Init(OBJ_TYPE::PROJECTILE, 20);
				CObjPool<CIceLance_P>::Init(OBJ_TYPE::PROJECTILE, 10);
				CObjPool<CIceSword_P>::Init(OBJ_TYPE::PROJECTILE, 20);
			}
		}

		CObjPool<CFireParticle>::Init(OBJ_TYPE::PARTICLE, 1000);
		CObjPool<CSnowParticle>::Init(OBJ_TYPE::PARTICLE, 1000);
	}
}

