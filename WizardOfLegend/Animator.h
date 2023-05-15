#pragma once
#include "Component.h"

class CObj;
class CAnimation;

class CAnimator : public CComponent
{
public:
	CAnimator(CObj* _pOwner);
	virtual ~CAnimator();

public:
	virtual void				Init() override;
	virtual void				LateUpdate() override;
	virtual void				Render(HDC _hDC) override;
	void						Release();

public:
	const bool					PushBack_Ani(CAnimation* _pAni);
	CAnimation*					Find_Ani(const wstring& _strName);
	virtual void				Play(const wstring& _strName, const bool& _bRepeat);

public:
	const bool					Is_Use_Dir() const { return m_bUseDir; }
	void						Set_Use_Dir(const bool _bUse) { m_bUseDir = _bUse; }
protected:
	map<wstring, CAnimation*>	m_mapAni;	// 모든 애니메니션
	CAnimation*					m_pCurAni;	// 현재 재생중인 애니메이션
	CAnimation*					m_pReservedAni; // 플레이 시도했지만 아직 이전 애니메이션이 끝나지 않아 짤린 애니메이션
	bool						m_bReservedRepeat;
	bool						m_bRepeat;  // 반복 재생 여부
	bool						m_bUseDir; // 애니메이션에서 Dir 구분을 하지 않는다면 체크
};

