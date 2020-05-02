//
// Created by 김태윤 on 2019-04-07.
//

#include <BlenderObject.hpp>

BlenderObject::BlenderObject(Engine::Mesh *mesh, Engine::Material *material, std::string file) : RenderObject(mesh, material) {
    points = std::vector<glm::vec3>();
    FormMesh(mesh, file);
}
/*
 Form mesh from obj file
 Parameters:
 mesh -> mesh to add vertices to
 file -> obj file name in string
 */
void BlenderObject::FormMesh(Engine::Mesh *mesh, std::string file) {
    mesh->AddAttribute(4);
    mesh->AddAttribute(4);

    auto importer = new ObjImporter();
    points = importer->ImportObj(file);

    auto *geometry = new Geometry();
    for (int i = 0; i < points.size() - 2; i = i + 3) {
        glm::vec3 position[] = {
                points[i],
                points[i + 1],
                points[i + 2]
        };
        glm::vec3 normal = geometry->ComputeNormal(glm::vec4(position[0], 0.0f), glm::vec4(position[1], 0.0f), glm::vec4(position[2], 0.0f));

        for (auto j : position) {
            mesh->AddVertexData(j);
            mesh->AddVertexData(1.0f);
            mesh->AddVertexData(normal);
            mesh->AddVertexData(1.0f);
        }
    }

    mesh->SetNumElements(points.size());
    mesh->CreateMesh();
}
