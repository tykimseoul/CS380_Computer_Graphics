#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <PickableObject.hpp>
#include <Sphere.hpp>
#include "Cylinder.hpp"
#include "Cone.hpp"
#include <BlenderObject.hpp>

class Snowman {
private:

public:
    Snowman();

    ~Snowman();

    PickableObject *body;
    PickableObject *head;
    Engine::RenderObject *leftArm;
    Engine::RenderObject *rightArm;
    Engine::RenderObject *leftEye;
    Engine::RenderObject *rightEye;
    Engine::RenderObject *nose;
    Engine::RenderObject *mouth;
    Engine::RenderObject *hat;
    Engine::RenderObject *cane;
    Engine::RenderObject *caneAnchor;
    Engine::RenderObject *bowtie;
    float handAngle = 40.0f;
    float caneAngle = 0.0f;
    float handDirection = 1.0f;
    float caneDirection = 1.0f;

    void ConstructSnowman(Sphere h, Sphere b);

    void AddArms(BlenderObject la, BlenderObject ra);

    void AddFace(Sphere le, Sphere re, Cone nose, Cylinder mouth);

    void AddHat(BlenderObject hat);

    void AddCane(BlenderObject cane, Sphere anchor);

    void AddBowTie(BlenderObject tie);

    void WaveHand(float deltaTime);

    void SwingCane(float deltaTime);

    void Render(Engine::Camera *camera);

    void RenderPicking(Engine::Camera *camera);
};
