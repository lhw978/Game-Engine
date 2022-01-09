 /*
    Copyright (c) 2018-2021 Piotr Doan. All rights reserved.
    Software distributed under the permissive MIT License.
*/

#include "Common/Debug.hpp"
#include "Common/Profile.hpp"

void Debug::Initialize()
{
    LOG_PROFILE_SCOPE("Initialize debug");

    // Enable debug memory allocator and memory leak detection.
#if defined(PLATFORM_WINDOWS) && defined(CONFIG_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);
#endif
}

bool Debug::IsDebuggerAttached()
{
#ifdef PLATFORM_WINDOWS
    return IsDebuggerPresent();
#else
    return false;
#endif
}
