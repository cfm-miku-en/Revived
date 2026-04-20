#include <string>
#include <codecvt>
#include <vector>

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#include <openvr.h>
#include <detours/detours.h>
#include "../ReviveOverlay/version.h"

extern FILE* g_LogFile;
#define LOG(x, ...) if (g_LogFile) fprintf(g_LogFile, x, __VA_ARGS__); \
					printf(x, __VA_ARGS__); \
					fflush(g_LogFile);

FILE* g_LogFile = NULL;

static const wchar_t* const s_BlockedProcesses[] = {
	L"roblox", L"robloxplayerbeta", L"robloxstudiobeta",
	L"chrome", L"firefox", L"msedge", L"opera", L"brave",
	L"steam", L"steamwebhelper",
	L"discord", L"spotify",
	L"explorer",
};

static bool IsBlockedProcess(const wchar_t* cmdLine)
{
	const wchar_t* start = wcsrchr(cmdLine, L'\\');
	start = start ? start + 1 : cmdLine;
	wchar_t base[MAX_PATH];
	wcsncpy(base, start, MAX_PATH - 1);
	base[MAX_PATH - 1] = L'\0';
	wchar_t* sp = wcschr(base, L' ');
	if (sp) *sp = L'\0';
	wchar_t* dot = wcsrchr(base, L'.');
	if (dot) *dot = L'\0';
	for (const wchar_t* name : s_BlockedProcesses)
	{
		if (_wcsicmp(base, name) == 0)
			return true;
	}
	return false;
}

bool IsOpenXRRuntimeInstalled()
{
	HKEY key;
	LONG error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Khronos\\OpenXR\\1", 0, KEY_READ, &key);
	if (error != ERROR_SUCCESS)
		return false;
	DWORD type, size = 0;
	error = RegQueryValueEx(key, L"ActiveRuntime", nullptr, &type, nullptr, &size);
	RegCloseKey(key);
	return error == ERROR_SUCCESS && type == REG_SZ && size > 0;
}

bool GetOculusBasePath(PWCHAR path, DWORD length)
{
	LONG error = ERROR_SUCCESS;

	HKEY oculusKey;
	error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Oculus VR, LLC\\Oculus", 0, KEY_READ | KEY_WOW64_32KEY, &oculusKey);
	if (error != ERROR_SUCCESS)
	{
		LOG("Unable to open Oculus key.");
		return false;
	}
	error = RegQueryValueEx(oculusKey, L"Base", NULL, NULL, (PBYTE)path, &length);
	if (error != ERROR_SUCCESS)
	{
		LOG("Unable to read Base path.");
		return false;
	}
	RegCloseKey(oculusKey);

	return true;
}

bool GetLibraryPath(PWCHAR path, DWORD length, PWCHAR guid)
{
	LONG error = ERROR_SUCCESS;

	// Open the libraries key
	WCHAR keyPath[MAX_PATH] = { L"Software\\Oculus VR, LLC\\Oculus\\Libraries\\" };
	HKEY oculusKey;

	// Open the library key
	wcsncat(keyPath, guid, MAX_PATH);
	error = RegOpenKeyExW(HKEY_CURRENT_USER, keyPath, 0, KEY_READ, &oculusKey);
	if (error != ERROR_SUCCESS)
	{
		LOG("Unable to open Library path key.");
		return false;
	}

	// Get the volume path to this library
	DWORD pathSize;
	error = RegQueryValueExW(oculusKey, L"Path", NULL, NULL, NULL, &pathSize);
	PWCHAR volumePath = (PWCHAR)malloc(pathSize);
	error = RegQueryValueExW(oculusKey, L"Path", NULL, NULL, (PBYTE)volumePath, &pathSize);
	RegCloseKey(oculusKey);
	if (error != ERROR_SUCCESS)
	{
		free(volumePath);
		LOG("Unable to read Library path.");
		return false;
	}

	// Resolve the volume path to a mount point
	DWORD total;
	WCHAR volume[50] = { L'\0' };
	wcsncpy(volume, volumePath, 49);
	GetVolumePathNamesForVolumeNameW(volume, path, length, &total);
	wcsncat(path, volumePath + 49, MAX_PATH);
	free(volumePath);

	return true;
}

