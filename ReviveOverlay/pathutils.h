#pragma once

#include <Windows.h>

inline bool IsVolumeGuidPath(const wchar_t* path)
{
	if (!path)
		return false;

	// Matches Win32 extended syntax "\\?\" or NT object manager syntax "\??\" followed by "Volume{"
	return (path[0] == L'\\' &&
		(path[1] == L'\\' || path[1] == L'?') &&
		path[2] == L'?' &&
		path[3] == L'\\' &&
		_wcsnicmp(path + 4, L"Volume{", 7) == 0);
}

inline bool ResolveOculusVolumePath(const wchar_t* volumePath, wchar_t* outPath, DWORD length)
{
	if (!volumePath || !outPath || length == 0)
		return false;

	// Volume GUID strings have 49 characters: \\?\Volume{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}\ 
	if (IsVolumeGuidPath(volumePath) && wcslen(volumePath) >= 49)
	{
		DWORD total = 0;
		// Win32 volume APIs strictly require "\\?\"; normalize NT "\??\" prefix to "\\?\"
		WCHAR volume[50] = L"\\\\?\\";
		wcsncpy(volume + 4, volumePath + 4, 45);
		volume[49] = L'\0';

		outPath[0] = L'\0';
		if (GetVolumePathNamesForVolumeNameW(volume, outPath, length, &total) && outPath[0] != L'\0')
		{
			const wchar_t* subPath = volumePath + 49;
			while (*subPath == L'\\' || *subPath == L'/') subPath++;
			wcsncat(outPath, subPath, length - 1);
		}
		else
		{
			wcsncpy(outPath, volumePath, length - 1);
			outPath[length - 1] = L'\0';
		}
	}
	else
	{
		wcsncpy(outPath, volumePath, length - 1);
		outPath[length - 1] = L'\0';
	}
	return true;
}
