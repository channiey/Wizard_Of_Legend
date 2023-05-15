#pragma once
#include "Projectile.h"
class CWaterBomb_P :
	public CProjectile
{
public:
	CWaterBomb_P(const OBJ_TYPE& _eType);
	virtual ~CWaterBomb_P();

public:
	virtual const bool	Init() override;
	virtual void		Update() override;
	virtual void		LateUpdate() override;
	virtual void		Render(HDC _hDC) override;
	void				Release();

public:
	virtual void		OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void		OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) {}
	virtual void		OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) {}

public:
	void				Reset(CObj* _pOwner);
	virtual void		Return() override;
	void				Set_MoveRenderMag(const float& _fMag) { m_fMoveRenderMag = _fMag; }
	void				Set_BombRenderMag(const float& _fMag) { m_fBombRenderMag = _fMag; }


private:
	virtual const bool	Init_Animator() override;
	void				Explode();
	virtual void		Move() {}

private:
	void				Render_Shadow(HDC _hDC); // 실제 포즈와 그림자 포즈 사이 거리에 비례하여 그림자의 크기를 결정하여 그린다.
	const bool			Check_Drop();			 // 물방울이 그라운드와 부딪혔는지 검사한다. (콜라이더 킴)

private:
	float				m_fMoveRenderMag;
	float				m_fBombRenderMag;

	VEC2				m_vDropPoint;	// 낙하지점 (그림자 포지션)
	VEC2				m_vShadowScale; // 그림자 크기 (실시간 변동)

	float				m_fDis;

	Graphics			m_Graphcis;
	bool				m_bBomb;
};

