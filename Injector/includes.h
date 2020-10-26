#pragma once

#define LARGURA 550
#define ALTURA 250
#define WINDOW_NAME "GhostInject"

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

#include <Windows.h>
#include <GLFW/glfw3.h>
#include <TlHelp32.h>
#include <string>
#include <sstream>
#include <typeinfo>
#include <iostream>
#include <tchar.h>
#include <vector>
#include <atlbase.h>
#include <fstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imgui/imgui_internal.h"

#include "mini.h"

#include "Screen.h"
#include "Funcs.h"