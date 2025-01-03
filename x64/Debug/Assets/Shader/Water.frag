// Uniform variables
uniform float time;            // Elapsed time (animation)
uniform vec2 resolution;       // Screen resolution

void main()
{
    // Normalize coordinates (range 0 to 1)
    vec2 uv = gl_FragCoord.xy / resolution;

    // Adjust aspect ratio to avoid distortion
    uv.y *= resolution.y / resolution.x;

    // Generate ripples with different frequencies and phases
    float ripple1 = sin(uv.x * 15.0 + time * 2.0) * 0.02;
    float ripple2 = cos(uv.y * 20.0 - time * 1.8) * 0.03;
    float ripple3 = sin((uv.x + uv.y) * 12.0 + time * 1.5) * 0.025;
    float ripple4 = cos((uv.x - uv.y) * 18.0 - time * 2.2) * 0.02;

    // Combine all ripple effects
    float ripples = ripple1 + ripple2 + ripple3 + ripple4;

    // Smooth transitions between ripples using a gradient
    float smoothRipples = smoothstep(0.0, 0.5, ripples) - smoothstep(0.5, 1.0, ripples);

    // Distort UV coordinates for dynamic ripple motion
    vec2 distortedUV = uv + vec2(ripples * 0.15, ripples * 0.1);

    // Base water color (aquatic blue)
    vec3 baseColor = vec3(0.0, 0.3, 0.5);

    // Add depth effect by varying brightness based on UV coordinates
    float depth = 0.6 + 0.4 * sin((uv.x + uv.y) * 8.0 + time * 1.0);

    // Calculate final color with ripples and depth
    vec3 waterColor = baseColor + vec3(smoothRipples * 0.1, smoothRipples * 0.1, smoothRipples * 0.1) + vec3(depth * 0.1);

    // Simulate light reflections for more realism
    float reflection = smoothstep(0.8, 1.0, sin((distortedUV.x + distortedUV.y) * 50.0 + time * 3.5));
    waterColor += vec3(0.3, 0.4, 0.6) * reflection;

    // Enhance brightness for a polished look
    waterColor = pow(waterColor, vec3(1.3));

    // Output the final color
    gl_FragColor = vec4(waterColor, 1.0);
}
