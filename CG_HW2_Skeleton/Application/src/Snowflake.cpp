#include <Snowflake.hpp>

/*
 Constructor for Snowflake object. Randomly position in the world.
 Parameters:
 mesh -> mesh to render
 material -> material to use
 */
Snowflake::Snowflake(Engine::Mesh *mesh, Engine::Material *material) : Engine::RenderObject(mesh, material) {
    ResetPosition();
}

// Add more functions
void Snowflake::ResetPosition() {
    float position = ((rand() - (RAND_MAX / 2)) % 50) / 10.0f;
    float yOffset = (rand() % 110) / 10.0f;
    float zOffset = (rand() % 10) / 10.0f;
    SetPosition(glm::vec3(position, 6.0f + yOffset, zOffset));
}
