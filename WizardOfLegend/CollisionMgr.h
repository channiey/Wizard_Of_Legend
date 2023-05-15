#pragma once

class CCollider;


union COLLIDER_ID
{
	struct 
	{
		UINT iLeft_id;
		UINT iRight_id;
	};
	ULONGLONG ID;
};

class CCollisionMgr
{
	SINGLETON(CCollisionMgr)

private:
	CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	const bool				Init();
	void					Update();

public:
	void					Check_Group(const OBJ_TYPE& _eLeft, const OBJ_TYPE& _eRight);
	void					Reset() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)OBJ_TYPE::TYPEEND); }

private:
	void					Collision_Update(const OBJ_TYPE& _eLeft, const OBJ_TYPE& _eRight); // �浹 �˻縦 ���� ��ȸ
	const bool				Is_Collision(CCollider* _pLeftCol, CCollider* _pRightCol, float* _fCX, float* _fCY);
	const bool				Is_Close(CCollider* _pLeftCol, CCollider* _pRightCol); // �� ������Ʈ�� �Ÿ��� ������ ���� ��� �浹 �˻縦 �������� �ʴ´�.

private:
	UINT					m_arrCheck[(UINT)OBJ_TYPE::TYPEEND];		 // �׷찣�� �浹 üũ�� �����ϴ� ���ﰢ�� üũ�ڽ�
	map<ULONGLONG, bool>	m_mapColInfo;								 // �浹ü���� ���� ������ �浹 ����
	float					m_fColDistance;								 // �浹 �˻縦 ������ �� �ִ� �� ������Ʈ�� �ּ� �Ÿ�
};

