#include "SceneManager.h"
#include "GUI.h"
GLFWwindow* myWindow;
static bool showMenu;
static int keystate;


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

	extern SceneManager* myScene;

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"

	bool opened;
	if (!ImGui::Begin("Menu", &opened, ImGuiWindowFlags_AlwaysAutoResize)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	if (ImGui::CollapsingHeader("Debug"))
	{
		ImGui::Text("Optimization");
		if (ImGui::Button("Draw Lines"))
		{
			myScene->setDebugBool(!myScene->getDebugBool());
		}

	}

	if (ImGui::CollapsingHeader("Post-Processing"))
	{
		if (ImGui::Button("Blur"))
		{
			myScene->setBlurBool(!myScene->getBlurBool());
		}

		if (ImGui::Button("Greyscale"))
		{
			myScene->setGreyBool(!myScene->getGreyBool());
		}

		if (ImGui::Button("Vignette"))
		{
			myScene->setVigBool(!myScene->getVigBool());
		}

	}

	ImGui::End();
}

void updateGUI() {
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfwGL3_NewFrame();

	
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

void initialiseGUI(GLFWwindow* inwindow) {
	myWindow = inwindow;
	bool f = ImGui_ImplGlfwGL3_Init(myWindow, true);
	showMenu = false;
	glfwSetInputMode(myWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void renderGUI() { ImGui::Render(); }

void cleanGUI() { ImGui_ImplGlfwGL3_Shutdown(); }
