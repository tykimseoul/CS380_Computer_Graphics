#include <NormalMaterial.hpp>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define BUF_SIZE 50

void NormalMaterial::CreateMaterial(int unit) {
    _program->AttachShader("./Resources/Materials/NormalVertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("./Resources/Materials/NormalFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();

    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "diffuse_reflectance");
    glUniform3fv(location, 1, (float *) &color);
    location = glGetUniformLocation(_program->GetProgramId(), "dayTexture");
    glUniform1i(location, unit);
}

//pass normal map to the fragment shader
void NormalMaterial::ApplyNormal(int unit) {
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "normalMap");
    glUniform1i(location, unit);
}

//pass night texture to the fragment shader
void NormalMaterial::ApplyNight(int unit) {
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "nightTexture");
    glUniform1i(location, unit);
}

//set if there is a night texture to this material
void NormalMaterial::IsEarth(int isEarth, int nightUnit) {
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "isEarth");
    glUniform1i(location, isEarth);
    if (isEarth == 1) {
        ApplyNight(nightUnit);
    }
}

void NormalMaterial::UpdateDiffuseReflectance(glm::vec3 color) {
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "diffuse_reflectance");
    glUniform3fv(location, 1, (float *) &color);
}

void NormalMaterial::ApplyShadow(int unit) {
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "shadowMap");
    glUniform1i(location, unit);
}

void NormalMaterial::UpdateLight(std::vector<Light> &lights, glm::vec3 atPosition) {
    GLuint pid = _program->GetProgramId();
    glUseProgram(pid);

    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 10.0f);
    glm::mat4 lightView = glm::lookAt(-lights[0].transform.GetPosition(),
                                      atPosition,
                                      glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
    GLuint location = glGetUniformLocation(pid, "lightSpaceMatrix");
    glUniformMatrix4fv(location, 1, GL_FALSE, (float *) &lightSpaceMatrix);

    char buf[BUF_SIZE];
    snprintf(buf, BUF_SIZE, "lights[%d].diffuse_illuminance", 0);
    location = glGetUniformLocation(pid, buf);
    glUniform3fv(location, 1, (float *) &(lights[0].diffuse_illuminance));

    glm::mat4 world_transform = lights[0].transform.GetWorldTransform();
    glm::vec4 local_pos = glm::vec4(0.0, 0.0, 0.0, 1.0);
    glm::vec4 world_pos = world_transform * local_pos;
    snprintf(buf, BUF_SIZE, "lights[%d].pos", 0);
    location = glGetUniformLocation(pid, buf);
    glUniform3fv(location, 1, (float *) &(world_pos));

    glm::vec4 local_dir = glm::vec4(0.0, 0.0, 1.0, 0.0);
    glm::vec4 world_dir = world_transform * local_dir;
    snprintf(buf, BUF_SIZE, "lights[%d].dir", 0);
    location = glGetUniformLocation(pid, buf);
    glUniform3fv(location, 1, (float *) &(world_dir));

    snprintf(buf, BUF_SIZE, "lights[%d].type", 0);
    location = glGetUniformLocation(pid, buf);
    glUniform1iv(location, 1, (int *) &(lights[0].type));

    snprintf(buf, BUF_SIZE, "lights[%d].enabled", 0);
    location = glGetUniformLocation(pid, buf);
    glUniform1iv(location, 1, (int *) &(lights[0].enabled));

}
