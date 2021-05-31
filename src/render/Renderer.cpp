#include "render/Renderer.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <entity/EntityDB.h>
#include <component/TextureComponent.h>
#include "stb_image.h"


unsigned int loadCubemap(std::vector<std::string> faces);

namespace se {
    const unsigned int SCR_WIDTH = 1200;
    const unsigned int SCR_HEIGHT = 88;
    CameraComponent camera{glm::vec3(0.0f, 0.0f, 10.0f)};
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;

    void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
//        if (firstMouse) {
//            lastX = xpos;
//            lastY = ypos;
//            firstMouse = false;
//        }
//
//        float xoffset = xpos - lastX;
//        float yoffset = lastY - ypos;
//
//        lastX = xpos;
//        lastY = ypos;
//
//        camera.processMouseMovement(xoffset, yoffset);
    }
}

float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
        5.0f, -0.5f, -5.0f, 2.0f, 2.0f
};

float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};

auto se::Renderer::init(int SCR_WIDTH, int SCR_HEIGHT) -> bool {
    glfwInit();
    //openGL 4.6 version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shirts & Jean", NULL, NULL);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetScrollCallback(window, scroll_callback);
    Input::initInput();
    glfwSetKeyCallback(window, Input::key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }


    staticMeshShader.loadShader("../resource/shaders/4.6.phong.vert", "../resource/shaders/4.6.phong.frag");
    textureShader.loadShader("../resource/shaders/4.6.texture.vert", "../resource/shaders/4.6.texture.frag");
    skyboxShader.loadShader("../resource/shaders/4.6.skybox.vert", "../resource/shaders/4.6.skybox.frag");
    testTexture = loadTexture("../resource/image/pepe_thumsup.png");
    staticModel.loadModel("../resource/model/GrassBlock/Grass_Block.obj");


    glGenVertexArrays(1, &testVAO);
    glGenBuffers(1, &testVBO);
    glBindVertexArray(testVAO);
    glBindBuffer(GL_ARRAY_BUFFER, testVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glBindVertexArray(0);

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    std::vector<std::string> faces
            {
                    "../resource/skybox/skybox/right.jpg",
                    "../resource/skybox/skybox/left.jpg",
                    "../resource/skybox/skybox/top.jpg",
                    "../resource/skybox/skybox/bottom.jpg",
                    "../resource/skybox/skybox/front.jpg",
                    "../resource/skybox/skybox/back.jpg"
            };
    skyboxTexture = loadCubemap(faces);



    return true;
}

auto se::Renderer::draw() -> void {
    processInput(window);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //텍스쳐
    drawTextures();

    //스태틱 메시
    //drawStaticMesh();

    //스카이 박스
    // drawSkybox();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void se::Renderer::drawTextures() {
    this->textureShader.activeShader();
    this->textureShader.setInt("texture1", 0);
    this->textureShader.setMat4("uView", camera.getViewMatrix());
    this->textureShader.setMat4("uProj", camera.getProjectionMatrix());

    for (int i = 0; i < 5; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        glBindVertexArray(this->testVAO);
        glBindTexture(GL_TEXTURE_2D, this->testTexture);
        model = glm::translate(model, glm::vec3(5.0f, 10.0f, -10 * i));
        this->textureShader.setMat4("uModel", glm::rotate(model, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}

void se::Renderer::drawTextures(EntityDB& db) {
    db.addRenderSystem(
            [this]() -> void {
                this->textureShader.activeShader();
                this->textureShader.setInt("texture1", 0);
                this->textureShader.setMat4("uView", camera.getViewMatrix());
                this->textureShader.setMat4("uProj", camera.getProjectionMatrix());
            },
            [this](TransformComponent &trans, TextureComponent &tex) -> void {
                GLuint vbo;

                glGenVertexArrays(1, &tex.vao);
                glGenBuffers(1, &vbo);
                glBindVertexArray(tex.vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
                glBindVertexArray(0);

                glBindVertexArray(tex.vao);
                glBindTexture(GL_TEXTURE_2D, tex.texture);
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, trans.position);
                this->textureShader.setMat4("uModel", glm::rotate(model, glm::radians(90.0f),
                                                                  glm::vec3(-1.0f, 0.0f, 0.0f)));
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
            }
    );
}

void se::Renderer::drawSkybox() {
    glDepthFunc(GL_LEQUAL);
    this->skyboxShader.activeShader();
    this->skyboxShader.setMat4("uView", camera.getViewMatrix());
    this->skyboxShader.setMat4("uProj", camera.getProjectionMatrix());

    glBindVertexArray(this->skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void se::Renderer::drawStaticMesh(EntityDB &edb) {
    // --- example ---

    edb.addRenderSystem(
            [this]() -> void {
                //MVP,계산은 역순
                this->staticMeshShader.activeShader();
                this->staticMeshShader.setMat4("uViewProj", camera.getProjectionMatrix() * camera.getViewMatrix());
                this->staticMeshShader.setFloat("uSpecPower", 128.0f);
                this->setLightUniforms(this->staticMeshShader);
                std::cout << "draw static mesh init \n";
            }, [this](se::TransformComponent &trans, se::StaticModelComponent &model) -> void {
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), trans.position);
                // quaternion 어케 쓰지?
                transform = glm::rotate(transform, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
                transform = glm::scale(transform, trans.scale);
                this->staticMeshShader.setMat4("uWorldTransform", transform);
                model.model->draw(this->staticMeshShader);
                //this->staticModel.draw(this->staticMeshShader);
                //std::cout << "draw static mesh : " << (model.model) << " : " << &(this->staticModel) << "\n";
            }
    );
    // --- end example ---
//
//    //MVP,계산은 역순
//    this->staticMeshShader.activeShader();
//    this->staticMeshShader.setMat4("uViewProj", camera.getProjectionMatrix() * camera.getViewMatrix());
//
//    this->staticMeshShader.setFloat("uSpecPower", 128.0f);
//    this->setLightUniforms(this->staticMeshShader);
//    //TRS,트랜스폼 적용
//    for (auto i = 0; i < 10; i++) {
//        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3((float) i * 3, 0.0f, 0.0f));
//        transform = glm::rotate(transform, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
//        transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));
//        this->staticMeshShader.setMat4("uWorldTransform", transform);
//        this->staticModel.draw(this->staticMeshShader);
//    }
}

auto se::Renderer::setLightUniforms(se::Shader &shader, const sem::Matrix4 &viewMat) -> void {
    sem::Matrix4 view = viewMat;
    view.invert();
    //shader.setVec3("cameraPos", view.getTranslation());
    //shader.setVec3("uAmbientLight", ambientLight);
    //shader.setVec3("uDirLight.mDirection", directionLight.direction);
    //shader.setVec3("uDirLight.mDiffuseColor", directionLight.diffuseColor);
    //shader.setVec3("uDirLight.mSpecColor", directionLight.specularColor);
}

auto se::Renderer::setLightUniforms(se::Shader &shader) -> void {
    shader.setVec3("cameraPos", camera.Position);
    shader.setVec3("uAmbientLight", glm::vec3(0.4f, 0.4f, 0.4f));
    shader.setVec3("uDirLight.direction", glm::vec3(0.7f * sin(glfwGetTime()), 0.7f, 0.7f * cos(glfwGetTime())));
    shader.setVec3("uDirLight.diffuseColor", glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setVec3("uDirLight.specColor", glm::vec3(0.5f, 1.0f, 0.5f));
}

unsigned int se::loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RED;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

auto se::processInput(GLFWwindow *window) -> void {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwTerminate();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(se::CameraMovement::FORWARD, 16ms);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(se::CameraMovement::BACKWARD, 16ms);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(se::CameraMovement::LEFT, 16ms);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(se::CameraMovement::RIGHT, 16ms);

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        camera.processMouseMovement(-20, 0);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        camera.processMouseMovement(20, 0);
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        camera.processMouseMovement(0, 20);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        camera.processMouseMovement(0, -20);
}

unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}




