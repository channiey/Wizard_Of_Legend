#include "stdafx.h"
#include "GhoulLarge.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Transform.h"
#include "Collider.h"
#include "Animator.h"
#include "Animation.h"
#include "Rigidbody.h"
#include "TimeMgr.h"
#include "CameraMgr.h"
#include "ObjMgr.h"
#include "Projectile.h"
#include "DamageUI.h"
#include "ObjPool.h"
#include "GhoulLargeAnimator.h"
#include "MainGame.h"
#include "CyclopsAttack_P.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "Gold.h"
CGhoulLarge::CGhoulLarge(const OBJ_TYPE& _eType)
	: CEnemy(_eType)
	, m_eState(CLOSE_ENEMY_STATE::TYPEEND)
	, m_pTarget(nullptr)
	, m_fAttackRange(0.f)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	, m_fProjTermX(0.f)
	, m_pProj(nullptr)
	, m_fAttackStartTime(0.f)
	, m_fAttackEndTime(0.f)
	, m_fAccTime(0.f)
	, m_bAttack(false)
{
}


CGhoulLarge::~CGhoulLarge()
{
}
const bool CGhoulLarge::Init()
{
	// Set Component
	Create_Transform();
	Create_Collider();
	Create_Rigidbody();

	m_pTransform->Init();
	m_pTransform->Set_Pos(VEC2(3300, 2600)); // 어차피 스폰 트리거에서 설정 (지금은 테스트 용도)
	m_pTransform->Set_Scale(VEC2(100, 100));

	m_pCollider->Init();
	m_pCollider->Set_Pos(m_pTransform->Get_Pos()); // 어차피 스폰 트리거에서 설정 (지금은 테스트 용도)
	m_pCollider->Set_OffSet(VEC2(0, m_pTransform->Get_Scale().fY * 0.5f));
	m_pCollider->Set_Scale(m_pTransform->Get_Scale());

	m_pRigidbody->Init();

	m_pAnimator = new CGhoulLargeAnimator(this);
	m_pAnimator->Init();
	m_listComponent.push_back(m_pAnimator);
	Init_Animator();
	m_pAnimator->Play(L"MOVE", true);

	// Renser Setting
	m_fRenderMag = 4.f;

	// Speed
	m_fSpeed = DF_SPEED * 0.35f;

	// HP
	m_fMaxHp = 80.f;
	m_fCurHP = m_fMaxHp;

	// AD
	m_fAD = DF_AD * 0.5f;

	// Set Field
	m_eState = CLOSE_ENEMY_STATE::SPAWN;
	m_fAttackRange = 70.f;
	m_fProjTermX = 20.f;
	m_fAttackStartTime = 0.4f;
	m_fAttackEndTime = 0.6f;

	m_pProj = new CCyclopsAttack_P(OBJ_TYPE::PROJECTILE, this);
	m_pProj->Init();
	m_pProj->Set_Active(false);
	m_pProj->Set_On(false);
	m_pProj->Get_Collider()->Set_Scale(VEC2{ 200.f, 200.f });

	m_pTransform->Set_Look(VEC2_RIGHT);

	m_pDropItem = new CGold(OBJ_TYPE::ITEM, ITEM_TYPE::GOLD);
	m_pDropItem->Init();
	return true;
}

void CGhoulLarge::Update()
{
	if (!m_bActive) return;

	Set_State(); // 플레이어와의 거리를 비교하여 Chase or Attack을 결정한다.

	if (CLOSE_ENEMY_STATE::ATTACK == m_eState)
	{
		m_fAccTime += DT;

		if (m_fAccTime >= m_fAttackStartTime)  // 공격 투사체 활성화
		{
			Set_Proj_Active(true);
		}
	}


	switch (m_eState)
	{
	case CLOSE_ENEMY_STATE::CHASE:
		Chase();
		break;
	case CLOSE_ENEMY_STATE::ATTACK:
		Attack();
		break;
	case CLOSE_ENEMY_STATE::HURT:
		break;
	case CLOSE_ENEMY_STATE::DEAD:
		break;
	default:
		break;
	}
}

