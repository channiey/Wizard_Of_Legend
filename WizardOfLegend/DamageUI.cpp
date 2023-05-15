#include "stdafx.h"
#include "DamageUI.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "CameraMgr.h"
#include "SelectFont.h"
#include "ObjPool.h"
#include "GameMgr.h"

CDamageUI::CDamageUI(const OBJ_TYPE & _eType)
	: CUI(_eType)
	, m_iDamage(0)
	, m_fAcc(0.f)
	, m_fLifeTime(0.f)
	, m_fForce(4000.f)
{
}

CDamageUI::CDamageUI(const OBJ_TYPE& _eType, const UI_TYPE& _eUiType)
	: CUI(_eType, _eUiType)
	, m_iDamage(0)
	, m_fAcc(0.f)
	, m_fLifeTime(0.f)
	, m_fForce(4000.f)
{
}


CDamageUI::~CDamageUI()
{
}

const bool CDamageUI::Init()
{
	// Set Component
	Create_Transform();
	m_pTransform->Init();
	m_pTransform->Set_Pos(VEC2_ZERO);

	Create_Rigidbody();
	m_pRigidbody->Init();
	m_pRigidbody->Set_Gravity(GRAVITY);


	// Set Variable
	m_iDamage = rand() % 30 + 10;
	
	m_fLifeTime = 0.7f;

	m_eRenderOrder = RENDER_ORDER::UI;

	return true;
}

void CDamageUI::Update()
{
	if (!m_bActive) return;

	m_fAcc += DT;

	if (m_fAcc >= m_fLifeTime)
	{
		m_bActive = false;
		m_fAcc = 0.f;

		CObjPool<CDamageUI>::Return_Obj(this);
	}
}

void CDamageUI::LateUpdate()
{
	if (!m_bActive) return;

	Component_LateUpdate();
}

void CDamageUI::Render(HDC _hDC)
{
	
	if (!m_bActive) return;

	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());

	SelectFont(_hDC, FONT_TYPE::RETRO, FONT_SIZE::SMALL);

	TCHAR tch[32] = {};
	swprintf_s(tch, L"%d", m_iDamage);
	TextOut(_hDC, int(vPos.fX), int(vPos.fY), tch, _tcsclen(tch));
}

void CDamageUI::Release()
{
}

void CDamageUI::Reset(const VEC2& _vPos)
{
	m_pTransform->Set_Pos(_vPos);

	m_pRigidbody->Zero_Velocity();

	m_pRigidbody->Add_Force(VEC2_UP * m_fForce);
}

