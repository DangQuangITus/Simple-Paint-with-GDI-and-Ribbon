
#pragma once

#include <windows.h>

// PURPOSE: Show contextual UI in the location indicated.
// RETURNS: S_OK when actual UI is shown (including when both parts are turned off), E_FAIL otherwise.
HRESULT ShowContextualUI(POINT& ptLocation, HWND hWnd);