#include <Fractal.hpp>
#include <list>
#include <math.h>
#include <set>


/*
 * Implement Fractal class to generate Koch snowflakes mesh (generate snowflake mesh using triangle list).
 * Koch snowflake: https://en.wikipedia.org/wiki/Koch_snowflake
 */

Fractal::Fractal() {
    //vertices of the base triangle
    std::vector<glm::vec3> base;

    //vertices of the fractal triangles
    std::vector<glm::vec3> points;

    //all color options
    std::vector<glm::vec4> colors;
}

//destructor
Fractal::~Fractal() {

}

/*
 Generate snowflake mesh. Size randomly generated.
 Parameters: mesh -> mesh to add snowflake vertices to
 */
void Fractal::GenerateSnowflake(Engine::Mesh *mesh, int level) {
    colors.push_back(glm::vec4(0.3f, 0.6f, 0.9f, 1.0f));
    colors.push_back(glm::vec4(0.4f, 0.7f, 0.1f, 1.0f));
    colors.push_back(glm::vec4(0.5f, 0.8f, 0.2f, 1.0f));
    colors.push_back(glm::vec4(0.6f, 0.3f, 0.9f, 1.0f));
    colors.push_back(glm::vec4(0.7f, 0.1f, 0.4f, 1.0f));
    colors.push_back(glm::vec4(0.4f, 0.4f, 0.8f, 1.0f));
    colors.push_back(glm::vec4(0.9f, 0.3f, 0.6f, 1.0f));
    colors.push_back(glm::vec4(0.1f, 0.4f, 0.7f, 1.0f));
    colors.push_back(glm::vec4(1.0f, 0.8f, 0.3f, 1.0f));

    //Create base triangle of random size
    CreateBase(0.5f + ((rand() - (RAND_MAX / 2)) % 30) / 100.0f);

    GenerateSnowflakeRecursively(level);

    //Add fractal triangles to a mesh. Select a color randomly.
    FormMesh(mesh, colors[rand() % 9]);
}

/*
 Generate fractal triangles for each side of base triangle.
 Parameters: levels -> how many levels to generate fractals
 */
void Fractal::GenerateSnowflakeRecursively(int levels) {
    for (int i = 0; i < base.size(); i++) {
        SnowflakeRecursion(base[i % base.size()], base[(i + 1) % base.size()], levels);
    }
}

/*
 Recursive function for fractal triangles.
 Parameters:
 pt1 -> starting point of an edge
 pt2 -> ending point of an edge
 level -> remaining levels to recurs
 */
void Fractal::SnowflakeRecursion(glm::vec3 pt1, glm::vec3 pt2, int level) {
//    printf("level: %d\n", level);
    if (level > 0) {
        std::vector<glm::vec3> triangle = CalculatePoints(pt1, pt2);
        points.push_back(triangle[1]);
        points.push_back(triangle[2]);
        points.push_back(triangle[3]);
        for (int i = 0; i < triangle.size() - 1; i++) {
            SnowflakeRecursion(triangle[i], triangle[i + 1], level - 1);
        }
    }
}

/*
 Calculate points of the fractal triangle on an edge.
 Parameters:
 pt1 -> starting point of an edge
 pt2 -> ending point of an edge
 */
std::vector<glm::vec3> Fractal::CalculatePoints(glm::vec3 pt1, glm::vec3 pt2) {
    std::vector<glm::vec3> result;
    result.push_back(pt1);
    result.push_back(pt1 + (1 / 3.0f) * (pt2 - pt1));
    glm::vec3 perpendicular = glm::rotate(pt2 - pt1, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glm::vec3 center = (pt2 + pt1) * 0.5f;
    result.push_back(center + glm::normalize(perpendicular) * glm::length(center - result[1]) * (float) sqrt(3));
    result.push_back(pt1 + (2 / 3.0f) * (pt2 - pt1));
    result.push_back(pt2);
    return result;
}

/*
 Create the base triangle of the snowflake.
 Parameters: size -> length of one edge of the triangle.
 */
void Fractal::CreateBase(float size) {
    float yOffset = size * sqrt(3) / 6.0f;
    glm::vec3 pt1 = glm::vec3(-size / 2.0f, -yOffset, 0.0f);
    glm::vec3 pt2 = glm::vec3(size / 2.0f, -yOffset, 0.0f);
    base.push_back(pt1);
    glm::vec3 perpendicular = glm::rotate(pt2 - pt1, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glm::vec3 center = (pt2 + pt1) * 0.5f;
    base.push_back(center + glm::normalize(perpendicular) * glm::length(center - base[0]) * (float) sqrt(3));
    base.push_back(pt2);

    points.push_back(base[0]);
    points.push_back(base[1]);
    points.push_back(base[2]);
}

/*
 Add the fractal triangles to the mesh.
 Parameters:
 mesh -> mesh to add background vertices to
 color -> color used to paint the snowflake
 */
void Fractal::FormMesh(Engine::Mesh *mesh, glm::vec4 color) {
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);
    for (int i = 0; i < points.size() - 2; i = i + 3) {
        glm::vec3 position[] = {
                points[i + 2],
                points[i + 1],
                points[i]
        };
//        printf("%s %s %s\n", glm::to_string(position[2]).c_str(), glm::to_string(position[1]).c_str(), glm::to_string(position[0]).c_str());
        for (int i = 0; i < 3; i++) {
            mesh->AddVertexData(position[i]);
            mesh->AddVertexData(1.0f);
            mesh->AddVertexData(color);
        }
    }
    mesh->SetNumElements(points.size());
    mesh->CreateMesh();
}
