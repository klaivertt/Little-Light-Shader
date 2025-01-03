#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define SCREEN_W 1400
#define SCREEN_H 900
#define BPP 32
int main()
{
    // Create the render window
    sfRenderWindow* window = sfRenderWindow_create((sfVideoMode) {SCREEN_W, SCREEN_H, BPP}, "Light Shader", sfResize | sfClose, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create window.\n");
        return EXIT_FAILURE;
    }
    sfClock* clock = sfClock_create();

    sfEvent event;

    // Create a fullscreen rectangle shape
    sfRectangleShape* rect = sfRectangleShape_create();
    if (!rect)
    {
        fprintf(stderr, "Failed to create rectangle shape.\n");
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }
    sfRectangleShape_setSize(rect, (sfVector2f) { SCREEN_W, SCREEN_H});
    sfRectangleShape_setPosition(rect, (sfVector2f) { 0, 0 });

    // Load the shader from file
    sfShader* shader = sfShader_createFromFile(NULL, NULL, "Light.frag");
    if (!shader)
    {
        fprintf(stderr, "Failed to load shader.\n");
        sfRectangleShape_destroy(rect);
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }

    // Set static shader uniforms
    sfShader_setFloatUniform(shader, "radius", 150.0f);
    sfShader_setVec3Uniform(shader, "lightColor", (sfGlslVec3) { 1.0, 0.8, 0.7 });

    // Main loop
    while (sfRenderWindow_isOpen(window))
    {
        // Handle events
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfTime elapsed = sfClock_getElapsedTime(clock);
        float timeInSeconds = sfTime_asSeconds(elapsed);

        // Set dynamic uniforms
        sfShader_setFloatUniform(shader, "screenHeight", SCREEN_H);
        sfShader_setFloatUniform(shader, "time", timeInSeconds);

        // Get the mouse position and pass it to the shader
        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
        sfShader_setVec2Uniform(shader, "lightPos", (sfGlslVec2) { mousePos.x, mousePos.y });

        // Clear the screen
        sfRenderWindow_clear(window, sfBlack);

        // Draw the rectangle with the shader
        sfRenderWindow_drawRectangleShape(window, rect, 
            &(sfRenderStates){.shader = shader,.blendMode = sfBlendAlpha,.transform = sfTransform_Identity,.texture = NULL}
        );

        // Display the rendered frame
        sfRenderWindow_display(window);
    }

    // Cleanup resources
    sfShader_destroy(shader);
    sfRectangleShape_destroy(rect);
    sfRenderWindow_destroy(window);

    return EXIT_SUCCESS;
}