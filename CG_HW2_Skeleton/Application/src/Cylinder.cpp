//
//  Cylinder.cpp
//  App
//
//  Created by 김태윤 on 02/04/2019.
//

#include "Cylinder.hpp"


Cylinder::Cylinder(Engine::Mesh *mesh, Engine::Material *material, float r, float h) : PickableObject(mesh, material) {
    std::vector<glm::vec3> points = std::vector<glm::vec3>();
    FormMesh(mesh, r, h);
}

/*
 Form mesh of a cylinder
 Parameters:
 mesh -> mesh to add vertices to
 r -> radius of the cylinder
 h -> height of the cylinder
 */
void Cylinder::FormMesh(Engine::Mesh *mesh, float r, float h) {
    SetSize(r, h);
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);
    int sections = 40;
    std::vector<std::vector<glm::vec3> > shell = std::vector<std::vector<glm::vec3> >();

    for (float i = height / 2.0; i >= -height / 2.0; i = i - height) {
        std::vector<glm::vec3> circumference = std::vector<glm::vec3>();
        for (float angle = 0; angle <= 360; angle = angle + 360 / sections) {
            circumference.push_back(glm::vec3(radius * cos(glm::radians(angle)), i, radius * sin(glm::radians(angle))));
        }
        shell.push_back(circumference);
    }

    for (int idx = 0; idx < shell[0].size() - 1; idx++) {
        points.push_back(glm::vec3(0.0, shell[0][0].y, 0.0));
        points.push_back(shell[0][idx]);
        points.push_back(shell[0][idx + 1]);
    }

    for (int idx = 0; idx < shell[1].size() - 1; idx++) {
        points.push_back(shell[1][idx + 1]);
        points.push_back(shell[1][idx]);
        points.push_back(glm::vec3(0.0, shell[1][0].y, 0.0));
    }

    for (int i = 0; i < shell.size() - 1; i++) {
        for (int idx = 0; idx < shell[i].size() - 1; idx++) {
            FillQuad(shell[i][idx], shell[i + 1][idx], shell[i + 1][idx + 1], shell[i][idx + 1]);
        }
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

void Cylinder::SetSize(float r, float h) {
    radius = r;
    height = h;
}

void Cylinder::FillQuad(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4) {
    //    printf("fill %s %s %s %s\n", glm::to_string(v1).c_str(), glm::to_string(v2).c_str(), glm::to_string(v3).c_str(), glm::to_string(v4).c_str());
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);
    points.push_back(v1);
    points.push_back(v3);
    points.push_back(v4);
}
