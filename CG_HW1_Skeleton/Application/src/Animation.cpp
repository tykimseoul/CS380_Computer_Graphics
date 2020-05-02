#include <Animation.hpp>

Animation::Animation() {
    //all snowflakes to animate
    snowflakes = std::vector<Snowflake *>();
}

Animation::~Animation() {
    snowflakes.clear();
}

/*
 Add a new snowflake to the list
 Parameters: snowflake -> snowflake object to add
 */
void Animation::AddSnowflake(Snowflake *snowflake) {
    snowflakes.push_back(snowflake);
}

/*
 Clear the list of snowflakes
 */
void Animation::ClearSnowflake(){
    snowflakes.clear();
}

/*
 Animate all snowflakes.
 Parameters:
 deltaTime -> time difference for calculating new position.
 camera -> camera to render for
 */
void Animation::Animate(float deltaTime, Engine::Camera camera) {
    for (auto snowflake:snowflakes) {
        AnimateSnowflake(snowflake, deltaTime)->Render(&camera);
        if (snowflake->GetPosition().y < -5) {
            snowflake->ResetPosition();
        }
    }
}

/*
 Animate a single snowflake.
 Parameters:
 snowflake -> snowflake object to animate.
 deltaTime -> time difference for calculating new position.
 */
Snowflake *Animation::AnimateSnowflake(Snowflake *snowflake, float deltaTime) {
    glm::vec3 currentPosition = snowflake->GetPosition();
    glm::mat4 currentOrientation = snowflake->GetOrientation();
    snowflake->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    snowflake->SetOrientation(glm::mat4(1.0f));
    float direction = ((rand() - RAND_MAX / 2) % 10) / 2.0f;
    float angularVelocity = 100.0f;

    //rotation animation
    // rotation should consider deltaTime
    snowflake->SetOrientation(glm::rotate(currentOrientation, glm::radians(deltaTime * angularVelocity), glm::vec3(0, 0, 1)));

    //fall downward and swing in random direction
    snowflake->SetPosition(currentPosition + deltaTime * glm::vec3(direction, -0.5f, 0.0f));
    return snowflake;
}
