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

// TODO: Finish this class
class GridLineRenderer {
private:
    std::shared_ptr<Shader> shader;
    GLuint vao;

    void initRenderData();
public:
    GridLineRenderer(std::shared_ptr<Shader> in_shader) : shader{ in_shader } { initRenderData(); }

    void DrawLine(const glm::vec2& begin, const glm::vec2& end, const glm::vec3& color);
};

class GridRenderer {
private:
    using Coord = std::pair<int, int>;
    using CoordVec = std::vector<Coord>;

    // cell size in pixels when drawn
    float cell_size;
public:
    // Default cell_size is 20
    GridRenderer(float cell_size = 20.0f) : cell_size{cell_size} {};
    
    void renderGrid(Grid &grid_to_render, CellRenderer &renderer, bool update_on_render = true);

    float getCellSize() { return cell_size; }
    void setCellSize(float in_size) { cell_size = cell_size; }

    // Converts from 
    Coord getScreenCoord(const Coord &grid_coord) { return {grid_coord.first * cell_size, grid_coord.second * cell_size}; }
    // Converts from world space to grid coordinates
    Coord getGridCoord(const Coord &screen_coord) { return  {screen_coord.first / cell_size, screen_coord.second / cell_size}; }
};
