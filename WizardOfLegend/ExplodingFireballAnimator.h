#pragma once
#include "Animator.h"
class CExplodingFireballAnimator :
	public CAnimator
{
public:
	CExplodingFireballAnimator(CObj* _pOwner);
	virtual ~CExplodingFireballAnimator();

public:
	virtual void	LateUpdate() override;
	virtual void	Play(const wstring& _strName, const bool& _bRepeat) override;
};

