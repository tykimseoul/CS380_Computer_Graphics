// STL
#include <iostream>

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
#include <Fractal.hpp>
#include <DefaultMaterial.hpp>
#include <PerVertexColorMaterial.hpp>
#include <Snowflake.hpp>
#include <Animation.hpp>

Animation *animation = new Animation();
std::vector<Snowflake *> snowflakes;
std::vector<Engine::RenderObject> stars;
int fractalLevel = 3;

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
 Generate an abstract-style triangle mesh
 Parameters: mesh -> mesh to add triangle vertices to
 */
void GenerateBackgroundMeshType1(Engine::Mesh *mesh) {
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);
    glm::vec3 position[] = {
            glm::vec3(-4.0f, -2.0f, 0.0f),
            glm::vec3(1.0f, -4.0f, 0.0f),
            glm::vec3(2.0f, 2.0f, 0.0f)
    };
    glm::vec4 colors[] = {
            glm::vec4(1.0f, 0.2f, 0.7f, 1.0f),
            glm::vec4(1.0f, 0.4f, 0.6f, 1.0f),
            glm::vec4(1.0f, 0.8f, 0.2f, 1.0f)
    };
    for (int i = 0; i < 3; i++) {
        mesh->AddVertexData(position[i]);
        mesh->AddVertexData(1.0f);
        mesh->AddVertexData(colors[i]);
    }
    mesh->SetNumElements(3);
    mesh->CreateMesh();
}

/*
 Generate another abstract-style triangle mesh
 Parameters: mesh -> mesh to add triangle vertices to
 */
void GenerateBackgroundMeshType2(Engine::Mesh *mesh) {
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);
    glm::vec3 position[] = {
            glm::vec3(-2.0f, -2.0f, 0.0f),
            glm::vec3(2.0f, -2.0f, 0.0f),
            glm::vec3(4.0f, 2.0f, 0.0f)
    };
    glm::vec4 colors[] = {
            glm::vec4(1.0f, 1.0f, 0.7f, 1.0f),
            glm::vec4(1.0f, 0.4f, 0.6f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.2f, 1.0f)
    };
    for (int i = 0; i < 3; i++) {
        mesh->AddVertexData(position[i]);
        mesh->AddVertexData(1.0f);
        mesh->AddVertexData(colors[i]);
    }
    mesh->SetNumElements(3);
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
    PerVertexColorMaterial *colorMaterial = new PerVertexColorMaterial();
    colorMaterial->CreateMaterial();

    for (int i = 0; i < count; i++) {
        Fractal fractal = Fractal();
        Engine::Mesh *mesh3 = new Engine::Mesh();
        fractal.GenerateSnowflake(mesh3, fractalLevel);
        Snowflake *snowflake = new Snowflake(mesh3, colorMaterial);
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
            case GLFW_KEY_Z:
                if (fractalLevel != 0) {
                    fractalLevel--;
                }
                break;
            case GLFW_KEY_X:
                animation->ClearSnowflake();
            default:
                break;
        }
    }
}

int main(int argc, char **argv) {
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    GLFWwindow *window;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 768, "Homework 1 - Snowflake Animation 20150860", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cout << "GLEW Error: " << glewGetErrorString(glewError) << std::endl;
        exit(1);
    }

    glfwSetKeyCallback(window, KeyCallback);
    srand(time(0));

    snowflakes = std::vector<Snowflake *>();
    Engine::Camera *mainCamera = new Engine::Camera();

    Engine::Mesh *mesh = new Engine::Mesh();
    Engine::Mesh *mesh1 = new Engine::Mesh();
    Engine::Mesh *mesh2 = new Engine::Mesh();
    Engine::Mesh *mesh3 = new Engine::Mesh();

    GenerateGradientRectangle(mesh);
    GenerateBackgroundMeshType1(mesh1);
    GenerateBackgroundMeshType2(mesh2);
    GenerateMoon(mesh3);

    AddNewSnowflakes(10);
    AddStars(20);

    PerVertexColorMaterial *colorMaterial = new PerVertexColorMaterial();
    colorMaterial->CreateMaterial();

    //Create objects
    Engine::RenderObject backgroundObject = Engine::RenderObject(mesh, colorMaterial);
    Engine::RenderObject triangle1 = Engine::RenderObject(mesh1, colorMaterial);
    Engine::RenderObject triangle2 = Engine::RenderObject(mesh2, colorMaterial);
    Engine::RenderObject moon = Engine::RenderObject(mesh3, colorMaterial);

    mainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 7.5f));

    double tempTime = 0;
    double snowflakeTime = 0;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        //Time calculation for animation
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - tempTime;
        double snowflakeElapsedTime = currentTime - snowflakeTime;

        if (snowflakeElapsedTime > 3) {
//            printf("adding\n");
            AddNewSnowflakes(2);
            snowflakeTime = currentTime;
        }

        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply constant animation based on elapsed time since the last drawing
        animation->Animate(elapsedTime, *mainCamera);

        triangle1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-120.0f), glm::vec3(1, 1, 2)));
        triangle1.SetPosition(glm::vec3(0, -1.5, -1));

        triangle2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(1, 1, 1)));
        triangle2.SetPosition(glm::vec3(-1, -1.5, 0));

        moon.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-20.0f), glm::vec3(0, 0, 1)));
        moon.SetPosition(glm::vec3(-5, 2, -1));

        // Render all stars
        for (auto &star:stars) {
            star.Render(mainCamera);
        }

        //Render other objects
        backgroundObject.Render(mainCamera);
        triangle1.Render(mainCamera);
        triangle2.Render(mainCamera);
        moon.Render(mainCamera);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        tempTime = currentTime;
    }

    delete mainCamera;
    delete mesh;
    delete colorMaterial;

    glfwTerminate();
    return 0;

}
