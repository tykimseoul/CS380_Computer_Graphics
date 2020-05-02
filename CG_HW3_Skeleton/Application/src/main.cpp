// STL
#include <iostream>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// include Engine
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <RenderObject.hpp>
#include <ModelLoader.hpp>

#include <DiffuseMaterial.hpp>
#include <PickingMaterial.hpp>
#include <Geometry.hpp>
#include <ModelLoader.hpp>

#include <picking.hpp>
#include <PickableObject.hpp>

#include <PhongMaterial.hpp>
#include <ToonMaterial.hpp>
#include <MyMaterial.hpp>
#include <Animation.hpp>

GLFWwindow *g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;

Engine::Camera *main_camera;

Engine::Mesh *ground_mesh;
Engine::Mesh *background_mesh;
Engine::Mesh *keldeo_mesh;
Engine::Mesh *absol_mesh;
Engine::Mesh *aurorus_mesh;
Engine::Mesh *azumarill_mesh;
Engine::Mesh *bayleef_mesh;
Engine::Mesh *pachirisu_mesh;
Engine::Mesh *glaceon_mesh;

DiffuseMaterial *diffuseMaterial;
DiffuseMaterial *backgroundMaterial;
ToonMaterial *toonMaterial;
PhongMaterial *phongMaterial;
MyMaterial *myMaterial;
Engine::RenderObject *ground_object;
Engine::RenderObject *background_object;
Engine::RenderObject *keldeo_object;
Engine::RenderObject *absol_object;
Engine::RenderObject *aurorus_object;
Engine::RenderObject *azumarill_object;
Engine::RenderObject *bayleef_object;
Engine::RenderObject *pachirisu_object;
Engine::RenderObject *glaceon_object;

std::vector<Light> lights;
std::vector<Engine::RenderObject *> renderObjects;

Animation *camera_animation;
Animation *light_animation;
Animation *spot1_animation;
Animation *spot2_animation;
Animation *spot3_animation;

int cycle = 0;
bool cursorCallbackEnabled = false;

void InitDayMaterial();

void InitToonMaterial();

void InitNightMaterial();

void initRenderObjects(Engine::Material *material);

static void MouseButtonCallback(GLFWwindow *a_window, int a_button, int a_action, int a_mods) {
    if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(a_window, &xpos, &ypos);
        xpos = xpos / ((double) g_window_width) * ((double) g_framebuffer_width);
        ypos = ypos / ((double) g_window_height) * ((double) g_framebuffer_height);
        int target = pick((int) xpos, (int) ypos, g_framebuffer_width, g_framebuffer_height);
    }
}

static void CursorPosCallback(GLFWwindow *a_window, double xpos, double ypos) {
    if (cursorCallbackEnabled) {
        float mouseSpeed = 0.3f;
        double deltaX = 1024.0 / 2 - xpos;
        double deltaY = 768.0 / 2 - ypos;
        float angle = mouseSpeed * (float) sqrt(deltaX * deltaX + deltaY * deltaY);
        glm::vec3 axis = glm::vec3(-deltaY, -deltaX, 0.0f);
        Engine::Transform transform;
        transform.SetPosition(glm::vec3(-1.5, 0, 0));
        transform.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis));
        keldeo_object->SetTransform(transform);
    }
}

static void KeyboardCallback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mods) {
    if (a_action == GLFW_PRESS) {
        switch (a_key) {
            case GLFW_KEY_H:
                std::cout << "CS380 Homework 3" << std::endl;
                std::cout << "keymaps:" << std::endl;
                std::cout << "h\t\t Help command" << std::endl;
                break;
            case GLFW_KEY_S:
                cycle++;
                cycle %= 3;
                switch (cycle) {
                    case 0:
                        InitDayMaterial();
                        break;
                    case 1:
                        InitToonMaterial();
                        break;
                    case 2:
                        InitNightMaterial();
                        break;
                    default:
                        break;
                }
                break;
            case GLFW_KEY_C:
                cursorCallbackEnabled = !cursorCallbackEnabled;
                break;
            default:
                break;
        }
    }
}

