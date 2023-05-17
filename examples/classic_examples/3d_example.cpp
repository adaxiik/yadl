#include <yadl/yadl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texCoord;
};


int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    Vertex vertices[] =
        {
            // front
            {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
            {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
            {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
            // back
            {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
            {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
            {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
            {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            // left
            {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
            {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
            {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
            {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            // right
            {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
            {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
            {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
            {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            // top
            {glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            {glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
            {glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
            {glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
            // bottom
            {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
            {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
            {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
            {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f)}

        };

    // 36 indices for a cube
    std::vector<glm::ivec3> indices =
        {
            // front
            glm::vec3(0, 1, 2),
            glm::vec3(2, 3, 0),
            // back
            glm::vec3(4, 5, 6),
            glm::vec3(6, 7, 4),
            // left
            glm::vec3(8, 9, 10),
            glm::vec3(10, 11, 8),
            // right
            glm::vec3(12, 13, 14),
            glm::vec3(14, 15, 12),
            // top
            glm::vec3(16, 17, 18),
            glm::vec3(18, 19, 16),
            // bottom
            glm::vec3(20, 21, 22),
            glm::vec3(22, 23, 20)};

    const int width = 300;
    const int height = 300;
    // const float scale = 20.0f;

    const float center_x = width / 2.0f;
    const float center_y = height / 2.0f;
    const float fov = glm::radians(60.0f);
    const int frame_count = 300;
    const int frame_delay = 2;

    using namespace yadl;

    Canvas canvas(width, height);
    Canvas texture("assets/box.png");
    DepthBuffer zBuffer(width, height);

    canvas.Clear(Color::Dark);
    zBuffer.Clear();

    Context ctx(canvas, zBuffer);
    Animation animation(canvas.GetWidth(), canvas.GetHeight());

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(fov, (float)width / (float)height, 0.1f, 100.0f);
    // glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    for (int i = 0; i < frame_count; i++)
    {
        canvas.Clear(Color::Dark);
        zBuffer.Clear();
        float angle = i * 2.0f * glm::pi<float>() / frame_count;

        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f)); // x
        rotation = glm::rotate(rotation, angle, glm::vec3(0.0f, 1.0f, 0.0f));                  // y
        rotation = glm::rotate(rotation, angle, glm::vec3(0.0f, 0.0f, 1.0f));                  // z

        
        glm::mat4 model = rotation;
        glm::mat4 mvp = projection * view * model;

        for (const auto &triangle : indices)
        {
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

            // tris::DrawTriangleZ(ctx, screenspace_tris, z0, z1, z2, Color::Red, Color::Green, Color::Blue);
            tris::DrawTexturedTriangleZ(ctx, screenspace_tris, z0, z1, z2, uv_tris, texture);
        }
        std::cout << "Rendering frame " << i + 1 << "/" << frame_count << std::endl;
        animation.AddFrame(canvas);
    }
    std::cout << "Saving animation..." << std::endl;
    io::SaveAsGIF("3d_example.gif", animation, frame_delay);

    return 0;
}
