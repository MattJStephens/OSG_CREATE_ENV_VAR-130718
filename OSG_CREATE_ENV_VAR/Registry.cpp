////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) � M�tth�w J�m�s �t�ph��s �  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "registry.h"

HKEY CRegistry::get_key_from_enum(enum Keys hKey)
{
	if(hKey== classesRoot) return HKEY_CLASSES_ROOT;
	if(hKey== currentUser) return HKEY_CURRENT_USER;
	if(hKey== localMachine) return HKEY_LOCAL_MACHINE;
	if(hKey== currentConfig) return HKEY_CURRENT_CONFIG;
	if(hKey== users) return HKEY_USERS;
	if(hKey== performanceData) return HKEY_PERFORMANCE_DATA;
	if(hKey== dynData) return HKEY_DYN_DATA;
	return 0;
}

BOOL CRegistry::OpenKey(enum Keys hKey, LPCWSTR szKey)
{	
	if(RegOpenKeyEx(get_key_from_enum(hKey),(LPCWSTR)szKey, 0, KEY_ALL_ACCESS, &m_hKey) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		m_hKey = NULL;
		return FALSE;
	}
}

BOOL CRegistry::CreateKey(enum Keys hKey, LPCWSTR szKey)
{	

	//KEY_ALL_ACCESS 
	if(RegCreateKeyEx(get_key_from_enum(hKey),(LPCWSTR)szKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_hKey, NULL) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		m_hKey = NULL;
		return FALSE;
	}
}

