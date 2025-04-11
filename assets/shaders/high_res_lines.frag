#version 120

uniform sampler2D u_texture; // SFML built in
uniform float flickerFactor;  // Flicker control

float scanLine(vec2 uv) {
    float contrast = 0.05;
    float transparency = 0.99; // 1.0 is full transparent, 0 is no passthru
    float amountOfLines = 1000;
    float darkLine = transparency + contrast * sin(uv.y * amountOfLines * 1.0);

    return darkLine;
}

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 cta = texture2D(u_texture, uv);

    // More defined scanline effect with subtle highlight
    float scanlineEffect = scanLine(uv);
    cta.rgb *= scanlineEffect;

    // Flicker logic
    if (flickerFactor > 0.0) {
        cta = vec4(0.0, 0.0, 0.0, 0.5); // flicker is full black with 75% opacity
    }

    // Tinting
    // vec4(RED, GREEN BLUE, ALPHA)
    gl_FragColor = cta * vec4(1.0, 1.0, 1.0, 1.0);
}