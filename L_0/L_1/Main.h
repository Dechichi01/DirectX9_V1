#pragma once

#include "stdafx.h"
#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <d3dx9.h>

#define RGB2BGR( Color ) (Color & 0xFF000000) | ((Color & 0xFF0000) >> 16) | (Color & 0x00FF00) | ((Color & 0x0000FF) << 16)



