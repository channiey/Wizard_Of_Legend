#pragma once
#include "Enemy.h"
class CIceBoss :
	public CEnemy
{
public:
	CIceBoss(const OBJ_TYPE& _eType);
	virtual ~CIceBoss();

public:
	virtual const bool			Init() override;
	virtual void				Update() override;
	virtual void				LateUpdate() override;
	virtual void				Render(HDC _hDC) override;
	virtual void				Release();

public:
	virtual void				OnCollision_Enter(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
	virtual void				OnCollision_Stay(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);
	virtual void				OnCollision_Exit(CCollider* _pOtherCol, const float& _fCX, const float& _fCY);

public:
	virtual const bool			Init_Animator() override;
	virtual void				Set_Active(const bool _bActive) override;
	void						Set_UI_Active(const bool _bActive);
	void						Set_Is_Spawn(const bool _bSpawn) { m_bSpawn = _bSpawn; }
	void						Set_Is_Slow(const bool _bSlow) { m_bSlow = _bSlow; }
	const bool					Is_Slow() const { return m_bSlow; }

	bool						m_bFirstAttacked;
	bool						m_bSecondAttacked;
	float						m_fAcc;
	float						m_fLimit;

	CObj*						m_pStatUI;

	bool						m_bSpawn;
	float						m_fSpawnReadyTime;
	bool						m_bUpdate;

	bool						m_bSlow;


	bool						m_bDeadCycle;
	float						m_bDeadCycleTime;
	bool						m_bStartDeadAni;
	bool						m_bBoom;
};

