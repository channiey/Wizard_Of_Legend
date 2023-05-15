#pragma once
class CDebug_Render
{
public:
	CDebug_Render();
	virtual ~CDebug_Render();

public:
	static void Render(HDC _hDC);
};

