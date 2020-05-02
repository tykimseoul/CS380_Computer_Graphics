//
//  Cone.cpp
//  App
//
//  Created by 김태윤 on 28/03/2019.
//

#include <Cone.hpp>


Cone::Cone(Engine::Mesh *mesh, Engine::Material *material, float r, float h) : Engine::RenderObject(mesh, material) {
    std::vector<glm::vec3> points = std::vector<glm::vec3>();
    FormMesh(mesh, r, h);
}

/*
 Form mesh of a cone
 Parameters:
 mesh -> mesh to add vertices to
 r -> radius of the cone
 h -> height of the cone
 */
void Cone::FormMesh(Engine::Mesh *mesh, float r, float h) {
    SetSize(r, h);
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);
    int sections = 40;
    glm::vec3 center = glm::vec3(0, 0, 0);
    glm::vec3 apex = glm::vec3(0, height, 0);
    std::vector<glm::vec3> circumference = std::vector<glm::vec3>();
    for (float angle = 0; angle <= 360; angle = angle + 360 / sections) {
        circumference.push_back(glm::vec3(radius * cos(glm::radians(angle)), 0.0f, radius * sin(glm::radians(angle))));
    }

    for (int idx = 0; idx < circumference.size() - 1; idx++) {
        points.push_back(center);
        points.push_back(circumference[idx]);
        points.push_back(circumference[idx + 1]);
        points.push_back(apex);
        points.push_back(circumference[idx]);
        points.push_back(circumference[idx + 1]);
    }

    auto *geometry = new Geometry();
    for (int i = 0; i < points.size() - 2; i = i + 3) {
        glm::vec3 position[] = {
                points[i + 2],
                points[i + 1],
                points[i]
        };
        glm::vec3 normal = geometry->ComputeNormal(glm::vec4(position[0], 0.0f), glm::vec4(position[1], 0.0f), glm::vec4(position[2], 0.0f));

//        printf("%d %s %s %s\n", i, glm::to_string(position[0]).c_str(), glm::to_string(position[1]).c_str(), glm::to_string(position[2]).c_str());
        for (int j = 0; j < 3; j++) {
            mesh->AddVertexData(position[j]);
            mesh->AddVertexData(1.0f);
            mesh->AddVertexData(normal);
            mesh->AddVertexData(1.0f);
        }
    }

    mesh->SetNumElements(points.size());
    mesh->CreateMesh();
}

void Cone::SetSize(float r, float h) {
    radius = r;
    height = h;
}
