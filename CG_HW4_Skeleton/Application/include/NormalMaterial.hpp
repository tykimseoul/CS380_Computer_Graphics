#pragma once

#include <Light.hpp>
#include <Material.hpp>

class NormalMaterial : public Engine::Material
{
private:
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    void ApplyNight(int unit);
public:
    void CreateMaterial(int unit);
    void ApplyNormal(int unit);
    void IsEarth(int isEarth, int nightUnit);
    void UpdateDiffuseReflectance(glm::vec3 color);
    void ApplyShadow(int unit);
    void UpdateLight(std::vector<Light> &lights, glm::vec3 atPosition);
};