BOOL CRegistry::SetValue(LPCWSTR lpValueName, LPCWSTR lpData)
{
	if(m_hKey != NULL)
	{
		DWORD dwType = REG_SZ;

		if(::RegSetValueEx(m_hKey, (LPCWSTR)lpValueName, 0, dwType, (LPBYTE)(LPCWSTR)lpData, wcslen(lpData)) == ERROR_SUCCESS)
		{
			::RegFlushKey(m_hKey);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRegistry::SetValue(LPCWSTR lpValueName, DWORD dwValue)
{
	if(m_hKey != NULL)
	{	
		DWORD dwType = REG_DWORD;		
		if(::RegSetValueEx(m_hKey, (LPCWSTR)lpValueName, 0, dwType, (LPBYTE)&dwValue, sizeof(DWORD)) == ERROR_SUCCESS)
		{
			::RegFlushKey(m_hKey);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL CRegistry::SetValue(LPCWSTR lpValueName, bool Value)
{
	if(m_hKey != NULL)
	{	
		DWORD dwType = REG_DWORD;		
		DWORD dwValue= 0;
		if(Value)dwValue= 1;
		if(::RegSetValueEx(m_hKey, (LPCWSTR)lpValueName, 0, dwType, (LPBYTE)&dwValue, sizeof(DWORD)) == ERROR_SUCCESS)
		{
			::RegFlushKey(m_hKey);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRegistry::SetValue(LPCWSTR lpValueName, UINT Value)
{
	if(m_hKey != NULL)
	{	
		DWORD dwType = REG_DWORD;		
		DWORD dwValue= (DWORD)Value;
		if(::RegSetValueEx(m_hKey, (LPCWSTR)lpValueName, 0, dwType, (LPBYTE)&dwValue, sizeof(DWORD)) == ERROR_SUCCESS)
		{
			::RegFlushKey(m_hKey);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRegistry::GetValue(LPCWSTR lpValueName, std::string* p_strValue)
{
	BOOL bRet = FALSE;
	LPSTR lpstrValue;
	DWORD dwType = REG_SZ;
	DWORD lpcbData;	
	bRet = FALSE;	
	lpstrValue = (LPSTR)malloc(sizeof(char) * 256);
	memset(lpstrValue, 0, sizeof(char) * 256);
	lpcbData = 1024;		
	if(::RegQueryValueEx(m_hKey, lpValueName, NULL,	&dwType, (BYTE*)(LPCWSTR)lpstrValue, &lpcbData) == ERROR_SUCCESS)
	{
		bRet = TRUE;
		*p_strValue = lpstrValue;
	}
	else
	{
		*p_strValue= "";
	}

	return bRet;
}		

BOOL CRegistry::GetValue(LPCWSTR lpValueName, DWORD* dwValue)
{
	BOOL bRet = FALSE;
	DWORD dwType = REG_DWORD;
	DWORD lpcbData = sizeof(DWORD);			
	if(RegQueryValueEx(m_hKey, lpValueName,	NULL, &dwType, (BYTE*)(DWORD)dwValue, &lpcbData) == ERROR_SUCCESS)
	{
		bRet = TRUE;
	}

	return bRet;
}

BOOL CRegistry::GetValue(LPCWSTR lpValueName, bool* Value)
{
	BOOL bRet = FALSE;
	DWORD dwType = REG_DWORD;
	DWORD lpcbData = sizeof(DWORD);	
	DWORD dwValue;	
	if(RegQueryValueEx(m_hKey, lpValueName,	NULL, &dwType, (BYTE*)(DWORD)&dwValue, &lpcbData) == ERROR_SUCCESS)
	{
		if(dwValue== 1)*Value= true;
		else *Value= false;
		bRet = TRUE;
	}

	return bRet;
}

BOOL CRegistry::GetValue(LPCWSTR lpValueName, UINT* Value)
{
	BOOL bRet = FALSE;
	DWORD dwType = REG_DWORD;
	DWORD lpcbData = sizeof(DWORD);	
	DWORD dwValue;	
	if(RegQueryValueEx(m_hKey, lpValueName,	NULL, &dwType, (BYTE*)(DWORD)&dwValue, &lpcbData) == ERROR_SUCCESS)
	{
		*Value= (UINT)(dwValue);
		bRet = TRUE;
	}

	return bRet;
}


BOOL CRegistry::DeleteKey(enum Keys hKey, LPCWSTR szKey)
{	
	return ::RegDeleteKey(get_key_from_enum(hKey),(LPCWSTR)szKey) == ERROR_SUCCESS;
}

BOOL CRegistry::DeleteValue(LPCWSTR lpValueName)
{	
	if(::RegDeleteValue(m_hKey,(LPCWSTR)lpValueName) == ERROR_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

void CRegistry::CloseKey()
{
	::RegCloseKey(m_hKey);
	m_hKey = NULL;
}

BOOL CRegistry::SaveKey(LPCWSTR lpszFileName)
{
	if(m_hKey != NULL)
		return ::RegSaveKey(m_hKey, lpszFileName, NULL) == ERROR_SUCCESS;
		return FALSE;
}

BOOL CRegistry::RestoreKey(LPCWSTR lpszFileName, DWORD dwFlags)
{
	if(m_hKey != NULL)
		return ::RegRestoreKey(m_hKey, lpszFileName, dwFlags) == ERROR_SUCCESS;
		return FALSE;
}

BOOL CRegistry::LoadKey(enum Keys hKey, LPCWSTR lpszSubKey, LPCWSTR lpszFileName)
{
	return ::RegLoadKey(get_key_from_enum(hKey), lpszSubKey, lpszFileName) == ERROR_SUCCESS;
}

/*
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM88NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMDIII7NMMMMMMMMMMMM8$Z$NMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMD??7I7I?MMMMMMMMMMZ7N?++++8I$MMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN=?7I?III?MMMMMMMMMM$N+?8NNNNZI?$$MMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM$?I7I77III?MMMMMMMMMMZ8?7NMM$7I?I7?+7MMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM??I?I77I???+MMMMMMMMMMM$++NMZII?++++77+7MMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM+??I?II?+++++MMMMMMMMMMMM$O+M7I?++++=+?Z+$$MMMMMMMM
MMMMMMMMNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNO?I??I???+==+++MMMMMMMMMMMMMM$MZ7I??++++?7D7I$MMMMMMM
MMMMMMMMMN8ZZDMNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN8Z+II77II???+=+?7MMMMMMMMMMMMMMMOM$?7I??+?IMMN??$MMMMMM
MMMMMMMMMD$$$$7?ONMMMMMMMMMMMMMMMMMMMMMMMMMMMMNMMMMMMMMMDDZ$7I7Z8??7777II??+=~~++MMMMMMMMMMMMMMMMMMMZ$$7II$MMND?+I8MMMMM
MMMMMMMMMM$Z$$ZO$I?ONMMMMMMMMMMMMMMMMMMMMMMMMMN888D8$$OOZZ$IIIZ8=?77I++I++=+~=++=MMMMMMMMMMMMMMMMMMMMM$I+??II??+778MMMMM
MMMMMMMMMMN7$O88Z$77?IZOONDO8NMMN8DOO8888OOZO8ZOZO8ZI$Z7?77?IZO=?II7Z77I?I++++===MMMMMMMMMMMMMMMMMMMMMMMMM$$7I77$DMMMMMM
MMMMMMMMMMMN$$77$IZ$$$7?I8ZO88OZ$7II$$IO8Z$ZOI7O7$ZZ77$II??I$=~?I$77I+II+++=~~=+MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMNI7II?77$777$II$OO8OZZ7?I77$ZZ7$Z777I?7II7777I$$$==?I7II?+?=====~=I78MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMDI7I??II?IZ7I77??OOZZ$II7??$7+?7?$777III7$II7III??II777?II??~====~+ZMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMNIII?I7I$7IIZI77I++IZZZI777$I7$7=I7$7?+I7???7$7I7III?+????$8?7?+=+IMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMDII777I?II7I77$Z7?+?$$77$IZ=I$$I7?IZI+7Z?+77I777II?I?7?IO?7$I+==+8MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMN?7IIII$7$$I7Z7777I$$$77$$?~$$?+II$7=?ZZ$??$77$7I?+++==+==?=+=++NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMM?I++++II?I$Z87$$777777ZO+77+7I+7I=?7OO$$$?7III??II??=+?+~=???$ZMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN
MMMMMMMMMMMMMMMMMMN$7???I?I?787$$$7+$77$OO?++I$I7?77??$$III?+?++???++?+?+??=?I?7?DMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMNII$I+?87I7$?II777IIIZ$Z8+??7I?7I+I=$$?++?+?=?+=+=++?++++=I???IINMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNMM
MMMMMMMMMMMMMMMMMMMO?IIIO$I?+???7I?7+++7?$7==II+I+77+I7Z$I7+++=+~++=+++==~+?==++?INMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNMMM
MMMMMMMMMMMMMMMMMMMM?+III?$??I+???I=+?+?7?OI+I7$+77=+$$$7I?=====+==++++=++??I=+=+7DNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMNI??7=III+I7+++=++II=I?I$:?7$=$+?$7==I+=~===+=+=+~====?+==++IIZDDNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMNZ$?I$+I?I+=7I=+++++I?~=$I?$=$7?7$$=~+$+?+=++++=====?+=+~=???I7ZDDDNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMNI7II7++7I+++++?+??+?++77ZI?7Z7+I$?=~I$7+=~++?+++++=++++=?+?II$788DDDDMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMM$II+++?++?I++?+????7I++I$$+II$?I7?=:=$$?=~+??~+~:~?=?=?+??Z$7Z$8OZO8DDNMMMMMMMMMMMMMMMMMMMMMMMMMMMD
MMMMMMMMMMMMMMMMMMMMMOI?=???I??II+7I++?IZ?=++$$?$+??7II=~+$$I~~?I====++++????+?I7ZZOD8ZOO888NNMMMMMMMMMMMMMMMMMMMMMMMMD8
MMMMMMMMMMMMMMMMMMMMMO7IIII+I??++?+?I?=IZ?=~?+III?++?II=++7Z?=~+==+=~~??IZ77I7ZO$$7ZO88O88888DNMMMMMNNMMMMMMMMMMMMMMM8OD
MMMMMMMMMMMMMMMMMMMMMO$7I?++??=++==??+=?$$==++?==I+=????=+I=~~=?=?I?=~:?+?I$I$78OOO8O8DOOOOOZO8DMMMMMMMMMMMMMMMMMMMN8ZO8
MMMMMMMMMMMMMMMMMMMMNZOO$I++7I+=++???==?$ZIII+I?++++=+I++~++?INNM8+=~=?++~I?I+7I7ZODOO8O8OO8OOZ$Z8NMMMMMMMMMMMMMMMM8$$O8
MMMMMMMMMMMMMMMMMMMMN8OZ7I$$ZZ7?+$~II+~~=+++?I7=?+++=+?+=:~$NO7Z=+::~II?++?7++?I?IOOZ8OOOOOOOOOZZ$O8NMMMMMMMMNMMMMDZ$ZOD
MMMMMMMMMMMMMMMMMMMMM8O$8O$Z$Z?++~I8DMMN8$7~=+I7+=++?+?+~~+?I?=~~=+II??=I+??+?I?IIZDOOOZZZOO8OO8Z$Z$$ODNMMMMNDNMMNZ7$$8N
MMMMMMMMMMMMMMMMMMMMMD8D8$$ZZI??=,:++OZZ$O8$~=77+?+?++?===$+=::,++?I++?I=I$?7?I??IZ8ZZOI77ZOO8OOO$ZZZZZDNNNN8ZDMM87I78NM
MMMMMMMMMMMMMMMMMMMMMMNOOZ$II$7I7I=~~==????7I=???????++++?$I?:~??:II+~=?77I?=+7I7O8Z$$OZ$7$ZOOOOOOOZZZZZDNNDO7OMNZ77ZNMM
MMMMMMMMMMMNMMMMMMMMMMN8ZZ77$7I?II?+++=:::+IZ=I+?+?+==+II7Z7+~=~+I==I+?7$D$77IZI$8ZI$$ZZ$7$$OZO88OOZ$ZZZZ888ZIZMD77$DMMM
MMMMMMMMMMMMMMNNMMMMMMMD87ZIII$I?7I?I+?+~:I$O7I?+++?+++77I7I?===~I=~$7787?I7778O887$Z$ZOII$$ZOOOOZOZ$$ZOZOOO$?$MD7$DMMND
MMMMMMMMMMMMMMMMMMMMMMMMZ$OZIZZZ77??7I?=++?7$7I???+++??+I7I+++=~=~:=:~+II?++7I$ZO7=8OZZO7I77$7OOOOZOZ$ZZZZO8$7$MDZDMMNNM
MMMMMMMMMMMMMMMMMMMMMMMMM$$OZZZ7$I$$I?~?+++I=$$II?=+?I??$$I+?~~~~+:~=~+I++:$$7ZO$IO8ZZ$OO$77$$OZOOOZZZ$ZOOOOO$ZMNDMMNMMN
MMMMMMMMMMMMMMNMMMMMMMMMMMDOOO8ODOZZI??~+++I7$8IIII?I??I$7I?=+I~:?=++.,:~====++:,+8Z$$$OZ$$$ZZZOZOOOZO$ZOOO88O8MMMMMMNNM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMDDZI$OD7$++++?+?IZZ$II?=I???I7+:~~=+::=:=$7?77I$7Z$IZD87$Z$OZZ$Z$$ZO7$OOOOZZOOO8DDDMMMMMMNMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMDD8$7O7??+=?I:+?I$?++++~==+?~,~==:==O?==O$:=IOZIZDZ+7$$I7Z$77$7Z$ZZOOOOOZ$OOO88NNMMMMNMMMM
MMMMMMMMMMMMMNMMMMMMMMMMNMMMMMMO8N8ZO?+=?+=+=~=II+~=~=?8=~:~~:+~~~~7?+.+$O8I$O+DI+I$$7$ZI?I7$Z$$ZOOOOOZ7ZZ888NNMMMNMMMMM
MMMMMMMMMMMMMNMMMMMMMMMMNNNMMMMN$8DNZII=~~~~~?:=+D?+??+~~~+~=~::?~~$.D8Z7?ZDMN+Z$=7$$I$7I777$$Z$ZOOOOOZ$Z8888DMMMMMMMMMM
MMMMMMMMMMMMMNNMMMMMMMMMMMMN$MMMMNDD~+.~~I=+++?:~~=ZZ$I:+=~==I~?++IN7D~O+DNND=Z$$I$$?I+I7I$7I7$$$OOOZZO$O8OOO8NMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMN$DMMMMMMMMND~+NII=?+++++=~~~~?$~+I?+==?ZI8DDMN8DND8=ZIO7I??I$7II77$?77$$Z$OO8$$ZZOO8OO8DMMMMMMMMM
MMMMMMMMMMMMMMMMMMM8MDNNMMMMMDZ==MNO8NDN:7?I+=+I?=+=++7?+$$OZ$ODD7MMNMDN88OZI7?II777III7I7$7$$77$$O8OOZZ8OOO88DNNMMMMMMM
MMMMMMMMMMMMMMNMMMMMMMMMMMN8NNM~MM,NZN7MMMNNODDO$Z7?++++++?=?7DDMMONN88ZOZ$77II7I$7~I7777$7I7777$$OOOO8D8OOO88DDNMMMNNNN
MMMMMMMMMMMMMMMNMMMMMMMM$M87M=NM=NNMMMMNO=MM8MZO$7++~?++?7?88N8NNNNNDOO$7777I?+7$$7+$$~I77777$7$$O8OOOD88OOOO8DNNMMMNNNN
MMMMMMMMMMMMNMNMMMMMMMMNONM+8M:MNNN$NM+MZMMMMNMMMDDO$77ZZ$DDMNNNDN8OZ$7I7777I7$ZZIII77I$$$$$$7$ZO8OODND88OOOO8DMMMMNNNDD
MMMMMMMMMNMMMMMMMMMDMMZNNZNMIMM:MD=DNDNNDMMMMMMMMNMNNNNNNNNNNNND8ZZ7$7?IIII?7$ZZ7I7$7$$7I$$7$$ZOO88DNDD8O88OO8NMMMMNNDDN
MMMMMMMMMMMMMMMNMDMMIMNNMN8NMNNMM+88NDNMNMMMMMMMNMMMMMMMMMMMNN8Z$$II??IIIII7$$$7I7$77$$$$$$Z$ZO88DDND88888OOODNMMMNNNNNN
MMMMMMMMMMMMMMMZMMMMMMNMNMNN=MMZIM7DDNNNNMNMMMMONMMMNMMMMNNN8Z777II777?+I7IZ$$?I77$777I$Z$$$8O$Z8NND888OOZOO8DNNMMNNNNMM
MMMMMMMMMMMMMNMDMNMMMMMMDMN8DNNZMMMMDDDD8DDNNDDDDNNMMMMMMNN8Z$$777777I?I7$Z$7II$7777$7ZOZZODOZODND888OOOOOO88DNNMMNNNMMM
MMMMMMMMMMMNMMMNMMMOMDMNM=MMNM77MMMMNDD8OOZZOO8OZOO8DNDD8OZ$77II777$77$Z$7I?IIII7$$$7ZOZO8ZZZODN8OOOZZZZZO888NMMMMMMMMMM
MMMMMMMMMMMMMMMNNMNMDNMN8NNNMOMMMMMMNND8OOOZ$ZOOZZZ$ZZ77I77I$$7$$OZ$$Z$7I??IIII7$7$ZZZZ88OO$OND8OOOZZZZZOO88DMMMMMMMMMMM
MMMMMMMMMMMMMMMNMMMMMMZMMNNN7NNMMMMMMMMN8OZZ$$7Z$ZZZ$7777777$$$$Z$$7I??II7I?III77$ZZOOZZZZ8D8888OOZ$ZZZZZO88NMMMMMMMMMMM
MMMMMMMMMMMMMMMMMNMMMNMMMMMNMNNMMMMMMMMNNDZ$ZZ$$7IIIIIIIIIIIIIIIIII?I?I7???I?I77$$ZOZZZ$OD8O8ND8OOZZOOOOO88DNMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMD8O$$777II??+????I????II77II?????III7$ZZZ$$$ZODDDDDNNDDND88DNNNNNNDNNNMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMDMMMMMMMMMMMMMMMMDD8OOZ$77777I7II?III77IIII??I?I7II7ZZZZ$ZZZZ8D88ODDNDDDNN88DNNNNNNNDNMNMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN888O8Z$7I$$III7II7IIIII????I7II7$$ZZ$$7$Z$ODDDO8NNNDDDNNDDDNNNNNNNDNNMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMDZZOOOOOZ777$777IIIII??IIII7777$$$O8OZ$$$OZODD8ODNNND8DNNDDNNNNNNNNDNNMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMO77$$$ZZOZOZ$II$7II$$$$$Z$$7I$$$O8DDOOZZZOOODD88DNNNDDNNNDDNNNNMNMNNMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8$$7I77$ZZZOO88O8O8$$$$$$$7$$7$$ZO8888OZZO8ODDNDDNNNNDNMNNNMMNMMMMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNO$???$7II$$Z$7Z88$$Z$ZZZZ$$$$ZOZ$ZZOO88OOODODNDNNNMNNNNMNNDNMMNMNNMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNZII77$Z777777$ZZ$$$O$Z$$$$$$$$O$$Z$ZZOOOO88D8DNNNNNNNNDNNDDNMMNNMNMMMMMMMNM
MMMMMMMMMMMMMNMMMMNM8NNNMMMMMMMMMMMMMMMMMMNNNDDDD8O8OOZZZ$$Z88OZZO$$$Z7$$ZOZ$OOOO888O888DDDNNDNNNNNNNNNNNNNNNNNNMMMMMMMM
MMMMMMMMMMMNMNNMMNNNMMMMMNMMMMMMMMMMMMMMMNNNNNNNNNNNNDDDDDDDDDDDDDDDDD8DD8DDDDDDDDNNDDNNNNNNNNNNNMNNNMNNNNNNNNNMMMMMMMMM
MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNNNNNNNNNNNNNNNNNNNNNNNNDDDDDDDNNNNNNNNNNNNNNNNNNNMNNNNMMMMMMMMMMMMMMMMMMM

*/