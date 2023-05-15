#include "stdafx.h"
#include "StatUI.h"
#include "Texture.h"
#include "ResMgr.h"
#include "ObjMgr.h"

CStatUI::CStatUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType, CObj* _pOnwer)
	: CUI(_eType, _eUiType)
	, m_pOwner(_pOnwer)
	, m_pTexFormat(nullptr)
	, m_pHP(nullptr)
	, m_fHPper(100.f)
	, m_fHPFull(0.f)
	, m_fHPMag(0.f)
{
	m_vFormatPos = VEC2{ 40.f, 40.f };
	m_vHPPos = VEC2{ 105.f, 49.f };
}


CStatUI::~CStatUI()
{
}

const bool CStatUI::Init()
{
	Load_Tex();
	
	if (nullptr == m_pTexFormat || nullptr == m_pHP) return false;

	int iWidth = m_pHP->Get_Image()->GetWidth();

	m_fHPFull = iWidth * 3.38f;

	m_fHPMag = m_pOwner->Get_CurHP() * 0.01f;

	return true;
}

void CStatUI::Update()
{
	return;
}

void CStatUI::LateUpdate()
{
	return;
}

void CStatUI::Render(HDC _hDC)
{
	if (!m_bActive) return;

	Calculate_HP();

	if (nullptr == m_pTexFormat || nullptr == m_pHP) return;

	UINT iWidth = 0, iHeight = 0;


	// Format
	iWidth = m_pTexFormat->Get_Image()->GetWidth();
	iHeight = m_pTexFormat->Get_Image()->GetHeight();

	m_Graphics.DrawImage(m_pTexFormat->Get_Image(),
		int(m_vFormatPos.fX),
		int(m_vFormatPos.fY),
		int(iWidth),
		int(iHeight));

	// HP
	iWidth = m_pHP->Get_Image()->GetWidth();
	iHeight = m_pHP->Get_Image()->GetHeight();

	m_Graphics.DrawImage(m_pHP->Get_Image(),
		int(m_vHPPos.fX),
		int(m_vHPPos.fY),
		int(m_fHPFull * m_fHPper),
		int(iHeight * 3));
}

void CStatUI::Release()
{
}

void CStatUI::Load_Tex()
{
	m_pTexFormat = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_STAT_FORMAT", L"../Data/Sprite/99_UI/Stat/Stat2.png");
	m_pHP = CResMgr::Get_Inst()->Load_Tex(L"PLAYER_STAT_HP", L"../Data/Sprite/99_UI/Stat/HPBarFill.png");
}

void CStatUI::Calculate_HP()
{
	if (nullptr == m_pOwner) return;

	m_fHPper = m_pOwner->Get_CurHP() / m_fHPMag * 0.01f;

	if (m_fHPper < 0.f)
		m_fHPper = 0.f;
}
