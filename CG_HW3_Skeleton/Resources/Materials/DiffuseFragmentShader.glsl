#version 330 core

in vec4 fragmentPosition;
in vec4 fragmentNormal;
out vec4 output_color;

struct Light{
    int type;
    vec3 diffuse_illuminance;
    vec3 pos;
    vec3 direction;
    float cutoffAngle;
    float spotExponent;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

uniform mat4 cameraTransform;
uniform int numLights;
uniform Light lights[4];

uniform vec3 diffuse_reflectance;

void main(){
    vec3 intensity = vec3(0);
    vec3 normal = normalize(fragmentNormal.xyz);
    vec4 lightPosition;
    vec3 tolight;
    float diffuse;
    float d;
    float attenuation;
    float spotAttenuation;
    mat4 worldToCamera = inverse(cameraTransform);
    for (int i=0; i<numLights; i++){
        Light light=lights[i];
        lightPosition = worldToCamera * vec4(light.pos, 1);
        tolight = lightPosition.xyz - fragmentPosition.xyz;
        d = length(tolight);
        tolight = normalize(tolight);
        attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
        switch (light.type){
            case 1://point
            break;
            case 2://direction
            return;
            case 3://spot
            float dDotL = dot(light.direction, -tolight);
            if (dDotL > cos(light.cutoffAngle)){
                spotAttenuation = pow(dDotL, light.spotExponent);
            } else {
                spotAttenuation=0.0f;
            }
            attenuation *= spotAttenuation;
            break;
        }
        diffuse = max(0, dot(normal, tolight));
        intensity += diffuse_reflectance * light.diffuse_illuminance * diffuse * attenuation;
    }
    output_color = vec4(intensity, 1);
    output_color.rgb = pow(output_color.rgb, vec3(1.0/2.2));
}
