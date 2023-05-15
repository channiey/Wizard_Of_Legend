#include "stdafx.h"
#include "EditorScene.h"
#include "InputMgr.h"
#include "CameraMgr.h"
#include "ResMgr.h"
#include "Texture.h"
#include "MainGame.h"
#include "Obj.h"
#include "Player.h"
#include "EventMgr.h"
#include "Transform.h"
#include "MapMgr.h"
#include "GameMgr.h"
#include "CollisionMgr.h"
#include "BmpMgr.h"
#include "SelectFont.h"
#include "Debug_Render.h"


/*

다른 씬 사용시 변경할 사항

1.Enter() : 불러올 Bmp 경로 설정

2.Render_Background() : 불러올 Bmp 이름 설정 / 스케일 변경

3.Key_Input() : 경로 설정

-------------------------------------------

4.Obj Mgr Render() : 타일 제외 설정

5.Tile Render() : return 주석

6.InputMgr Init() : 매개변수 false

7. Scene Manager 시작 씬 에디터 씬으로
*/





CEditorScene::CEditorScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos)
	: CScene(_eType, _vStartPos)
	, m_pTexBack(nullptr)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	, m_eType(TILE_TYPE::TYPEEND)
	, m_bFirstClick(false)
{
	ZeroMemory(&m_vLT, sizeof(VEC2));
	ZeroMemory(&m_vRB, sizeof(VEC2));
}

CEditorScene::~CEditorScene()
{
}

const bool CEditorScene::Init()
{
	return true;
}

void CEditorScene::Update()
{
	CScene::Update();

	Set_TileType(); // 생성할 타일의 타입을 결정

	Key_Input();
}

void CEditorScene::LateUpdate()
{
	CScene::LateUpdate();
}

void CEditorScene::Render(HDC _hDC)
{
	Render_Background(_hDC);

	//CScene::Render(_hDC);

	CMapMgr::Get_Inst()->Render(_hDC);

	CDebug_Render::Render(_hDC);

	Render_Text(_hDC);

	Render_CurRect(_hDC);
}

void CEditorScene::Enter()
{
	CGameMgr::Get_Inst()->Set_DevMode(true);

	// Bmp Set 경로 설정 Map_Field_2
	
	//CBmpMgr::Get_Inst()->Insert_Bmp(L"../Data/TileMap/02.Boss/Boss_Stage_Large.bmp", L"Map_Boss");
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Data/TileMap/01.Field/Map_Field_2.bmp", L"Map_Field");

	/*CObj* pPlayer = new CPlayer(OBJ_TYPE::PLAYER);
	pPlayer->Init();
	pPlayer->Get_Transform()->Set_Pos(m_vStartPos);
	CEventMgr::Get_Inst()->Add_Obj(pPlayer);

	CCameraMgr::Get_Inst()->Set_Target(pPlayer);*/
	//CMapMgr::Get_Inst()->Create_All_Tile(MAPCX, MAPCY);
}

void CEditorScene::Exit()
{
	CCollisionMgr::Get_Inst()->Reset();
}

void CEditorScene::Render_Text(HDC _hDC)
{
	TCHAR tch[128] = {};
	VEC2 tPos{ WINCX * 0.5f, 20};
	int iTermLen = 30;
	int iTerm = 0;

	SelectFont(_hDC, FONT_TYPE::DEFAULT, FONT_SIZE::SMALL);

	SetTextAlign(_hDC, TA_CENTER);

	swprintf_s(tch, L"Save - 'O' / Load - 'P'");
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));

	//swprintf_s(tch, L"1.Tile - Ground");
	//TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));

	swprintf_s(tch, L"0. - Render Rect");
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));

	swprintf_s(tch, L"1.Rect - Wall");
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));

	swprintf_s(tch, L"2.Rect - Deadzone");
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));
	
}

void CEditorScene::Key_Input()
{
	if (CInputMgr::Get_Inst()->Key_Down(VK_LBUTTON))
	{
		if (!m_bFirstClick) // LT 생성
		{
			m_bFirstClick = true;
			m_vLT = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y });
		}
		else                // RB 생성
		{
			m_bFirstClick = false;
			m_vRB = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y });
			
			// Set Data
			VEC2 vCreateScale = VEC2{ m_vRB.fX - m_vLT.fX, m_vRB.fY - m_vLT.fY };
			VEC2 vCreatePos = VEC2{ m_vLT.fX + vCreateScale.fX * 0.5f, m_vLT.fY + vCreateScale.fY * 0.5f };
			TILE_TYPE eType = m_eType;

			// Create Rect
			CMapMgr::Get_Inst()->Create_RectTile(vCreatePos, vCreateScale, eType);
		}
	}
	else if (CInputMgr::Get_Inst()->Key_Down(VK_RBUTTON))
	{
		VEC2 vClickPos = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y });

		CMapMgr::Get_Inst()->Remove_RectTile(vClickPos);
	}


	if (CInputMgr::Get_Inst()->Key_Down('O'))
	{
		// 맵 에디터 사용시 경로 변경 위치
		//CMapMgr::Get_Inst()->Save_Tile(L"../Data/TileMap/02.Boss/Boss_Stage_Large_Tile.dat");
		CMapMgr::Get_Inst()->Save_Tile(L"../Data/TileMap/01.Field/Map_Field_2_Tile.dat");
		//
		//<< "저장 완료!";
	}
	else if (CInputMgr::Get_Inst()->Key_Down('P'))
	{
		// 맵 에디터 사용시 경로 변경 위치
		//CMapMgr::Get_Inst()->Load_Tile(L"../Data/TileMap/02.Boss/Boss_Stage_Large_Tile.dat");
		CMapMgr::Get_Inst()->Load_Tile(L"../Data/TileMap/01.Field/Map_Field_2_Tile.dat");
		//cout << "로드 완료!";
	}

}

void CEditorScene::Render_Background(HDC _hDC)
{
	VEC2 vScale = VEC2(6515, 4748);

	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(VEC2(vScale.fX * 0.5f, vScale.fY * 0.5f));

	BitBlt(_hDC, int(vPos.fX - (vScale.fX * 0.5f)), int(vPos.fY - (vScale.fY * 0.5f)), int(vScale.fX), int(vScale.fY), CBmpMgr::Get_Inst()->Find_Image(L"Map_Field"), 0, 0, SRCCOPY);
}

void CEditorScene::Render_CurRect(HDC _hDC)
{
	if (!m_bFirstClick) return;

	VEC2 vLT = m_vLT;
	VEC2 vRB = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y });

	VEC2 vScale = VEC2{ vRB.fX - vLT.fX, vRB.fY - vLT.fY };
	
	vLT = CCameraMgr::Get_Inst()->Get_RenderPos(vLT);

	Rect rc{ int(vLT.fX), int(vLT.fY), int(vScale.fX), int(vScale.fY)};

	Pen pen(Color(255, 0, 0, 255), 3);

	m_Graphics.DrawRectangle(&pen, rc);
}

void CEditorScene::Set_TileType()
{
	if (CInputMgr::Get_Inst()->Key_Down('1'))
		m_eType = TILE_TYPE::WALL;
	else if (CInputMgr::Get_Inst()->Key_Down('2'))
		m_eType = TILE_TYPE::DEADZONE;
}
