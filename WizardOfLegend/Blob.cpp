#include "stdafx.h"
#include "Blob.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Animator.h"
#include "Animation.h"
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "DamageUI.h"
#include "ObjPool.h"
#include "Projectile.h"
#include "MainGame.h"
#include "CameraMgr.h"
#include "BolbAnimator.h"
#include "Blob_P.h"
#include "SoundMgr.h"
#include "GameMgr.h"
#include "Gold.h"
CBlob::CBlob(const OBJ_TYPE& _eType)
	: CEnemy(_eType)
	, m_eState(BLOB_STATE::SPAWN)
	, m_fIdleTime(0.5f)
	, m_fJumpPower(4500.f)
	, m_pTarget(nullptr)	
	, m_fAccTime(0.f)
	, m_Graphics(CMainGame::Get_Inst()->Get_SubDC())
	, m_fmoveX(150.f)
	, m_fJumpTime(0.7f)
	, m_pProj(nullptr)
{
	ZeroMemory(&m_tLine, sizeof(LINE));
}


CBlob::~CBlob()
{
	Release();
}

const bool CBlob::Init()
{
	Create_Transform();
	Create_Collider();
	Create_Rigidbody();

	m_pTransform->Init();
	m_pTransform->Set_Pos(VEC2(3215+300, 1375)); // 어차피 스폰 트리거에서 설정 (지금은 테스트 용도) 3300, 2600
	m_pTransform->Set_Scale(VEC2(50, 50));

	m_pCollider->Init();
	m_pCollider->Set_Pos(m_pTransform->Get_Pos()); // 어차피 스폰 트리거에서 설정 (지금은 테스트 용도)
	m_pCollider->Set_OffSet(VEC2(0, m_pTransform->Get_Scale().fY * 0.5f));
	m_pCollider->Set_Scale(m_pTransform->Get_Scale());

	m_pRigidbody->Init();
	m_pRigidbody->Set_Gravity(GRAVITY);
	m_pRigidbody->Set_Active(false);

	m_pAnimator = new CBolbAnimator(this);
	m_pAnimator->Init();
	m_listComponent.push_back(m_pAnimator);
	Init_Animator();
	m_pAnimator->Play(L"SPAWN", true);

	// Renser Setting
	m_fRenderMag = 3.5f;

	// Speed
	m_fSpeed = DF_SPEED * 0.35f;

	// HP
	m_fMaxHp = 30.f;
	m_fCurHP = m_fMaxHp;

	// AD
	m_fAD = DF_AD * 0.5f;

	// Target
	Set_Target();

	m_pTransform->Set_Look(VEC2_RIGHT); //왜인지 이 몬스터는 처음에 룩벡이 up상태라 애니메이션에서 eCurDir이 up하니까 없으니 오류

	// Proj
	m_pProj = new CBlob_P(OBJ_TYPE::PROJECTILE, this);
	m_pProj->Init();
	m_pProj->Set_Active(false);	

	m_pDropItem = new CGold(OBJ_TYPE::ITEM, ITEM_TYPE::GOLD);
	m_pDropItem->Init();

	return true;
}

void CBlob::Update()
{
	if (!m_bActive) return;

	switch (m_eState)
	{
	case BLOB_STATE::IDLE:
	{
		m_fAccTime += DT; // 정해진 시간동안 아이들 상태로 대기한 뒤, 시간이 지나면 점프 상태로 전환한다.

		if (m_fAccTime >= m_fIdleTime)
		{
			Set_Jump(true);
			m_fAccTime = 0.f;
		}
	}
		break;
	case BLOB_STATE::JUMP:
	{
		// Move X
		float fMoveX = m_fmoveX * DT;

		if (0.f > m_pTransform->Get_Look().fX)
			fMoveX *= -1.f;

		m_pTransform->Add_Pos_X(fMoveX);
		

		// 체공시간을 체크하여 점프를 해제한다.
		m_fAccTime += DT; 

		if (m_fAccTime >= m_fJumpTime)
		{
			Set_Jump(false);
			m_fAccTime = 0.f;
			m_pAnimator->Play(L"IDLE", true);
		}
	}
		break;
	case BLOB_STATE::HURT:
	{
		if (Fall())
		{

		}
	}
		break;
	case BLOB_STATE::DEAD:
		break;
	default:
		break;
	}
}

void CBlob::LateUpdate()
{
	if (!m_bActive) return;

	Component_LateUpdate();
}

