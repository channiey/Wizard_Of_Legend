#include "stdafx.h"
#include "TitleScene.h"
#include "MainGame.h"
#include "InputMgr.h"
#include "EventMgr.h"
#include "BmpMgr.h"
#include "ResMgr.h"
#include "GameMgr.h"
#include "Texture.h"
#include "SelectFont.h"
#include "EventMgr.h"
#include "SoundMgr.h"
#include "TimeMgr.h"
#include "CameraMgr.h"
#include "InputMgr.h"
CTitleScene::CTitleScene(const SCENE_TYPE& _eType, const VEC2& _vStartPos)
	: CScene(_eType, _vStartPos)
	, m_pTexLogo(nullptr)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	, m_bMenuMode(false)
	, m_fAcc(0.f)
	, m_fFlashTime(0.75f)
	, m_bRender(true)
	, m_bStart(false)
	, m_fStartAcc(0.f)
{
}


CTitleScene::~CTitleScene()
{
}

const bool CTitleScene::Init()
{
	// 개발자 모드 설정
	CGameMgr::Get_Inst()->Set_DevMode(true);

	// 배경 맵 로드
	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Data/Sprite/00_Background/Background0.bmp", L"TITLE_BACKGROUND"); 

	CBmpMgr::Get_Inst()->Insert_Bmp(L"../Data/Sprite/00_Background/TitleLogo.bmp", L"TITLE_LOGO");

	m_pTexLogo = CResMgr::Get_Inst()->Load_Tex(L"TITLE_LOGO", L"../Data/Sprite/00_Background/TitleLogo1.png");

	m_arrRect[0] = RECT{-130, -60, 127, -34 };
	m_arrRect[1] = RECT{ -113, 20, 117, 49 };
	m_arrRect[2] = RECT{ -70, 95, 75, 130 };
	m_arrRect[3] = RECT{ -60, 180, 60, 210 };
	m_arrRect[4] = RECT{ -43, 260, 42, 290 };

	for (int i = 0; i < 4; ++i)
		m_arrOn[i] = false;

	return true;
}

void CTitleScene::Enter()
{
	/*CSoundMgr::Get_Inst()->StopSound(SOUND_BGM);
	CSoundMgr::Get_Inst()->PlayBGM(L"TitleScreen.wav", 0.5f);*/
}

void CTitleScene::Exit()
{
}

void CTitleScene::Update()
{
	if(!m_bStart)
	{
		m_fStartAcc += DT;
		if (m_fStartAcc >= 15.f)
		{
			CSoundMgr::Get_Inst()->StopSound(SOUND_BGM);
			CSoundMgr::Get_Inst()->PlayBGM(L"TitleScreen.wav", 0.5f);
			m_bStart = true;
		}
	}
	CScene::Update();
	
	if(m_bMenuMode)
		Is_Mouse_On();

	if (!m_bMenuMode)
	{
		if (CInputMgr::Get_Inst()->Key_Down(VK_SPACE))
		{
			m_bMenuMode = true;
			CSoundMgr::Get_Inst()->StopSound(SOUND_TITLE);
			CSoundMgr::Get_Inst()->PlaySound(L"EnemyCast.wav", SOUND_TITLE, 0.6f);
		}


		// 글자 깜빡임
		m_fAcc += DT;
		if (m_fAcc >= m_fFlashTime)
		{
			m_fAcc = 0.f;
			m_bRender = !m_bRender;
		}
	}

	if (m_bMenuMode)
	{
		if (CInputMgr::Get_Inst()->Key_Down(VK_SPACE))
		{
			CEventMgr::Get_Inst()->Change_Scene(SCENE_TYPE::LOADING);
			CSoundMgr::Get_Inst()->PlaySound(L"EnemyCast.wav", SOUND_TITLE, 0.6f);
			CSoundMgr::Get_Inst()->StopSound(SOUND_BGM);
		}
	}

	
}

void CTitleScene::LateUpdate()
{
	CScene::LateUpdate();
}

void CTitleScene::Render(HDC _hDC)
{
	Render_Background(_hDC);

	CScene::Render(_hDC);
}

