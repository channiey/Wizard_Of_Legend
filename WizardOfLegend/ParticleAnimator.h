#pragma once
#include "Animator.h"
class CParticleAnimator :
	public CAnimator
{
public:
	CParticleAnimator(CObj* _pOwner);
	virtual ~CParticleAnimator();

public:
	virtual void	LateUpdate() override;
	virtual void	Play(const wstring& _strName, const bool& _bRepeat) override;

};

