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
	void					Collision_Update(const OBJ_TYPE& _eLeft, const OBJ_TYPE& _eRight); // 충돌 검사를 위한 순회
	const bool				Is_Collision(CCollider* _pLeftCol, CCollider* _pRightCol, float* _fCX, float* _fCY);
	const bool				Is_Close(CCollider* _pLeftCol, CCollider* _pRightCol); // 두 오브젝트간 거리가 기준을 넘을 경우 충돌 검사를 진행하지 않는다.

private:
	UINT					m_arrCheck[(UINT)OBJ_TYPE::TYPEEND];		 // 그룹간의 충돌 체크를 지정하는 역삼각형 체크박스
	map<ULONGLONG, bool>	m_mapColInfo;								 // 충돌체간의 이전 프레임 충돌 정보
	float					m_fColDistance;								 // 충돌 검사를 진행할 수 있는 두 오브젝트간 최소 거리
};