void CGhoulLarge::LateUpdate()
{
	if (!m_bActive) return;

	Component_LateUpdate();
}

void CGhoulLarge::Render(HDC _hDC)
{
	if (!m_bActive) return;

	// Render Shadow
	int iShadowWidth = 140, iShadowHeight = 70;
	VEC2 vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());
	Rect rc{ INT(vPos.fX - iShadowWidth * 0.5f),
		INT(vPos.fY - iShadowHeight * 0.5f + m_pTransform->Get_Scale().fY - 10),
		INT(iShadowWidth),
		INT(iShadowHeight) };
	SolidBrush brush(Color(120, 10, 10, 10));
	m_Graphics.FillEllipse(&brush, rc);

	// Component Render
	Component_Render(_hDC);
}

void CGhoulLarge::Release()
{
	CEventMgr::Get_Inst()->Delete_Obj(m_pProj);
}

void CGhoulLarge::OnCollision_Enter(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (!m_bActive) return;
	CObj* pObj = _pOtherCol->Get_Owner();

	if (OBJ_TYPE::PROJECTILE == pObj->Get_ObjType())
	{
		if (OBJ_TYPE::PLAYER == static_cast<CProjectile*>(pObj)->Get_Owner()->Get_ObjType())
		{
			// 데미지 설정
			m_fCurHP -= pObj->Get_AD();
			if (m_fCurHP < 0)
			{
				m_pRigidbody->Zero_Velocity();
				m_eState = CLOSE_ENEMY_STATE::DEAD;
				m_pAnimator->Play(L"DEAD", false); // 이 애니메이션 끝나면 애니메이터에서 m_bDead true 처리 후 삭제
				m_fCurHP = 0.f;

				// 죽었을 때 충돌처리와 넉백을 막기 위해 비활성화
				m_pRigidbody->Set_Active(false);
				m_pCollider->Set_Active(false);

				CSoundMgr::Get_Inst()->StopSound(SOUND_GHOUL);
				CSoundMgr::Get_Inst()->PlaySound(L"EnemyDead1.wav", SOUND_GHOUL, 1.f);
				CGameMgr::Get_Inst()->Plus_Kill();

				m_pDropItem->Set_Active(true);
				m_pDropItem->Get_Transform()->Set_Pos(VEC2{ m_pTransform->Get_Pos().fX, m_pTransform->Get_Pos().fY - 10 });
				CEventMgr::Get_Inst()->Add_Obj(m_pDropItem);
			}
			else
			{
				m_eState = CLOSE_ENEMY_STATE::HURT; // 이 애니메이션 끝나면 스테이트 변경 함수 호출, 벨로시티 초기화
				m_pAnimator->Play(L"HURT", false);

				CSoundMgr::Get_Inst()->StopSound(SOUND_GHOUL);
				CSoundMgr::Get_Inst()->PlaySound(L"EnemyHurt1.wav", SOUND_GHOUL, 1.f);

			}

			// 데미지 스킨
			CDamageUI* pUI = static_cast<CDamageUI*>(CObjPool<CDamageUI>::Get_Obj());
			if (nullptr != pUI)
			{
				pUI->Set_UIType(UI_TYPE::FIXED);
				CGameMgr::Get_Inst()->Add_PlayerToEnemyDamage(pUI->Get_Damage());
				pUI->Reset(m_pTransform->Get_Pos());
			}

			if (pObj->Get_Transform()->Get_Theta() == 0.f)
			{
				VEC2 vLook = pObj->Get_Transform()->Get_Look();
				if (fabs(vLook.fX) > fabs(vLook.fY))
				{
					m_pRigidbody->Zero_Velocity();
					m_pRigidbody->Add_Force(vLook * DF_KNOCKBACK_SPEED);
				}
				else
				{
					m_pRigidbody->Zero_Velocity();
					m_pRigidbody->Add_Force(vLook * DF_KNOCKBACK_SPEED * -1.f);
				}
			}
			else
			{
				KnockBack(pObj->Get_Transform()->Get_Theta());
			}
		}
	}
}

