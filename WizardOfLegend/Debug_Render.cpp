#include "stdafx.h"
#include "Debug_Render.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "CameraMgr.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "Transform.h"
#include "InputMgr.h"
#include "GameMgr.h"
#include "SelectFont.h"

CDebug_Render::CDebug_Render()
{
}


CDebug_Render::~CDebug_Render()
{
}

void CDebug_Render::Render(HDC _hDC)
{
	if (!CGameMgr::Get_Inst()->Is_DevMode()|| !CGameMgr::Get_Inst()->Is_DevText()) return;

	SelectFont(_hDC, FONT_TYPE::DEFAULT, FONT_SIZE::SMALL, 0);

	TCHAR tch[128] = {};
	VEC2 tPos{ 10, 120 };
	int iTermLen = 20;
	int iTerm = 1;
	SetTextAlign(_hDC, TA_LEFT);

	swprintf_s(tch, L"FPS : %d", CTimeMgr::Get_Inst()->Get_FPS());
	TextOut(_hDC, int(tPos.fX), int(tPos.fY), tch, _tcsclen(tch));

	swprintf_s(tch, L"DT : %.4f", CTimeMgr::Get_Inst()->Get_fDT());
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));


	VEC2 vPos{};

	/*CObj* pPlayer = CObjMgr::Get_Inst()->Get_Player();

	if (nullptr != pPlayer)
	{
		vPos = pPlayer->Get_Transform()->Get_Pos();

		swprintf_s(tch, L"W_Pos_Player : %.0f  %.0f", vPos.fX, vPos.fY);
		TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));

		vPos = CCameraMgr::Get_Inst()->Get_RenderPos(vPos);
		swprintf_s(tch, L"L_Pos_Player  : %.0f  %.0f", vPos.fX, vPos.fY);
		TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));
	}*/

	// ¸¶¿ì½º


	vPos = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y }); 
	swprintf_s(tch, L"MOUSE_W : %.0f  %.0f", vPos.fX, vPos.fY);
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));

	vPos = VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y };
	swprintf_s(tch, L"MOUSE_L : %.0f  %.0f", vPos.fX, vPos.fY);
	TextOut(_hDC, int(tPos.fX), int(tPos.fY + iTermLen * iTerm++), tch, _tcsclen(tch));

}
