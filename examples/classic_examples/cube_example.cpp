#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <yadl/canvas.hpp>
#include <yadl/context.hpp>
#include <yadl/io.hpp>
#include <yadl/shape.hpp>
#include <yadl/animation.hpp>

int main(int argc, char const *argv[])
{
    using namespace yadl;
    (void)argc;
    (void)argv;

    glm::vec3 vertices[] = {
        glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f, 0.5f, 0.5f)};

    glm::ivec2 edges[] = {
        glm::ivec2(0, 1),
        glm::ivec2(1, 2),
        glm::ivec2(2, 3),
        glm::ivec2(3, 0),
        glm::ivec2(4, 5),
        glm::ivec2(5, 6),
        glm::ivec2(6, 7),
        glm::ivec2(7, 4),
        glm::ivec2(0, 4),
        glm::ivec2(1, 5),
        glm::ivec2(2, 6),
        glm::ivec2(3, 7)};

    const int width = 300;
    const int height = 300;

    const int line_width = std::min(width, height) / 300;
    const int circle_radius = std::min(width, height) / 100;

    const float scale = 800.0f;
    const float center_x = width / 2.0f;
    const float center_y = height / 2.0f;
    const float fov = 60.0f;
    const int frame_count = 300;
    const int frame_delay = 2;

    Canvas canvas(width, height);
    Animation animation(canvas.GetWidth(), canvas.GetHeight());
    canvas.Clear(Color::Dark);
    Context ctx(canvas);

    for (int i = 0; i < frame_count; i++)
    {
        float angle = i * 2.0f * glm::pi<float>() / frame_count;

        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f)); // x
        rotation = glm::rotate(rotation, angle, glm::vec3(0.0f, 1.0f, 0.0f));                  // y
        rotation = glm::rotate(rotation, angle, glm::vec3(0.0f, 0.0f, 1.0f));                  // z

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
        glm::mat4 model = translation * rotation;
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(fov, (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 mvp = projection * view * model;

        canvas.Clear(Color::Dark);

        ctx.SetColor(Color::White);
        for (int i = 0; i < 12; ++i)
        {
            glm::vec4 v1 = mvp * glm::vec4(vertices[edges[i].x], 1.0f);
            glm::vec4 v2 = mvp * glm::vec4(vertices[edges[i].y], 1.0f);
            ctx.SetPosition(v1.x * scale + center_x, v1.y * scale + center_y).SetThickness(line_width);
            shape::DrawLine(ctx, v2.x * scale + center_x, v2.y * scale + center_y);
        }

        ctx.SetColor(Color::Red);
        for (int i = 0; i < 8; ++i)
        {
            glm::vec4 v = mvp * glm::vec4(vertices[i], 1.0f);
            ctx.SetPosition(v.x * scale + center_x, v.y * scale + center_y);
            shape::DrawFilledCircle(ctx, circle_radius);
        }

        animation.AddFrame(canvas);
    }

    io::SaveAsGIF("cube.gif", animation, frame_delay);

    return 0;
}
