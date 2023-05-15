#include "stdafx.h"
#include "CameraHurtUI.h"
#include "ResMgr.h"
#include "Texture.h"

CCameraHurtUI::CCameraHurtUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType)
	: CUI(_eType, _eUiType)
	, m_pTex(nullptr)
{
}


CCameraHurtUI::~CCameraHurtUI()
{
}

const bool CCameraHurtUI::Init()
{
	Load_Tex();

	return true;
}

void CCameraHurtUI::Update()
{
}

void CCameraHurtUI::LateUpdate()
{
}

void CCameraHurtUI::Render(HDC _hDC)
{
	if (!m_bActive) return;

	m_Graphics.DrawImage(m_pTex->Get_Image(),
		0,
		0,
		WINCX,
		WINCY);
}

void CCameraHurtUI::Release()
{
}

void CCameraHurtUI::Load_Tex()
{
	m_pTex = CResMgr::Get_Inst()->Load_Tex(L"CAMERA_HURT", L"../Data/Sprite/08.Effect/CameraHurt/CameraHurtEffectEdit.png");
}
