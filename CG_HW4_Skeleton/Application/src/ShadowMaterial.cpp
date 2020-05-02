#include <ShadowMaterial.hpp>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define BUF_SIZE 50
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/string_cast.hpp>

//TODO
void ShadowMaterial::CreateMaterial() {
    _program->AttachShader("./Resources/Materials/ShadowVertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("./Resources/Materials/ShadowFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();
}

void ShadowMaterial::UpdateLightSpace(std::vector<Light> &lights, glm::vec3 atPosition) {
    GLuint pid = _program->GetProgramId();
    glUseProgram(pid);

    char buf[BUF_SIZE];
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 10.0f);
    glm::mat4 lightView = glm::lookAt(-lights[0].transform.GetPosition(),
                                      atPosition,
                                      glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
//        printf("%s\n", glm::to_string(lightSpaceMatrix).c_str());
    GLuint location = glGetUniformLocation(pid, "lightSpaceMatrix");
    glUniformMatrix4fv(location, 1, GL_FALSE, (float *) &lightSpaceMatrix);
}


