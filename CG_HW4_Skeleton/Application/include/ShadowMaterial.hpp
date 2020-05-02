#pragma once

#include <Material.hpp>
#include "NormalMaterial.hpp"

class ShadowMaterial : public Engine::Material
{
private:
    //TODO
public:
    void CreateMaterial();
    void UpdateLightSpace(std::vector<Light> &lights, glm::vec3 atPosition);
};
