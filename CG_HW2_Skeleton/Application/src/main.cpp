// STL
#include <iostream>
#include <math.h>
#include <stdlib.h>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// include Engine
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <RenderObject.hpp>

// include application headers
#include <DefaultMaterial.hpp>
#include <PickingMaterial.hpp>
#include <LineMaterial.hpp>
#include <Geometry.hpp>
#include <Animation.hpp>
#include <picking.hpp>
#include <PickableObject.hpp>
#include <Snowman.hpp>
#include <PerVertexColorMaterial.hpp>
#include <Fractal.hpp>
#include <Snowflake.hpp>
#include <Cone.hpp>
#include <Sphere.hpp>
#include <Cylinder.hpp>
#include <BlenderObject.hpp>

#define _USE_MATH_DEFINES

Animation *animation = new Animation();
std::vector<Snowflake *> snowflakes;
std::vector<Engine::RenderObject> stars;
std::vector<Engine::RenderObject> mountains;
std::vector<Engine::RenderObject> clouds;
int fractalLevel = 3;
Snowman snowman = Snowman();

GLFWwindow *g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;

bool z_rotation = false;
bool body_rotation = false;
bool rotation_on = false;

/*
 Generate background mesh. A rectangle that fills the window.
 Parameters: mesh -> mesh to add background vertices to
 */
void GenerateGradientRectangle(Engine::Mesh *mesh) {
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);
    float size = 14.0f;
    glm::vec3 position[] = {
            glm::vec3(-size, -size, -10.0f),
            glm::vec3(size, -size, -10.0f),
            glm::vec3(size, size, -10.0f),
            glm::vec3(-size, size, -10.0f),
            glm::vec3(-size, -size, -10.0f),
            glm::vec3(size, size, -10.0f)
    };
    glm::vec4 colors[] = {
            glm::vec4(0.1f, 0.1f, 0.2f, 1.0f),
            glm::vec4(0.4f, 0.1f, 0.1f, 1.0f),
            glm::vec4(0.0f, 0.2f, 0.4f, 1.0f),
            glm::vec4(0.1f, 0.2f, 0.3f, 1.0f),
            glm::vec4(0.1f, 0.1f, 0.2f, 1.0f),
            glm::vec4(0.0f, 0.2f, 0.4f, 1.0f)
    };
    for (int i = 0; i < 6; i++) {
        mesh->AddVertexData(position[i]);
        mesh->AddVertexData(1.0f);
        mesh->AddVertexData(colors[i]);
    }
    mesh->SetNumElements(6);
    mesh->CreateMesh();
}

/*
 Generate ground mesh that lies at the bottom.
 Parameters: mesh -> mesh to add ground vertices to
 */
void GenerateGround(Engine::Mesh *mesh) {
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);
    float size = 14.0f;
    glm::vec3 position[] = {
            glm::vec3(-size, -size, 0),
            glm::vec3(size, -size, 0),
            glm::vec3(size, size, 0),
            glm::vec3(-size, size, 0),
            glm::vec3(-size, -size, 0),
            glm::vec3(size, size, 0)
    };
    glm::vec4 colors[] = {
            glm::vec4(0.6f, 0.9f, 0.7f, 1.0f),
            glm::vec4(0.6f, 0.1f, 0.9f, 1.0f),
            glm::vec4(0.8f, 0.8f, 0.6f, 1.0f),
            glm::vec4(0.7f, 0.7f, 0.9f, 1.0f),
            glm::vec4(0.6f, 0.9f, 0.7f, 1.0f),
            glm::vec4(0.8f, 0.8f, 0.6f, 1.0f)
    };
    for (int i = 0; i < 6; i++) {
        mesh->AddVertexData(position[i]);
        mesh->AddVertexData(1.0f);
        mesh->AddVertexData(colors[i]);
    }
    mesh->SetNumElements(6);
    mesh->CreateMesh();
}

/*
 Generate star mesh. Use randomly generated color.
 Parameters: mesh -> mesh to add star vertices to
 */
