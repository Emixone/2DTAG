#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include "entity.h"

void Vec2_normalize(Vec2 *vec)
{
   float squared = sqrt(powf(vec->x, 2.f) + powf(vec->y, 2.f));
    vec->x /= squared;
    vec->y /= squared;
}
float clamp(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }

}

void Entity_move(Entity *entity)
{
    // B - obliczenie predkosci
    entity->speed.x = clamp(entity->direction.x * SPEED_INCREMENT + entity->speed.x, -MAX_SPEED, MAX_SPEED);
    entity->speed.y = clamp(entity->direction.y * SPEED_INCREMENT + entity->speed.y, -MAX_SPEED, MAX_SPEED);
    /*
    entity->speed.x / sqrt((entity->speed.x * entity->speed.y) + (entity->speed.y + entity->speed.y))
    entity->speed.y / sqrt((entity->speed.x * entity->speed.y) + (entity->speed.y + entity->speed.y))
    */
    if(entity->direction.x == 0)
    {
        entity->speed.x = 0;
    }
    if(entity->direction.y == 0)
    {
        entity->speed.y = 0;
    }

    Vec2_normalize(&entity->speed);

    // C - aktualizacja pozycji
    entity->position.x += entity->speed.x;
    entity->position.y += entity->speed.y;

    
}

