#include <PerVertexColorMaterial.hpp>

void PerVertexColorMaterial::CreateMaterial() {
    _program->AttachShader("./Resources/PerVertexColorMaterial/VertexShader.glsl", GL_VERTEX_SHADER);
    _program->AttachShader("./Resources/PerVertexColorMaterial/FragmentShader.glsl", GL_FRAGMENT_SHADER);
    _program->LinkShader();

    glUseProgram(_program->GetProgramId());
    GLuint location = glGetUniformLocation(_program->GetProgramId(), "color");
    glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    glUniform4fv(location, 1, (float *) (&color));
}