void CGhoulLarge::OnCollision_Stay(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (!m_bActive) return;
}

void CGhoulLarge::OnCollision_Exit(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (!m_bActive) return;
}


void CGhoulLarge::Chase()
{
	if (nullptr == m_pTarget) return;

	VEC2 vLook = m_pTarget->Get_Transform()->Get_Pos() - m_pTransform->Get_Pos();

	vLook.Normalize();

	m_pTransform->Add_Pos(vLook * m_fSpeed * DT);

	m_pProj->Get_Transform()->Set_Theta(acosf(vLook.fX));

	if (0.f < vLook.fX)
		m_pTransform->Set_Look(VEC2_RIGHT);
	else
		m_pTransform->Set_Look(VEC2_LEFT);

	m_pAnimator->Play(L"MOVE", true);
}

void CGhoulLarge::Attack()
{
	if (m_bAttack) return;

	m_bAttack = true;

	VEC2 vPos = m_pTransform->Get_Pos();

	if (0.f < m_pTransform->Get_Look().fX)
		vPos.fX += m_fProjTermX;
	else
		vPos.fX -= m_fProjTermX;

	m_pProj->Get_Transform()->Set_Pos(vPos);
	m_pProj->Get_Collider()->Set_Pos(vPos);

	m_pAnimator->Play(L"ATTACK", false);

	
}

void CGhoulLarge::KnockBack(const float & _fTheta)
{
	VEC2 vKnockBack{ cosf(_fTheta), sinf(_fTheta) };
	vKnockBack.Normalize();

	if (fabs(vKnockBack.fX) > fabs(vKnockBack.fY))
	{
		m_pRigidbody->Zero_Velocity();
		m_pRigidbody->Add_Force(vKnockBack * DF_KNOCKBACK_SPEED);
	}
	else
	{
		m_pRigidbody->Zero_Velocity();
		m_pRigidbody->Add_Force(vKnockBack * DF_KNOCKBACK_SPEED * -1.f);
	}
}

void CGhoulLarge::Set_State(const CLOSE_ENEMY_STATE & _eState)
{
	m_eState = _eState; m_fAccTime = 0.f;

	if(CLOSE_ENEMY_STATE::ATTACK != _eState)
	{
		m_bAttack = false;
		Set_Proj_Active(false);
	}
}

void CGhoulLarge::Set_Proj_Active(const bool & _bActive)
{
	if (_bActive)
	{
		if (!m_pProj->Is_On())
		{
			m_pProj->Set_On(_bActive);
			CSoundMgr::Get_Inst()->StopSound(SOUND_GHOUL);
			CSoundMgr::Get_Inst()->PlaySound(L"BlazingBlitzEnd.wav", SOUND_GHOUL, 0.7f);
		}

	}
	else
	{
		if (m_pProj->Is_On())
		{
			m_pProj->Set_On(_bActive);
		}
	}
	
}

void CGhoulLarge::Set_Active(const bool _bActive)
{
	m_bActive = _bActive;

	if (m_bActive)
	{
		m_pAnimator->Play(L"SPAWN", false);
		m_eState = CLOSE_ENEMY_STATE::SPAWN;
		m_pProj->Set_Active(true);
		CEventMgr::Get_Inst()->Add_Obj(m_pProj); 
		
		CSoundMgr::Get_Inst()->StopSound(SOUND_GHOUL);
		CSoundMgr::Get_Inst()->PlaySound(L"CardSpawn.wav", SOUND_GHOUL, 1.f);

	}
}


const bool CGhoulLarge::Set_Target()
{
	if (nullptr == m_pTarget)
		m_pTarget = CObjMgr::Get_Inst()->Get_Player();

	if (nullptr == m_pTarget)
		return false;
	else
		return true;
}

const float CGhoulLarge::Calculate_Distance()
{
	if (nullptr == m_pTarget)
		if (!Set_Target())
			return m_fAttackRange;

	VEC2 vLook = m_pTransform->Get_Pos() - m_pTarget->Get_Transform()->Get_Pos();

	return vLook.Length();
}