bool GetDefaultLibraryPath(PWCHAR path, DWORD length)
{
	LONG error = ERROR_SUCCESS;

	// Open the libraries key
	WCHAR keyPath[MAX_PATH] = { L"Software\\Oculus VR, LLC\\Oculus\\Libraries\\" };
	HKEY oculusKey;
	error = RegOpenKeyExW(HKEY_CURRENT_USER, keyPath, 0, KEY_READ, &oculusKey);
	if (error != ERROR_SUCCESS)
	{
		LOG("Unable to open Libraries key.");
		return false;
	}

	// Get the default library
	WCHAR guid[40] = { L'\0' };
	DWORD guidSize = sizeof(guid);
	error = RegQueryValueExW(oculusKey, L"DefaultLibrary", NULL, NULL, (PBYTE)guid, &guidSize);
	RegCloseKey(oculusKey);
	if (error != ERROR_SUCCESS)
	{
		LOG("Unable to read DefaultLibrary guid.");
		return false;
	}

	// Open the default library key
	wcsncat(keyPath, guid, MAX_PATH);
	error = RegOpenKeyExW(HKEY_CURRENT_USER, keyPath, 0, KEY_READ, &oculusKey);
	if (error != ERROR_SUCCESS)
	{
		LOG("Unable to open Library path key.");
		return false;
	}

	// Get the volume path to this library
	DWORD pathSize;
	error = RegQueryValueExW(oculusKey, L"Path", NULL, NULL, NULL, &pathSize);
	PWCHAR volumePath = (PWCHAR)malloc(pathSize);
	error = RegQueryValueExW(oculusKey, L"Path", NULL, NULL, (PBYTE)volumePath, &pathSize);
	RegCloseKey(oculusKey);
	if (error != ERROR_SUCCESS)
	{
		free(volumePath);
		LOG("Unable to read Library path.");
		return false;
	}

	// Resolve the volume path to a mount point
	DWORD total;
	WCHAR volume[50] = { L'\0' };
	wcsncpy(volume, volumePath, 49);
	GetVolumePathNamesForVolumeNameW(volume, path, length, &total);
	wcsncat(path, volumePath + 49, MAX_PATH);
	free(volumePath);

	return true;
}

static std::wstring RegReadSZ(HKEY root, LPCWSTR subkey, LPCWSTR value, DWORD flags = KEY_READ)
{
	HKEY key;
	if (RegOpenKeyExW(root, subkey, 0, flags, &key) != ERROR_SUCCESS)
		return L"";
	DWORD size = 0;
	RegQueryValueExW(key, value, nullptr, nullptr, nullptr, &size);
	if (!size) { RegCloseKey(key); return L""; }
	std::wstring result(size / sizeof(wchar_t), L'\0');
	RegQueryValueExW(key, value, nullptr, nullptr, (PBYTE)result.data(), &size);
	RegCloseKey(key);
	while (!result.empty() && result.back() == L'\0')
		result.pop_back();
	return result;
}

class StringArray
{
public:
	void add(const std::string& str)
	{
		strings.push_back(str);
		ptrs.push_back(strings.back().c_str());
	}

	void clear()
	{
		strings.clear();
		ptrs.clear();
	}

	const char** c_str()
	{
		return ptrs.data();
	}

	bool empty()
	{
		return ptrs.empty();
	}

	size_t size()
	{
		return ptrs.size();
	}

private:
	std::vector<std::string> strings;
	std::vector<const char*> ptrs;
};

