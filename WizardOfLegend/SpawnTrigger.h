#pragma once
#include "Obj.h"

class CSpawnTrigger :
	public CObj
{
public:
	CSpawnTrigger(const OBJ_TYPE& _eType);
	virtual ~CSpawnTrigger();

public:
	virtual void		OnCollision_Enter(CCollider* _pOtherCol, const float& _fCX, const float& _fCY) override;

protected:
	
	virtual const bool	Spawn_CurLevel();
	// 필드인 맵에 접근하여 현재 스폰 레벨에 해당하는 오브젝트 벡터를 받아와 이벤트 매니저에 추가하고 다 활성화 시킨다. 

	virtual const bool	Is_EndLevel();
	// 매 업데이트 마다 m_bDead 여부를 조사하여 트루라면 벡터에서 제거한다 (메모리 해제는 아니다)
	// 만약 현재 레벨 벡터의 사이즈가 0이 된다면 스폰 레벨을 증가시키고 Spawn 함수를 호출한다.

	// 마지막 스폰 레벨에서 모든 스폰 객체가 사망했다면
	// 스폰트리거 자신을 삭제한다.

protected:
	UINT						m_iMaxLevel;	// 몇 차례에 걸쳐 몬스터를 스폰할 것인지
	UINT						m_iCurLevel;	// 현재 차례
	map<UINT, vector<CObj*>>	m_mapSpawn;		// 키 - 차례, 밸류 - 해당 차례에 스폰할 오브젝트 벡터
};

