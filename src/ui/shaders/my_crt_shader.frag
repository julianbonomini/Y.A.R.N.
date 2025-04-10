#version 120

uniform sampler2D u_texture;
uniform float CRT_CURVE_AMNTx;  // Curve amount on x
uniform float CRT_CURVE_AMNTy;  // Curve amount on y
uniform float scanLineMultiplier;  // SCAN_LINE_MULT value, adjusted for SFML
uniform vec4 colorMultiplier;  // Equivalent to 'v_color' in SFML (can be used for color tinting)

void main() {
    vec2 tc = gl_TexCoord[0].xy;

    // Distance from the center (for curvature effect)
    float dx = abs(0.5 - tc.x);
    float dy = abs(0.5 - tc.y);

    // Square the distances to smooth the edges of the curvature
    dx *= dx;
    dy *= dy;

    // Apply curvature effect on x and y axes
    tc.x -= 0.5;
    tc.x *= 1.0 + (dy * CRT_CURVE_AMNTx);
    tc.x += 0.5;

    tc.y -= 0.5;
    tc.y *= 1.0 + (dx * CRT_CURVE_AMNTy);
    tc.y += 0.5;

    // Sample the texture with the new coordinates
    vec4 cta = texture2D(u_texture, tc);

    // Add scanline effect (sin wave for darkening effect)
    cta.rgb += sin(tc.y * scanLineMultiplier) * 0.02;

    // Ensure the coordinates are within bounds
    if (tc.y > 1.0 || tc.x < 0.0 || tc.x > 1.0 || tc.y < 0.0) {
        cta = vec4(0.0);  // Black out pixels outside the valid texture space
    }

    // Apply the color multiplier (could be used to adjust brightness or tint)
    gl_FragColor = cta * colorMultiplier;
}
