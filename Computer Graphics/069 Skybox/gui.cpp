
#include "imgui.h"
#include "imgui_impl_glfw_GL3.h"
#include <GLFW\glfw3.h>
#include "gui.h"
GLFWwindow* myWindow;
static bool showMenu;

//#include "imgui_impl_glfw_gl3.h"

static void ShowStats(bool opened) {
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	if (!ImGui::Begin("Overlay", &opened, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings)) {
		ImGui::End();
		return;
	}

	ImGui::Text("Please work!!!!!!!");
	ImGui::Separator();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
	ImGui::End();
}

static void Menu() {
	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	bool show_test_window = true;
	static bool showTestWindow = false;

	bool opened;
	if (!ImGui::Begin("Menu", &opened, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	if (ImGui::Button("Show Test Window"))
		showTestWindow ^= 1;

	if (ImGui::CollapsingHeader("Yo!"))
	{
		ImGui::Text("hello");
	}

	if (showTestWindow)
		ImGui::ShowTestWindow();
	ImGui::End();
}

void UpdateUI() {
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfwGL3_NewFrame();
	
	static int keystate;
	int newkeystate = glfwGetKey(myWindow, 'M');
	if (newkeystate && newkeystate != keystate) {
		showMenu = !showMenu;
		if (showMenu) {
			glfwSetInputMode(myWindow, GLFW_CURSOR,
				io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(myWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	keystate = newkeystate;

	ShowStats(true);

	if (showMenu) {
		Menu();
	}
}

void initUI(GLFWwindow* inwindow) {
	myWindow = inwindow;
	bool f = ImGui_ImplGlfwGL3_Init(myWindow, true);
	showMenu = false;
}

void DrawUI() { ImGui::Render(); }

void ShutDownUI() { ImGui_ImplGlfwGL3_Shutdown(); }
