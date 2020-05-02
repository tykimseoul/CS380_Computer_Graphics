// 2019 Spring CS380 Homework 4
// You have to implement
// 1. Normal mapping
//	NormalMaterial.cpp, NormalMaterial.hpp
//	Vertex has new attribute vec4 tangent (see ModelLoader.cpp or Geometry.cpp)
// 2. Shadow Mapping
//	ShadowMap.hpp, ShadowMaterial.cpp, ShadowMaterial.hpp
//
// You can create/modify any functions or files in APP
// Use the OpenGL documentation
// You could use glm::ortho(), glm::lookAt() for shadow mapping

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
#include <Texture.hpp>

#include <DiffuseMaterial.hpp>
#include <PickingMaterial.hpp>
#include <NormalMaterial.hpp>
#include <ShadowMaterial.hpp>
#include <Geometry.hpp>
#include <ModelLoader.hpp>

#include <picking.hpp>
#include <PickableObject.hpp>

#include <CubeMaterial.hpp>
#include <Animation.hpp>

#include <ShadowMap.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/string_cast.hpp>

#define SUN_ORBIT_TIME 20.0f
#define MOON_ORBIT_TIME 10.0f
#define EARTH_ROTATION_TIME 5.0f

GLFWwindow *g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;

Engine::Camera *main_camera;

Engine::Mesh *cube_mesh;
Engine::Mesh *earth_mesh;
Engine::Mesh *moon_mesh;
Engine::Mesh *shadow_mesh;
Engine::Mesh *sun_anchor;
Engine::Mesh *moon_anchor;
Engine::Mesh *sampleCube;

NormalMaterial *earthMaterial;
NormalMaterial *moonMaterial;
DiffuseMaterial *sampleMaterial;
ShadowMaterial *shadowMaterial;
CubeMaterial *cubemap_material;

Engine::RenderObject *cube_object;
Engine::RenderObject *cubemap_object;
Engine::RenderObject *earth_object;
Engine::RenderObject *moon_object;
Engine::RenderObject *moon_anchor_object;
Engine::RenderObject *sun_anchor_object;
Engine::RenderObject *shadow_object;
Engine::RenderObject *sample_object;

std::vector<Light> lights;

Animation *earth_animation;
Animation *camera_animation;
Animation *moon_animation;
Animation *sun_animation;

std::vector<Animation *> animations;

int globalUnit = 0;
int shadowUnit = 23;

Engine::Transform *oriT;
bool cursorCallbackEnabled = false;

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
        oriT->SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis));
    }
}

