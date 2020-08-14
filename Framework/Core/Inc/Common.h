#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#define ENABLE_DIRECTX11
//#define ENABLE_OPENGL

//Win32 headers
#include <objbase.h>
#include <Windows.h>

//Standard headers
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "commdlg.h"

// External Headers
#include <RapidJSON/Inc/document.h>
#include <RapidJSON/Inc/filereadstream.h>
#include <RapidJSON/Inc/filewritestream.h>