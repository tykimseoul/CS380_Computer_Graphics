#pragma once

#include <Material.hpp>
#include <Light.hpp>

class MyMaterial : public Engine::Material{
private:
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
public:
    void CreateMaterial();
    void UpdateAmbientReflectance(glm::vec3 color);
    void UpdateDiffuseReflectance(glm::vec3 color);
    void UpdateSpecularReflectance(glm::vec3 color);
    void UpdateCameraPosition(glm::vec3 position);
    void UpdateShininess(float shininess);
    void UpdateLight(std::vector<Light> &lights);
};
