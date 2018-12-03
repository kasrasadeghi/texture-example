#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include "Graphics.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/ResourceManager.h"

#include "Graphics/Texture.h"

#include <iostream>

#include <glm/common.hpp>
// #include <glm/vec3.hpp>

struct TextureBatch {

};

struct Instance {
  glm::vec2 pos;
  glm::vec2 size;
  glm::vec4 aColor;
  float texOffset = 0;
  float rotation = 0;
};

int main() {
  RenderWindow window("Texture Example");
  auto& shader = ResourceManager::getShader(SHADER_INDEX::TEXTURE);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  float textureCount = 5;

  float vertices[] = {
    // positions     // texture coords
    -0.5f, -0.5f,    0.0f,                0.0f, // bottom left
    -0.5f,  0.5f,    0.0f,                1.0f, // top left 
     0.5f, -0.5f,    1.0f / textureCount, 0.0f, // bottom right
     0.5f,  0.5f,    1.0f / textureCount, 1.0f, // top right
  };

  auto view = View()
    .center(10, 10)
    .radius(10, 10);

  Instance instances[] = {
    {.pos = {10, 10}, .size = {1, 1}, .aColor = {1, 1, 1, 1}}
  };
  
  uint VAO; // vertex array object
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  uint VBO; // vertex buffer object
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  {// global attributes
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // texture coord attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
  }

  uint IBO; // instanced vertex buffer object
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ARRAY_BUFFER, IBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(instances), instances, GL_STATIC_DRAW);
  {// instance attributes

    int currentAttr = 2;
    // posOffset
    glEnableVertexAttribArray(currentAttr);
    glVertexAttribPointer(currentAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)0);
    glVertexAttribDivisor(currentAttr, 1);
    currentAttr++;

    // size
    glEnableVertexAttribArray(currentAttr);
    glVertexAttribPointer(currentAttr, 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)(2 * sizeof(float)));
    glVertexAttribDivisor(currentAttr, 1);
    currentAttr++;

    // aColor
    glEnableVertexAttribArray(currentAttr);
    glVertexAttribPointer(currentAttr, 4, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)(4 * sizeof(float)));
    glVertexAttribDivisor(currentAttr, 1);
    currentAttr++;

    // texOffset
    glEnableVertexAttribArray(currentAttr);
    glVertexAttribPointer(currentAttr, 1, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)(8 * sizeof(float)));
    glVertexAttribDivisor(currentAttr, 1);
    currentAttr++;

    // rotation
    glEnableVertexAttribArray(currentAttr);
    glVertexAttribPointer(currentAttr, 1, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)(9 * sizeof(float)));
    glVertexAttribDivisor(currentAttr, 1);
    currentAttr++;
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  Texture t;

  shader.use();
  shader.setMat4("projection", view.proj());

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  while (window.isOpen()) {
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, t.id);

    // render container
    shader.use();
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, sizeof(instances) / sizeof(Instance));

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    window.swapBuffers();
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}
