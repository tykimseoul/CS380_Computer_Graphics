#include <ToonMaterial.hpp>
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define BUF_SIZE 50
void ToonMaterial::CreateMaterial(){
    _program->AttachShader("./Resources/Materials/VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("./Resources/Materials/ToonFragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();

    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "diffuse_reflectance");
    glUniform3fv(location, 1, (float *) &color);
}

void ToonMaterial::UpdateAmbientReflectance(glm::vec3 color){
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "ambient_reflectance");
    glUniform3fv(location, 1, (float *) &color);
}

void ToonMaterial::UpdateDiffuseReflectance(glm::vec3 color){
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "diffuse_reflectance");
    glUniform3fv(location, 1, (float *) &color);
}

void ToonMaterial::UpdateSpecularReflectance(glm::vec3 color){
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "specular_reflectance");
    glUniform3fv(location, 1, (float *) &color);
}

void ToonMaterial::UpdateCameraPosition(glm::vec3 position) {
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "camera_position");
    glUniform3fv(location, 1, (float *) &position);
}

void ToonMaterial::UpdateShininess(float shininess) {
    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "shininess");
    glUniform1f(location, shininess);
}

void ToonMaterial::UpdateLight(std::vector<Light> &lights){
    GLuint pid = _program->GetProgramId();
    glUseProgram(pid);

    int numLights = MIN(lights.size(), MAX_LIGHTS);
    GLuint location = glGetUniformLocation(pid, "numLights");
    glUniform1i(location, numLights);

    char buf[BUF_SIZE];
    for (int i = 0; i < numLights; i++) {
        Light light = lights[i];
        if (light.enabled == 1) {
            snprintf(buf, BUF_SIZE, "lights[%d].diffuse_illuminance", i);
            location = glGetUniformLocation(pid, buf);
            glUniform3fv(location, 1, (float *) &(light.diffuse_illuminance));

            snprintf(buf, BUF_SIZE, "lights[%d].ambient_illuminance", i);
            location = glGetUniformLocation(pid, buf);
            glUniform3fv(location, 1, (float *) &(light.ambient_illuminance));

            snprintf(buf, BUF_SIZE, "lights[%d].specular_illuminance", i);
            location = glGetUniformLocation(pid, buf);
            glUniform3fv(location, 1, (float *) &(light.specular_illuminance));

            glm::mat4 world_transform = light.transform.GetWorldTransform();
            glm::vec4 local_pos = glm::vec4(0.0, 0.0, 0.0, 1.0);
            glm::vec4 world_pos = world_transform * local_pos;
            snprintf(buf, BUF_SIZE, "lights[%d].pos", i);
            location = glGetUniformLocation(pid, buf);
            glUniform3fv(location, 1, (float *) &(world_pos));

            snprintf(buf, BUF_SIZE, "lights[%d].type", i);
            location = glGetUniformLocation(pid, buf);
            glUniform1i(location, light.type);

            snprintf(buf, BUF_SIZE, "lights[%d].constantAttenuation", i);
            location = glGetUniformLocation(pid, buf);
            glUniform1f(location, light.constantAttenuation);

            snprintf(buf, BUF_SIZE, "lights[%d].linearAttenuation", i);
            location = glGetUniformLocation(pid, buf);
            glUniform1f(location, light.linearAttenuation);

            snprintf(buf, BUF_SIZE, "lights[%d].quadraticAttenuation", i);
            location = glGetUniformLocation(pid, buf);
            glUniform1f(location, light.quadraticAttenuation);

            if (light.type == SpotLight) {
                snprintf(buf, BUF_SIZE, "lights[%d].direction", i);
                location = glGetUniformLocation(pid, buf);
                glUniform3fv(location, 1, (float *) &(light.direction));

                snprintf(buf, BUF_SIZE, "lights[%d].cutoffAngle", i);
                location = glGetUniformLocation(pid, buf);
                glUniform1f(location, light.cutoffAngle);

                snprintf(buf, BUF_SIZE, "lights[%d].spotExponent", i);
                location = glGetUniformLocation(pid, buf);
                glUniform1f(location, light.spotExponent);
            }
        }

    }
}
