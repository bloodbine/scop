#include "includes/GUI.hpp"

GUI::GUI(GLFWwindow *window, Texture &texture, float (&modelMatrix)[16]): texture(texture), modelMatrix(modelMatrix)
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

    this->scale[0] = 1.0f;
    this->scale[1] = 1.0f;
    this->scale[2] = 1.0f;

    this->renderModeList["Fill"] = GL_FILL;
    this->renderModeList["Line"] = GL_LINE;
    this->renderModeList["Point"] = GL_POINT;
    this->renderMode = "Fill";

    std::string default_texture = this->texture.GetTexturePath();
    for (const auto& entry : std::filesystem::directory_iterator("textures/")) {
        std::filesystem::path filepath = entry;
        if (std::filesystem::is_regular_file(entry) && filepath.extension() == ".png") {
            if (filepath == default_texture)
                this->textureIDx = this->textureList.size();
            this->textureList.push_back(std::string(filepath.filename()));
        }
    };
    this->textureList.push_back("None");

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
        ImGui::SliderFloat3("Light Colour", this->lightRGB, 0.0f, 5.0f, "%.2f");
        ImGui::SliderFloat3("Light Ambient Strength", this->lightAmbientStrength, 0.0f, 1.0f, "%.2f");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Texture Settings")) {
        if (ImGui::BeginCombo("Texture Select", this->textureList[this->textureIDx].data())) {
            for (int i = 0; i < static_cast<int>(textureList.size()); i++) {
                const bool is_selected = (textureIDx == i);
                if (ImGui::Selectable((textureList[i]).c_str(), is_selected)) {
                    this->textureIDx = i;
                    if (this->renderMode == "Fill")
                        texture.SetTexture(textureList[textureIDx]);
                    else
                        texture.SetTexture(textureList[textureList.size() - 1]);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Render Mode", this->renderMode.c_str())) {
            for (std::map<std::string, GLenum>::iterator it = this->renderModeList.begin(); it != this->renderModeList.end(); it++) {
                const bool is_selected = (this->renderMode == it->first);
                if (ImGui::Selectable(it->first.c_str(), is_selected)) {
                    this->renderMode = it->first;
                    glPolygonMode(GL_FRONT_AND_BACK, it->second);
                    if (this->renderMode != "Fill")
                        texture.SetTexture(textureList[textureList.size() - 1]);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Model Transformations")) {
        ImGui::SliderFloat3("Scale", this->scale, 0.2f, 5.0f, "%.1f");
        this->modelMatrix[0]  = this->scale[0];
        this->modelMatrix[5]  = this->scale[1];
        this->modelMatrix[10] = this->scale[2];
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
