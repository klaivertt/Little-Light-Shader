// Uniform variables passed from the application

// Position of the light (mouse position)
uniform vec2 lightPos;  

// Radius of the light
uniform float radius;    
// Color of the light
uniform vec3 lightColor;    
// Height of the screen (used to correct Y-axis)
uniform float screenHeight;

void main()
{
    // Correct the Y-axis to match OpenGL's coordinate system
    vec2 correctedLightPos = vec2(lightPos.x, screenHeight - lightPos.y);

    // Current pixel position in screen coordinates
    vec2 pos = gl_FragCoord.xy;

    // Calculate the distance between the current pixel and the light source
    float dist = length(pos - correctedLightPos);

    // Compute light intensity based on distance
    float intensity = 1.0 - smoothstep(0.0, radius, dist);

    // Final color of the pixel with applied light effect
    vec3 color = lightColor * intensity;

    // Output the final color to the fragment
    gl_FragColor = vec4(color, 0.8);
}
