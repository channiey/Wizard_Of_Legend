#include "stdafx.h"
#include "GoldUI.h"
#include "Texture.h"
#include "ResMgr.h"
#include "ObjMgr.h"
#include "SelectFont.h"
CGoldUI::CGoldUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType)
	: CUI(_eType, _eUiType)
	, m_pTex(nullptr) 
{
	ZeroMemory(&m_vImgPos, sizeof(VEC2));
	ZeroMemory(&m_vTextPos, sizeof(VEC2));
}


CGoldUI::~CGoldUI()
{
}

const bool CGoldUI::Init()
{
	Load_Tex();

	m_vImgPos = VEC2{ WINCX * 0.5f, WINCY - 70.f };
	m_vTextPos = VEC2{ int(m_vImgPos.fX + 40), int(m_vImgPos.fY - 10) };

	return true;
}

void CGoldUI::Update()
{
}

void CGoldUI::LateUpdate()
{
}

void CGoldUI::Render(HDC _hDC)
{
	Receive_Gold();

	// Render Image
	UINT iWidth = m_pTex->Get_Image()->GetWidth();
	UINT iHeight = m_pTex->Get_Image()->GetHeight();

	float fMag = 2.f;

	m_Graphics.DrawImage(m_pTex->Get_Image(),
		int(m_vImgPos.fX - (iWidth * fMag * 0.5f)),
		int(m_vImgPos.fY - (iHeight * fMag * 0.5f)),
		int(iWidth * fMag),
		int(iHeight * fMag));


	// Render Text(Gold)
	SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::SMALL);

	TCHAR tch[32] = {};
	swprintf_s(tch, L"%d", m_iGold);
	TextOut(_hDC, int(m_vTextPos.fX), int(m_vTextPos.fY), tch, _tcsclen(tch));
}

void CGoldUI::Release()
{
}

void CGoldUI::Load_Tex()
{
	m_pTex = CResMgr::Get_Inst()->Load_Tex(L"GOLD_UI", L"../Data/Sprite/99_UI/Gold.png");
}

void CGoldUI::Receive_Gold()
{
	CObj* pPlayer = CObjMgr::Get_Inst()->Get_Player();

	if (nullptr == pPlayer) return;

	m_iGold = pPlayer->Get_Gold();
}
