#pragma once

#include "shader.h"

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "grid.h"

#include <memory>
#include <vector>

// yoinked/inspired from https://learnopengl.com/In-Practice/2D-Game/Rendering-Sprites
class CellRenderer {
private:
    std::shared_ptr<Shader> shader;
    GLuint quadVAO{};

    void initRenderData();
public:
    // Take a shader reference and make a shared_ptr from it
    CellRenderer(std::shared_ptr<Shader> in_shader) : shader{in_shader} { initRenderData(); }

    void DrawCell(const glm::vec2& pos, const glm::vec2& size = glm::vec2(10.0f, 10.0f),
        const glm::vec3& color = glm::vec3(0.0f, 0.0f, 0.0f));
    
    std::shared_ptr<Shader> getShader() { return shader; }
};

class GridRenderer {
private:
    using Coord = std::pair<int, int>;
    using CoordVec = std::vector<Coord>;

    Grid grid_bog;
public:
    GridRenderer(const CoordVec& in_vecs) : grid_bog{ in_vecs } {};
    
    void renderGrid(CellRenderer& renderer);
};
