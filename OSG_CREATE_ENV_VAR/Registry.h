////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  M.Jim Stephens 2012
//
////////////////////////////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "windows.h"
#include <string>
	
#define OSG_DES_KEY TEXT("SOFTWARE\\OSG_DES")

class CRegistry
{
public:
	enum cregRestoreEnum
	{
		regVolatile = REG_WHOLE_HIVE_VOLATILE,
		regRefresh = REG_REFRESH_HIVE,
		regNoLazy = REG_NO_LAZY_FLUSH
	};

	enum Keys
	{
		classesRoot,
		currentUser ,
		localMachine ,
		currentConfig,
		users ,
		performanceData ,	//Windows NT/2000
		dynData //Windows 95/98
	};


	CRegistry()
		{
			m_hKey = NULL;
		};

	~CRegistry()
		{
			CloseKey();
			m_hKey = NULL;
		};

	HKEY get_key_from_enum(enum Keys hKey);

	BOOL OpenKey(enum Keys hKey, LPCWSTR szKey);
	BOOL CreateKey(enum Keys hKey, LPCWSTR szKey);
	BOOL DeleteKey(enum Keys hKey, LPCWSTR szKey);
	BOOL DeleteValue(LPCWSTR lpValueName);
	BOOL GetValue(LPCWSTR lpValueName, std::string* p_strValue);
	BOOL GetValue(LPCWSTR lpValueName, DWORD* dwValue);	
	BOOL GetValue(LPCWSTR lpValueName, bool* Value);
	BOOL GetValue(LPCWSTR lpValueName, UINT* Value);
	BOOL SetValue(LPCWSTR lpValueName, LPCWSTR lpData);
	BOOL SetValue(LPCWSTR lpValueName, DWORD dwValue);
	BOOL SetValue(LPCWSTR lpValueName, bool Value);
	BOOL SetValue(LPCWSTR lpValueName, UINT Value);
	BOOL SaveKey(LPCWSTR lpszFileName);
	BOOL RestoreKey(LPCWSTR lpszFileName, DWORD dwFlag);
	BOOL LoadKey(enum Keys hKey, LPCWSTR lpszSubKey, LPCWSTR lpszFileName);
	void CloseKey();	

protected:
	HKEY m_hKey;
};