#include "stdafx.h"
#include "WarningUI.h"
#include "Texture.h"
#include "ResMgr.h"
#include "ObjMgr.h"
#include "EventMgr.h"
#include "TimeMgr.h"
#include "BossStageMgr.h"
#include "CameraMgr.h"
#include "SoundMgr.h"

CWarningUI::CWarningUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType)
	: CUI(_eType, _eUiType)
	, m_pTex(nullptr)
	, m_fLifeTime(3.5f) 
	, m_fAcc(0.f)
	, m_bLife(true)
	, m_bRender(true)
{
	ZeroMemory(&m_vImgPos, sizeof(VEC2));
}


CWarningUI::~CWarningUI()
{
}

const bool CWarningUI::Init()
{
	Load_Tex();

	m_vImgPos = VEC2{ WINCX * 0.5f, WINCY * 0.2f};

	m_bActive = false;
	return true;
}

void CWarningUI::Update()
{
	if (!m_bActive) return;

	Flash();
	Check_Self_Delete();
}

void CWarningUI::LateUpdate()
{
	if (!m_bActive) return;
}

void CWarningUI::Render(HDC _hDC)
{
	if (!m_bActive || !m_bRender) return;


	// Draw Red tone
	Rect rc{ 0, 0, WINCX, WINCY };
	SolidBrush brush(Color(50, 255, 0, 0));
	m_Graphics.FillRectangle(&brush, rc);



	// Draw UI
	UINT iWidth = m_pTex->Get_Image()->GetWidth();
	UINT iHeight = m_pTex->Get_Image()->GetHeight();

	float fMag = 2.f;

	m_Graphics.DrawImage(m_pTex->Get_Image(),
		int(m_vImgPos.fX - (iWidth * fMag * 0.5f)),
		int(m_vImgPos.fY - (iHeight * fMag * 0.5f)),
		int(iWidth * fMag),
		int(iHeight * fMag));
	
}

void CWarningUI::Load_Tex()
{
	m_pTex = CResMgr::Get_Inst()->Load_Tex(L"WARNING_UI", L"../Data/Sprite/99_UI/WarningSign.png");
}

void CWarningUI::Check_Self_Delete()
{
	if (!m_bLife) return;

	m_fAcc += DT;

	if (m_fAcc >= m_fLifeTime)
	{
		m_bLife = false;
		CEventMgr::Get_Inst()->Delete_Obj(this);

		CBossStageMgr::Get_Inst()->Camera_Move_To_Boss();
	}
}

void CWarningUI::Flash()
{
	if (m_fAcc <= 1.f) return;


	float f = trunc(m_fAcc);

	if (m_fAcc - f <= 0.5f) // 0.5초 마다 깜빡임을 반복한다.
	{
		m_bRender = false;

		if (CCameraMgr::Get_Inst()->Is_Shaking())
			CCameraMgr::Get_Inst()->Stop_Shaking();
	}

	else
	{
		/*if (m_bRender)
		{
			CSoundMgr::Get_Inst()->StopSound(SOUND_EFFECT);
			CSoundMgr::Get_Inst()->PlaySound(L"Warning.wav", SOUND_EFFECT, 1.f);
		}*/
		m_bRender = true;

		if (!CCameraMgr::Get_Inst()->Is_Shaking())
			CCameraMgr::Get_Inst()->Shake_Camera(0.5f, 10.f);
	}
}

void CWarningUI::Set_Active(const bool _bActive)
{
	m_bActive = _bActive;

	if (m_bActive)
	{
		CSoundMgr::Get_Inst()->StopSound(SOUND_BOSS);
		CSoundMgr::Get_Inst()->PlaySound(L"Warning.wav", SOUND_BOSS, 0.7f);
		CCameraMgr::Get_Inst()->Shake_Camera(1.f, 10.f);

	}

}