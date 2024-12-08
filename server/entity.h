#define MAX_SPEED 0.1f
#define SPEED_INCREMENT 0.02f

typedef struct
{
    float x;
    float y;
} Vec2;

typedef struct
{
    int x;
    int y;
} Direction;

typedef struct
{
    Direction direction;
    Vec2 speed;
    Vec2 position;
} Entity;

void Entity_move(Entity *entity);
