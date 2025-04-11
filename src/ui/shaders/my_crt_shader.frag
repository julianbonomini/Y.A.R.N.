#version 120

uniform sampler2D u_texture; // SFML built in
uniform float scanLineMultiplier;  // SCAN_LINE_MULT value, adjusted for SFML
uniform vec4 colorMultiplier;  // Equivalent to 'v_color' in SFML (can be used for color tinting)
uniform float flickerFactor;  // Control flicker (0: no flicker, 1: full flicker)
uniform float distortion;
uniform float distortionRate;

vec2 curveUV(vec2 uv) {
    vec2 centered = uv * 2.0 - 1.0;
    float r2 = dot(centered, centered);
    float edgeFactor = smoothstep(0.2, 1.0, r2); // Adjust these values to control the falloff

    // Apply distortion based on distance from center
//    float distortionAmount = distortion * (1.0 - sqrt(r2)); // Experiment with this formula!
    float distortionAmount = distortion * (1.0 - pow(r2, distortionRate)); // More of the screen affected

    centered *= 1.0 - distortionAmount * edgeFactor;

    return centered * 0.5 + 0.5;
}

void main() {
    vec2 uv = gl_TexCoord[0].xy;

    vec2 distortedUV = curveUV(uv);

    // Sample the texture
    vec4 cta = texture2D(u_texture, distortedUV);

    // Scanline effect (optional tweak to Y coordinate)
    cta.rgb += sin(uv.y * scanLineMultiplier) * 0.02;

    // Flicker logic
    if (flickerFactor > 0.0) {
        cta = vec4(0.0, 0.0, 0.0, 0.75);  // Adjust flicker opacity as needed
    }

    // Discard pixels outside bounds
    if (distortedUV.x < 0.0 || distortedUV.x > 1.0 || distortedUV.y < 0.0 || distortedUV.y > 1.0) {
        cta = vec4(0.0);  // Black outside screen
    }

    // Final color adjustment
    gl_FragColor = cta * colorMultiplier;
}