static void FramebufferResizeCallback(GLFWwindow *a_window, int width, int height) {
    glViewport(0, 0, width, height);
    reallocatePickingTexture(width, height);

    int window_width, window_height;
    glfwGetWindowSize(g_window, &window_width, &window_height);
    g_window_width = (float) window_width;
    g_window_height = (float) window_height;
    g_framebuffer_width = width;
    g_framebuffer_height = height;

    Engine::ProjectionData proj = main_camera->GetProjection();
    proj.aspectRatio = (float) width / (float) height;
    main_camera->SetProjection(proj.aspectRatio, proj.fov, proj.zNear, proj.zFar);
}

static void InitScene() {
    main_camera = new Engine::Camera();
    main_camera->GetTransform()->SetPosition(glm::vec3(0.0f, 1.0f, 10.0f));

    Engine::ModelLoader *loader = new Engine::ModelLoader("Resources/Models/scene.obj");
    Engine::ModelLoader *keldeoLoader = new Engine::ModelLoader("Resources/Models/keldeo.ply");
    Engine::ModelLoader *absolLoader = new Engine::ModelLoader("Resources/Models/absol.ply");
    Engine::ModelLoader *aurorusLoader = new Engine::ModelLoader("Resources/Models/aurorus.ply");
    Engine::ModelLoader *azumarillLoader = new Engine::ModelLoader("Resources/Models/azumarill.ply");
    Engine::ModelLoader *bayleefLoader = new Engine::ModelLoader("Resources/Models/bayleef.ply");
    Engine::ModelLoader *pachirisuLoader = new Engine::ModelLoader("Resources/Models/pachirisu.ply");
    Engine::ModelLoader *glaceonLoader = new Engine::ModelLoader("Resources/Models/glaceon.ply");

    ground_mesh = new Engine::Mesh();
    background_mesh = new Engine::Mesh();
    keldeo_mesh = new Engine::Mesh();
    absol_mesh = new Engine::Mesh();
    aurorus_mesh = new Engine::Mesh();
    azumarill_mesh = new Engine::Mesh();
    bayleef_mesh = new Engine::Mesh();
    pachirisu_mesh = new Engine::Mesh();
    glaceon_mesh = new Engine::Mesh();
    loader->GenerateMesh(ground_mesh, 0);
    loader->GenerateMesh(background_mesh, 0);
    keldeoLoader->GenerateMesh(keldeo_mesh, 0);
    absolLoader->GenerateMesh(absol_mesh, 0);
    aurorusLoader->GenerateMesh(aurorus_mesh, 0);
    azumarillLoader->GenerateMesh(azumarill_mesh, 0);
    bayleefLoader->GenerateMesh(bayleef_mesh, 0);
    pachirisuLoader->GenerateMesh(pachirisu_mesh, 0);
    glaceonLoader->GenerateMesh(glaceon_mesh, 0);

    backgroundMaterial = new DiffuseMaterial();
    backgroundMaterial->CreateMaterial();
    diffuseMaterial = new DiffuseMaterial();
    diffuseMaterial->CreateMaterial();
    toonMaterial = new ToonMaterial();
    toonMaterial->CreateMaterial();
    phongMaterial = new PhongMaterial();
    phongMaterial->CreateMaterial();
    myMaterial = new MyMaterial();
    myMaterial->CreateMaterial();

    ground_object = new Engine::RenderObject(ground_mesh, diffuseMaterial);
    background_object = new Engine::RenderObject(background_mesh, backgroundMaterial);
    background_object->GetTransform()->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    background_object->GetTransform()->SetPosition(glm::vec3(0, 0, -15));
    background_object->GetTransform()->SetScale(glm::vec3(10));

    Light sun = Light();
    sun.transform.SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
    lights.push_back(sun);

    Light spot1 = Light();
    spot1.transform.SetPosition(glm::vec3(0.0, 5.0, 0.0));
    spot1.direction = glm::normalize(glm::vec3(0, -1, 0));
    spot1.cutoffAngle = glm::radians(10.0f);
    spot1.quadraticAttenuation = 0.001;
    spot1.spotExponent = 200.0;
    lights.push_back(spot1);

    Light spot2 = Light();
    spot2.transform.SetPosition(glm::vec3(0.0, 5.0, 0.0));
    spot2.direction = glm::normalize(glm::vec3(0, -1, 0));
    spot2.cutoffAngle = glm::radians(10.0f);
    spot2.quadraticAttenuation = 0.001;
    spot2.spotExponent = 200.0;
    lights.push_back(spot2);

    Light spot3 = Light();
    spot3.transform.SetPosition(glm::vec3(0.0, 5.0, 0.0));
    spot3.direction = glm::normalize(glm::vec3(0, -1, 0));
    spot3.cutoffAngle = glm::radians(10.0f);
    spot3.quadraticAttenuation = 0.001;
    spot3.spotExponent = 200.0;
    lights.push_back(spot3);

    InitDayMaterial();
}

