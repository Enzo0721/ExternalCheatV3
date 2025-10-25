#include "gui.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx9.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "settings.h"
#include <filesystem>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;
		
	switch (message)
	{
	case WM_SIZE: {
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}return 0;
	}
	case WM_SYSCOMMAND: {
		if ((wideParameter & 0xfff0) == SC_KEYMENU)// disable alt app menu
			return 0;
	}break;
	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;
	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(longParameter);
	}return 0;
	case WM_MOUSEMOVE: {
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{ };

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 &&
				gui::position.x <= gui::WIDTH &&
				gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(
					gui::window,
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}
	}return 0;
	}
	return DefWindowProcW(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName, const char* className)noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExA(&windowClass);

	window = CreateWindowA(
		className,
		windowName,
		WS_POPUP,
		100,
		100,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0

	);
	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyWindow()noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	ImVec4* colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.22f, 0.13f, 0.35f, 0.56f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.22f, 0.13f, 0.35f, 0.56f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.22f, 0.13f, 0.35f, 0.78f);
	colors[ImGuiCol_Button] = ImVec4(0.22f, 0.13f, 0.35f, 0.56f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.13f, 0.35f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.13f, 0.35f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.22f, 0.13f, 0.35f, 0.56f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.13f, 0.35f, 0.85f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.22f, 0.13f, 0.35f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.18f, 0.18f, 0.18f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.22f, 0.13f, 0.35f, 0.76f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.22f, 0.13f, 0.35f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.22f, 0.13f, 0.35f, 0.32f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.22f, 0.13f, 0.35f, 0.59f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.22f, 0.13f, 0.35f, 0.72f);
	colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.13f, 0.35f, 0.59f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	ImGui::GetStyle().FrameRounding = 2.0f;
	ImGui::GetStyle().WindowPadding = ImVec2(8 , 3);
	ImGui::GetStyle().WindowBorderSize = 1.00f;
	ImGui::GetStyle().WindowRounding = 3.00f;
	ImGui::GetStyle().ScrollbarRounding = 2.00f;
	ImGui::GetStyle().TabRounding = 2.00f;
	ImGui::GetStyle().WindowTitleAlign = ImVec2(0.50f, 0.50f);
	ImGui::GetStyle().FramePadding = ImVec2(30, 3);


	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 15.f);

	io.IniFilename = NULL;


	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}
	const auto result = device->Present(0, 0, 0, 0);

	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

void gui::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0});
	ImGui::SetNextWindowSize({ float {WIDTH}, float{HEIGHT}});
	ImGui::Begin(
		"External Cheat",
		&exit,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	//////////////////////////////////////////////////////////////////////////////////////
	
	if (ImGui::BeginTabBar("tabs"))
	{
		if (ImGui::BeginTabItem("aimbot"))
		{
			ImGui::Checkbox("aimbot", &settings::toggle_aimbot);
			ImGui::SliderInt("aimbot fov", &settings::aimbotFOV, 0, 180);
			ImGui::SliderFloat("aimbot smoothness", &settings::aimbotSmoothness, 1, 100);
			ImGui::SliderFloat("Recoil Assist", &settings::aimbotRecoilAssist, 0.0f, 1.0f, "%.2f");
			struct BoneOption { const char* name; int index; };
			static const BoneOption boneOptions[] = {
				{"Pelvis", 3}, {"Lower Chest", 4}, {"Chest", 5}, {"Upper Chest", 6},
				{"Neck", 7}, {"Head", 8}, {"Left Hand", 13}, {"Right Hand", 14},
				{"Left Foot", 39}, {"Right Foot", 42}
			};

			ImGui::Text("Aimbot Target Bones:");
			for (const auto& bone : boneOptions) {
				bool selected = settings::aimbotBone.count(bone.index) > 0;
				if (ImGui::Checkbox(bone.name, &selected)) {
					if (selected)
						settings::aimbotBone.insert(bone.index);
					else
						settings::aimbotBone.erase(bone.index);
				}
				//arrange in two columns
				if ((&bone - boneOptions) % 2 == 1) ImGui::SameLine(200);
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("visuals"))
		{
			
			ImGui::Checkbox("glow", &settings::toggle_glow);
			ImGui::Checkbox("health glow", &settings::toggle_healthGlow);
			ImGui::Checkbox("team glow", &settings::toggle_teamGlow);
			ImGui::Checkbox("chams", &settings::toggle_chams);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("misc"))
		{
			if (ImGui::Checkbox("bunnyhop", &settings::toggle_bhop))
			{

			}
			if (ImGui::Checkbox("no flash", &settings::toggle_noFlash))
			{

			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("config"))
		{
			static char saveNameBuf[128] = "myconfig";
            static int selectedSaveIndex = -1;
            auto files = settings::listConfigFiles();

            ImGui::Separator();
            ImGui::Text("Configs");
			ImGui::InputText("##configname", saveNameBuf, sizeof(saveNameBuf));
            ImGui::SameLine();
            if (ImGui::Button("Save"))
            {
                std::string name = saveNameBuf;
                if (name.empty()) name = "unnamed";
                std::string filename = name;
                if (filename.find(".ecfg") == std::string::npos)
                    filename += ".ecfg";
                settings::saveConfig(filename);
                files = settings::listConfigFiles();
                selectedSaveIndex = -1;
            }

            ImGui::Spacing();
            ImGui::Text("Available saves:");
            if (!files.empty())
            {
                static std::vector<const char*> filePtrs;
                filePtrs.clear();
                for (auto& s : files) filePtrs.push_back(s.c_str());

                if (ImGui::ListBox("##savelist", &selectedSaveIndex, filePtrs.data(), (int)filePtrs.size(), 6))
                {
                    // selection changed
                }

                ImGui::SameLine();
                if (ImGui::Button("Load") && selectedSaveIndex >= 0 && selectedSaveIndex < (int)files.size())
                {
                    settings::loadConfigFrom(files[selectedSaveIndex]);
                }
            }
            else
            {
                ImGui::TextDisabled("No .ecfg saves found");
            }

			ImGui::ColorEdit4("glow color", settings::glowColor);
			ImGui::ColorEdit4("team glow color", settings::teamGlowColor);
			ImGui::ColorEdit3("chams color", settings::chamsColor);
			const char* glowTypes[] = { "standard", "full bloom", "aura pulse", "outline", "outline pulse"};
			ImGui::PushItemWidth(200);
			ImGui::ListBox("glow type", &settings::glowType, glowTypes, IM_ARRAYSIZE(glowTypes), IM_ARRAYSIZE(glowTypes));
			ImGui::PopItemWidth();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	//ImGui::ShowDemoWindow();
	//ImGui::ShowStyleEditor();
	
	//////////////////////////////////////////////////////////////////////////////////////
	
	ImGui::End();
}
