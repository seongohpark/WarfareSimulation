// ********************************************************************
// * 클래스명: CommonUtil
// * 설    명: 데이터 전달처리 공통 모듈을 정의한다.
// * 작 성 자: PSO
// * 작성일자: 2023. 1. 23
// *********************************************************************
// * 수정이력: 1. ~
// ********************************************************************

#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>

#include "CommonUtil.h"

using std::string;
using std::vector;
using std::map;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;
using std::stringstream;
using std::to_string;
using std::stoi;


CCommonUtility::CCommonUtility()
{	
}

CCommonUtility::~CCommonUtility()
{	
}

//------------------------------------------------------------------------
//
//
//
//
//------------------------------------------------------------------------
string CCommonUtility::GetValue(string strFileName, string strSectionName, string strKeyName)
{
	std::ifstream IniFile(strFileName);
	if (!IniFile.is_open())
    {
        std::cerr << "[ERROR] Failed to open INI file: " << strFileName << std::endl;
        return "";
    }
	
	const auto Trim = [](std::string& Value)
    {
        Value.erase(Value.begin(), std::find_if(Value.begin(), Value.end(), [](unsigned char Ch)
                {
                    return !std::isspace(Ch);
                }));

        Value.erase(std::find_if(Value.rbegin(), Value.rend(), [](unsigned char Ch)
                {
                    return !std::isspace(Ch);
                }).base(), Value.end());
    };
	
	std::string CurrentSection;
    std::string Line;
	
	while (std::getline(IniFile, Line))
    {
        // Windows 형식(CRLF)의 '\r' 제거
        if (!Line.empty() && Line.back() == '\r')
        {
            Line.pop_back();
        }
		
		// UTF-8 BOM 제거
        if (Line.size() >= 3 
		&& static_cast<unsigned char>(Line[0]) == 0xEF 
		&& static_cast<unsigned char>(Line[1]) == 0xBB 
		&& static_cast<unsigned char>(Line[2]) == 0xBF)
        {
            Line.erase(0, 3);
        }
		
		Trim(Line);

        // 빈 줄 건너뛰기
        if (Line.empty())
        {
            continue;
        }
		
		// 주석 건너뛰기
        if (Line[0] == ';' || Line[0] == '#')
        {
            continue;
        }
		
		// 섹션 처리: [OracleSettings]
        if (Line.front() == '[' && Line.back() == ']')
        {
            CurrentSection = Line.substr(1, Line.size() - 2);
            Trim(CurrentSection);
            continue;
        }
		
		// 찾으려는 섹션이 아니면 건너뛰기
        if (CurrentSection != strSectionName)
        {
            continue;
        }
		
		// 첫 번째 '=' 위치 검색
        const std::size_t EqualPosition = Line.find('=');
        if (EqualPosition == std::string::npos)
        {
            continue;
        }
		
		std::string Key = Line.substr(0, EqualPosition);
        std::string Value = Line.substr(EqualPosition + 1);

        Trim(Key);
        Trim(Value);
		
		if (Key == strKeyName)
        {
            return Value;
        }
	}	
	
	std::cerr << "[ERROR] INI value not found. Section=[" << strSectionName << "], Key=" << strKeyName << std::endl;

    return "";
}