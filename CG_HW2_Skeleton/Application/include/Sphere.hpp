//
//  Sphere.hpp
//  App
//
//  Created by 김태윤 on 28/03/2019.
//

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <RenderObject.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include "PickableObject.hpp"
#include "Geometry.hpp"

class Sphere : public PickableObject {
private:
    std::vector<glm::vec3> points;
    std::vector<glm::vec4> colors;
    float radius = 1.0f;
public:
    Sphere(Engine::Mesh *mesh, Engine::Material *material, float r);

    void FormMesh(Engine::Mesh *mesh);

    void FillQuad(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);
    
    void SetRadius(float r);
};
