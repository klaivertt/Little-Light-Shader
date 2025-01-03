// Uniform variables
  // Position de la lumière
uniform vec2 lightPos;       
  // Rayon de la lumière
uniform float radius;    
  // Couleur de la lumière
uniform vec3 lightColor;     
// Hauteur de l'écran
uniform float screenHeight;    
 // Temps écoulé pour l'animation
uniform float time;           

void main()
{
    // Correction de l'axe Y pour la position de la lumière
    vec2 correctedLightPos = vec2(lightPos.x, screenHeight - lightPos.y);

    // Coordonnées du pixel courant
    vec2 pos = gl_FragCoord.xy;

    // Calcul de la distance entre le pixel et la lumière
    float dist = length(pos - correctedLightPos);

    // Calcul de l'intensité de la lumière
    float intensity = 1.0 - smoothstep(0.0, radius, dist);

    // Couleur de base avec l'effet de lumière
    vec3 baseLight = lightColor * intensity;

    // Ajout de l'effet d'eau (ondulations animées)
    float wave = sin(pos.x * 0.05 + time) * 0.02 + sin(pos.y * 0.07 + time * 1.5) * 0.02;

    // Modification de la position pour simuler la réfraction
    vec2 refractedPos = pos + wave * 50.0 * normalize(pos - correctedLightPos);

    // Réintégration de la lumière avec les ondulations
    float refractedDist = length(refractedPos - correctedLightPos);
    float refractedIntensity = 1.0 - smoothstep(0.0, radius, refractedDist);
    vec3 refractedLight = lightColor * refractedIntensity;

    // Couleur finale combinant lumière et effet d'eau
    vec3 finalColor = mix(baseLight, refractedLight, 0.7);

    // Ajout d'une teinte bleutée pour l'eau
    finalColor = mix(finalColor, vec3(0.0, 0.2, 0.5), 0.3);

    // Sortie de la couleur finale
    gl_FragColor = vec4(finalColor, 0.8);
}
