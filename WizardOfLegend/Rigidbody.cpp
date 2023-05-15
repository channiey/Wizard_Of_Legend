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

	m_vForce += (m_vGravity * m_fMass);							// �߷� �ۿ� (F = ma  �� = ���� X ���ӵ�)

	m_vAcc = m_vForce / m_fMass;								// ���ӵ� = ���� �ѷ� / ����

	m_vVelocity += m_vAcc * DT;									// �ӵ� = ���ӵ� * �ð� (V += A * DT = ����ӵ� (�ӵ� = ���ӵ��� ����)) 

	if (m_vVelocity.Length() > m_fMaxSpeed)						// �ִ� �ӷ� ����
	{
		m_vVelocity.Normalize();
		m_vVelocity *= m_fMaxSpeed;
	}

	VEC2 vPos = m_pOwner->Get_Transform()->Get_Pos(); // �ݸ��� üũ ���� �÷��̾� ���� ������

	vPos += m_vVelocity;

	m_pOwner->Get_Transform()->Set_Pos(vPos);

	m_vForce.Zero();
	m_vAcc.Zero();
}

void CRigidbody::Render(HDC _hDC)
{
	if (!m_bActive) return;
}
