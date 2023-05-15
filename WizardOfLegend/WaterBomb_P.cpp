#include "stdafx.h"
#include "WaterBomb_P.h"
#include "Transform.h"
#include "Collider.h"
#include "Animator.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "EventMgr.h"
#include "ObjPool.h"
#include "GameMgr.h"
#include "SoundMgr.h"
#include "TimeMgr.h"
#include "Texture.h"
#include "ResMgr.h"
#include "Tile.h"
#include "CameraMgr.h"
#include "MathLib.h"
#include "WaterBombAnimatior.h"
#include "MainGame.h"
#include "InputMgr.h"

CWaterBomb_P::CWaterBomb_P(const OBJ_TYPE& _eType)
	: CProjectile(_eType)
	, m_fMoveRenderMag(3.5f)
	, m_fBombRenderMag(4.f)
	, m_fDis(0.f)
	, m_Graphcis(CMainGame::Get_Inst()->Get_SubDC())
	, m_bBomb(false)
{
	ZeroMemory(&m_vDropPoint, sizeof(VEC2));
	ZeroMemory(&m_vShadowScale, sizeof(VEC2));
}


CWaterBomb_P::~CWaterBomb_P()
{
	Release();
}

const bool CWaterBomb_P::Init()
{
	// Component Setting
	Create_Transform();
	Create_Collider();
	Create_Rigidbody();

	m_pTransform->Init();
	m_pTransform->Set_Pos(DF_POS);
	m_pTransform->Set_Scale(VEC2(200.f, 200.f));

	m_pCollider->Init();
	m_pCollider->Set_Pos(m_pTransform->Get_Pos());
	m_pCollider->Set_Scale(m_pTransform->Get_Scale());

	m_pRigidbody->Init();
	m_pRigidbody->Set_Gravity(GRAVITY);

	m_pTransform->Set_Active(false);
	m_pCollider->Set_Active(false);
	m_pRigidbody->Set_Active(false);

	// Animator Setting
	m_pAnimator = new CWaterBombAnimatior(this);
	m_pAnimator->Init();
	m_pAnimator->Set_Use_Dir(false);
	m_listComponent.push_back(m_pAnimator);
	Init_Animator();

	m_pAnimator->Play(L"FLY", true);

	// Render Setting
	m_fRenderMag = m_fMoveRenderMag;

	// theta 결정
	m_pTransform->Set_Theta(0);

	return true;
}

void CWaterBomb_P::Update()
{
	if (!m_bActive) return;
	
	if (m_bMove) Move();

	Check_Drop();
}

void CWaterBomb_P::LateUpdate()
{
	if (!m_bActive) return;

	Component_LateUpdate();
}

void CWaterBomb_P::Render(HDC _hDC)
{
	if (!m_bActive) return;

	if(!m_bBomb)
		Render_Shadow(_hDC);

	Component_Render(_hDC);
}

void CWaterBomb_P::Release()
{
}

void CWaterBomb_P::OnCollision_Enter(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	switch (_pOtherCol->Get_Owner()->Get_ObjType())
	{
	case OBJ_TYPE::PLAYER:
		break;
	case OBJ_TYPE::ENEMY:
	{
		Explode();
	}
	break;
	case OBJ_TYPE::NPC:
		break;
	case OBJ_TYPE::PROJECTILE:
		break;
	case OBJ_TYPE::ITEM:
		break;
	case OBJ_TYPE::TILE:
		break;
	case OBJ_TYPE::EFFECT:
		break;
	case OBJ_TYPE::TYPEEND:
		break;
	default:
		break;
	}
}



