#pragma once
#include "Animator.h"
class CWaterBombAnimatior :
	public CAnimator
{
public:
	CWaterBombAnimatior(CObj* _pOwner);
	virtual ~CWaterBombAnimatior();

public:
	virtual void	LateUpdate() override;
	virtual void	Play(const wstring& _strName, const bool& _bRepeat) override;
};

