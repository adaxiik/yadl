#include <yadl/yadl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include "3rdparty/tiny_obj_loader.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
};


bool LoadModel(const std::string& path, std::vector<Vertex>& vertices, std::vector<int>& indices)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str()))
    {
        return false;
    }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex = {};

            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertices.push_back(vertex);
            indices.push_back(vertices.size() - 1);
        }
    }

    return true;
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <model path>" << std::endl;
        return 1;
    }

    std::vector<Vertex> vertices;
    std::vector<int> indices;

    if (!LoadModel(argv[1], vertices, indices))
    {
        std::cout << "Failed to load model" << std::endl;
        return 1;
    }

    std::cout << "Loaded model with " << vertices.size() << " vertices" << std::endl;

    const int width = 600;
    const int height = 600;
    // const float scale = 20.0f;

    const float center_x = width / 2.0f;
    const float center_y = height / 2.0f;
    const float fov = glm::radians(60.0f);
    const int frame_count = 150;
    const int frame_delay = 4;

    using namespace yadl;

    Canvas canvas(width, height);
    Canvas texture("assets/wetfloor.jpg");
    DepthBuffer zBuffer(width, height);

    canvas.Clear(Color::Dark);
    zBuffer.Clear();

    Context ctx(canvas, zBuffer);
    Animation animation(canvas.GetWidth(), canvas.GetHeight());

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, -1.1f, 1.1f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(fov, (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.f));
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.3f, 0.0f));
    glm::mat4 flip = glm::scale(glm::mat4(1.0f), glm::vec3(1.f, -1.f, 1.f));


    for (int i = 0; i < frame_count; i++)
    {
        canvas.Clear(Color::Dark);
        zBuffer.Clear();
        float angle = i * 2.0f * glm::pi<float>() / frame_count;

        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)); // x
        
        glm::mat4 model = scale * flip * rotation * translation;
        glm::mat4 mvp = projection * view * model;

        for (size_t j = 0; j < indices.size(); j += 3)
        {
            int triangle[3] = { indices[j], indices[j + 1], indices[j + 2] };
            glm::vec3 transformed_vertices[3];
            // we project the vertices to the screen space
            for (int i = 0; i < 3; i++)
            {
                glm::vec3 v = vertices[triangle[i]].position;
                glm::vec4 projected = mvp * glm::vec4(v, 1.0f);

                projected.x /= projected.w;
                projected.y /= projected.w;
                projected.x = (projected.x + 1.0f) * center_x;
                projected.y = (projected.y + 1.0f) * center_y;

                transformed_vertices[i] = glm::vec3(projected);
            }

            tris::FloatTriangle screenspace_tris(transformed_vertices[0].x, transformed_vertices[0].y,
                                                 transformed_vertices[1].x, transformed_vertices[1].y,
                                                 transformed_vertices[2].x, transformed_vertices[2].y);

            tris::FloatTriangle uv_tris(vertices[triangle[0]].texCoord.x, vertices[triangle[0]].texCoord.y,
                                        vertices[triangle[1]].texCoord.x, vertices[triangle[1]].texCoord.y,
                                        vertices[triangle[2]].texCoord.x, vertices[triangle[2]].texCoord.y);

            float z0 = transformed_vertices[0].z;
            float z1 = transformed_vertices[1].z;
            float z2 = transformed_vertices[2].z;

            tris::DrawTexturedTriangleZ(ctx, screenspace_tris, z0, z1, z2, uv_tris, texture);
            // tris::DrawTriangleZ(ctx, screenspace_tris, z0, z1, z2, Color::Red, Color::Green, Color::Blue);

        }
        std::cout << "Rendering frame " << i + 1 << "/" << frame_count << std::endl;
        animation.AddFrame(canvas);
    }

    io::SaveAsGIF("wetfloor.gif", animation, frame_delay);



    return 0;
}
