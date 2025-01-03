// Uniform variables
uniform float time;           // Temps écoulé (pour l'animation des ondulations)
uniform vec2 resolution;      // Résolution de l'écran (largeur et hauteur)

void main()
{
    // Coordonnées normalisées (entre 0 et 1)
    vec2 uv = gl_FragCoord.xy / resolution;

    // Ajustement de l'aspect pour éviter les déformations
    uv.y *= resolution.y / resolution.x;

    // Génération des ondulations
    float wave1 = sin(uv.x * 10.0 + time * 2.0) * 0.05;
    float wave2 = cos(uv.y * 15.0 - time * 1.5) * 0.03;
    float wave3 = sin((uv.x + uv.y) * 8.0 + time * 1.0) * 0.02;

    // Combinaison des ondulations
    float waves = wave1 + wave2 + wave3;

    // Couleur de base (bleu aquatique)
    vec3 baseColor = vec3(0.0, 0.3, 0.5);

    // Ajout d'une variation de teinte selon les ondulations
    vec3 finalColor = baseColor + vec3(waves * 0.1, waves * 0.15, waves * 0.2);

    // Augmenter la luminosité pour simuler un effet de réflexion
    finalColor += vec3(0.1, 0.1, 0.15) * smoothstep(0.0, 0.2, waves);

    // Sortie de la couleur finale
    gl_FragColor = vec4(finalColor, 1.0);
}
