#include "Debug.h"
#include "Commons.h"
#include <string>

Debug::Debug(SDL_Renderer* renderer) {
	mRenderer = renderer;
	ImGui::CreateContext();
	ImGuiSDL::Initialize(mRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
};

Debug::~Debug() {
	mRenderer = nullptr;
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Debug::Update(SDL_Event* e) {
	ImGui_ImplSDL2_ProcessEvent(e);
}

void Debug::ShowDebugWindow() {
	ImGui::NewFrame();
	ImGui::SetNextWindowSize(ImVec2(160, 120));
	ImGui::Begin("Debug");
	ImGui::End();
	ImGui::EndFrame();
}

void Debug::Render() {
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
}

//void Debug::DrawRect(SDL_Rect rect, RGBA color) {
//    SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);
//    int draw = SDL_RenderDrawRect(mRenderer, &rect);
//    SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0x00);
//};