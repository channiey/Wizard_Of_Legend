#pragma once
class CRes
{
public:
	CRes();
	virtual ~CRes();

public:
	const wstring&	Get_Key() const						{ return m_strKey; }
	//const wstring&	Get_Path() const					{ return m_strPath; }
	
	void			Set_Key(const wstring& _strKey)		{ m_strKey = _strKey; }
	//void			Set_Path(const wstring& _strPath)	{ m_strPath = _strPath; }

protected:
	wstring m_strKey;
	//wstring m_strPath;
};

