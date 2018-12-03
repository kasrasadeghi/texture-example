#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Graphics.h"
#include "Graphics/stb_image.h"

#include <iostream>

struct Texture {
  uint id;
  int width;
  int height;

  Texture() {
    // load and create a texture 
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int nrChannels;
    unsigned char *data = stbi_load("textures/textures.png", &width, &height, &nrChannels, 4);

    if (nrChannels == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else if (nrChannels == 4) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    if (data) {
      glGenerateMipmap(GL_TEXTURE_2D);

      std::cout << "nrChannels " << nrChannels << std::endl;
    } else {
      std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
  }
};

struct Instance {
  float x;
  float y;
  float texOffset;
};

int main() {
  RenderWindow window("Texture Example");
  auto& shader = ResourceManager::getShader(SHADER_INDEX::TEXTURE);

  float textureCount = 5;

  float vertices[] = {
    // positions     // colors           // texture coords
    -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,   0.0f,                0.0f, // bottom left
    -0.5f,  0.5f,    1.0f, 1.0f, 0.0f,   0.0f,                1.0f, // top left 
     0.5f, -0.5f,    0.0f, 1.0f, 0.0f,   1.0f / textureCount, 0.0f, // bottom right
     0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f / textureCount, 1.0f, // top right
  };

  auto view = View()
    .center(10, 10)
    .radius(10, 10);

  Instance instances[] = {
    {.x = 1, .y = 1, .texOffset = 0},
    {.x = 5, .y = 1, .texOffset = 1 / textureCount},
    {.x = 8, .y = 1, .texOffset = 2 / textureCount},
    {.x = 3, .y = 1, .texOffset = 1 / textureCount},
    {.x = 5, .y = 7, .texOffset = 3 / textureCount},
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    // texture coord attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
  }

  uint IBO; // instanced vertex buffer object
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ARRAY_BUFFER, IBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(instances), instances, GL_STATIC_DRAW);
  {// instance attributes
    // position offset attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)0);
    glVertexAttribDivisor(3, 1);

    // texture coord attribute
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Instance), (void*)(2 * sizeof(float)));
    glVertexAttribDivisor(4, 1);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  Texture t;

  shader.use();
  shader.setMat4("projection", view.proj());

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