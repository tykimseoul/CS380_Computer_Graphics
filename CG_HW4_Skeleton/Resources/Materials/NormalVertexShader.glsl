#version 330 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texCoordIn;
layout(location = 3) in vec4 tangent;

uniform mat4 worldTransform;
uniform mat4 cameraTransform;
uniform mat4 projectionMatrix;
uniform vec4 color;
uniform mat4 lightSpaceMatrix;

out vec4 fragmentPosition;
out vec4 frag_color;
out vec4 fragmentNormal;
out vec2 texCoord;
out mat3 TBN;
out vec4 fragmentPositionLightSpace;
out mat4 mvm;

mat4 NormalMatrix(mat4 MVM){
    mat4 invm = inverse(MVM);
    invm[3][0] = 0;
    invm[3][1] = 0;
    invm[3][2] = 0;

    return transpose(invm);
}

//calculate TBN matrix from normal and tangent vectors
mat3 TBNMatrix(vec4 normalVector, vec4 tangentVector){
    vec3 T = normalize(vec3(worldTransform * tangentVector));
    vec3 N = normalize(vec3(worldTransform * normalVector));
    vec3 B = normalize(cross(N, T));
    return mat3(T, B, N);
}

void main(){
    texCoord = texCoordIn;
    // Output position of the vertex, in clip space : MVP * position
    mat4 MVM = inverse(cameraTransform) * worldTransform;
    mat4 NVM = NormalMatrix(MVM);

    vec4 wPosition = MVM * vec4(pos);
    fragmentPosition = wPosition;
    fragmentNormal = NVM * normal;
    gl_Position = projectionMatrix * wPosition;

    frag_color = color;
    TBN = TBNMatrix(normal, tangent);

    fragmentPositionLightSpace = lightSpaceMatrix * worldTransform *pos;
    mvm=MVM;
}