void GenerateStar(Engine::Mesh *mesh) {
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);
    glm::vec3 position[] = {
            glm::vec3(0.1f, -0.1f, 0.0f),
            glm::vec3(0.1f, 0.1f, 0.0f),
            glm::vec3(-0.1f, 0.1f, 0.0f),

            glm::vec3(0.1f, -0.1f, 0.0f),
            glm::vec3(-0.1f, 0.1f, 0.0f),
            glm::vec3(-0.1f, -0.1f, 0.0f),

            glm::vec3(0.1f, 0.1f, 0.0f),
            glm::vec3(0.0f, 0.9f, 0.0f),
            glm::vec3(-0.1f, 0.1f, 0.0f),

            glm::vec3(-0.1f, 0.1f, 0.0f),
            glm::vec3(-0.9f, 0.0f, 0.0f),
            glm::vec3(-0.1f, -0.1f, 0.0f),

            glm::vec3(-0.1f, -0.1f, 0.0f),
            glm::vec3(0.0f, -0.9f, 0.0f),
            glm::vec3(0.1f, -0.1f, 0.0f),

            glm::vec3(0.1f, -0.1f, 0.0f),
            glm::vec3(0.9f, 0.0f, 0.0f),
            glm::vec3(0.1f, 0.1f, 0.0f)
    };
    float r = (rand() % 10) / 10.0f;
    float g = (rand() % 10) / 10.0f;
    float b = (rand() % 10) / 10.0f;
    glm::vec4 color = glm::vec4(r, g, b, 10.0f);
    glm::vec4 colors[] = {
            color,
            color,
            color,

            color,
            color,
            color,

            color,
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            color,

            color,
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            color,

            color,
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            color,

            color,
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
            color
    };
    for (int i = 0; i < 18; i++) {
        mesh->AddVertexData(position[i]);
        mesh->AddVertexData(1.0f);
        mesh->AddVertexData(colors[i]);
    }
    mesh->SetNumElements(18);
    mesh->CreateMesh();
}

/*
 Generate moon mesh
 Parameters: mesh -> mesh to add moon vertices to
 */
void GenerateMoon(Engine::Mesh *mesh) {
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);
    glm::vec3 position[] = {
            glm::vec3(0.4f, 0.0f, 0.0f),
            glm::vec3(0.1f, 0.3f, 0.0f),
            glm::vec3(-0.1f, 0.1f, 0.0f),
            glm::vec3(0.1f, 0.3f, 0.0f),
            glm::vec3(-0.4f, 0.4f, 0.0f),
            glm::vec3(-0.1f, 0.1f, 0.0f),
            glm::vec3(0.0f, 0.6f, 0.0f),
            glm::vec3(-0.4f, 0.4f, 0.0f),
            glm::vec3(0.1f, 0.3f, 0.0f),
            glm::vec3(0.0f, 0.6f, 0.0f),
            glm::vec3(-0.5f, 0.8f, 0.0f),
            glm::vec3(-0.4f, 0.4f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(-0.5f, 0.8f, 0.0f),
            glm::vec3(0.0f, 0.6f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(-0.4f, 1.2f, 0.0f),
            glm::vec3(-0.5f, 0.8f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(-0.1f, 1.5f, 0.0f),
            glm::vec3(-0.4f, 1.2f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.1f, 1.3f, 0.0f),
            glm::vec3(-0.4f, 1.2f, 0.0f),
            glm::vec3(0.1f, 1.3f, 0.0f),
            glm::vec3(-0.1f, 1.5f, 0.0f),
            glm::vec3(-0.4f, 1.2f, 0.0f),
            glm::vec3(-0.1f, 1.5f, 0.0f),
            glm::vec3(0.1f, 1.3f, 0.0f),
            glm::vec3(0.4f, 1.6f, 0.0f)
    };
    glm::vec4 colors[] = {
            glm::vec4(1.0f, 1.0f, 0.1f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.5f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.3f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.1f, 1.0f),
    };
    for (int i = 0; i < 30; i++) {
        mesh->AddVertexData(position[i]);
        mesh->AddVertexData(1.0f);
        mesh->AddVertexData(colors[i]);
    }
    mesh->SetNumElements(30);
    mesh->CreateMesh();
}

/*
 Add snowflakes to the world.
 Parameters: count -> number of snowflakes to create
 */
void AddNewSnowflakes(int count) {
    auto *colorMaterial = new PerVertexColorMaterial();
    colorMaterial->CreateMaterial();

    for (int i = 0; i < count; i++) {
        Fractal fractal = Fractal();
        auto *mesh3 = new Engine::Mesh();
        fractal.GenerateSnowflake(mesh3, fractalLevel);
        auto *snowflake = new Snowflake(mesh3, colorMaterial);
        snowflakes.push_back(snowflake);
    }
    for (int i = snowflakes.size() - count; i < snowflakes.size(); i++) {
        animation->AddSnowflake(snowflakes.at(i));
    }
}

/*
 Add stars to the world.
 Parameters: count -> number of stars to create
 */
void AddStars(int count) {
    PerVertexColorMaterial *colorMaterial = new PerVertexColorMaterial();
    colorMaterial->CreateMaterial();

    for (int i = 0; i < count; i++) {
        Engine::Mesh *mesh3 = new Engine::Mesh();
        GenerateStar(mesh3);
        stars.push_back(Engine::RenderObject(mesh3, colorMaterial));
    }
    for (auto &star:stars) {
        star.SetScale(glm::vec3(0.3, 0.3, 0.3));
        float angle = rand() % 90;
        star.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 0, 1)));
        float xOffset = ((rand() - (RAND_MAX / 2)) % 70) / 10.0f;
        float yOffset = ((rand() - (RAND_MAX / 2)) % 15) / 10.0f;
        star.SetPosition(glm::vec3(xOffset, 3 + yOffset, -1.1));
    }
}

