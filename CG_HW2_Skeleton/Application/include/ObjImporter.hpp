//
// Created by 김태윤 on 2019-04-05.
//

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>
#include <Mesh.hpp>
#include <Material.hpp>
#include <cstdlib>
#include <fstream>
#include <string>
#include <regex>


class ObjImporter {
private:
    std::vector<glm::vec3> points;
public:
    ObjImporter();

    std::vector<glm::vec3> ImportObj(std::string file);
};