#include <DiffuseMaterial.hpp>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define BUF_SIZE 50

void DiffuseMaterial::CreateMaterial(int unit) {
//    _program->AttachShader("Resources\\Materials\\VertexShader.glsl", GL_VERTEX_SHADER);
//    _program->AttachShader("Resources\\Materials\\DiffuseFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->AttachShader("./Resources/Materials/VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("./Resources/Materials/DiffuseFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();

    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "diffuse_reflectance");
    glUniform3fv(location, 1, (float *) &color);
    location = glGetUniformLocation(_program->GetProgramId(), "ourTexture");
    glUniform1i(location, unit);
}

void DiffuseMaterial::UpdateDiffuseReflectance(glm::vec3 color) {
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "diffuse_reflectance");
    glUniform3fv(location, 1, (float *) &color);
}

void DiffuseMaterial::UpdateLight(std::vector<Light> &lights) {
    GLuint pid = _program->GetProgramId();
    glUseProgram(pid);

    int numLights = MIN(lights.size(), MAX_LIGHTS);
    GLuint location = glGetUniformLocation(pid, "numLights");
    glUniform1i(location, numLights);

    char buf[BUF_SIZE];
    for (int i = 0; i < numLights; i++) {
        snprintf(buf, BUF_SIZE, "lights[%d].diffuse_illuminance", i);
        location = glGetUniformLocation(pid, buf);
        glUniform3fv(location, 1, (float *) &(lights[i].diffuse_illuminance));

        glm::mat4 world_transform = lights[i].transform.GetWorldTransform();
        glm::vec4 local_pos = glm::vec4(0.0, 0.0, 0.0, 1.0);
        glm::vec4 world_pos = world_transform * local_pos;
        snprintf(buf, BUF_SIZE, "lights[%d].pos", i);
        location = glGetUniformLocation(pid, buf);
        glUniform3fv(location, 1, (float *) &(world_pos));

        glm::vec4 local_dir = glm::vec4(0.0, 0.0, 1.0, 0.0);
        glm::vec4 world_dir = world_transform * local_dir;
        snprintf(buf, BUF_SIZE, "lights[%d].dir", i);
        location = glGetUniformLocation(pid, buf);
        glUniform3fv(location, 1, (float *) &(world_dir));

        snprintf(buf, BUF_SIZE, "lights[%d].type", i);
        location = glGetUniformLocation(pid, buf);
        glUniform1iv(location, 1, (int *) &(lights[i].type));

        snprintf(buf, BUF_SIZE, "lights[%d].enabled", i);
        location = glGetUniformLocation(pid, buf);
        glUniform1iv(location, 1, (int *) &(lights[i].enabled));

    }
}

void DiffuseMaterial::ApplyDepth(int unit) {
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "depthMap");
    glUniform1i(location, unit);
}
