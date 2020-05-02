#pragma once

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <Mesh.hpp>
#include <list>
#include <cstdlib>

/*
 * Implement Fractal class to generate Koch snowflakes mesh (generate snowflake mesh using triangle list).
 * Koch snowflake: https://en.wikipedia.org/wiki/Koch_snowflake
 */
class Fractal {
private:
    std::vector<glm::vec3> base;
    std::vector<glm::vec3> points;
    std::vector<glm::vec4> colors;
public:
    Fractal();

    ~Fractal();

    void GenerateSnowflake(Engine::Mesh *mesh, int level);

    void GenerateSnowflakeRecursively(int levels);

    void SnowflakeRecursion(glm::vec3 pt1, glm::vec3 pt2, int level);

    std::vector<glm::vec3> CalculatePoints(glm::vec3 pt1, glm::vec3 pt2);

    glm::vec3 Rotate(glm::vec3 vec, int angle, glm::vec3 axis);

    void CreateBase(float size);

    void FormMesh(Engine::Mesh *mesh, glm::vec4 color);
};