void CBlob::Render(HDC _hDC)
{
	if (!m_bActive) return;

	// Render Shadow
	VEC2 vPos{};
	Rect rc{};
	int iShadowWidth = 60, iShadowHeight = 30;

	if(BLOB_STATE::JUMP == m_eState)
		vPos = CCameraMgr::Get_Inst()->Get_RenderPos(VEC2{ m_pTransform->Get_Pos().fX, Calculate_Line_Y() });
	else
		vPos = CCameraMgr::Get_Inst()->Get_RenderPos(m_pTransform->Get_Pos());

	rc = { INT(vPos.fX - iShadowWidth * 0.5f),
			INT(vPos.fY - iShadowHeight * 0.5f + m_pTransform->Get_Scale().fY - 25.f),
			INT(iShadowWidth),
			INT(iShadowHeight) };
	SolidBrush brush(Color(120, 10, 10, 10));
	m_Graphics.FillEllipse(&brush, rc);

	Component_Render(_hDC);
}

void CBlob::Release()
{
	CEventMgr::Get_Inst()->Delete_Obj(m_pProj);
}

void CBlob::OnCollision_Enter(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
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
				m_eState = BLOB_STATE::DEAD;
				m_pAnimator->Play(L"DEAD", false); // 이 애니메이션 끝나면 애니메이터에서 m_bDead true 처리 후 삭제
				m_fCurHP = 0.f;

				// 죽었을 때 충돌처리와 넉백을 막기 위해 비활성화
				m_pRigidbody->Set_Active(false);
				m_pCollider->Set_Active(false);

				CSoundMgr::Get_Inst()->StopSound(SOUND_BLOB);
				CSoundMgr::Get_Inst()->PlaySound(L"BlobDead.wav", SOUND_BLOB, 0.5f);

				CGameMgr::Get_Inst()->Plus_Kill();

				m_pDropItem->Set_Active(true);
				m_pDropItem->Get_Transform()->Set_Pos(VEC2{ m_pTransform->Get_Pos().fX, m_pTransform->Get_Pos().fY - 10 });
				CEventMgr::Get_Inst()->Add_Obj(m_pDropItem);

			}
			else
			{
				//m_eState = BLOB_STATE::HURT; // 이 애니메이션 끝나면 스테이트 변경 함수 호출, 벨로시티 초기화
				//m_pAnimator->Play(L"HURT", false);

			
				CSoundMgr::Get_Inst()->StopSound(SOUND_BLOB);
				CSoundMgr::Get_Inst()->PlaySound(L"BlobHurt.wav", SOUND_BLOB, 0.5f);

			}

			// 데미지 스킨
			CDamageUI* pUI = static_cast<CDamageUI*>(CObjPool<CDamageUI>::Get_Obj());
			if (nullptr != pUI)
			{
				pUI->Set_UIType(UI_TYPE::FIXED);
				pUI->Reset(m_pTransform->Get_Pos());
				CGameMgr::Get_Inst()->Add_PlayerToEnemyDamage(pUI->Get_Damage());
			}
		}
	}
}

void CBlob::OnCollision_Stay(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (!m_bActive) return;
}

void CBlob::OnCollision_Exit(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (!m_bActive) return;
}


const float CBlob::Calculate_Line_Y()
{
	VEC2 vCurPos = m_pTransform->Get_Pos();

	float fY = ((m_tLine.p2.fY - m_tLine.p1.fY) / (m_tLine.p2.fX - m_tLine.p1.fX)) * (vCurPos.fX - m_tLine.p1.fX) + m_tLine.p1.fY;

	return fY;
}

const bool CBlob::Fall()
{
	return false;
}

