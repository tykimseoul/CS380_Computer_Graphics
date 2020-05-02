#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext.hpp>
#include <vector>
#include <Snowflake.hpp>
#include <cstdlib>

/*
 * Animation class supports continuous animation by changing RenderObject's transform with elasped time in seconds
 */

class Animation {
private:
    std::vector<Snowflake *> snowflakes;
public:
    Animation();
    
    ~Animation();
    
    void AddSnowflake(Snowflake *snowflake);
    
    void ClearSnowflake();
    
    void Animate(double deltaTime, Engine::Camera camera);
    
    Snowflake *AnimateSnowflake(Snowflake *snowflake, float deltaTime);
};
