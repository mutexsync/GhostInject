#include "includes.h"

bool b_opened = true;

int selectable_method = 0;

Funcs funcs;

ImVec4 rgba(float r, float g, float b, float a)
{
    return (ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f));
}

ImVec4 hex(int hexValue)
{
	auto r = GetRValue(hexValue);
	auto g = GetGValue(hexValue);
	auto b = GetBValue(hexValue);
	return ImVec4(r, g, b, 1.f);
}

namespace ImGui
{
	void StyleMyColor()
    {
        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        colors[ImGuiCol_Text]                   = rgba(255.f, 255.f, 255.f, 255.f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = rgba(55.f, 55.f, 55.f, 255);
        colors[ImGuiCol_FrameBgHovered]         = rgba(70.f, 70.f, 70.f, 255.f);
        colors[ImGuiCol_FrameBgActive]          = rgba(70.f, 70.f, 70.f, 255.f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = rgba(30.f, 30.f, 30.f, 255.f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = rgba(106.f, 90.f, 205.f, 255.f);
        colors[ImGuiCol_SliderGrabActive]       = rgba(115.f, 90.f, 205.f, 255.f);
        colors[ImGuiCol_Button]                 = rgba(106.f, 90.f, 205.f, 255.f);
        colors[ImGuiCol_ButtonHovered]          = rgba(72.f, 61.f, 139.f, 255.f);
        colors[ImGuiCol_ButtonActive]           = rgba(25.f, 25.f, 112.f, 255.f);
        colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab]                    = ImLerp(colors[ImGuiCol_Header],       colors[ImGuiCol_TitleBgActive], 0.80f);
        colors[ImGuiCol_TabHovered]             = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive]              = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        colors[ImGuiCol_TabUnfocused]           = ImLerp(colors[ImGuiCol_Tab],          colors[ImGuiCol_TitleBg], 0.80f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImLerp(colors[ImGuiCol_TabActive],    colors[ImGuiCol_TitleBg], 0.40f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
}

ImFont* fArialG = nullptr;
ImFont* fArialP = nullptr;

void Screen::InitConfigs()
{
    ImGui::StyleMyColor();
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();
	style.ScrollbarSize = 9.0f;
	style.WindowRounding = 0.f;
	style.ChildRounding = 4.0f;
	style.PopupRounding = 1.0f;
	style.FrameRounding = 3.0f;
	style.ScrollbarRounding = 1.0f;
	style.GrabRounding = 1.0f;
	style.TabRounding = 1.0f;

	style.WindowTitleAlign.x = 0.5f;

    fArialG = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 25);
    fArialP = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 15);
}

struct Config
{
    char process_name[100];
    int delay_inject = 0;
    const char* path;
} Config;

vector<string> paths_dll;
string item_current;
static string dawgduawdiawhgduawihduiahdiawda;

void create_file(int pos, string str)
{
    mINI::INIFile file("C:\\Windows\\Temp\\injetor.ini");
    mINI::INIStructure ini;
    stringstream _dll_name____;
    _dll_name____ << "dll" << pos;
    ini["path"][_dll_name____.str()] = str;
    file.generate(ini);
}

void write_file(int pos, string str)
{
    mINI::INIFile file("C:\\Windows\\Temp\\injetor.ini");
    mINI::INIStructure ini;
    file.read(ini);
    stringstream _dll_name____;
    _dll_name____ << "dll" << pos;
    if (ini["path"].has(_dll_name____.str()))
    {
        pos++;
        _dll_name____.str("");
        _dll_name____ << "dll" << pos;
    }
    ini["path"][_dll_name____.str()] = str;
    file.write(ini);
}

void read_file()
{
    mINI::INIFile file("C:\\Windows\\Temp\\injetor.ini");
    mINI::INIStructure ini;
    file.read(ini);
    for (size_t i = 0; i < 1000; i++)
    {
        stringstream _dll_name____;
        _dll_name____ << "dll" << i;
        string value = ini.get("path").get(_dll_name____.str());
        if (!value.empty())
        {
            paths_dll.push_back(value);
        }
    }
}

void remove_dll(string current)
{
    mINI::INIFile file("C:\\Windows\\Temp\\injetor.ini");
    mINI::INIStructure ini;
    file.read(ini);
    for (size_t i = 0; i < 1000; i++)
    {
        stringstream _dll_name____;
        _dll_name____ << "dll" << i;
        string value = ini.get("path").get(_dll_name____.str());
        if (value == current)
        {
            ini["path"].remove(_dll_name____.str());
        }
    }
    file.write(ini);
}

void Screen::DrawInject()
{
	if (!b_opened)
		exit(0);

    static bool init = true;
    if (init)
    {
        read_file();
        init = false;
    }

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(LARGURA, ALTURA));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, rgba(255, 0, 0, 255));
    ImGui::PushStyleColor(ImGuiCol_Text, rgba(255, 255, 255, 255));
    ImGui::PushFont(fArialP);
	ImGui::Begin(WINDOW_NAME, &b_opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
	{
        ImGui::PushStyleColor(ImGuiCol_ChildBg, rgba(20, 20, 20, 255));
        ImGui::BeginChild("first", ImVec2((LARGURA / 2) - 5, ALTURA - 40), true);
        {
            static DWORD pid = 0;
            ImGui::Text("Process Name:");
            ImGui::SameLine();
            ImGui::PushItemWidth(150.f);
            ImGui::InputText("##1337", Config.process_name, 100);
            ImGui::PopItemWidth();
            stringstream process_;
            stringstream info_process_;
            process_ << Config.process_name << ".exe";
            pid = funcs.GetPID(process_.str().c_str());
            if (_strcmpi(Config.process_name, ""))
            {
                info_process_ << "[" << pid << "] " << Config.process_name << ".exe";
                ImGui::Text(info_process_.str().c_str());
            }
            ImGui::Text("Delay Inject:");
            ImGui::SameLine();
            ImGui::PushItemWidth(55.f);
            ImGui::SliderInt("", &Config.delay_inject, 0, 60);
            ImGui::SameLine();
            ImGui::Text("seconds");
            ImGui::RadioButton("LoadLibrary", &selectable_method, 0);
            ImGui::RadioButton("ManualMap", &selectable_method, 1);
            if (ImGui::Button("Close Process", ImVec2(ImGui::CalcTextSize("Close Process").x + 20, 30)))
            {
                if (funcs.CloseProcess(pid))
                {
                    for (size_t i = 0; i < 100; i++)
                    {
                        Config.process_name[i] = '\0';
                    }
                }
            }
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, rgba(0, 150, 0, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, rgba(0, 100, 0, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, rgba(0, 70, 0, 255));
            if (ImGui::Button("Inject", ImVec2(ImGui::CalcTextSize("Inject").x + 20, 30)))
            {
                if (pid != 0)
                {
                    if (funcs.inject(pid, Config.path, Config.delay_inject, selectable_method))
                    {
                        MessageBoxA(0, "Injected successfully", WINDOW_NAME, MB_ICONINFORMATION | MB_OK);
                    }
                }
                else
                {
                    MessageBoxA(0, "Processo inválido!", WINDOW_NAME, MB_ICONERROR | MB_OK);
                    for (size_t i = 0; i < 100; i++)
                    {
                        Config.process_name[i] = '\0';
                    }
                }
            }
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::SameLine((LARGURA / 2) + 7);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, rgba(20, 20, 20, 255));
        ImGui::BeginChild("second", ImVec2((LARGURA / 2) - 14, ALTURA - 40), true, ImGuiWindowFlags_NoScrollbar);
        {
            ImGui::SetNextItemWidth(240.f);
            ImGui::ListBoxHeader("##1335");
            {
                for (size_t i = 0; i < paths_dll.size(); i++)
                {
                    const auto selecionavel = item_current == paths_dll[i];
                    // cout << i << ":" << paths_dll[i] << endl;
                    if (ImGui::Selectable(paths_dll[i].c_str(), selecionavel))
                    {
                        item_current = paths_dll[i].c_str();
                    }
                    if (selecionavel)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
            }
            ImGui::ListBoxFooter();
            if (!item_current.empty())
            {
                Config.path = item_current.c_str();
            }
            if (ImGui::Button("Add DLL", ImVec2(ImGui::CalcTextSize("Add DLL").x + 60, 30)))
            {
                OPENFILENAME ofn = { 0 };
                TCHAR szFileName[MAX_PATH] = { 0 };
                HWND hWnd = NULL;
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = sizeof(szFileName);
                if (GetOpenFileNameA(&ofn))
                {
                    dawgduawdiawhgduawihduiahdiawda = szFileName;
                    ifstream file_("C:\\Windows\\Temp\\injetor.ini");
                    if (file_.good())
                    {
                        write_file(paths_dll.size(), dawgduawdiawhgduawihduiahdiawda);
                    }
                    else
                    {
                        create_file(paths_dll.size(), dawgduawdiawhgduawihduiahdiawda);
                    }                   
                    paths_dll.push_back(dawgduawdiawhgduawihduiahdiawda);
                }
            }
            ImGui::SameLine(130.f);
            if (ImGui::Button("Remove DLL", ImVec2(ImGui::CalcTextSize("Remove DLL").x + 35, 30)))
            {
                for (size_t j = 0; j < paths_dll.size(); j++)
                {
                    if (paths_dll[j].find(item_current) != string::npos)
                    {
                        remove_dll(item_current);
                        paths_dll.erase(paths_dll.begin()+j);
                    }
                }
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleColor();
	}
	ImGui::End();
    ImGui::PopFont();
}