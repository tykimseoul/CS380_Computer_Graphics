//
// Created by 김태윤 on 2019-04-07.
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
#include <ObjImporter.hpp>
#include <Geometry.hpp>


class BlenderObject : public Engine::RenderObject {
private:
    std::vector<glm::vec3> points;
public:
    BlenderObject(Engine::Mesh *mesh, Engine::Material *material, std::string file);

    void FormMesh(Engine::Mesh *mesh, std::string file);
};