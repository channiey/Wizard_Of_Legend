#pragma once
#include "Enemy.h"

class CCyclopsAttack_P;

class CGhoulLarge :
	public CEnemy
{
public:
	CGhoulLarge(const OBJ_TYPE& _eType);
	virtual ~CGhoulLarge();
public:
	virtual const bool			Init() override;
	virtual void				Update() override;
	virtual void				LateUpdate() override;
	virtual void				Render(HDC _hDC) override;
	virtual void				Release() override;

public:
	virtual void				OnCollision_Enter(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
	virtual void				OnCollision_Stay(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
	virtual void				OnCollision_Exit(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);

public:
	virtual const bool			Init_Animator() override;

private:
	const float					Calculate_Distance();
	void						Set_State();
	const bool					Set_Target();
	void						Chase();
	void						Attack();
	void						KnockBack(const float& _fTheta);

public:
	const CLOSE_ENEMY_STATE&	Get_State() const { return m_eState; }

	void						Set_State(const CLOSE_ENEMY_STATE& _eState);
	void						Set_Proj_Active(const bool& _bActive);
	virtual void				Set_Active(const bool _bActive) override;

private:
	CLOSE_ENEMY_STATE			m_eState;
	CObj*						m_pTarget;
	float						m_fAttackRange; // ������ ������ Ÿ�ٰ��� �Ÿ�
	Graphics					m_Graphics;
	float						m_fProjTermX; // ������Ÿ���� ������ X�κ��� �󸶳� ����߸���
	CCyclopsAttack_P*			m_pProj; // ���� ����ü

	float						m_fAttackStartTime; // ���� �ִϸ��̼��� ���� ����ü�� on�� �ð�
	float						m_fAttackEndTime;
	float						m_fAccTime;
	bool						m_bAttack; // ������ �ߴ���
};

