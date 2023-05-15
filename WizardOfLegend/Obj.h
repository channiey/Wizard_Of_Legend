#pragma once

class CComponent;
class CTransform;
class CCollider; 
class CAnimator;
class CRigidbody;

class CObj
{
public:
	explicit CObj(const OBJ_TYPE& _eType);
	CObj(const CObj& _rhs);
	virtual ~CObj();

public:
	virtual const bool			Init() PURE;
	virtual void				Update() PURE;
	virtual void				LateUpdate() PURE;
	virtual void				Render(HDC _hDC) PURE;
	virtual void				Release();

public:
	virtual void				OnCollision_Enter(CCollider* _pOtherCol, const float& _fCX, const float& _fCY) {}
	virtual void				OnCollision_Stay(CCollider* _pOtherCol, const float& _fCX, const float& _fCY) {}
	virtual void				OnCollision_Exit(CCollider* _pOtherCol, const float& _fCX, const float& _fCY) {}

public:
	const bool					Create_Transform();					
	const bool					Create_Collider();					
	const bool					Create_Animator();			
	const bool					Create_Rigidbody();

	virtual const bool			Init_Animator() { return true; }

	CTransform*					Get_Transform() const				{ return m_pTransform; }
	CCollider* 					Get_Collider() const				{ return m_pCollider; }
	CAnimator* 					Get_Animator() const				{ return m_pAnimator; }
	CRigidbody*					Get_Rigidbody()	const				{ return m_pRigidbody; }
	void						Component_LateUpdate();
	void						Component_Render(HDC _hDC);

public:
	const OBJ_TYPE&				Get_ObjType() const					{ return m_eObjType; }
	const float&				Get_Speed() const					{ return m_fSpeed; }
	const float&				Get_MaxHP() const					{ return m_fMaxHp; }
	const float&				Get_CurHP() const					{ return m_fCurHP; }
	const float&				Get_AD() const						{ return m_fAD; }
	const float&				Get_RenderMag() const				{ return m_fRenderMag ;}
	const RENDER_ORDER&			Get_RenderOrder() const				{ return m_eRenderOrder; }
	const int&					Get_Gold() const					{ return m_iGold; }

	void						Set_ObjType(const OBJ_TYPE& _eType) { m_eObjType = _eType; }
	void						Set_Dead()							{ m_bDead = true; }
	void						Set_Speed(const float& _fSpeed)		{ m_fSpeed = _fSpeed; }
	void						Set_MaxHp(const float& _fMaxHP)		{ m_fMaxHp = _fMaxHP; }
	void						Set_CurHP(const float& _fCurHP)		{ m_fCurHP = _fCurHP; }
	void						Set_AD(const float& _fAD)			{ m_fAD = _fAD; }
	void						Set_Invincible(const bool& _bInvin) { m_bInvincible = _bInvin; }
	void						Set_Lock(const bool& _bLock)		{ m_bLock = _bLock; }
	virtual void				Set_Active(const bool _bActive)		{ m_bActive = _bActive; }
	void						Set_Gold(const int& _iGold)			{ m_iGold = _iGold; }
	void						Set_RenderMag(const float& _fMag) { m_fRenderMag = _fMag; }

	void						Add_Speed(const float& _fSpeed)		{ m_fSpeed += _fSpeed; }
	void						Add_MaxHp(const float& _fMaxHP)		{ m_fMaxHp += _fMaxHP; }
	void						Add_CurHP(const float& _fCurHP)		{ m_fCurHP += _fCurHP; }
	void						Add_AD(const float& _fAD)			{ m_fAD += _fAD; }
	void						Add_Gold(const int& _iGold)			{ m_iGold += _iGold; }

	const bool					Is_Dead() const						{ return m_bDead; }
	const bool					Is_Lock() const						{ return m_bLock; }
	const bool					Is_Invincible() const				{ return m_bInvincible; }
	const bool					Is_Active() const					{ return m_bActive; }
protected:
	CTransform*					m_pTransform;
	CCollider*					m_pCollider;
	CAnimator*					m_pAnimator;
	CRigidbody*					m_pRigidbody;

	OBJ_TYPE					m_eObjType;
	RENDER_ORDER				m_eRenderOrder;

	float						m_fMaxTime;
	float						m_fCurTume;
	list<CComponent*>			m_listComponent;

	float						m_fSpeed;
	float						m_fDashSpeed;
	float						m_fCurHP;
	float						m_fMaxHp;
	float						m_fAD;
	float						m_fKonckBackTime;
	int							m_iGold;

	bool						m_bDead;
	bool						m_bInvincible; // 무적
	bool						m_bLock;       // 움직임 제한

	float						m_fRenderMag; // 렌더링 배율
	bool						m_bActive;

};

