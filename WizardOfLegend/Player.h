#pragma once
#include "Obj.h"

class CSkill;

class CPlayer : public CObj
{
public:
	explicit CPlayer(const OBJ_TYPE& _eType);
	virtual ~CPlayer();

public:
	virtual const bool			Init() override;
	virtual void				Update() override;
	virtual void				LateUpdate() override;
	virtual void				Render(HDC _hDC) override;
	virtual void				Release();

public:
	virtual const bool			Init_Animator() override;

public:
	virtual void				OnCollision_Enter(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void				OnCollision_Stay(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;
	virtual void				OnCollision_Exit(CCollider * _pOtherCol, const float& _fCX, const float& _fCY) override;

public:
	CSkill*						Get_Skill(const SKILL_KEY_TYPE& _eType) { return m_SkillList[(UINT)_eType]; }

private:
	void						Move();
	void						Skill();
	void						Set_LookFromMouse();
	void						Skill_Update();

public:
	void						Set_PortalAniBool(const bool _b) { m_bPortalAni = _b; }
	

private:
	VEC2						m_vMouse; // 마우스 - 플레이어 벡터
	bool						m_bAttackType; // 기본 공격 번갈아가면서 위 아래
	CSkill*						m_SkillList[(UINT)SKILL_KEY_TYPE::TYPEEND];
	bool						m_bPortalAni; // 포탈 애니메이션 진행 여부

	float m_fStepTime;
};

