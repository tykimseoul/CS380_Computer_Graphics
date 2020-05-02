#version 330 core

in vec4 fragmentPosition;
in vec4 frag_color;
in vec4 fragmentNormal;
in vec2 texCoord;
in mat3 TBN;
in vec4 fragmentPositionLightSpace;
in mat4 mvm;

//textures
uniform sampler2D dayTexture;
uniform sampler2D normalMap;
uniform sampler2D nightTexture;
uniform sampler2D shadowMap;
uniform int isEarth;
out vec4 output_color;

struct Light{
    int type;
    vec3 diffuse_illuminance;
    vec3 pos;
    vec3 dir;
    int enabled;
};

uniform mat4 cameraTransform;
uniform Light lights[3];

uniform vec3 diffuse_reflectance;

float ShadowCalculation(vec4 fragPosLightSpace){
    // perspective division
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // to [0,1] range
    projCoords = projCoords * 0.5 + vec3(0.5);
    // get closest depth value from light's perspective
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth < closestDepth-0.328  ? 0.9 : 0.1;

    return shadow;
}

void main(){
    vec3 intensity = vec3(0);
    //get normal value from the normal map image
    //then calculate normal value using TBN matrix
    vec3 normal = texture(normalMap, texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);

    mat4 worldToCamera = inverse(cameraTransform);
    if (lights[0].enabled == 1){
        vec3 tolight = vec3(0);
        vec4 pos = worldToCamera * vec4(lights[0].pos, 1);
        vec3 dir = -normalize((worldToCamera * vec4(lights[0].dir, 0)).xyz);
        if (lights[0].type == 1){
            tolight = normalize(pos.xyz - fragmentPosition.xyz);
        }
        else if (lights[0].type == 2){
            tolight = dir;
        }

        float diffuse = max(0, dot(normal, tolight));


        intensity += diffuse_reflectance * lights[0].diffuse_illuminance * diffuse;
    }

    // if it is Earth, the color is the weighted average of day and night textures
    // otherwise, use the day texture
    float shadow = ShadowCalculation(fragmentPositionLightSpace);
    if (isEarth==1){
        output_color = shadow*vec4(intensity, 1) * texture(dayTexture, texCoord) + (1-shadow)*vec4(1.0-intensity, 1) * texture(nightTexture, texCoord);
    } else {
        output_color = vec4(intensity, 1) * texture(dayTexture, texCoord);
    output_color = shadow*output_color;
    }
    output_color.rgb = pow(output_color.rgb, vec3(1.0/2.2));


}
