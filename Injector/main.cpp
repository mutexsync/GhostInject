#include "includes.h"

Screen screen;

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(LARGURA, ALTURA, WINDOW_NAME, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetWindowAttrib(window, GLFW_RESIZABLE, FALSE);
    glfwSetWindowAttrib(window, GLFW_DECORATED, FALSE);

    /*AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);*/

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    screen.InitConfigs();

    while (!glfwWindowShouldClose(window))
    {

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        screen.DrawInject();

        ImGui::Render();

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}