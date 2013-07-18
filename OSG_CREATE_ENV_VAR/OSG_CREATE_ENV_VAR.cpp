//////////////////////////////////////////////////////////////////////
//	MJim - Alpha
//	Stephens Atomics 
//	Copyright (C) 2013  Matthew James Stephens, B.Eng- Robotics(Hons)
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//	<http://www.gnu.org/licenses/>   
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string>
#include "Registry.h"
#include <windows.h>
#include <stdio.h>
//#pragma comment(lib, "cmcfg32.lib")

BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    ) 
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if ( !LookupPrivilegeValue( 
            NULL,            // lookup privilege on local system
            lpszPrivilege,   // privilege to lookup 
            &luid ) )        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError() ); 
        return FALSE; 
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if ( !AdjustTokenPrivileges(
           hToken, 
           FALSE, 
           &tp, 
           sizeof(TOKEN_PRIVILEGES), 
           (PTOKEN_PRIVILEGES) NULL, 
           (PDWORD) NULL) )
    { 
          printf("AdjustTokenPrivileges error: %u\n", GetLastError() ); 
          return FALSE; 
    } 

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
          printf("The token does not have the specified privilege. \n");
          return FALSE;
    } 

    return TRUE;
}




std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

bool get_all_evironment_variables()
{
	LPTSTR lpszVariable; 
    LPTCH lpvEnv; 
 
    // Get a pointer to the environment block. 
 
    lpvEnv = GetEnvironmentStrings();

    // If the returned pointer is NULL, exit.
    if (lpvEnv == NULL)
    {
        printf("GetEnvironmentStrings failed (%d)\n", GetLastError()); 
        return 0;
    }
 
    // Variable strings are separated by NULL byte, and the block is 
    // terminated by a NULL byte. 

    lpszVariable = (LPTSTR) lpvEnv;

    while (*lpszVariable)
    {
        _tprintf(TEXT("%s\n"), lpszVariable);
        lpszVariable += lstrlen(lpszVariable) + 1;
    }
    FreeEnvironmentStrings(lpvEnv);
	return 0;
}
//
//To programmatically add or modify system environment variables, add them to the HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment registry key,
//	then broadcast a WM_SETTINGCHANGE message with lParam set to the string "Environment". This allows applications, such as the shell, to pick up your updates. 
//


bool write_to_registry(HKEY hkey, std::wstring Name, std::wstring Data)
{
	DWORD size = Data.length()* sizeof(TCHAR);	
	if(RegSetValueEx(hkey, Name.c_str(), 0, REG_EXPAND_SZ, (LPBYTE)(Data.c_str()), size) == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	bool ret = get_all_evironment_variables();
	std::wstring VARNAME = L"test";
	std::wstring value = L"test";	
	HKEY hkey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		 printf("good\n"); 
	}
	else
	{
		 printf("bad (%d)\n", GetLastError()); 
	}
	
	write_to_registry(hkey, L"GLEW_BIN", L"%GLEW_ROOT%\\bin");
	write_to_registry(hkey, L"GLEW_INC", L"%GLEW_ROOT%\\include");
	write_to_registry(hkey, L"GLEW_LIB", L"%GLEW_ROOT%\\lib");
	write_to_registry(hkey, L"GLEW_ROOT", L"C:\\mjs\\GLEW\\glew-1.7.0");

	write_to_registry(hkey, L"GLUT_BIN", L"%GLUT_ROOT%\\glut-3.7.6-bin");
	write_to_registry(hkey, L"GLUT_INC", L"%GLUT_ROOT%\\glut-3.7.6-bin");
	write_to_registry(hkey, L"GLUT_LIB", L"%GLUT_ROOT%\\glut-3.7.6-bin");
	write_to_registry(hkey, L"GLUT_ROOT", L"c:\\mjs\\GLUT");
	
	write_to_registry(hkey, L"OSG_DEBUG", L"C:\\mjs\\OSG\\OpenSceneGraph-3.0.1-VS10.0.30319-x86-debug-12741");
	write_to_registry(hkey, L"OSG_DEBUG_BIN", L"%OSG_DEBUG%\\bin");
	write_to_registry(hkey, L"OSG_DEBUG_INC", L"%OSG_DEBUG%\\include");
	write_to_registry(hkey, L"OSG_DEBUG_LIB", L"%OSG_DEBUG%\\lib");

	write_to_registry(hkey, L"OSG_RELEASE", L"C:\\mjs\\OSG\\OpenSceneGraph-3.0.1-VS10.0.30319-x86-release-12741");
	write_to_registry(hkey, L"OSG_RELEASE_BIN", L"%OSG_RELEASE%\\bin");
	write_to_registry(hkey, L"OSG_RELEASE_INC", L"%OSG_RELEASE%\\include");
	write_to_registry(hkey, L"OSG_RELEASE_LIB", L"%OSG_RELEASE%\\lib");

	write_to_registry(hkey, L"PHYSX_BIN", L"%PHYSX_ROOT%\\Bin\\win32");
	write_to_registry(hkey, L"PHYSX_CHARACTER_INC", L"%PHYSX_ROOT%\\SDKs\\NxCharacter\\include");
	write_to_registry(hkey, L"PHYSX_COMMON_INC", L"%PHYSX_ROOT%\\Samples\\SampleCommonCode\\src");
	write_to_registry(hkey, L"PHYSX_COOKING_INC", L"%PHYSX_ROOT%\\SDKs\\Cooking\\include");
	write_to_registry(hkey, L"PHYSX_FOUNDATION_INC", L"%PHYSX_ROOT%\\SDKs\\Foundation\\include");
	write_to_registry(hkey, L"PHYSX_LIB", L"%PHYSX_ROOT%\\SDKs\\lib\\win32");
	write_to_registry(hkey, L"PHYSX_LOADER_INC", L"%PHYSX_ROOT%\\SDKs\\PhysXLoader\\include");
	write_to_registry(hkey, L"PHYSX_PHYSICS_INC", L"%PHYSX_ROOT%\\SDKs\\Physics\\include");
	write_to_registry(hkey, L"PHYSX_ROOT", L"C:\\mjs\\PhysX SDK 2.8.4\\v2.8.4_win");

	write_to_registry(hkey, L"BOOST_ROOT", L"C:\\mjs\\boost_1_50_0");
	write_to_registry(hkey, L"BOOST_LIB", L"%BOOST_ROOT%\\libs");
	write_to_registry(hkey, L"BOOST_INC", L"%BOOST_ROOT%\\boost");

	write_to_registry(hkey, L"PYTHON_ROOT", L"C:\\Python27");
	write_to_registry(hkey, L"PYTHON_LIB", L"%PYTHON_ROOT%\\libs");
	write_to_registry(hkey, L"PYTHON_INC", L"%PYTHON_ROOT%\\include");

	write_to_registry(hkey, L"OPENAL_ROOT", L"C:\\Program Files (x86)\\OpenAL 1.1 SDK");
	write_to_registry(hkey, L"OPENAL_LIB", L"%OPENAL_ROOT%\\libs\\Win32");
	write_to_registry(hkey, L"OPENAL_INC", L"%OPENAL_ROOT%\\include");
	write_to_registry(hkey, L"OPENAL_BIN", L"%OPENAL_ROOT%\\libs\\Win32");

	//TODO
	// add location of the dlls to Path in the environment variables
	return 0;
}