void InitDayMaterial() {
    initRenderObjects(myMaterial);

    lights[0].type = PointLight;
    lights[0].diffuse_illuminance = glm::vec3(0.9f, 0.9f, 0.9f);
    lights[0].ambient_illuminance = glm::vec3(0.9, 0.9, 0.9);
    lights[0].specular_illuminance = glm::vec3(0.9, 0.9, 0.9);

    lights[1].type = SpotLight;
    lights[1].diffuse_illuminance = glm::vec3(0.8f, 0.8f, 0.8f);
    lights[1].ambient_illuminance = glm::vec3(0.5, 0.5, 0.5);
    lights[1].specular_illuminance = glm::vec3(0.8, 0.8, 0.8);

    lights[2].type = SpotLight;
    lights[2].diffuse_illuminance = glm::vec3(0.8f, 0.8f, 0.8f);
    lights[2].ambient_illuminance = glm::vec3(0.5, 0.5, 0.5);
    lights[2].specular_illuminance = glm::vec3(0.8, 0.8, 0.8);

    lights[3].type = SpotLight;
    lights[3].diffuse_illuminance = glm::vec3(0.8f, 0.8f, 0.8f);
    lights[3].ambient_illuminance = glm::vec3(0.5, 0.5, 0.5);
    lights[3].specular_illuminance = glm::vec3(0.8, 0.8, 0.8);

    backgroundMaterial->UpdateDiffuseReflectance(glm::vec3(1.0f, 1.0f, 1.0f));
}

void InitToonMaterial() {
    initRenderObjects(toonMaterial);

    lights[0].type = PointLight;
    lights[0].diffuse_illuminance = glm::vec3(0.6f, 0.3f, 0.8f);
    lights[0].ambient_illuminance = glm::vec3(0.2, 0.2, 0.2);
    lights[0].specular_illuminance = glm::vec3(0.5, 0.5, 0.5);

    lights[1].type = SpotLight;
    lights[1].diffuse_illuminance = glm::vec3(0.8f, 0.8f, 0.8f);
    lights[1].ambient_illuminance = glm::vec3(0.5, 0.5, 0.5);
    lights[1].specular_illuminance = glm::vec3(0.8, 0.8, 0.8);

    lights[2].type = SpotLight;
    lights[2].diffuse_illuminance = glm::vec3(0.8f, 0.8f, 0.8f);
    lights[2].ambient_illuminance = glm::vec3(0.5, 0.5, 0.5);
    lights[2].specular_illuminance = glm::vec3(0.8, 0.8, 0.8);

    lights[3].type = SpotLight;
    lights[3].diffuse_illuminance = glm::vec3(0.8f, 0.8f, 0.8f);
    lights[3].ambient_illuminance = glm::vec3(0.5, 0.5, 0.5);
    lights[3].specular_illuminance = glm::vec3(0.8, 0.8, 0.8);

    backgroundMaterial->UpdateDiffuseReflectance(glm::vec3(0.1f, 0.7f, 0.1f));
}

void InitNightMaterial() {
    initRenderObjects(phongMaterial);

    lights[0].type = PointLight;
    lights[0].diffuse_illuminance = glm::vec3(0.1f, 0.3f, 0.6f);
    lights[0].ambient_illuminance = glm::vec3(0.2, 0.2, 0.2);
    lights[0].specular_illuminance = glm::vec3(0.8, 0.8, 0.8);

    lights[1].type = SpotLight;
    lights[1].diffuse_illuminance = glm::vec3(0.9f, 0.1f, 0.1f);
    lights[1].ambient_illuminance = glm::vec3(0.5, 0.5, 0.5);
    lights[1].specular_illuminance = glm::vec3(0.9, 0.9, 0.9);

    lights[2].type = SpotLight;
    lights[2].diffuse_illuminance = glm::vec3(0.1f, 0.9f, 0.1f);
    lights[2].ambient_illuminance = glm::vec3(0.5, 0.5, 0.5);
    lights[2].specular_illuminance = glm::vec3(0.9, 0.9, 0.9);

    lights[3].type = SpotLight;
    lights[3].diffuse_illuminance = glm::vec3(0.1f, 0.1f, 0.9f);
    lights[3].ambient_illuminance = glm::vec3(0.5, 0.5, 0.5);
    lights[3].specular_illuminance = glm::vec3(0.9, 0.9, 0.9);

    backgroundMaterial->UpdateDiffuseReflectance(glm::vec3(0.1f, 0.1f, 0.8f));
}

