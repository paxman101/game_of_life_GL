#include "renderer.h"
#include "util.h"

#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"

// copy paste of learnopengl tutorial
void CellRenderer::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 
        1.0f, 0.0f, 
        0.0f, 0.0f, 

        0.0f, 1.0f, 
        1.0f, 1.0f, 
        1.0f, 0.0f, 
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CellRenderer::DrawCell(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec3 &color) {
    // prepare transformations
    shader->use();
    glm::mat4 model{ 1.0f };
    model = glm::translate(model, glm::vec3(pos,0));
    
    model = glm::scale(model, glm::vec3(size,1));

    shader->setMatrix4("model", model);
    shader->setVector3f("cell_color", color);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void GridLineRenderer::initRenderData() {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos  
        0.0f, 1.0f
    };

    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GridRenderer::renderGrid(Grid &grid_to_render, CellRenderer &renderer, bool update_on_render) {
    for (const Coord& coord : grid_to_render.alive_cells) {
        renderer.DrawCell(coordToVec(getScreenCoord(coord)), {cell_size, cell_size}, {1.0f, 1.0f, 1.0f});
    }
    if (update_on_render) {
        grid_to_render.update();
    }
}
