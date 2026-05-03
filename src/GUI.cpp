#include "includes/GUI.hpp"

GUI::GUI(GLFWwindow *window, Texture &texture): texture(texture)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    this->lightRGB[0] = 1.0f;
    this->lightRGB[1] = 1.0f;
    this->lightRGB[2] = 1.0f;

    this->lightAmbientStrength[0] = 0.2f;
    this->lightAmbientStrength[1] = 0.2f;
    this->lightAmbientStrength[2] = 0.2f;

    std::string default_texture = this->texture.GetTexturePath();
    for (const auto& entry : std::filesystem::directory_iterator("textures/")) {
        std::filesystem::path filepath = entry;
        if (std::filesystem::is_regular_file(entry) && filepath.extension() == ".png") {
            if (filepath == default_texture)
                this->textureIDx = this->textureList.size();
            this->textureList.push_back(std::string(filepath));
        }
    };

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::RenderWindow()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Model Options");
    if (ImGui::BeginMenu("Light Settings")) {
        ImGui::SliderFloat3("Light Colour", this->lightRGB, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat3("Light Ambient Strength", this->lightAmbientStrength, 0.0f, 1.0f, "%.2f");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Texture Settings")) {
        if (ImGui::BeginCombo("Texture Select", this->textureList[this->textureIDx].data())) {
            for (int i = 0; i < static_cast<int>(textureList.size()); i++) {
                const bool is_selected = (textureIDx == i);
                if (ImGui::Selectable(textureList[i].c_str(), is_selected)) {
                    this->textureIDx = i;
                    std::cout << "Test: " << textureList[textureIDx] << std::endl;
                    texture.SetTexture(textureList[textureIDx]);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::EndMenu();
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

float* GUI::getLightRGB(void)
{
    return this->lightRGB;
};

float* GUI::getLightAmbientStrength(void) {
    return this->lightAmbientStrength;
};