void initRenderObjects(Engine::Material *material) {
    keldeo_object = new Engine::RenderObject(keldeo_mesh, material);
    keldeo_object->GetTransform()->SetPosition(glm::vec3(-1.5, 0, 0));

    absol_object = new Engine::RenderObject(absol_mesh, material);
    absol_object->GetTransform()->SetPosition(glm::vec3(-4.5, 0, 0));

    aurorus_object = new Engine::RenderObject(aurorus_mesh, material);
    aurorus_object->GetTransform()->SetPosition(glm::vec3(1.5, 0, 0));
    aurorus_object->GetTransform()->SetScale(glm::vec3(0.5));

    bayleef_object = new Engine::RenderObject(bayleef_mesh, material);
    bayleef_object->GetTransform()->SetPosition(glm::vec3(4.5, 0, 0));

    azumarill_object = new Engine::RenderObject(azumarill_mesh, material);
    azumarill_object->GetTransform()->SetPosition(glm::vec3(-2, 0, 3));
    azumarill_object->GetTransform()->SetScale(glm::vec3(0.7));

    pachirisu_object = new Engine::RenderObject(pachirisu_mesh, material);
    pachirisu_object->GetTransform()->SetPosition(glm::vec3(0, 0, 3));

    glaceon_object = new Engine::RenderObject(glaceon_mesh, material);
    glaceon_object->GetTransform()->SetPosition(glm::vec3(2, 0, 3));
    glaceon_object->GetTransform()->SetScale(glm::vec3(0.7));

    renderObjects.clear();
    renderObjects.push_back(keldeo_object);
    renderObjects.push_back(absol_object);
    renderObjects.push_back(aurorus_object);
    renderObjects.push_back(azumarill_object);
    renderObjects.push_back(bayleef_object);
    renderObjects.push_back(pachirisu_object);
    renderObjects.push_back(glaceon_object);
}

