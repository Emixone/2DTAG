#include <math.h>
#include <stdio.h>
#include "../client/entity.h"

int main(void)
{
    // Arrange
    Entity e1 = {0};
    Entity e2 = {0};
    e1.direction.x = 1;
    e1.direction.y = 0;
    e2.direction.x = 1;
    e2.direction.y = 1;

    // Act
    Entity_move(&e1);
    Entity_move(&e2);

    // Assert
    float e1Distance = sqrt(powf(e1.position.x, 2.f) + powf(e1.position.y, 2.f));
    float e2Distance = sqrt(powf(e2.position.x, 2.f) + powf(e2.position.y, 2.f));

    // Ustalamy 0.0001 jako precyzje. Floatow nie mozemy porownywac dokladnie, z racji ze
    // z natury sa nieprecyzyjne.
    if ((e1Distance - e2Distance) > 0.0001) {
        fprintf(stderr, "Test failed: expected e1Distance to be equal to e2Distance, but got: %f %f\n", e1Distance, e2Distance);
        return 1;
    }
    else {
        printf("Success!\n");
        return 0;
    }
}