/*
 Add mountains to the world.
 Parameters: count -> number of mountains to create
 */
void AddMountains(int count) {
    auto *mountainMaterial = new DefaultMaterial();
    mountainMaterial->CreateMaterial();
    mountainMaterial->UpdateColor(glm::vec4(0.6, 0.8, 0.7, 1.0));

    for (int i = 0; i < count; i++) {
        auto *mesh3 = new Engine::Mesh();
        float radiusOffset = ((rand() - (RAND_MAX / 2)) % 50) / 100.0f;
        float heightOffset = ((rand() - (RAND_MAX / 2)) % 100) / 100.0f;
        mountains.push_back(Cone(mesh3, mountainMaterial, 4.0 + radiusOffset, 3.0 + heightOffset));
    }
    for (auto &mountain:mountains) {
        float xOffset = ((rand() - (RAND_MAX / 2)) % 90) / 10.0f;
        float yOffset = ((rand() - (RAND_MAX / 2)) % 20) / 100.0f;
        float zOffset = ((rand() - (RAND_MAX / 2)) % 20) / 100.0f;
        mountain.SetPosition(glm::vec3(xOffset, -2.1 + yOffset, -5 + zOffset));
    }
}

/*
 Add clouds to the world.
 Parameters: count -> number of clouds to create
 */
void AddClouds(int count) {
    auto *cloudMaterial = new DefaultMaterial();
    cloudMaterial->CreateMaterial();
    cloudMaterial->UpdateColor(glm::vec4(0.9, 0.9, 0.9, 0.1));

    for (int i = 0; i < count; i++) {
        auto *mesh3 = new Engine::Mesh();
        clouds.push_back(BlenderObject(mesh3, cloudMaterial, "cloud.obj"));
    }
    for (auto &cloud:clouds) {
        float scaleOffset = ((rand() - (RAND_MAX / 2)) % 20) / 100.0f;
        cloud.SetScale(glm::vec3(0.4 + scaleOffset));
        float xOffset = ((rand() - (RAND_MAX / 2)) % 70) / 10.0f;
        float yOffset = ((rand() - (RAND_MAX / 2)) % 15) / 10.0f;
        float zOffset = ((rand() - (RAND_MAX / 2)) % 20) / 100.0f;
        cloud.SetPosition(glm::vec3(xOffset, 3 + yOffset, -1 + zOffset));
    }
}

/*
 Callback function for keyboard input
 Parameters:
 key -> the key pressed
 action -> only GLFW_PRESS action interested
 */
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_A:
                AddNewSnowflakes(2);
                printf("pressed A\n");
                break;
            case GLFW_KEY_W:
                fractalLevel++;
                break;
            case GLFW_KEY_X:
                if (fractalLevel != 0) {
                    fractalLevel--;
                }
                break;
            case GLFW_KEY_Z:
                z_rotation = !z_rotation;
                break;
            case GLFW_KEY_C:
                animation->ClearSnowflake();
            case GLFW_KEY_H:
                std::cout << "CS380 Homework 2" << std::endl;
                std::cout << "keymaps:" << std::endl;
                std::cout << "h\t\t Help command" << std::endl;
                break;
            case GLFW_KEY_Q:
                rotation_on = !rotation_on;
                break;
            default:
                break;
        }
    }
}

// TODO: Fill up GLFW mouse button callback function
static void MouseButtonCallback(GLFWwindow *a_window, int a_button, int a_action, int a_mods) {
    //example code for get x position and y position of mouse click
    if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(a_window, &xpos, &ypos);
        xpos = xpos / ((double) g_window_width) * ((double) g_framebuffer_width);
        ypos = ypos / ((double) g_window_height) * ((double) g_framebuffer_height);
        int target = pick((int) xpos, (int) ypos, g_framebuffer_width, g_framebuffer_height);
        std::cout << "Picked object index: " << target << std::endl;
    }else if (a_button == GLFW_MOUSE_BUTTON_RIGHT && a_action == GLFW_PRESS) {
        body_rotation = !body_rotation;
    }
}

