#include "stdafx.h"
#include "SpawnTrigger.h"
#include "Transform.h"
#include "Collider.h"
#include "EventMgr.h"

CSpawnTrigger::CSpawnTrigger(const OBJ_TYPE& _eType)
	: CObj(_eType)
	, m_iMaxLevel(0)
	, m_iCurLevel(0)
{
}

CSpawnTrigger::~CSpawnTrigger()
{
}

void CSpawnTrigger::OnCollision_Enter(CCollider * _pOtherCol, const float & _fCX, const float & _fCY)
{
	if (OBJ_TYPE::PLAYER == _pOtherCol->Get_Owner()->Get_ObjType())
	{
		m_pCollider->Set_Active(false);
		++m_iCurLevel;
		Spawn_CurLevel();
	}
}

const bool CSpawnTrigger::Spawn_CurLevel()
{
	if (m_iMaxLevel < m_iCurLevel) return false;

	map<UINT, vector<CObj*>>::iterator iter = m_mapSpawn.find(m_iCurLevel);

	if (iter == m_mapSpawn.end())
		return false;
	else
	{
		for (auto& iter : iter->second)
		{
			if (nullptr == iter) continue;

			iter->Set_Active(true);
			CEventMgr::Get_Inst()->Add_Obj(iter);
		}
	}

	return true;
}

const bool CSpawnTrigger::Is_EndLevel()
{
	map<UINT, vector<CObj*>>::iterator mapIter = m_mapSpawn.find(m_iCurLevel);

	if (mapIter == m_mapSpawn.end())
		return false;
	else
	{
		for (vector<CObj*>::iterator iter = mapIter->second.begin(); iter != mapIter->second.end();)
		{
			if ((*iter)->Is_Dead())
				iter = mapIter->second.erase(iter); // 몬스터의 메모리 해제는 몬스터 자기 자신이 호출한다.
			else
				++iter;
		}
		
		if (mapIter->second.empty()) // 현재 레벨의 모든 오브젝트가 사망
		{
			++m_iCurLevel; // 현재 단계를 1 증가

			if (m_iCurLevel > m_iMaxLevel) // 마지막 단계였다면
			{
				CEventMgr::Get_Inst()->Delete_Obj(this); // 자기 자신을 삭제한다.
				return true;
			}
			else
			{
				Spawn_CurLevel();
			}
		}
	}

	return true;
}

