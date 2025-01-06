// Uniform variables
// Light position
uniform vec2 lightPos;       
// Radius of light
uniform float radius;    
// Color of light
uniform vec3 lightColor;     
// Screen height
uniform float screenHeight;    
// Elapsed time for animation
uniform float time;           

void main()
{
    // Y-axis correction for light position
    vec2 correctedLightPos = vec2(lightPos.x, screenHeight - lightPos.y);

    // Coordinates of current pixel
    vec2 pos = gl_FragCoord.xy;

    // Calculation of distance between pixel and light
    float dist = length(pos - correctedLightPos);

    // Calculating light intensity
    float intensity = 1.0 - smoothstep(0.0, radius, dist);

    // Base color with light effect
    vec3 baseLight = lightColor * intensity;

    // Add water effect (animated ripples)
    float wave = sin(pos.x * 0.05 + time) * 0.02 + sin(pos.y * 0.07 + time * 1.5) * 0.02;

    // Change position to simulate refraction
    vec2 refractedPos = pos + wave * 50.0 * normalize(pos - correctedLightPos);

    // Light reintegration with ripples
    float refractedDist = length(refractedPos - correctedLightPos);
    float refractedIntensity = 1.0 - smoothstep(0.0, radius, refractedDist);
    vec3 refractedLight = lightColor * refractedIntensity;

    // Final color combining light and water effect
    vec3 finalColor = mix(baseLight, refractedLight, 0.7);

    // Adding a bluish tint to water
    finalColor = mix(finalColor, vec3(0.0, 0.2, 0.5), 0.3);
    
    // Final color output
    gl_FragColor = vec4(finalColor, 0.8);
}
