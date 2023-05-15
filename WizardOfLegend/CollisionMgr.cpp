#include "stdafx.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "Collider.h"
#include "Component.h"

CCollisionMgr* CCollisionMgr::m_pInst = nullptr;

CCollisionMgr::CCollisionMgr()
	: m_fColDistance(200.f)
{
}

CCollisionMgr::~CCollisionMgr()
{
}

const bool CCollisionMgr::Init()
{
	Reset();
	return true;
}

void CCollisionMgr::Update()
{
	for (UINT iRow = 0; iRow < (UINT)OBJ_TYPE::TYPEEND; ++iRow)
	{
		for (UINT iCol = iRow; iCol < (UINT)OBJ_TYPE::TYPEEND; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol)) // ���� �� �� �ڸ� ��Ʈ�� �˻�
			{
				Collision_Update((OBJ_TYPE)iRow, (OBJ_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::Collision_Update(const OBJ_TYPE& _eLeft, const OBJ_TYPE& _eRight)
{
	list<CObj*>* pListLeft = CObjMgr::Get_Inst()->Get_ObjList(_eLeft);
	list<CObj*>* pListRight = CObjMgr::Get_Inst()->Get_ObjList(_eRight);

	map<ULONGLONG, bool>::iterator iter;

	if (nullptr == pListLeft || nullptr == pListRight) return;

	for (auto& iterLeft : *pListLeft)
	{
		if (_eLeft == OBJ_TYPE::PROJECTILE && _eRight == OBJ_TYPE::TILE && pListLeft->size() > 2 && iterLeft == pListLeft->back())
			int j = 0;

		if (nullptr == iterLeft->Get_Collider() || !(iterLeft->Get_Collider()->Get_Owner()->Is_Active())) continue;

		for (auto& iterRight : *pListRight)
		{
			if (nullptr == iterRight->Get_Collider() || !(iterRight->Get_Collider()->Get_Owner()->Is_Active()) || iterLeft == iterRight) continue;
			
			CCollider* pLeftCol = iterLeft->Get_Collider();
			CCollider* pRightCol = iterRight->Get_Collider();

			if (!pLeftCol->Is_Active() || !pRightCol->Is_Active()) continue;

			//if (!Is_Close(pLeftCol, pRightCol)) continue; // �Ÿ� ��� �浹 �˻� ����

			// �� �浹ü ���� ���� ���̵� ����
			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->Get_ID();
			ID.iRight_id = pRightCol->Get_ID();

			iter = m_mapColInfo.find(ID.ID);

			// �� �浹ü �� ���� �˻� (�ʿ� �����Ͱ� ������ �ʿ� ����Ѵ�)
			if (m_mapColInfo.end() == iter) 
			{
				m_mapColInfo.insert({ ID.ID, false });
				iter = m_mapColInfo.find(ID.ID);
			}

			float fCX = 0.f, fCY = 0.f;

			if (Is_Collision(pLeftCol, pRightCol, &fCX, &fCY))
			{
				if (iter->second)
				{
					pLeftCol->OnCollision_Stay(pRightCol, fCX, fCY);
					pRightCol->OnCollision_Stay(pLeftCol, fCX, fCY);
				}
				else
				{
					pLeftCol->OnCollision_Enter(pRightCol, fCX, fCY);
					pRightCol->OnCollision_Enter(pLeftCol, fCX, fCY);

					iter->second = true;
				}
			}
			else
			{
				if (iter->second)
				{
					pLeftCol->OnCollision_Exit(pRightCol, fCX, fCY);
					pRightCol->OnCollision_Exit(pLeftCol, fCX, fCY);
					iter->second = false;
				}
			}
		}
	}
}

const bool CCollisionMgr::Is_Collision(CCollider* _pLeftCol, CCollider* _pRightCol, float* _fCX, float* _fCY)
{
	VEC2 vLeftPos = _pLeftCol->Get_Pos();
	VEC2 vLeftScale = _pLeftCol->Get_Scale();

	VEC2 vRightPos = _pRightCol->Get_Pos();
	VEC2 vRightScale = _pRightCol->Get_Scale();

	if (fabs(vRightPos.fX - vLeftPos.fX) < (vLeftScale.fX + vRightScale.fX) * 0.5f
		&& fabs(vRightPos.fY - vLeftPos.fY) < (vLeftScale.fY + vRightScale.fY) * 0.5f)
	{
		*_fCX = (vLeftScale.fX + vRightScale.fX) * 0.5f - fabs(vRightPos.fX - vLeftPos.fX);
		*_fCY = (vLeftScale.fY + vRightScale.fY) * 0.5f - fabs(vRightPos.fY - vLeftPos.fY);

		return true;
	}

	return false;
}

void CCollisionMgr::Check_Group(const OBJ_TYPE & _eLeft, const OBJ_TYPE & _eRight)
{	
	// �� ���� ���� �׷� Ÿ���� ������ ��´�. ū ���� ���� ���

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow)
	{
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))		// üũ ���� �˻�
		m_arrCheck[iRow] &= ~(1 << iCol);	// �̹� üũ �Ǿ��ٸ� �ٽ� ����
	else
		m_arrCheck[iRow] |= (1 << iCol);	// üũ ���ٸ� üũ
}
 
const bool CCollisionMgr::Is_Close(CCollider * _pLeftCol, CCollider * _pRightCol) // �浹 �˻� �Ÿ� ��� ����ȭ
{
	if (nullptr == _pLeftCol || nullptr == _pRightCol) return false;

	VEC2 vLeftPos = _pLeftCol->Get_Pos();
	VEC2 vRightPos = _pRightCol->Get_Pos();

	float fDis = sqrtf(pow(vRightPos.fX - vLeftPos.fX, 2) + pow(vRightPos.fY - vLeftPos.fY, 2));

	if (m_fColDistance < fDis)
		return false;
	else
		return true;
}
