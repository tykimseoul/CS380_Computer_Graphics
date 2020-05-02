#pragma once

#include <glm/glm.hpp>

#include <Transform.hpp>

#define MAX_LIGHTS 4
// Feel free to increase maximum number of lights!

enum LightType{
	PointLight = 1,
	DirectionalLight = 2,
	SpotLight = 3,
};

struct Light{
	// TODO: support spotlight and (point light or directional light)
	// Note - you also need to modify struct Light in .glsl files!
	LightType type;
	int enabled = 1;		// Your light should be able to turn on/off independently!

	Engine::Transform transform;
	glm::vec3 diffuse_illuminance;
    glm::vec3 ambient_illuminance;
    glm::vec3 specular_illuminance;
	glm::vec3 direction;
	float cutoffAngle;
	float spotExponent = 50.0f;
	float constantAttenuation = 0.01;
	float linearAttenuation =0.1;
	float quadraticAttenuation =0.01;
};