const bool CBlob::Set_Jump(const bool _bJump)
{
	if (nullptr == m_pTarget)
		if (!Set_Target())
			return false;

	if (BLOB_STATE::JUMP != m_eState && _bJump) // 점프 시작
	{
		// 스테이트 변경
		m_eState = BLOB_STATE::JUMP;

		// 리지드바디 활성화
		m_pRigidbody->Set_Active(true);

		// 방향벡터 생성
		VEC2 vLook = (m_pTarget->Get_Transform()->Get_Pos() - m_pTransform->Get_Pos()).Normalized(); 
		if (0.f < vLook.fX)
			m_pTransform->Set_Look(VEC2_RIGHT);
		else
			m_pTransform->Set_Look(VEC2_LEFT);

		// 성규님 피드백 반영 점프 루틴
		{
			// 플레이어와 몬스터간 X 차이 / 몬스터가 움직이는 x 스피드(현재 100 설정)
			float t = fabs(m_pTransform->Get_Pos().fX - m_pTarget->Get_Transform()->Get_Pos().fX) / m_fmoveX;

			// (초기속도) : t1일 때의 속도값 = p / m
			float v1 = m_fJumpPower / m_pRigidbody->Get_Mass();

			// 점프 파워
			float p = m_fJumpPower;

			// 플레이어와 몬스터간 Y 차이
			float d = m_pTransform->Get_Pos().fY - m_pTarget->Get_Transform()->Get_Pos().fY;
			

			// 최종 충격량 계산
			float fImpulse = (d / (v1 * t) + 0.5f) * p;

			if (fImpulse > 5000.f || fImpulse < -1000.f || v1 == 0 || t == 0) // 점프가 불가능한 경우 예외 처리
			{
				m_pRigidbody->Set_Active(false);
				m_eState = BLOB_STATE::IDLE;
				return false;
			}

			// 적용(실질적으로는 Force 즉 힘이 아닌 충격량이 올바른 표현이다.
			m_pRigidbody->Add_Force(VEC2{ 0.f, -fImpulse });
			m_pAnimator->Play(L"JUMP", true);
			CSoundMgr::Get_Inst()->StopSound(SOUND_BLOB);
			CSoundMgr::Get_Inst()->PlaySound(L"BlobAttack.wav", SOUND_BLOB, 0.5f);


		}

		m_tLine = LINE{ m_pTransform->Get_Pos(), m_pTarget->Get_Transform()->Get_Pos() };
	}
	else if(BLOB_STATE::JUMP == m_eState && !_bJump) // 점프 끝 (착지 세팅)
	{
		if (BLOB_STATE::HURT != m_eState && BLOB_STATE::DEAD != m_eState)
		{
			m_eState = BLOB_STATE::IDLE;

			m_pRigidbody->Set_Active(false);
			m_pRigidbody->Zero_Velocity();
			//m_tLine = LINE{ VEC2_ZERO, VEC2_ZERO };
		}
	}
	return true;
}


const bool CBlob::Set_Target()
{
	m_pTarget = CObjMgr::Get_Inst()->Get_Player();
	
	if (nullptr == m_pTarget) 
		return false;
	else
		return true;
}

const bool CBlob::Init_Animator()
{
	// IDLE, JUMP, HURT, DEAD

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
	// IDLE
	{	
		fDuratuon = 0.2f;
		pAnimation = new CAnimation(L"IDLE", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_IDLE_RIGHT_0", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobIdle0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_IDLE_RIGHT_1", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobIdle1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_IDLE_LEFT_0", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobIdle0.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_IDLE_LEFT_1", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobIdle1.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// JUMP
	{
		fDuratuon = 0.1f;
		pAnimation = new CAnimation(L"JUMP", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_JUMP_RIGHT_0", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobJump.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_JUMP_LEFT_0", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobJump.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// HURT
	{
		fDuratuon = 0.3f;
		pAnimation = new CAnimation(L"HURT", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_HURT_RIGHT_0", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobHurt.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_HURT_LEFT_0", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobHurt.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	// DEAD
	{
		fDuratuon = 0.1f;
		pAnimation = new CAnimation(L"DEAD", m_pAnimator);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_DEAD_RIGHT_0", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobDead0.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_DEAD_RIGHT_1", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobDead1.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_DEAD_RIGHT_2", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobDead2.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_DEAD_RIGHT_3", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobDead3.png");
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::RIGHT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_DEAD_LEFT_0", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobDead0.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_DEAD_LEFT_1", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobDead1.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_DEAD_LEFT_2", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobDead2.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		pTex = CResMgr::Get_Inst()->Load_Tex(L"BLOB_DEAD_LEFT_3", L"../Data/Sprite/04_Enemy/0_Blob/Used/BlobDead3.png", true);
		pAnimation->PushBack_Tex(pTex, DIR_TYPE::LEFT, fDuratuon);

		m_pAnimator->PushBack_Ani(pAnimation);
	}

	return true;
}

void CBlob::Set_Active(const bool _bActive)
{
	m_bActive = _bActive;

	if (m_bActive)
	{
		m_pTransform->Set_Look(VEC2_RIGHT);
		m_pAnimator->Play(L"SPAWN", false);
		m_eState = BLOB_STATE::SPAWN;
		m_pProj->Set_Active(true);
		CEventMgr::Get_Inst()->Add_Obj(m_pProj);

		CSoundMgr::Get_Inst()->StopSound(SOUND_BLOB);
		CSoundMgr::Get_Inst()->PlaySound(L"CardSpawn.wav", SOUND_BLOB, 1.f);

	}
}

