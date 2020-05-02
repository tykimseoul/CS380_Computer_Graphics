//
//  Cone.hpp
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
#include <cstdlib>
#include "Geometry.hpp"

class Cone : public Engine::RenderObject {
private:
    std::vector<glm::vec3> points;
    float radius = 1.0f;
    float height = 2.0f;
public:
    Cone(Engine::Mesh *mesh, Engine::Material *material, float r, float h);

    void FormMesh(Engine::Mesh *mesh, float r, float h);

    void SetSize(float r, float h);
};