void CTitleScene::Render_Background(HDC _hDC)
{
	// 배경화면을 그린다.======================================================================
	BitBlt(_hDC, 0, 0, WINCX, WINCY, CBmpMgr::Get_Inst()->Find_Image(L"TITLE_BACKGROUND"), 0, 0, SRCCOPY);

	if (nullptr == m_pTexLogo) return;

	UINT iWidth = m_pTexLogo->Get_Image()->GetWidth();
	UINT iHeight = m_pTexLogo->Get_Image()->GetHeight();

	float fMag = 1.f;
	float fY = 0.f;


	// 배경 검정 알파 처리
	{
		Rect rc{ 0, 0, WINCX, WINCY };

		SolidBrush brush(Color(80, 0, 0, 0));;

		if (m_bMenuMode)
			brush.SetColor(Color(200, 0, 0, 0));

		m_Graphics.FillRectangle(&brush, rc);
	}

	// 로고 =======================================================================================
	{
		// 로고의 높이를 결정한다.
		if (!m_bMenuMode)
			fY = VEC2_SCREEN_CENTER.fY - iHeight * 0.5f * fMag;
		else
			fY = VEC2_SCREEN_CENTER.fY - iHeight * 0.5f * fMag - 170.f;

		// 로고를 그린다.
		m_Graphics.DrawImage(m_pTexLogo->Get_Image(),
			int(VEC2_SCREEN_CENTER.fX - iWidth * 0.5f * fMag),
			int(fY),
			int(iWidth* fMag),
			int(iHeight* fMag));
	}


	// 텍스트를 출력한다.==========================================================================
	{
		if (!m_bMenuMode) // 대기화면
		{
			// 폰트 출력
			if (m_bRender)
			{
				// 그림자
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::MEDIUM, TRANSPARENT, true);

				TCHAR tch[128] = {};
				VEC2 tPos{ WINCX * 0.5f, WINCY * 0.5f + 150.f };

				SetBkMode(_hDC, TRANSPARENT);
				SetTextAlign(_hDC, TA_CENTER);

				swprintf_s(tch, L"Please enter any key...");
				TextOut(_hDC, int(tPos.fX + 2), int(tPos.fY - 2), tch, _tcsclen(tch));

				// 흰색
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::MEDIUM);

				swprintf_s(tch, L"Please enter any key...");
				TextOut(_hDC, int(tPos.fX), int(tPos.fY), tch, _tcsclen(tch));
			}


		}
		else  // 메뉴화면
		{
			TCHAR tch[128] = {};
			VEC2 tPos{ WINCX * 0.5f, WINCY * 0.5f - 120.f };
			SetTextAlign(_hDC, TA_CENTER);
			
			float fYTerm = 80.f;
			int iYCount = 1;

			if(m_arrOn[0])
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::PLUS_LARGE);
			}
			else
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::LARGE);
			}
			swprintf_s(tch, L"SINGLE PLAY");
			TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));

			if (m_arrOn[1])
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::PLUS_LARGE);
			}
			else
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::LARGE);
			}
			swprintf_s(tch, L"MAP EDITOR");
			TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));


			if (m_arrOn[2])
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::PLUS_LARGE);
			}
			else
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::LARGE);
			}
			swprintf_s(tch, L"OPTION");
			TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));


			if (m_arrOn[3])
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::PLUS_LARGE);
			}
			else
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::LARGE);
			}
			swprintf_s(tch, L"ABOUT");
			TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));

			if (m_arrOn[4])
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::PLUS_LARGE);
			}
			else
			{
				SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::LARGE);
			}
			swprintf_s(tch, L"EXIT");
			TextOut(_hDC, int(tPos.fX), int(tPos.fY + fYTerm * iYCount++), tch, _tcsclen(tch));

		}
	}



}

void CTitleScene::Is_Mouse_On()
{
	VEC2 vMousePos = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y });
	for (int i = 0; i < 5; ++i)
	{
		if ((m_arrRect[i].left <= vMousePos.fX && vMousePos.fX <= m_arrRect[i].right) &&
			(m_arrRect[i].top <= vMousePos.fY && vMousePos.fY <= m_arrRect[i].bottom))
		{
			if (!m_arrOn[i])
			{
				m_arrOn[i] = true;
				CSoundMgr::Get_Inst()->StopSound(SOUND_MENU);
				CSoundMgr::Get_Inst()->PlaySound(L"PaperFlap0.wav", SOUND_MENU, 1.f);

			}
		}
		else
		{
			m_arrOn[i] = false;
		}
	}
}
