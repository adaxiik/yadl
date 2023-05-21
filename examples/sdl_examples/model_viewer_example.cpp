#include "sdl_core/sdl_core.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

#include <cmath>

#include <yadl/pixel.hpp>
#include <yadl/canvas.hpp>
#include <yadl/io.hpp>
#include <yadl/depth_buffer.hpp>
#include <yadl/tris.hpp>
#include <yadl/shape.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

class Camera
{
    // heavily inspired by https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
    // c:

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldup;

    float m_yaw;
    float m_pitch;
    const float m_movementSpeed{10};
    const float m_mouseSensitivity{0.1 };
    
    

    void UpdateCameraVectors()
    {
        glm::vec3 front;
        front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
        front.y = std::sin(glm::radians(m_pitch));
        front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);

        m_right = glm::normalize(glm::cross(m_front, m_worldup));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }
public:
    void OnUpdate(float delta)
    {
        if(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_W)))
            m_position += m_front * m_movementSpeed * delta;
        if(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_S)))
            m_position -= m_front * m_movementSpeed * delta;    
        if(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_A)))
            m_position -= m_right * m_movementSpeed * delta;
        if(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_D)))
            m_position += m_right * m_movementSpeed * delta;
        if(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Q)))
            m_position -= m_up * m_movementSpeed * delta;
        if(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_E)))
            m_position += m_up * m_movementSpeed * delta;

        int x, y;
        SDL_GetRelativeMouseState(&x, &y);
        m_yaw += x * m_mouseSensitivity;
        m_pitch -= y * m_mouseSensitivity;

        if (m_pitch > 89.f)
            m_pitch = 89.f;
        if (m_pitch < -89.f)
            m_pitch = -89.f;
        
        UpdateCameraVectors();
        
    }

    Camera(float x, float y, float z)
     : m_position(glm::vec3(x, y, z))
     , m_front(glm::vec3(0, 0, -1))
     , m_up(glm::vec3(0, 1, 0))
     , m_worldup(glm::vec3(0, 1, 0))
     , m_yaw(-90)
     , m_pitch(0)
    {
        UpdateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

};