int wmain(int argc, wchar_t *argv[]) {
	if (argc < 2) {
		printf("usage: ReviveInjector.exe <executable path>\n");
		return -1;
	}

	WCHAR LogPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, LogPath)))
	{
		wcsncat(LogPath, L"\\Revive", MAX_PATH);
		
		BOOL exists = PathFileExists(LogPath);
		if (!exists)
			exists = CreateDirectory(LogPath, NULL);

		wcsncat(LogPath, L"\\ReviveInjector.txt", MAX_PATH);
		if (exists)
			g_LogFile = _wfopen(LogPath, L"w");
	}

	LOG("Launched injector with: %ls\n", GetCommandLine());

	LOG("Env: Revive version = " REV_VERSION_STRING "\n");

	std::wstring oculusVer = RegReadSZ(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Oculus VR, LLC\\Oculus", L"DriverVersion", KEY_READ | KEY_WOW64_32KEY);
	if (oculusVer.empty())
    {
        LOG("Env: Meta Horizon driver = unknown\n");
    }
    else
    {
        LOG("Env: Meta Horizon driver = %ls\n", oculusVer.c_str());
    }
    if (vr::VR_IsRuntimeInstalled())
    {
        char steamvrPath[MAX_PATH] = {};
        uint32_t pathLen = 0;
        vr::VR_GetRuntimePath(steamvrPath, MAX_PATH, &pathLen);
        LOG("Env: SteamVR = installed, runtime path = %s\n", steamvrPath);
    }
    else
    {
        LOG("Env: SteamVR = not installed\n");
    }
    std::wstring xrRuntime = RegReadSZ(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Khronos\\OpenXR\\1", L"ActiveRuntime");
    if (xrRuntime.empty())
    {
        LOG("Env: OpenXR runtime = not installed\n");
    }
    else
    {
        LOG("Env: OpenXR runtime = %ls\n", xrRuntime.c_str());
    }
    {
        std::string gpuDriver = "unknown";
        IDXGIFactory* factory = nullptr;
        if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
        {
            IDXGIAdapter* adapter = nullptr;
            if (SUCCEEDED(factory->EnumAdapters(0, &adapter)))
            {
                LARGE_INTEGER ver;
                if (SUCCEEDED(adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &ver)))
                {
                    char buf[64];
                    sprintf_s(buf, "%u.%u.%u.%u",
                        (UINT)((ver.QuadPart >> 48) & 0xFFFF),
                        (UINT)((ver.QuadPart >> 32) & 0xFFFF),
                        (UINT)((ver.QuadPart >> 16) & 0xFFFF),
                        (UINT)(ver.QuadPart & 0xFFFF));
                    gpuDriver = buf;
                }
                adapter->Release();
            }
            factory->Release();
        }
        LOG("Env: GPU driver = %s\n", gpuDriver.c_str());
    }
	char moduleDir[MAX_PATH];
	GetModuleFileNameA(NULL, moduleDir, MAX_PATH);
	PathRemoveFileSpecA(moduleDir);

	bool debug = false;
	StringArray dlls;
	std::string appKey;
	wchar_t path[MAX_PATH] = { 0 };
	for (int i = 1; i < argc; i++)
	{
		if (wcscmp(argv[i], L"/openxr") == 0)
		{
			dlls.add(moduleDir + std::string("\\LibReviveXR64.dll"));
		}
		else if (wcscmp(argv[i], L"/proxy") == 0)
		{
			dlls.add(moduleDir + std::string("\\LibOVRProxy64.dll"));
		}
		else if (wcscmp(argv[i], L"/app") == 0)
		{
			appKey = "revive.app." + std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(argv[++i]);
		}
		else if (wcscmp(argv[i], L"/base") == 0)
		{
			if (!GetOculusBasePath(path, MAX_PATH))
				return -1;
		}
		else if (wcscmp(argv[i], L"/library") == 0)
		{
			if (!GetLibraryPath(path, MAX_PATH, argv[++i]))
			{
				if (!GetDefaultLibraryPath(path, MAX_PATH))
				{
					return -1;
				}
			}
			wcsncat(path, L"\\", MAX_PATH);
		}
		else if (wcscmp(argv[i], L"/debug") == 0)
		{
			debug = true;
		}
		else
		{
			// Concatenate all other arguments
			wcsncat(path, argv[i], MAX_PATH);
			wcsncat(path, L" ", MAX_PATH);
		}
	}

	if (*path && IsBlockedProcess(path))
	{
		const wchar_t* name = wcsrchr(path, L'\\');
		LOG("Injection skipped: %ls is in the non-game blocklist\n", name ? name + 1 : path);
		return 0;
	}

	if (dlls.empty())
	{
		if (IsOpenXRRuntimeInstalled())
		{
			dlls.add(moduleDir + std::string("\\LibReviveXR64.dll"));
		}
		else
		{
			dlls.add(moduleDir + std::string("\\openvr_api64.dll"));
			dlls.add(moduleDir + std::string("\\LibRevive64.dll"));
		}
	}
	
	LOG("Command for injector is: %ls\n", path);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	wchar_t workingDir[MAX_PATH];
	wcsncpy(workingDir, path, MAX_PATH);

	// Remove extension
	wchar_t* ext = wcsstr(workingDir, L".exe");
	if (ext)
		*ext = L'\0';

	// Remove filename
	wchar_t* file = wcsrchr(workingDir, L'\\');
	if (file)
		*file = L'\0';

	if (!DetourCreateProcessWithDlls(NULL, path, NULL, NULL, FALSE,
		debug ? CREATE_SUSPENDED | DEBUG_ONLY_THIS_PROCESS : 0,
		NULL, (file && ext) ? workingDir : NULL, &si, &pi,
		(DWORD)dlls.size(), dlls.c_str(), NULL))
	{
		LOG("Failed to create process\n");
		return -1;
	}

	if (debug)
	{
		if (!DebugActiveProcessStop(pi.dwProcessId))
		{
			LOG("Failed to stop debugging\n");
			return -1;
		}

		if (ResumeThread(pi.hThread) == -1)
		{
			LOG("Failed to resume process\n");
			return -1;
		}
	}

	LOG("Succesfully injected!\n");

	if (!appKey.empty())
	{
		vr::EVRInitError err;
		vr::VR_Init(&err, vr::VRApplication_Utility);
		if (err == vr::VRInitError_None)
		{
			if (vr::VRApplications()->IdentifyApplication(pi.dwProcessId, appKey.c_str()) == vr::VRApplicationError_None)
				LOG("Identified application as: %s\n", appKey.c_str());
			vr::VR_Shutdown();
		}
	}
	return 0;
}
