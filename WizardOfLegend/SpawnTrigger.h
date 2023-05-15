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
	// �ʵ��� �ʿ� �����Ͽ� ���� ���� ������ �ش��ϴ� ������Ʈ ���͸� �޾ƿ� �̺�Ʈ �Ŵ����� �߰��ϰ� �� Ȱ��ȭ ��Ų��. 

	virtual const bool	Is_EndLevel();
	// �� ������Ʈ ���� m_bDead ���θ� �����Ͽ� Ʈ���� ���Ϳ��� �����Ѵ� (�޸� ������ �ƴϴ�)
	// ���� ���� ���� ������ ����� 0�� �ȴٸ� ���� ������ ������Ű�� Spawn �Լ��� ȣ���Ѵ�.

	// ������ ���� �������� ��� ���� ��ü�� ����ߴٸ�
	// ����Ʈ���� �ڽ��� �����Ѵ�.

protected:
	UINT						m_iMaxLevel;	// �� ���ʿ� ���� ���͸� ������ ������
	UINT						m_iCurLevel;	// ���� ����
	map<UINT, vector<CObj*>>	m_mapSpawn;		// Ű - ����, ��� - �ش� ���ʿ� ������ ������Ʈ ����
};