void CGhoulLarge::Set_State()
{
	if (CLOSE_ENEMY_STATE::HURT == m_eState
		|| CLOSE_ENEMY_STATE::DEAD == m_eState
		|| CLOSE_ENEMY_STATE::ATTACK == m_eState
		|| CLOSE_ENEMY_STATE::SPAWN == m_eState)
		return;

	if (Calculate_Distance() < m_fAttackRange)
		m_eState = CLOSE_ENEMY_STATE::ATTACK;
	else
	
		m_eState = CLOSE_ENEMY_STATE::CHASE;
}

const bool CGhoulLarge::Init_Animator()
{
	CTexture* pTex;
	CAnimation* pAnimation;
	float fDuratuon;

	// SPAWN
	{
		fDuratuon = 0.05f;

		pAnimation = new CAnimation(L"SPAWN", m_pAnimator);

		// C:\Users\ChanYoon\Desktop\Programming\Projects\Wizard of Legend\ProjectFile\WizardOfLegend\Data\Sprite\08.Effect\MonsterSpawn

		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_0", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_1", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_2", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_3", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_4", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_5", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_6", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_6.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_7", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_7.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_8", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_8.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_9", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_9.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_10", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_10.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_11", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_11.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_12", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_12.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_13", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_13.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_14", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_14.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_15", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_15.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_16", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_16.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_17", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_17.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_18", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_18.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_19", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_19.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_20", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_20.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_21", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_21.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_22", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_22.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_23", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_23.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_24", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_24.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_25", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_25.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_26", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_26.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"MON_STET_SPAWN_27", L"../Data/Sprite/08.Effect/MonsterSpawn/CardSpawnUnindexed_27.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		m_pAnimator->PushBack_Ani(pAnimation);
	}
	// MOVE (2)
	{
		fDuratuon = 0.1f;

		pAnimation = new CAnimation(L"MOVE", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_RIGHT_0", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_RIGHT_1", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_RIGHT_2", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_RIGHT_3", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_RIGHT_4", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_LEFT_0", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight0.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_LEFT_1", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight1.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_LEFT_2", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight2.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_LEFT_3", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight3.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_MOVE_LEFT_4", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeRunRight4.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		m_pAnimator->PushBack_Ani(pAnimation);
	}
	// ATTACK (2) -> 방향을 4가지 사용하려면 모든 애니메이션 방향이 4개 존재해야 한다. 현재 애니메이션 클래스 구조상으로는
	{
		fDuratuon = 0.1f;

		pAnimation = new CAnimation(L"ATTACK", m_pAnimator);
		
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_RIGHT_0", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_RIGHT_1", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_RIGHT_2", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_RIGHT_3", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_RIGHT_4", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon + 0.5f);

		
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_LEFT_0", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_0.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_LEFT_1", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_1.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_LEFT_2", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_2.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_LEFT_3", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_3.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);
		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_ATTACK_LEFT_4", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeAttack0_4.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon + 0.5f);

		m_pAnimator->PushBack_Ani(pAnimation);
	}
	// HURT (2)
	{
		fDuratuon = 0.8f;

		pAnimation = new CAnimation(L"HURT", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_HURT_RIGHT_0", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_HURT_LEFT_0", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead0.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		m_pAnimator->PushBack_Ani(pAnimation);
	}
	// DEAD (2)
	{
		fDuratuon = 0.2f;

		pAnimation = new CAnimation(L"DEAD", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_RIGHT_0", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_RIGHT_1", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_RIGHT_2", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_RIGHT_3", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_RIGHT_4", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_RIGHT_5", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_LEFT_0", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_LEFT_1", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_LEFT_2", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_LEFT_3", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_LEFT_4", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead4.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"GHOULLARGE_DEAD_LEFT_5", L"../Data/Sprite/04_Enemy/0_GhoulLarge/GhoulLargeDead5.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);


		m_pAnimator->PushBack_Ani(pAnimation);
	}
	return false;
}
