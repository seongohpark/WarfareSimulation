#pragma once

#include <vector>
#include <map>

struct Record
{
	std::string		m_strComments;
	char			m_cCommented;
	std::string		m_strSection;
	std::string		m_strKey;
	std::string		m_strValue;
};

class CCommonUtility
{
public:
	explicit CCommonUtility();
	virtual ~CCommonUtility();

	static std::string GetValue(std::string strFileName, std::string strSectionName, std::string strKeyName);
};