// TODO: Fill up GLFW cursor position callback function
static void CursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    if (!rotation_on) {
        return;
    }
    float mouseSpeed = 0.3f;
    double deltaX = 1024.0 / 2 - xpos;
    double deltaY = 768.0 / 2 - ypos;
    if (z_rotation) {
        auto angle = (float) atan(deltaY / deltaX);
        if (body_rotation) {
            snowman.body->SetOrientation(glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0, 0.0, 1.0)));
        } else {
            snowman.head->SetOrientation(glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0, 0.0, 1.0)));
        }
    } else {
        float angle = mouseSpeed * (float) sqrt(deltaX * deltaX + deltaY * deltaY);
        glm::vec3 axis = glm::vec3(-deltaY, -deltaX, 0.0f);
        if (body_rotation) {
            snowman.body->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis));
        } else if (angle < 100) {
            snowman.head->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis));
        }
    }
}

int main(int argc, char **argv) {
    // Initialize GLFW library
    if (!glfwInit()) {
        return -1;
    }

    // Create window and OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    g_window = glfwCreateWindow(g_window_width, g_window_height, "HW2 - Do you want to build a snowman?", NULL, NULL);
    if (!g_window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(g_window);
    glfwGetFramebufferSize(g_window, &g_framebuffer_width, &g_framebuffer_height);

    // Initialize GLEW library
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        std::cout << "GLEW Error: " << glewGetErrorString(glew_error) << std::endl;
        exit(1);
    }

    srand(time(0));
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
    glfwSetCursorPosCallback(g_window, CursorPosCallback);
    glfwSetKeyCallback(g_window, KeyCallback);

    // Initialize framebuffer object and picking textures
    pickingInitialize(g_framebuffer_width, g_framebuffer_width);

    snowflakes = std::vector<Snowflake *>();
    stars = std::vector<Engine::RenderObject>();
    clouds = std::vector<Engine::RenderObject>();
    mountains = std::vector<Engine::RenderObject>();
    auto *mainCamera = new Engine::Camera();
    mainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 7.5f));

    auto *mesh = new Engine::Mesh();
    auto *mesh1 = new Engine::Mesh();
    auto *mesh2 = new Engine::Mesh();
    auto *mesh3 = new Engine::Mesh();

    auto *coneMesh = new Engine::Mesh();

    GenerateGradientRectangle(mesh);
    GenerateGround(mesh2);
    GenerateMoon(mesh3);

    AddNewSnowflakes(10);
    AddStars(20);
    AddClouds(4);
    AddMountains(15);


    auto *colorMaterial = new PerVertexColorMaterial();
    colorMaterial->CreateMaterial();

    //Create objects
    Engine::RenderObject backgroundObject = Engine::RenderObject(mesh, colorMaterial);
    Engine::RenderObject groundObject = Engine::RenderObject(mesh2, colorMaterial);
    Engine::RenderObject moon = Engine::RenderObject(mesh3, colorMaterial);
    groundObject.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1, 0, 0)));
    groundObject.SetPosition(glm::vec3(0, -3.5, 0));
    moon.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-20.0f), glm::vec3(0, 0, 1)));
    moon.SetPosition(glm::vec3(-5, 2, -2));

    //make snowman meshes
    auto *bodyMesh = new Engine::Mesh();
    auto *headMesh = new Engine::Mesh();
    auto *leftArmMesh = new Engine::Mesh();
    auto *rightArmMesh = new Engine::Mesh();
    auto *leftLegMesh = new Engine::Mesh();
    auto *rightLegMesh = new Engine::Mesh();
    auto *leftEyeMesh = new Engine::Mesh();
    auto *rightEyeMesh = new Engine::Mesh();
    auto *noseMesh = new Engine::Mesh();
    auto *mouthMesh = new Engine::Mesh();
    auto *hatMesh = new Engine::Mesh();
    auto *caneMesh = new Engine::Mesh();
    auto *anchorMesh = new Engine::Mesh();
    auto *tieMesh = new Engine::Mesh();

    auto *anchorMaterial = new DefaultMaterial();
    anchorMaterial->CreateMaterial();
    anchorMaterial->UpdateColor(glm::vec4(0.1, 0.9, 0.1, 1.0));

    auto *snowMaterial = new DefaultMaterial();
    snowMaterial->CreateMaterial();
    snowMaterial->UpdateColor(glm::vec4(0.9, 0.9, 0.9, 1.0));

    auto *woodMaterial = new DefaultMaterial();
    woodMaterial->CreateMaterial();
    woodMaterial->UpdateColor(glm::vec4(0.5, 0.2, 0.1, 1.0));

    auto *eyeMaterial = new DefaultMaterial();
    eyeMaterial->CreateMaterial();
    eyeMaterial->UpdateColor(glm::vec4(0.7, 0.7, 0.7, 1.0));

    auto *noseMaterial = new DefaultMaterial();
    noseMaterial->CreateMaterial();
    noseMaterial->UpdateColor(glm::vec4(0.8, 0.2, 0.0, 1.0));

    auto *mouthMaterial = new DefaultMaterial();
    mouthMaterial->CreateMaterial();
    mouthMaterial->UpdateColor(glm::vec4(0.7, 0.1, 0.1, 1.0));

    auto *hatMaterial = new DefaultMaterial();
    hatMaterial->CreateMaterial();
    hatMaterial->UpdateColor(glm::vec4(0.1, 0.1, 0.1, 1.0));

    auto *caneMaterial = new DefaultMaterial();
    caneMaterial->CreateMaterial();
    caneMaterial->UpdateColor(glm::vec4(0.7, 0.1, 0.1, 1.0));

    auto *tieMaterial = new DefaultMaterial();
    tieMaterial->CreateMaterial();
    tieMaterial->UpdateColor(glm::vec4(0.9, 0.4, 0.4, 1.0));

    Sphere b = Sphere(bodyMesh, snowMaterial, 1.6f);
    Sphere h = Sphere(headMesh, snowMaterial, 1.0f);
    BlenderObject la = BlenderObject(leftArmMesh, woodMaterial, "arm.obj");
    BlenderObject ra = BlenderObject(rightArmMesh, woodMaterial, "arm.obj");
    Cylinder ll = Cylinder(leftLegMesh, woodMaterial, 0.2f, 1.0f);
    Cylinder rl = Cylinder(rightLegMesh, woodMaterial, 0.2f, 1.0f);
    Sphere le = Sphere(leftEyeMesh, eyeMaterial, 0.1f);
    Sphere re = Sphere(rightEyeMesh, eyeMaterial, 0.1f);
    Cone nose = Cone(noseMesh, noseMaterial, 0.1f, 0.2f);
    Cylinder mouth = Cylinder(mouthMesh, mouthMaterial, 0.1f, 0.5f);
    BlenderObject hat = BlenderObject(hatMesh, hatMaterial, "hat.obj");
    BlenderObject cane = BlenderObject(caneMesh, caneMaterial, "cane.obj");
    Sphere anchor = Sphere(anchorMesh, anchorMaterial, 0.1f);
    BlenderObject tie = BlenderObject(tieMesh, tieMaterial, "bowtie.obj");
    snowman.ConstructSnowman(h, b);
    snowman.AddArms(la, ra);
    snowman.AddFace(le, re, nose, mouth);
    snowman.AddHat(hat);
    snowman.AddCane(cane, anchor);
    snowman.AddBowTie(tie);

    double tempTime = 0;
    double snowflakeTime = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - tempTime;
        double snowflakeElapsedTime = currentTime - snowflakeTime;

        if (snowflakeElapsedTime > 3) {
            AddNewSnowflakes(2);
            snowflakeTime = currentTime;
        }

        // First Pass: Object Selection (Slide No. 20)
        // this is for picking the object using mouse interaction
        // binding framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);
        // Background: RGB = 0x000000 => objectID: 0
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render your objects that you want to select using mouse interaction here
        snowman.RenderPicking(mainCamera);

        // Second Pass: Object Rendering (Slide No. 11)
        // Drawing object again
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply constant animation based on elapsed time since the last drawing
        animation->Animate(elapsedTime, *mainCamera);

        // Render all stars, clouds and mountains
        for (auto &star:stars) {
            star.Render(mainCamera);
        }
        for (auto &cloud:clouds) {
            cloud.Render(mainCamera);
        }
        for (auto &mountain:mountains) {
            mountain.Render(mainCamera);
        }

        //Render other objects
        backgroundObject.Render(mainCamera);
        moon.Render(mainCamera);
        groundObject.Render(mainCamera);
        snowman.WaveHand(elapsedTime);
        snowman.SwingCane(elapsedTime);
        snowman.Render(mainCamera);

        /* Swap front and back buffers */
        glfwSwapBuffers(g_window);

        /* Poll for and process events */
        glfwPollEvents();
        tempTime = currentTime;
    }

    // Delete resources
    delete mainCamera;

    glfwTerminate();
    return 0;

}
