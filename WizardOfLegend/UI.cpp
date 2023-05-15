#include "stdafx.h"
#include "UI.h"
#include "Transform.h"
#include "CameraMgr.h"
#include "InputMgr.h"
#include "MainGame.h"
CUI::CUI(const OBJ_TYPE & _eType)
	: CObj(_eType)
	, m_pParentUI(nullptr)
	, m_eUiType(UI_TYPE::FIXED)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	, m_pOnwer(nullptr)
{
}
CUI::CUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType)
	: CObj(_eType)
	, m_pParentUI(nullptr)
	, m_eUiType(_eUiType)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	, m_pOnwer(nullptr)
{
	ZeroMemory(&m_vFinalPos,sizeof(VEC2));
	m_eRenderOrder = RENDER_ORDER::UI;
}


CUI::~CUI()
{
	Release();
}

const bool CUI::Init()
{
	/*Create_Transform();

	m_pTransform->Init();
	m_pTransform->Set_Pos(VEC2(WINCX * 0.5f, 200));
	m_pTransform->Set_Scale(VEC2(200, 100));*/

	return true;
}

void CUI::Update()
{
	//Update_Child();
}

void CUI::LateUpdate()
{
	/*Component_LateUpdate();

	Set_FinalPos(); 
	
	Check_Mouse_On();

	LateUpdate_Child();*/
}

void CUI::Render(HDC _hDC)
{
	//Component_Render(_hDC);

	//VEC2 vPos = Get_FinalPos();
	//VEC2 vScale = m_pTransform->Get_Scale();

	//if (m_eUiType == UI_TYPE::MOVABLE)
	//	vPos = CCameraMgr::Get_Inst()->Get_RenderPos(vPos);

	//Render_Child(_hDC);
}

void CUI::Release()
{
	
}

void CUI::Set_FinalPos()
{
	m_vFinalPos = m_pTransform->Get_Pos();

	if (nullptr != m_pParentUI)
		m_vFinalPos += m_pParentUI->Get_FinalPos();

}

void CUI::Update_Child()
{
	for (auto& iter : m_vecChildUI)
		iter->Update();
}

void CUI::LateUpdate_Child()
{
	for (auto& iter : m_vecChildUI)
		iter->LateUpdate();
}

void CUI::Render_Child(HDC _hDC)
{
	for (auto& iter : m_vecChildUI)
		iter->Render(_hDC);
}

void CUI::Check_Mouse_On()
{
	VEC2 vMousePos = VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y };
	VEC2 vScale = m_pTransform->Get_Scale();

	if (m_eUiType == UI_TYPE::MOVABLE)
		vMousePos = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ vMousePos.fX, vMousePos.fY });
	
	if ((m_vFinalPos.fX <= vMousePos.fX && vMousePos.fX <= m_vFinalPos.fX + vScale.fX)
		&& (m_vFinalPos.fY <= vMousePos.fY && vMousePos.fY <= m_vFinalPos.fY + vScale.fY))
	{
		m_bMouseOn = true;
	}
	else
		m_bMouseOn = false;
}

void CUI::Mouse_On()
{
}

void CUI::Mouse_Down()
{
}

void CUI::Mouse_Up()
{
}

void CUI::Mouse_Clicked()
{
}
