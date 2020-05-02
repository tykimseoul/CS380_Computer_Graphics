//
//  Sphere.cpp
//  App
//
//  Created by 김태윤 on 28/03/2019.
//

#include "Sphere.hpp"


Sphere::Sphere(Engine::Mesh *mesh, Engine::Material *material, float r) : PickableObject(mesh, material) {
    std::vector<glm::vec3> points = std::vector<glm::vec3>();
    SetRadius(r);
    FormMesh(mesh);
}

/*
 Form mesh of a sphere
 Parameters:
 mesh -> mesh to add vertices to
 */
void Sphere::FormMesh(Engine::Mesh *mesh) {
    int azimuthalSections = 30;
    int polarSections = azimuthalSections * 2;
    //2D vector of vertices
    std::vector<std::vector<glm::vec3> > shell = std::vector<std::vector<glm::vec3> >();

    mesh->AddAttribute(4);
    mesh->AddAttribute(4);

    //double for-loop that adds vertices to shell vector
    for (float azimuthalAngle = 0; azimuthalAngle <= 180; azimuthalAngle = azimuthalAngle + 180 / azimuthalSections) {
        std::vector<glm::vec3> layer = std::vector<glm::vec3>();
        for (float polarAngle = 0; polarAngle < 360; polarAngle = polarAngle + 360 / polarSections) {
//            printf("theta: %f, phi: %f, sphere: %s\n", azimuthalAngle, polarAngle,
//                   glm::to_string(glm::vec3(radius * sin(glm::radians(azimuthalAngle)) * cos(glm::radians(polarAngle)), radius * cos(glm::radians(azimuthalAngle)),
//                                            radius * sin(glm::radians(azimuthalAngle)) * sin(glm::radians(polarAngle)))).c_str());

            layer.push_back(glm::vec3(radius * sin(glm::radians(azimuthalAngle)) * cos(glm::radians(polarAngle)), radius * cos(glm::radians(azimuthalAngle)),
                                      radius * sin(glm::radians(azimuthalAngle)) * sin(glm::radians(polarAngle))));
        }
        shell.push_back(layer);
    }

    // add vertices from shell to points vector in counter-clockwise order
    for (int az = 0; az < shell.size() - 1; az++) {
        for (int po = 0; po < shell[0].size(); po++) {
            FillQuad(shell[az][po], shell[az + 1][po], shell[az + 1][(po + 1) % polarSections], shell[az][(po + 1) % polarSections]);
        }
    }

    // add vertex data to mesh
    auto *geometry = new Geometry();
    for (int i = 0; i < points.size() - 2; i = i + 3) {
        glm::vec3 position[] = {
                points[i + 2],
                points[i + 1],
                points[i]
        };
        glm::vec3 normal = geometry->ComputeNormal(glm::vec4(position[0], 0.0f), glm::vec4(position[1], 0.0f), glm::vec4(position[2], 0.0f));

//        printf("%d %s %s %s\n", i, glm::to_string(position[0]).c_str(), glm::to_string(position[1]).c_str(), glm::to_string(position[2]).c_str());
//        printf("normal: %s\n", glm::to_string(normal).c_str());
        for (auto x : position) {
            mesh->AddVertexData(x);
            mesh->AddVertexData(1.0f);
            mesh->AddVertexData(normal);
            mesh->AddVertexData(1.0f);
        }
    }

    mesh->SetNumElements(points.size()*20);
    mesh->CreateMesh();
}

void Sphere::FillQuad(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4) {
//    printf("fill %s %s %s %s\n", glm::to_string(v1).c_str(), glm::to_string(v2).c_str(), glm::to_string(v3).c_str(), glm::to_string(v4).c_str());
    points.push_back(v1);
    points.push_back(v2);
    points.push_back(v3);
    points.push_back(v1);
    points.push_back(v3);
    points.push_back(v4);
}

void Sphere::SetRadius(float r) {
    radius = r;
}
