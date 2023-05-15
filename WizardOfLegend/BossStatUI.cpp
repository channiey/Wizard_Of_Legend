#include "stdafx.h"
#include "BossStatUI.h"
#include "Obj.h"
#include "Texture.h"
#include "ResMgr.h"
#include "EventMgr.h"
#include "TimeMgr.h"

CBossStatUI::CBossStatUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType, CObj* _pOnwer)
	: CUI(_eType, _eUiType)
	, m_pOwner(_pOnwer)
	, m_pTexFormat(nullptr)
	, m_pHP(nullptr)
	, m_fHPper(100.f)
	, m_fHPFullX(0.f)
	, m_bStartGimmick(false)
{
	m_vFormatPos = VEC2{ WINCX * 0.5f, 65.f };
	m_vHPPos = VEC2{ WINCX * 0.5f - 132, 55.f };
}


CBossStatUI::~CBossStatUI()
{
}

const bool CBossStatUI::Init()
{
	Load_Tex();

	if (nullptr == m_pTexFormat || nullptr == m_pHP) return false;

	int iWidth = m_pHP->Get_Image()->GetWidth();

	m_fHPFullX = iWidth * 3.f;

	m_fHPMag = m_pOwner->Get_CurHP() * 0.01f;

	return true;
}

void CBossStatUI::Update()
{
	if (!m_bActive) return;

	if (m_bStartGimmick)
		Start_Gimmick();
}

void CBossStatUI::LateUpdate()
{
	if (!m_bActive) return;

	if (m_pOwner->Is_Dead())
		CEventMgr::Get_Inst()->Delete_Obj(this);
}

void CBossStatUI::Render(HDC _hDC)
{
	if (!m_bActive) return;

	if(!m_bStartGimmick)
		Calculate_HP();

	if (nullptr == m_pTexFormat || nullptr == m_pHP) return;

	UINT iWidth = 0, iHeight = 0;

	// Format
	iWidth = m_pTexFormat->Get_Image()->GetWidth();
	iHeight = m_pTexFormat->Get_Image()->GetHeight();

	m_Graphics.DrawImage(m_pTexFormat->Get_Image(),
		int(m_vFormatPos.fX - iWidth * 1.5f),
		int(m_vFormatPos.fY - iHeight * 1.5f),
		int(iWidth * 3),
		int(iHeight * 3));

	// HP
	iWidth = m_pHP->Get_Image()->GetWidth();
	iHeight = m_pHP->Get_Image()->GetHeight();

	m_Graphics.DrawImage(m_pHP->Get_Image(),
		int(m_vHPPos.fX),
		int(m_vHPPos.fY),
		int(m_fHPFullX * m_fHPper),
		int(iHeight * 3));



}

void CBossStatUI::Release()
{
}

const bool CBossStatUI::Reset(CObj * _pOnwer)
{
	if (nullptr == _pOnwer) return false; 
	
	m_pOwner = _pOnwer;
	
	m_fHPMag = m_pOwner->Get_CurHP() * 0.01f;

	return true;
}

void CBossStatUI::Set_Active(const bool _bActive)
{
	m_bActive = _bActive;

	if (m_bActive)
	{
		m_bStartGimmick = true;
		m_fHPper = 0.f;
	}
}

void CBossStatUI::Load_Tex()
{
	m_pTexFormat = CResMgr::Get_Inst()->Load_Tex(L"BOSS_STAT_FORMAT", L"../Data/Sprite/99_UI/BossStatBar/EnemyHealthBarBG.png");
	m_pHP = CResMgr::Get_Inst()->Load_Tex(L"BOSS_STAT_HP", L"../Data/Sprite/99_UI/BossStatBar/EnemyHealthBarFill.png");
}

void CBossStatUI::Calculate_HP()
{
	if (nullptr == m_pOwner) return;

	m_fHPper = m_pOwner->Get_CurHP() / m_fHPMag * 0.01f;

	if (m_fHPper < 0.f)
		m_fHPper = 0.f;
}

void CBossStatUI::Start_Gimmick()
{
	m_fHPper += DT * 0.7f;

	if (1.f <= m_fHPper)
	{
		m_fHPper = 1.f;
		m_bStartGimmick = false;
	}
}