using namespace yadl;
class ModelViewerExample : public SDLCore
{
private:
    std::string m_model_path{"assets/box.obj"};
    std::string m_texture_path{"assets/box.png"};

    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
    };

    std::vector<Vertex> m_vertices;
    std::vector<int> m_indices;
    Canvas m_texture;
    DepthBuffer m_zbuffer;

    Camera m_camera{0, 0, 5};

    const float fov = glm::radians(60.0f);
    bool m_mouseLocked{false};

    bool LoadModel()
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err
        , m_model_path.c_str()))
            return false;

        m_vertices.clear();
        m_indices.clear();

        for (const auto &shape : shapes)
        {
            for (const auto &index : shape.mesh.indices)
            {
                Vertex vertex = {};

                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]};

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.f - attrib.texcoords[2 * index.texcoord_index + 1]};

                m_vertices.push_back(vertex);
                m_indices.push_back(m_vertices.size() - 1);
            }
        }

        return true;
    }

    virtual void OnStart()
    {
        m_zbuffer = DepthBuffer(m_canvas.GetWidth(), m_canvas.GetHeight());
        m_ctx.SetDepthBuffer(m_zbuffer);
    }

    virtual void OnUpdate(double deltatime)
    {        
        if(m_mouseLocked)
            m_camera.OnUpdate(deltatime);

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        {
            m_mouseLocked = !m_mouseLocked;
            SDL_SetRelativeMouseMode(m_mouseLocked ? SDL_TRUE : SDL_FALSE);
        }
    }

    virtual void OnRender(double deltaTime)
    {
        m_canvas.Clear(Color::Dark);
        m_zbuffer.Clear();

        int32_t width = m_canvas.GetWidth();
        int32_t height = m_canvas.GetHeight();

        glm::mat4 view = m_camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(fov, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
        glm::mat4 mvp = projection * view * glm::mat4(1.0f);

        for (size_t i = 0; i < m_indices.size(); i += 3)
        {
            Vertex &v0 = m_vertices[m_indices[i + 0]];
            Vertex &v1 = m_vertices[m_indices[i + 1]];
            Vertex &v2 = m_vertices[m_indices[i + 2]];

            glm::vec4 projected0 = mvp * glm::vec4(v0.position, 1.0f);
            glm::vec4 projected1 = mvp * glm::vec4(v1.position, 1.0f);
            glm::vec4 projected2 = mvp * glm::vec4(v2.position, 1.0f);

            // if (projected0.w < 0 || projected1.w < 0 || projected2.w < 0) // near plane clipping
            //     continue;


            projected0.x = ((projected0.x / projected0.w) + 1.f) * width / 2.f;
            projected0.y = (2.f - ((projected0.y / projected0.w) + 1.f)) * height / 2.f;
            projected1.x = ((projected1.x / projected1.w) + 1.f) * width / 2.f;
            projected1.y = (2.f - ((projected1.y / projected1.w) + 1.f)) * height / 2.f;
            projected2.x = ((projected2.x / projected2.w) + 1.f) * width / 2.f;
            projected2.y = (2.f - ((projected2.y / projected2.w) + 1.f)) * height / 2.f;


            if (projected0.x < 0 || projected0.x > width || projected0.y < 0 || projected0.y > height ||
                projected1.x < 0 || projected1.x > width || projected1.y < 0 || projected1.y > height ||
                projected2.x < 0 || projected2.x > width || projected2.y < 0 || projected2.y > height)
                continue;

            // projected0.x = std::clamp(projected0.x, 0.f, static_cast<float>(width));
            // projected0.y = std::clamp(projected0.y, 0.f, static_cast<float>(height));
            // projected1.x = std::clamp(projected1.x, 0.f, static_cast<float>(width));
            // projected1.y = std::clamp(projected1.y, 0.f, static_cast<float>(height));
            // projected2.x = std::clamp(projected2.x, 0.f, static_cast<float>(width));
            // projected2.y = std::clamp(projected2.y, 0.f, static_cast<float>(height));


            tris::FloatTriangle screenspace_tris(projected0.x, projected0.y,
                                                projected1.x, projected1.y,
                                                projected2.x, projected2.y);

            tris::FloatTriangle uv_tris(v0.texCoord.x, v0.texCoord.y,
                                        v1.texCoord.x, v1.texCoord.y,
                                        v2.texCoord.x, v2.texCoord.y);

            

            // tris::DrawTriangleZ(m_ctx,screenspace_tris, projected0.z, projected1.z, projected2.z, Color::Red, Color::Green, Color::Blue);
            tris::DrawTexturedTriangleZ(m_ctx, screenspace_tris, projected0.z, projected1.z, projected2.z, uv_tris, m_texture);
        }
    }
    virtual void OnImGuiRender(double deltaTime)
    {
        ImGui::Begin("Model Viewer Example");
        ImGui::Text("FPS: %.2f", 1.0 / deltaTime);

        ImGui::Separator();

        ImGui::InputText("Model Path", &m_model_path);
        ImGui::InputText("Texture Path", &m_texture_path);
        if (ImGui::Button("Load Model"))
        {
            if (LoadModel())
                std::cout << "Model loaded successfully!" << std::endl;
            else
                std::cout << "Failed to load model!" << std::endl;

            auto texture = io::Load(m_texture_path);
            if(!texture.has_value())
            {
                std::cout << "Failed to load texture!" << std::endl;
                m_indices.clear();
                m_vertices.clear();
            }
            else
            {
                m_texture = texture.value();
            }
        }

        ImGui::End();
    }

public:
    ModelViewerExample() : SDLCore(640/2,480/2) {}
    virtual ~ModelViewerExample() = default;
};

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    ModelViewerExample example;
    example.Start();
    return 0;
}