//set keyboard interactions
static void KeyboardCallback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mods) {
    if (a_action == GLFW_PRESS) {
        switch (a_key) {
            case GLFW_KEY_H:
                std::cout << "CS380 LAB 4" << std::endl;
                std::cout << "keymaps:" << std::endl;
                std::cout << "h\t\t Help command" << std::endl;
                std::cout << "r\t\t rotate the camera -20 degrees with respect to world's y axis" << std::endl;
                std::cout << "l\t\t rotate the camera +20 degrees with respect to world's y axis" << std::endl;
                break;
            case GLFW_KEY_L:
                oriT->SetOrientation(glm::rotate(oriT->GetOrientation(), glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
                break;
            case GLFW_KEY_R:
                oriT->SetOrientation(glm::rotate(oriT->GetOrientation(), glm::radians(+20.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
                break;
            case GLFW_KEY_C:
                cursorCallbackEnabled = !cursorCallbackEnabled;
                break;
            case GLFW_KEY_X:
                oriT->SetPosition(oriT->GetPosition() + glm::vec3(1, 0, 0));
                break;
            case GLFW_KEY_Z:
                oriT->SetPosition(oriT->GetPosition() + glm::vec3(-1, 0, 0));
                break;
            case GLFW_KEY_S:
                oriT->SetPosition(oriT->GetPosition() + glm::vec3(0, 1, 0));
                break;
            case GLFW_KEY_A:
                oriT->SetPosition(oriT->GetPosition() + glm::vec3(0, -1, 0));
                break;
            case GLFW_KEY_W:
                oriT->SetPosition(oriT->GetPosition() + glm::vec3(0, 0, 1));
                break;
            case GLFW_KEY_Q:
                oriT->SetPosition(oriT->GetPosition() + glm::vec3(0, 0, -1));
                break;
            default:
                break;
        }
    }
}

static void FramebufferResizeCallback(GLFWwindow *a_window, int width, int height) {
    glViewport(0, 0, width, height);
    reallocatePickingTexture(width, height, 0);
    reallocateShadowTexture(width, height, 0);

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

// init scene with light, earth anc moon
static void InitScene() {
    main_camera = new Engine::Camera();
    main_camera->GetTransform()->SetPosition(glm::vec3(0.0f, 1.0f, 6.0f));
    oriT = new Engine::Transform;
    main_camera->GetTransform()->AddParent(oriT);

    //load obj files
    Engine::ModelLoader *earthLoader = new Engine::ModelLoader("./Resources/Models/earth.obj");
    Engine::ModelLoader *moonLoader = new Engine::ModelLoader("./Resources/Models/moon.obj");

    cube_mesh = new Engine::Mesh();
    earth_mesh = new Engine::Mesh();
    moon_mesh = new Engine::Mesh();
    sun_anchor = new Engine::Mesh();
    moon_anchor = new Engine::Mesh();
    shadow_mesh = new Engine::Mesh();
    sampleCube = new Engine::Mesh();

    Geometry geometry = Geometry();
    geometry.GenerateCube(cube_mesh);
    geometry.GenerateCube(sun_anchor);
    geometry.GenerateCube(moon_anchor);
    geometry.GenerateCube(sampleCube);

    earthLoader->GenerateMesh(earth_mesh, 0);
    moonLoader->GenerateMesh(shadow_mesh, 0);
    moonLoader->GenerateMesh(moon_mesh, 0);

    //importing texture and normal images
    Engine::TextureLoader *earthTextureLoader = new Engine::TextureLoader(globalUnit++, "./Resources/Models/earth_diffuse.jpg");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    Engine::TextureLoader *earthNormalLoader = new Engine::TextureLoader(globalUnit++, "./Resources/Models/earth_normal.jpg");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    Engine::TextureLoader *earthNightLoader = new Engine::TextureLoader(globalUnit++, "./Resources/Models/earth_night.jpg");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    Engine::TextureLoader *earthCloudLoader = new Engine::TextureLoader(globalUnit++, "./Resources/Models/earth_cloud.jpg");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    Engine::TextureLoader *moonTextureLoader = new Engine::TextureLoader(globalUnit++, "./Resources/Models/moon_diffuse.jpg");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    Engine::TextureLoader *moonNormalLoader = new Engine::TextureLoader(globalUnit++, "./Resources/Models/moon_normal.jpg");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	std::string path_prefix = "Resources\\Textures\\skybox\\";
    std::string path_prefix = "./Resources/Textures/milkyWay_";
//    Engine::TextureLoader *textureloader2 = new Engine::TextureLoader(5, path_prefix + "right.jpg", path_prefix + "left.jpg",
//                                                                      path_prefix + "top.jpg", path_prefix + "bottom.jpg", path_prefix + "front.jpg", path_prefix + "back.jpg");
    Engine::TextureLoader *textureloader2 = new Engine::TextureLoader(globalUnit++, path_prefix + "right.png", path_prefix + "left.png",
                                                                      path_prefix + "top.png", path_prefix + "top.png", path_prefix + "front.png", path_prefix + "back.png");
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    cubemap_material = new CubeMaterial();
    cubemap_material->CreateMaterial(textureloader2->GetUnit());
    cubemap_object = new Engine::RenderObject(cube_mesh, cubemap_material);

    //apply texture and normal to each materials
    earthMaterial = new NormalMaterial();
    earthMaterial->CreateMaterial(earthTextureLoader->GetUnit());
    earthMaterial->ApplyNormal(earthNormalLoader->GetUnit());
    earthMaterial->IsEarth(1, earthNightLoader->GetUnit());
    earthMaterial->ApplyShadow(shadowUnit);

    moonMaterial = new NormalMaterial();
    moonMaterial->CreateMaterial(moonTextureLoader->GetUnit());
    moonMaterial->ApplyNormal(moonNormalLoader->GetUnit());
    moonMaterial->IsEarth(0, 0);
    moonMaterial->ApplyShadow(shadowUnit);

    shadowMaterial = new ShadowMaterial();
    shadowMaterial->CreateMaterial();

    sampleMaterial = new DiffuseMaterial();
    sampleMaterial->CreateMaterial(2);
    sampleMaterial->ApplyDepth(shadowUnit);

    cube_object = new Engine::RenderObject(cube_mesh, cubemap_material);
    earth_object = new Engine::RenderObject(earth_mesh, earthMaterial);
    moon_object = new Engine::RenderObject(moon_mesh, moonMaterial);
    moon_anchor_object = new Engine::RenderObject(moon_anchor, cubemap_material);
    sun_anchor_object = new Engine::RenderObject(sun_anchor, cubemap_material);
    shadow_object = new Engine::RenderObject(shadow_mesh, shadowMaterial);
    sample_object = new Engine::RenderObject(sampleCube, sampleMaterial);

    Engine::Transform *earth_T = earth_object->GetTransform();
    earth_T->SetScale(glm::vec3(0.005));
    earth_T->SetPosition(glm::vec3(0, 1, 0));
    earth_T->SetOrientation(glm::rotate(earth_T->GetOrientation(), glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0)));
    earth_object->SetTransform(*earth_T);

    Engine::Transform *moon_T = moon_object->GetTransform();
    moon_T->SetScale(glm::vec3(0.003));
    moon_T->SetPosition(glm::vec3(0, 1, 2.7));
    moon_T->SetOrientation(glm::rotate(moon_T->GetOrientation(), glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0)));
    moon_object->SetTransform(*moon_T);
    shadow_object->SetTransform(*moon_T);

    Engine::Transform *cube_T = cube_object->GetTransform();
    cube_T->SetPosition(glm::vec3(1.5f, 0.8f, 0.0f));
    cube_T->SetScale(glm::vec3(0.5f));
    cube_object->SetTransform(*cube_T);
    Engine::Transform *anchor_T = moon_anchor_object->GetTransform();
    anchor_T->SetScale(glm::vec3(0.5f));
    anchor_T->SetPosition(glm::vec3(0, 1, 0));
    moon_anchor_object->SetTransform(*anchor_T);
    sun_anchor_object->SetTransform(*anchor_T);

    Engine::Transform *plane_T = sample_object->GetTransform();
    plane_T->SetScale(glm::vec3(2.0f));
    plane_T->SetPosition(glm::vec3(0, 0, -3));
    sample_object->SetTransform(*plane_T);


    Light sun2 = Light();
    sun2.type = DirectionalLight; //directional light
    sun2.transform.SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    sun2.transform.LookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    sun2.diffuse_illuminance = glm::vec3(1.0f, 1.0f, 1.0f);
    lights.push_back(sun2);
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

    sun_animation = new Animation(sun_anchor_object->GetTransform(), SUN_ORBIT_TIME, AnimationType::Orientation);
    Engine::Transform sun_0;
    sun_0.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    sun_animation->AddKeyframe(0.0f, sun_0);

    Engine::Transform sun_1;
    sun_1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    sun_animation->AddKeyframe(SUN_ORBIT_TIME / 2.0f, sun_1);

    Engine::Transform sun_2;
    sun_2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    sun_animation->AddKeyframe(SUN_ORBIT_TIME, sun_2);

    lights[0].transform.AddParent(sun_anchor_object->GetTransform());

    earth_animation = new Animation(earth_object->GetTransform(), EARTH_ROTATION_TIME, AnimationType::Orientation);
    Engine::Transform earth_0;
    earth_0.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    earth_animation->AddKeyframe(0.0f, earth_0);

    Engine::Transform earth_1;
    earth_1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    earth_animation->AddKeyframe(EARTH_ROTATION_TIME / 2.0f, earth_1);

    Engine::Transform earth_2;
    earth_2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    earth_animation->AddKeyframe(EARTH_ROTATION_TIME, earth_2);

    moon_animation = new Animation(moon_object->GetTransform(), MOON_ORBIT_TIME, AnimationType::Orientation);

    Engine::Transform moon_0;
    moon_0.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    moon_animation->AddKeyframe(0.0f, moon_0);

    Engine::Transform moon_1;
    moon_1.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    moon_animation->AddKeyframe(MOON_ORBIT_TIME / 2.0f, moon_1);

    Engine::Transform moon_2;
    moon_2.SetOrientation(glm::rotate(glm::mat4(1.0f), glm::radians(360.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    moon_animation->AddKeyframe(MOON_ORBIT_TIME, moon_2);

//    moon_object->GetTransform()->AddParent(moon_anchor_object->GetTransform());

//    animations.push_back(sun_animation);
//    animations.push_back(earth_animation);
//    animations.push_back(moon_animation);
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
    g_window = glfwCreateWindow(g_window_width, g_window_height, "HW4", NULL, NULL);
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

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
    glfwSetCursorPosCallback(g_window, CursorPosCallback);
    glfwSetKeyCallback(g_window, KeyboardCallback);
    glfwSetFramebufferSizeCallback(g_window, FramebufferResizeCallback);

    // Initialize framebuffer object and picking textures
    pickingInitialize(g_framebuffer_width, g_framebuffer_height, 0);
    shadowInitialize(g_framebuffer_width, g_framebuffer_height, shadowUnit);
    animations = std::vector<Animation *>();
    InitScene();
    InitAnimation();

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

        // TODO
        // Second Pass: Shadow Mapping (See the first pass and picking.hpp to understand how you implement render to texture)
        glBindFramebuffer(GL_FRAMEBUFFER, shadow_fbo);
        glBindTexture(GL_TEXTURE_2D, shadow_tex);
        glClear(GL_DEPTH_BUFFER_BIT);


        shadowMaterial->UpdateLightSpace(lights, earth_object->GetTransform()->GetPosition());
        earth_object->SetMaterial(shadowMaterial);
        earth_object->Render(main_camera);
        moon_object->SetMaterial(shadowMaterial);
        moon_object->Render(main_camera);
//        shadow_object->Render(main_camera);
//        sample_object->SetMaterial(shadowMaterial);
//        sample_object->Render(main_camera);



        // Third Pass: Object Rendering
        // Drawing object again
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor((GLclampf) (128. / 255.), (GLclampf) (200. / 255.), (GLclampf) (255. / 255.), (GLclampf) 0.);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Render here */
        for (auto a:animations) {
            a->Animate(elapsed_time);
        }

        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
        cubemap_material->IsSkybox(1);
        cubemap_object->Render(main_camera);
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE);

        cubemap_material->IsSkybox(0);
//        cube_object->Render(main_camera);
//        moon_anchor_object->Render(main_camera);

        earthMaterial->UpdateLight(lights, earth_object->GetTransform()->GetPosition());
        earthMaterial->UpdateDiffuseReflectance(glm::vec3(0.9f, 0.9f, 0.9f));
        earth_object->SetMaterial(earthMaterial);
        earth_object->Render(main_camera);

        moonMaterial->UpdateLight(lights, earth_object->GetTransform()->GetPosition());
        moonMaterial->UpdateDiffuseReflectance(glm::vec3(0.9f, 0.9f, 0.9f));
        moon_object->SetMaterial(moonMaterial);
        moon_object->Render(main_camera);

//        sample_object->SetMaterial(sampleMaterial);
//        sample_object->Render(main_camera);

//        shadowMaterial->UpdateLightSpace(lights, earth_object->GetTransform()->GetPosition());
//        shadow_object->Render(main_camera);

//        sample_object->Render(main_camera);

        /* Swap front and back buffers */
        glfwSwapBuffers(g_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Delete resources
    delete main_camera;
    deletePickingResources();
    deleteShadowResources();
    glfwTerminate();
    return 0;

}