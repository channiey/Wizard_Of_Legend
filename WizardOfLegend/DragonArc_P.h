#pragma once
#include "Projectile.h"
class CTexture;

class CDragonArc_P :
	public CProjectile
{
public:
	CDragonArc_P(const OBJ_TYPE& _eType);
	virtual ~CDragonArc_P();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

public:
	virtual void		OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void		OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void		OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;

public:
	void				Reset(CObj* _pOwner, const VEC2& _vPos, const float& _fDamage, const float& _fTheta);
	virtual void		Return() override;

private:
	virtual const bool	Init_Animator() override;
	void				Explode();
	virtual void		Move() override;

private:
	float				m_fMinTheta;
	float				m_fMaxTheta;
	static bool			g_bUp;
	bool				m_bUp;
	float				m_fTheta;

	float				m_fMoveRenderMag;
	float				m_fBombRenderMag;

	CTexture*			m_pTex;
};

