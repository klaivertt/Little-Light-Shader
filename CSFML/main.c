#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.h>
#define _USE_MATH_DEFINES
#include <math.h>

#pragma region Define and Struct
#define SCREEN_W 1400
#define SCREEN_H 900
#define BPP 32

typedef struct MainData 
{
    sfRenderWindow* window;
    sfRectangleShape* rect;
    sfShader* shader;
    sfClock* clock;
    sfBool waterMode;
} MainData;
#pragma endregion


#pragma region Define Function 
sfBool loadResources(MainData* _data);
void update(MainData* _data);
void draw(MainData* _data);
void cleanup(MainData* _data);
#pragma endregion


#pragma region Function
int main()
{
    MainData data = { 0 };

    // Load resources
    if (!loadResources(&data))
    {
        return EXIT_FAILURE;
    }

    // Main loop
    while (sfRenderWindow_isOpen(data.window))
    {
        // Handle events
        sfEvent event;
        while (sfRenderWindow_pollEvent(data.window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(data.window);
        }

        // Update and draw
        update(&data);
        draw(&data);
    }

    // Cleanup resources
    cleanup(&data);

    return EXIT_SUCCESS;
}

// Function to load resources
sfBool loadResources(MainData* _data)
{
    // Create the render window
    _data->window = sfRenderWindow_create((sfVideoMode) {SCREEN_W, SCREEN_H, BPP},"Light Shader", sfResize | sfClose,NULL);
    if (!_data->window)
    {
        fprintf(stderr, "Failed to create window.\n");
        return sfFalse;
    }

    // Create the rectangle shape
    _data->rect = sfRectangleShape_create();
    if (!_data->rect)
    {
        fprintf(stderr, "Failed to create rectangle shape.\n");
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    sfRectangleShape_setSize(_data->rect, (sfVector2f) { SCREEN_W, SCREEN_H });
    sfRectangleShape_setPosition(_data->rect, (sfVector2f) { 0, 0 });

    // Load the shader from file
    _data->shader = sfShader_createFromFile(NULL, NULL, "Light.frag");
    if (!_data->shader)
    {
        fprintf(stderr, "Failed to load shader.\n");
        sfRectangleShape_destroy(_data->rect);
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    // Create a clock for timing
    _data->clock = sfClock_create();
    if (!_data->clock)
    {
        fprintf(stderr, "Failed to create clock.\n");
        sfShader_destroy(_data->shader);
        sfRectangleShape_destroy(_data->rect);
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    // Set static shader uniforms
    sfShader_setFloatUniform(_data->shader, "radius", 150.0f);
    sfShader_setVec3Uniform(_data->shader, "lightColor", (sfGlslVec3) { 1.0f, 0.8f, 0.7f });

    return sfTrue;
}

// Function to update uniforms
void update(MainData* _data)
{
    // Get the elapsed time
    sfTime elapsed = sfClock_getElapsedTime(_data->clock);
    float timeInSeconds = sfTime_asSeconds(elapsed);

    // Update dynamic uniforms
    sfShader_setFloatUniform(_data->shader, "screenHeight", SCREEN_H);
    sfShader_setFloatUniform(_data->shader, "time", timeInSeconds);

    // Get the mouse position and pass it to the shader
    sfVector2i mousePos = sfMouse_getPositionRenderWindow(_data->window);
    sfShader_setVec2Uniform(_data->shader, "lightPos", (sfGlslVec2) { (float)mousePos.x, (float)mousePos.y });
}

// Function to draw the frame
void draw(MainData* _data)
{
    // Clear the screen
    sfRenderWindow_clear(_data->window, sfBlack);

    // Draw the rectangle with the shader
    sfRenderWindow_drawRectangleShape(_data->window, _data->rect,
        &(sfRenderStates) {.shader = _data->shader, .blendMode = sfBlendAlpha, .transform = sfTransform_Identity, .texture = NULL }
    );

    // Display the rendered frame
    sfRenderWindow_display(_data->window);
}

// Function to clean up resources
void cleanup(MainData* _data)
{
    if (_data->shader)
    {
        sfShader_destroy(_data->shader);
    }
    if (_data->rect)
    {
        sfRectangleShape_destroy(_data->rect);
    }
    if (_data->clock)
    {
        sfClock_destroy(_data->clock);
    }
    if (_data->window)
    {
        sfRenderWindow_destroy(_data->window);
    }
}
#pragma endregion