void CWaterBomb_P::Reset(CObj * _pOwner)
{	

	// 콜라이더는 폭발 하고나서 애니메이터에서 켜준다.

	// Set Component
	m_pTransform->Set_Active(true);
	m_pRigidbody->Set_Active(true);
	m_pRigidbody->Zero_Force();
	m_pRigidbody->Zero_Velocity();



	VEC2 vMousePos = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ CInputMgr::Get_Inst()->Get_Mouse_Pos().x, CInputMgr::Get_Inst()->Get_Mouse_Pos().y });

	// 낙하 지점 세팅 (스크린 내 랜덤)
	VEC2 vLT = CCameraMgr::Get_Inst()->Get_RealPos(VEC2_ZERO);
	VEC2 vRB = CCameraMgr::Get_Inst()->Get_RealPos(VEC2{ WINCX, WINCY });

	m_vDropPoint = vMousePos; // CMathLib::Get_Random_Pos(vLT, vRB);


	// 포지션 세팅
	VEC2 vWaterPos{ vMousePos.fX, vLT.fY - 200.f };


	m_pTransform->Set_Pos(vWaterPos);
	m_pCollider->Set_Pos(vWaterPos);


	// 무게 결정
	float fMass = rand() % 5 + 1 + DF_MASS;
	m_pRigidbody->Set_Mass(fMass);

	m_bMove = true;
	m_fRenderMag = m_fMoveRenderMag;
	m_pAnimator->Play(L"FLY", true);

	m_bBomb = false; // 이전 폭발 정보를 초기화



	// 콜라이더는 폭발하고 나서 애니메이터에서 켜준다.
}

void CWaterBomb_P::Return()
{
	CObjPool<CWaterBomb_P>::Return_Obj(this);
}

const bool CWaterBomb_P::Init_Animator()
{

	// C:\Users\ChanYoon\Desktop\WizardOfLegend_2\Data\Sprite\08.Effect\Aqua

	CTexture* pTex;
	CAnimation* pAnimation;
	float fDuration = 0.f;

	// FLY
	{
		fDuration = 0.1f;

		pAnimation = new CAnimation(L"FLY", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_MOVE_0", L"../Data/Sprite/08.Effect/Aqua/AquaPrison_15.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_MOVE_1", L"../Data/Sprite/08.Effect/Aqua/AquaPrison_16.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_MOVE_2", L"../Data/Sprite/08.Effect/Aqua/AquaPrison_17.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		m_pAnimator->PushBack_Ani(pAnimation);
	}
	// BOOMB
	{
		fDuration = 0.1f;

		pAnimation = new CAnimation(L"BOOMB", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_BOMB_0", L"../Data/Sprite/08.Effect/Aqua/WaterBombExplosion_0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_BOMB_1", L"../Data/Sprite/08.Effect/Aqua/WaterBombExplosion_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_BOMB_2", L"../Data/Sprite/08.Effect/Aqua/WaterBombExplosion_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_BOMB_3", L"../Data/Sprite/08.Effect/Aqua/WaterBombExplosion_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_BOMB_4", L"../Data/Sprite/08.Effect/Aqua/WaterBombExplosion_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_BOMB_5", L"../Data/Sprite/08.Effect/Aqua/WaterBombExplosion_5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"WATER_BOMB_BOMB_6", L"../Data/Sprite/08.Effect/Aqua/WaterBombExplosion_6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::DOWN, fDuration);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	return true;
}

void CWaterBomb_P::Explode()
{
	m_pAnimator->Play(L"BOOMB", false);
	m_fRenderMag = m_fBombRenderMag;
	m_bMove = false;

	CSoundMgr::Get_Inst()->StopSound(SOUND_PLAYER_SKILL);
	CSoundMgr::Get_Inst()->PlaySound(L"AquaBeamStart.wav", SOUND_PLAYER_SKILL, 1.f);

	m_pCollider->Set_Active(true);
	m_pRigidbody->Set_Active(false);
	m_pRigidbody->Zero_Velocity();
	m_pRigidbody->Zero_Force();
	
	m_bBomb = true;
	CCameraMgr::Get_Inst()->Shake_Camera(0.2f, 30.f);
}


void CWaterBomb_P::Render_Shadow(HDC _hDC)
{
	// m_fDis를 가지고 결정한다.

	int iShadowWidth = 150, iShadowHeight = 100;

	VEC2 vShadowPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_vDropPoint);

	Rect rc{ INT(vShadowPos.fX - iShadowWidth * 0.5f),
			INT(vShadowPos.fY - iShadowHeight * 0.5f + m_pTransform->Get_Scale().fY * 0.5f),
			INT(iShadowWidth),
			INT(iShadowHeight) };
	SolidBrush brush(Color(120, 10, 10, 10));

	m_Graphcis.FillEllipse(&brush, rc);
}

const bool CWaterBomb_P::Check_Drop()
{
	if (m_pTransform->Get_Pos().fY > m_vDropPoint.fY && !m_bBomb)
		Explode();

	return false;
}
