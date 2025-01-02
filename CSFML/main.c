#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.h>
#define _USE_MATH_DEFINES
#include <math.h>

int main()
{
    // Create the render window
    sfRenderWindow* window = sfRenderWindow_create(
        (sfVideoMode) {
        800, 600, 32
    }, "Light Shader", sfResize | sfClose, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create window.\n");
        return EXIT_FAILURE;
    }

    sfEvent event;

    // Create a fullscreen rectangle shape
    sfRectangleShape* rect = sfRectangleShape_create();
    if (!rect)
    {
        fprintf(stderr, "Failed to create rectangle shape.\n");
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }
    sfRectangleShape_setSize(rect, (sfVector2f) { 800, 600 });
    sfRectangleShape_setPosition(rect, (sfVector2f) { 0, 0 });

    // Load the shader from file
    sfShader* shader = sfShader_createFromFile(NULL, NULL, "shader.frag");
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

        // Set dynamic uniforms
        sfShader_setFloatUniform(shader, "screenHeight", 600.0f);

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