static void InitAnimation() {
    camera_animation = new Animation(main_camera->GetTransform(), 3, AnimationType::Position | AnimationType::Orientation);

    Engine::Transform camera_0;
    camera_0.SetPosition(glm::vec3(0.0f, 1.0f, 4.0f));
    camera_0.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    camera_animation->AddKeyframe(0.0f, camera_0);

    Engine::Transform camera_1;
    camera_1.SetPosition(glm::vec3(0.0f, 2.0f, 4.0f));
    camera_1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    camera_animation->AddKeyframe(1.5f, camera_1);


    light_animation = new Animation(&lights[0].transform, 5, AnimationType::Orientation);

    Engine::Transform light_0;
    light_0.SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
    light_0.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    light_animation->AddKeyframe(0.0f, light_0);

    Engine::Transform light_1;
    light_0.SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
    light_1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    light_animation->AddKeyframe(2.5f, light_1);

    Engine::Transform light_2;
    light_0.SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
    light_2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    light_animation->AddKeyframe(5.0f, light_2);


    spot1_animation = new Animation(&lights[1].transform, 2, AnimationType::All);
    Engine::Transform spot1_0;
    spot1_0.SetPosition(glm::vec3(-5.0, 15.0, -5.0));
    spot1_animation->AddKeyframe(0.0f, spot1_0);
    Engine::Transform spot1_1;
    spot1_1.SetPosition(glm::vec3(5.0, 15.0, 5.0));
    spot1_animation->AddKeyframe(1.0f, spot1_1);
    lights[1].transform.AddParent(&lights[0].transform);

    spot2_animation = new Animation(&lights[2].transform, 2, AnimationType::All);
    Engine::Transform spot2_0;
    spot2_0.SetPosition(glm::vec3(5.0, 15.0, -5.0));
    spot2_animation->AddKeyframe(0.0f, spot2_0);
    Engine::Transform spot2_1;
    spot2_1.SetPosition(glm::vec3(-5.0, 15.0, 5.0));
    spot2_animation->AddKeyframe(1.0f, spot2_1);
    lights[2].transform.AddParent(&lights[0].transform);

    spot3_animation = new Animation(&lights[3].transform, 2, AnimationType::All);
    Engine::Transform spot3_0;
    spot3_0.SetPosition(glm::vec3(0.0, 15.0, 5.0));
    spot3_animation->AddKeyframe(0.0f, spot3_0);
    Engine::Transform spot3_1;
    spot3_1.SetPosition(glm::vec3(0.0, 15.0, -5.0));
    spot3_animation->AddKeyframe(1.0f, spot3_1);
    lights[3].transform.AddParent(&lights[0].transform);
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
    g_window = glfwCreateWindow(g_window_width, g_window_height, "Homework 3 - Let there be light and Blinn-toon reflection model", NULL, NULL);
    if (!g_window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(g_window);

    // Initialize GLEW library
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        std::cout << "GLEW Error: " << glewGetErrorString(glew_error) << std::endl;
        exit(1);
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

    lights = std::vector<Light>();
    renderObjects = std::vector<Engine::RenderObject *>();
    InitScene();
    InitAnimation();

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
    glfwSetCursorPosCallback(g_window, CursorPosCallback);
    glfwSetKeyCallback(g_window, KeyboardCallback);
    glfwSetFramebufferSizeCallback(g_window, FramebufferResizeCallback);

    // Initialize framebuffer object and picking textures
    pickingInitialize(g_framebuffer_width, g_framebuffer_width);

    float prev_time = (float) glfwGetTime();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        float total_time = (float) glfwGetTime();
        float elapsed_time = total_time - prev_time;
        prev_time = total_time;

        // First Pass: Object Selection
        // this is for picking the object using mouse interaction
        // binding framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);
        // Background: RGB = 0x000000 => objectID: 0
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* RenderPicking here */

        // Second Pass: Object Rendering
        // Drawing object again
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor((GLclampf) (128. / 255.), (GLclampf) (200. / 255.), (GLclampf) (255. / 255.), (GLclampf) 0.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Render here */
//        camera_animation->Animate(elapsed_time);
        light_animation->Animate(elapsed_time);
        spot1_animation->Animate(elapsed_time);
        spot2_animation->Animate(elapsed_time);
        spot3_animation->Animate(elapsed_time);

        diffuseMaterial->UpdateLight(lights);
        diffuseMaterial->UpdateDiffuseReflectance(glm::vec3(0.5f, 0.7f, 0.5f));
        ground_object->Render(main_camera);

        toonMaterial->UpdateLight(lights);
        toonMaterial->UpdateDiffuseReflectance(glm::vec3(0.9f, 0.3f, 0.9f));
        toonMaterial->UpdateAmbientReflectance(glm::vec3(0.1, 0.1, 0.1));
        toonMaterial->UpdateSpecularReflectance(glm::vec3(0.9, 0.9, 0.9));
        toonMaterial->UpdateCameraPosition(main_camera->GetTransform()->GetPosition());
        toonMaterial->UpdateShininess(500.0);

        phongMaterial->UpdateLight(lights);
        phongMaterial->UpdateDiffuseReflectance(glm::vec3(0.9f, 0.3f, 0.9f));
        phongMaterial->UpdateAmbientReflectance(glm::vec3(0.1, 0.1, 0.1));
        phongMaterial->UpdateSpecularReflectance(glm::vec3(0.9, 0.9, 0.9));
        phongMaterial->UpdateCameraPosition(main_camera->GetTransform()->GetPosition());
        phongMaterial->UpdateShininess(500.0);

        myMaterial->UpdateLight(lights);
        myMaterial->UpdateDiffuseReflectance(glm::vec3(0.9f, 0.3f, 0.9f));
        myMaterial->UpdateAmbientReflectance(glm::vec3(0.1, 0.1, 0.1));
        myMaterial->UpdateSpecularReflectance(glm::vec3(0.9, 0.9, 0.9));
        myMaterial->UpdateCameraPosition(main_camera->GetTransform()->GetPosition());
        myMaterial->UpdateShininess(500.0);

        backgroundMaterial->UpdateLight(lights);

        for (auto renderObject:renderObjects) {
            renderObject->Render(main_camera);
        }

        background_object->Render(main_camera);


        /* Swap front and back buffers */
        glfwSwapBuffers(g_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Delete resources
    delete main_camera;

    glfwTerminate();
    return 0;

}
