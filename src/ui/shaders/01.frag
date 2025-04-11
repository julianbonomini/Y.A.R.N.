#version 120

uniform sampler2D u_texture; // SFML built in
uniform float flickerFactor;  // Flicker control

vec2 curveUV(vec2 uv) {
    vec2 centered = uv * 2.0 - 1.0;
    float r2 = dot(centered, centered);
    float edgeFactor = smoothstep(0.2, 1.0, r2); // Adjust these values to control the falloff

    // Apply distortion based on distance from center
    float distortion = 0.2;
    float distortionRate = 0.1;
    float distortionAmount = distortion * (1.0 - pow(r2, distortionRate)); // More of the screen affected

    centered *= 1.0 - distortionAmount * edgeFactor;
    return centered * 0.5 + 0.5;
}

float scanLine(vec2 uv) {
    float contrast = 0.01;
    float transparency = 0.99; // 1.0 is full transparent, 0 is no passthru
    float amountOfLines = 500;
    float darkLine = transparency + contrast * sin(uv.y * amountOfLines * 1.0);

    return darkLine;
}

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec2 distortedUV = curveUV(uv);
    vec4 cta = texture2D(u_texture, distortedUV);

    // More defined scanline effect with subtle highlight
    float scanlineEffect = scanLine(uv);
    cta.rgb *= scanlineEffect;

    // Flicker logic
    if (flickerFactor > 0.0) {
        cta = vec4(0.0, 0.0, 0.0, 0.75); // flicker is full black with 75% opacity
    }

    // Tinting
    gl_FragColor = cta * vec4(1.0, 1.0, 1.0, 1.0);
}