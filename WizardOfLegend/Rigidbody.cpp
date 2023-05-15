#include "stdafx.h"
#include "Rigidbody.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "Obj.h"

CRigidbody::CRigidbody(CObj* _pOwner)
	: CComponent(_pOwner)
	, m_fMass(DF_MASS)
	, m_fMaxSpeed(30.f)
{
	ZeroMemory(&m_vForce, sizeof(VEC2));
	ZeroMemory(&m_vAcc, sizeof(VEC2));
	ZeroMemory(&m_vVelocity, sizeof(VEC2));
	ZeroMemory(&m_vGravity, sizeof(VEC2));
}

CRigidbody::~CRigidbody()
{
}

void CRigidbody::Init()
{
}

void CRigidbody::LateUpdate()
{
	if (!m_bActive || nullptr == m_pOwner) return;

	m_vForce += (m_vGravity * m_fMass);							// 중력 작용 (F = ma  힘 = 질량 X 가속도)

	m_vAcc = m_vForce / m_fMass;								// 가속도 = 힘의 총량 / 질량

	m_vVelocity += m_vAcc * DT;									// 속도 = 가속도 * 시간 (V += A * DT = 현재속도 (속도 = 가속도의 누적)) 

	if (m_vVelocity.Length() > m_fMaxSpeed)						// 최대 속력 제한
	{
		m_vVelocity.Normalize();
		m_vVelocity *= m_fMaxSpeed;
	}

	VEC2 vPos = m_pOwner->Get_Transform()->Get_Pos(); // 콜리전 체크 이전 플레이어 최종 포지션

	vPos += m_vVelocity;

	m_pOwner->Get_Transform()->Set_Pos(vPos);

	m_vForce.Zero();
	m_vAcc.Zero();
}

void CRigidbody::Render(HDC _hDC)
{
	if (!m_bActive) return;
}
