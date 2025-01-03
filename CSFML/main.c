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
    sfRectangleShape* rectLight;
    sfRectangleShape* rectWater;
    sfRectangleShape* rectWaterLight;
    sfShader* lightShader;
    sfShader* waterShader;
    sfShader* waterLightShader;
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
            switch (event.type)
            {
            case sfEvtClosed:
                sfRenderWindow_close(data.window);
                break;

            case sfEvtKeyPressed:
                switch (event.key.code)
                {
                case sfKeyA:
                    // Toggle water mode
                    data.waterMode = !data.waterMode; 
                    break;

                case sfKeyEscape:
                    // Close window on Escape
                    sfRenderWindow_close(data.window);
                    break;

                default:
                    break;
                }
                break;
            default:
                break;
            }
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
    _data->window = sfRenderWindow_create(
        (sfVideoMode) {
        SCREEN_W, SCREEN_H, BPP
    },
        "Light Shader",
        sfResize | sfClose,
        NULL
    );
    if (!_data->window)
    {
        fprintf(stderr, "Failed to create window.\n");
        return sfFalse;
    }

    // Create the rectangle shapes
    _data->rectLight = sfRectangleShape_create();
    if (!_data->rectLight)
    {
        fprintf(stderr, "Failed to create rectLight.\n");
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    _data->rectWater = sfRectangleShape_create();
    if (!_data->rectWater)
    {
        fprintf(stderr, "Failed to create rectWater.\n");
        sfRectangleShape_destroy(_data->rectLight);
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    _data->rectWaterLight = sfRectangleShape_create();
    if (!_data->rectWaterLight)
    {
        fprintf(stderr, "Failed to create rectWaterLight.\n");
        sfRectangleShape_destroy(_data->rectWater);
        sfRectangleShape_destroy(_data->rectLight);
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    // Set sizes and positions for rectangles
    sfRectangleShape_setSize(_data->rectLight, (sfVector2f) { SCREEN_W, SCREEN_H });
    sfRectangleShape_setPosition(_data->rectLight, (sfVector2f) { 0, 0 });

    sfRectangleShape_setSize(_data->rectWater, (sfVector2f) { SCREEN_W, SCREEN_H });
    sfRectangleShape_setPosition(_data->rectWater, (sfVector2f) { 0, 0 });

    sfRectangleShape_setSize(_data->rectWaterLight, (sfVector2f) { SCREEN_W, SCREEN_H });
    sfRectangleShape_setPosition(_data->rectWaterLight, (sfVector2f) { 0, 0 });

    // Load shaders
    _data->lightShader = sfShader_createFromFile(NULL, NULL, "Assets/Shader/Light.frag");
    if (!_data->lightShader)
    {
        fprintf(stderr, "Failed to load lightShader.\n");
        sfRectangleShape_destroy(_data->rectWaterLight);
        sfRectangleShape_destroy(_data->rectWater);
        sfRectangleShape_destroy(_data->rectLight);
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    _data->waterShader = sfShader_createFromFile(NULL, NULL, "Assets/Shader/Water.frag");
    if (!_data->waterShader)
    {
        fprintf(stderr, "Failed to load waterShader.\n");
        sfShader_destroy(_data->lightShader);
        sfRectangleShape_destroy(_data->rectWaterLight);
        sfRectangleShape_destroy(_data->rectWater);
        sfRectangleShape_destroy(_data->rectLight);
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    _data->waterLightShader = sfShader_createFromFile(NULL, NULL, "Assets/Shader/WaterLight.frag");
    if (!_data->waterLightShader)
    {
        fprintf(stderr, "Failed to load waterLightShader.\n");
        sfShader_destroy(_data->waterShader);
        sfShader_destroy(_data->lightShader);
        sfRectangleShape_destroy(_data->rectWaterLight);
        sfRectangleShape_destroy(_data->rectWater);
        sfRectangleShape_destroy(_data->rectLight);
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    // Create a clock for timing
    _data->clock = sfClock_create();
    if (!_data->clock)
    {
        fprintf(stderr, "Failed to create clock.\n");
        sfShader_destroy(_data->waterLightShader);
        sfShader_destroy(_data->waterShader);
        sfShader_destroy(_data->lightShader);
        sfRectangleShape_destroy(_data->rectWaterLight);
        sfRectangleShape_destroy(_data->rectWater);
        sfRectangleShape_destroy(_data->rectLight);
        sfRenderWindow_destroy(_data->window);
        return sfFalse;
    }

    // Set static shader uniforms
    sfShader_setFloatUniform(_data->lightShader, "radius", 150.0f);
    sfShader_setVec3Uniform(_data->lightShader, "lightColor", (sfGlslVec3) { 1.0f, 0.8f, 0.7f });
    sfShader_setFloatUniform(_data->lightShader, "screenHeight", SCREEN_H);
    
    sfShader_setVec2Uniform(_data->waterShader, "resolution", (sfGlslVec2) { SCREEN_W, SCREEN_H });

    sfShader_setFloatUniform(_data->waterLightShader, "radius", 150.0f);
    sfShader_setVec3Uniform(_data->waterLightShader, "lightColor", (sfGlslVec3) { 1.0f, 0.8f, 0.9f });
    sfShader_setFloatUniform(_data->waterLightShader, "screenHeight", SCREEN_H);

    return sfTrue;
}

// Function to update uniforms
void update(MainData* _data)
{
    // Get the elapsed time
    sfTime elapsed = sfClock_getElapsedTime(_data->clock);
    float dt = sfTime_asSeconds(elapsed);

    // Update dynamic uniforms
    sfShader_setFloatUniform(_data->waterShader, "time", dt);
    sfShader_setFloatUniform(_data->waterLightShader, "time", dt);

    // Get the mouse position and pass it to the shader
    sfVector2i mousePos = sfMouse_getPositionRenderWindow(_data->window);
    sfShader_setVec2Uniform(_data->lightShader, "lightPos", (sfGlslVec2) { (float)mousePos.x, (float)mousePos.y });
    sfShader_setVec2Uniform(_data->waterLightShader, "lightPos", (sfGlslVec2) { (float)mousePos.x, (float)mousePos.y });
}

// Function to draw the frame
void draw(MainData* _data)
{
    // Clear the screen
    sfRenderWindow_clear(_data->window, sfWhite);
    if (_data->waterMode)
    {
        // Draw the rectangle with the Water Light shader and the Water Shader
        sfRenderWindow_drawRectangleShape(_data->window, _data->rectWater,
            &(sfRenderStates) {.shader = _data->waterShader, .blendMode = sfBlendAlpha, .transform = sfTransform_Identity, .texture = NULL }
        );  
        
        sfRenderWindow_drawRectangleShape(_data->window, _data->rectWaterLight,
            &(sfRenderStates) {.shader = _data->waterLightShader, .blendMode = sfBlendAlpha, .transform = sfTransform_Identity, .texture = NULL }
        );
    }
    else
    {
        // Draw the rectangle with the Light shader
        sfRenderWindow_drawRectangleShape(_data->window, _data->rectLight,
            &(sfRenderStates) {.shader = _data->lightShader, .blendMode = sfBlendAlpha, .transform = sfTransform_Identity, .texture = NULL }
        );
    }

    // Display the rendered frame
    sfRenderWindow_display(_data->window);
}

// Function to clean up resources
void cleanup(MainData* _data)
{
    if (_data->lightShader)
    {
        sfShader_destroy(_data->lightShader);
    }
    if (_data->rectLight)
    {
        sfRectangleShape_destroy(_data->rectLight);
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