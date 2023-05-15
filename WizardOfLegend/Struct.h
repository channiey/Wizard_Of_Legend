#pragma once

typedef struct myVector2
{
	float fX;
	float fY;

	myVector2() : fX(0.0f), fY(0.0f) {}
	myVector2(const float& _fX, const float& _fY)	: fX(_fX), fY(_fY) {}
	myVector2(const int& _iX, const int& _iY)		: fX(float(_iX)), fY(float(_iY)) {}
	myVector2(const float& _fX, const int& _iY)		: fX(_fX), fY(float(_iY)) {}
	myVector2(const int& _iX, const float& _fY)		: fX(float(_iX)), fY(_fY) {}

	myVector2				operator -	(const myVector2& _v) const		{ return myVector2{ fX - _v.fX, fY - _v.fY }; };
	myVector2				operator +	(const myVector2& _v) const		{ return myVector2{ fX + _v.fX, fY + _v.fY }; };
	myVector2				operator *	(const float& _f) const			{ return myVector2{ fX * _f, fY * _f }; };
	myVector2				operator /	(const float& _f) const			{ return myVector2{ fX / _f, fY / _f }; };
	const myVector2&		operator += (const myVector2& _v)			{ fX += _v.fX; fY += _v.fY; return *this; };
	const myVector2&		operator -= (const myVector2& _v)			{ fX -= _v.fX; fY -= _v.fY; return *this; };
	const myVector2&		operator -= (const float& _f)				{ fX -= _f; fY -= _f; return *this; };
	const myVector2&		operator *= (const float& _f)				{ fX *= _f;	fY *= _f; return *this; };
	const myVector2&		operator /= (const float& _f)				{ fX /= _f;	fY /= _f; return *this; };
	bool					operator != (const myVector2& _v)			{ return(fX != _v.fX || fY != _v.fY); }
	bool					operator == (const myVector2& _v)			{ return(fX == _v.fX && fY == _v.fY); }

	void					Zero()										{ fX = 0.f; fY = 0.f; }
	const myVector2			Zeroed()									{ return myVector2(0.f, 0.f); }
	const float				Length() const								{ return sqrt(pow(fX, 2) + pow(fY, 2)); }
	const myVector2			Normalized() const							{ return myVector2(fX / Length(), fY / Length()); }
	void					Normalize()									{ fX /= Length(); fY /= Length(); }
	void					Reverse()									{ fX *= -1.f; fY *= -1.f; }
	const myVector2			Reversed() const							{ return myVector2(fX * -1.f, fY * -1.f); }
	const myVector2			Reversed_X() const							{ return myVector2(fX * -1.f, fY); }
	const myVector2			Reversed_Y() const							{ return myVector2(fX, fY * -1.f); }

	const float				Dot(const myVector2& _v) const				{ return (fX *_v.fX) + (fY * _v.fY); }
	const float				Get_Distance(const myVector2& _v) const		{ return ((*this) - _v).Length(); } 
	const float				Get_Angle(const myVector2& _v) const		{ return acos(Dot(_v) / (Length() * _v.Length())); } 

	const myVector2			Lerp(const myVector2& _v1, const myVector2& _v2, float _fT) 
	{
		if (_fT < 0)
			_fT = 0.f;
		else if (1.f < _fT)
			_fT = 1.f;

		return VEC2{ _v1 + (_v2 - _v1) * _fT }; 
	}
}VEC2;

typedef struct MyEvent
{
	DWORD_PTR	lParam;
	wstring		wParam;
	EVENT_TYPE _eType;

}EVENT;


struct tagFinder
{
public:
	tagFinder(const TCHAR* pString) : m_pString(pString) {}

public:
	template<typename T>
	bool	operator()(T& MyPair)
	{
		return !lstrcmp(m_pString, MyPair.first);
	}

private:
	const TCHAR* m_pString;
};

struct DeleteMap
{
	template<typename T>
	void operator()(T& Temp)
	{
		if (Temp.second)
		{
			delete Temp.second;
			Temp.second = nullptr;
		}
	}
};

typedef struct MyLine
{
	VEC2 p1;
	VEC2 p2;
}LINE;