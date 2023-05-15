#include "stdafx.h"
#include "Portal.h"
#include "PortalKey.h"
#include "PortalEffect.h"
#include "EventMgr.h"
#include "Transform.h"
#include "Collider.h"
#include "Texture.h"
#include "ResMgr.h"
#include "MainGame.h"
#include "CameraMgr.h"
#include "UI.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"

CPortal::CPortal(const OBJ_TYPE& _eType)
	:CObj(_eType)
	, m_pKeyUI(nullptr)
	, m_pEffect(nullptr)
	, m_pTexPortal(nullptr)
	, m_pTexPortalDrawing(nullptr)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
{
}


CPortal::~CPortal()
{
}

const bool CPortal::Init()
{
	// Create Component
	Create_Transform();
	Create_Collider();

	m_pTransform->Init();
	m_pTransform->Set_Pos(VEC2_ZERO);
	m_pTransform->Set_Scale(VEC2{ 300, 300 });

	m_pCollider->Init();
	m_pCollider->Set_Pos(VEC2_ZERO);
	m_pCollider->Set_Scale(VEC2{ 300, 300 });


	// Load Text;
	m_pTexPortal = CResMgr::Get_Inst()->Load_Tex(L"PORTAL",	L"../Data/Sprite/08.Effect/Portal/TeleportLandingPad.png");
	m_pTexPortalDrawing = CResMgr::Get_Inst()->Load_Tex(L"PORTAL_DRAWING", L"../Data/Sprite/08.Effect/Portal/TeleportCircle.png");

	// Set Field
	m_pKeyUI = new CPortalKey(OBJ_TYPE::UI, UI_TYPE::FIXED);
	m_pKeyUI->Init();
	static_cast<CUI*>(m_pKeyUI)->Set_Onwer(this);

	CEventMgr::Get_Inst()->Add_Obj(m_pKeyUI);
	m_pEffect = new CPortalEffect(OBJ_TYPE::EFFECT, this);
	m_pEffect->Init();
	CEventMgr::Get_Inst()->Add_Obj(m_pEffect);

	return true;
}

void CPortal::Update()
{
	if (!m_bActive) return;
}

void CPortal::LateUpdate()
{
	if (!m_bActive) return;
}

void CPortal::Render(HDC _hDC)
{
	if (!m_bActive) return;

	if (nullptr == m_pTexPortal || nullptr == m_pTexPortalDrawing) return;

	// Render Image1
	UINT iWidth = m_pTexPortal->Get_Image()->GetWidth();
	UINT iHeight = m_pTexPortal->Get_Image()->GetHeight();

	float fMag = 3.f;
	float fYTerm = 50.f;

	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());

	m_Graphics.DrawImage(m_pTexPortal->Get_Image(),
		int(vPos.fX - iWidth * fMag * 0.5f),
		int(vPos.fY - iHeight * fMag * 0.5f + fYTerm),
		int(iWidth * fMag),
		int(iHeight * fMag));


	// Render Image2
	iWidth = m_pTexPortalDrawing->Get_Image()->GetWidth();
	iHeight = m_pTexPortalDrawing->Get_Image()->GetHeight();

	m_Graphics.DrawImage(m_pTexPortalDrawing->Get_Image(),
		int(vPos.fX - iWidth * fMag * 0.5f),
		int(vPos.fY - iHeight * fMag * 0.5f + fYTerm),
		int(iWidth * fMag),
		int(iHeight * fMag));

	// Set Render 
	m_eRenderOrder = RENDER_ORDER::PORTAL;


	// Render Component

	//Component_Render(_hDC);

}

void CPortal::OnCollision_Enter(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
}

void CPortal::OnCollision_Stay(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (OBJ_TYPE::PLAYER == _pOtherCol->Get_Owner()->Get_ObjType())
	{
		m_pKeyUI->Set_Active(true);

		if (CInputMgr::Get_Inst()->Key_Down('F'))
		{
			switch (CSceneMgr::Get_Inst()->Get_CurScene()->Get_SceneType())
			{
			case SCENE_TYPE::VILLAGE:
			{
				Effect(START_POS_VILLAGE);
			}
				break;
			case SCENE_TYPE::FIELD:
			{
				Effect(START_POS_FIELD);
				CEventMgr::Get_Inst()->Change_Scene(SCENE_TYPE::BOSS);
			}
				break;
			case SCENE_TYPE::BOSS:
				Effect(START_POS_BOSS);
				break;
			}
		}
	}
}

void CPortal::OnCollision_Exit(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (OBJ_TYPE::PLAYER == _pOtherCol->Get_Owner()->Get_ObjType())
	{
		m_pKeyUI->Set_Active(false);
		

	}
}

void CPortal::Effect(const VEC2& _vPos)
{
	m_pEffect->On(_vPos);
}
