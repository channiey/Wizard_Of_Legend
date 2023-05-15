#pragma once
class CMathLib
{
public:
	CMathLib();
	virtual ~CMathLib();

	static const float Get_Angle(const VEC2& _vOrigin, const VEC2& _vTarget)
	{
		float	fWidth = 0.f, fHeight = 0.f, fDiagonal = 0.f;
		float	fRadian = 0.f;

		fWidth = _vTarget.fX - _vOrigin.fX;
		fHeight = _vTarget.fY - _vOrigin.fY;

		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);
		fRadian = acosf(fWidth / fDiagonal);

		if (_vTarget.fY > _vOrigin.fY)
			fRadian = (2 * PI) - fRadian;

		return fRadian;
	}

	static const float&	Get_Dist(const VEC2& _vOrigin, const VEC2& _vTarget)
	{
		VEC2 vDiff = _vOrigin - _vTarget;
		return vDiff.Length();
	}

	// LT, RB를 받은 다음 해당 영역 내에서 나올 수 있는 랜덤 포지션을 반환한다.
	static const VEC2 Get_Random_Pos(const VEC2& _vLT, const VEC2& _vRB)
	{
		float fExtra = 50.f;
		VEC2 vScale { _vRB.fX - _vLT.fX - fExtra, _vRB.fY - _vLT.fY - fExtra };

		if (vScale.fX < 0 || vScale.fY < 0) return VEC2_ZERO;

		VEC2 vPos{ _vLT.fX + vScale.fX * 0.5f, _vLT.fY + vScale.fY * 0.5f };

		int iX = int(rand() % int(vScale.fX * 0.5f));
		int iY = int(rand() % int(vScale.fY * 0.5f));

		int iRan1 = rand() % 2;
		int iRan2 = rand() % 2;

		iRan1 = iRan1 == 0 ? iRan1 = -1 : iRan1 = iRan1;
		iRan2 = iRan2 == 0 ? iRan2 = -1 : iRan2 = iRan2;

		iX *= iRan1;
		iY *= iRan2;

		vPos.fX += iX;
		vPos.fY += iY;

		return vPos;
	}
};

