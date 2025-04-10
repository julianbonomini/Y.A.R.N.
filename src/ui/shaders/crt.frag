#version 120

uniform sampler2D u_texture;  // The texture to sample
uniform float CRT_CURVE_AMNTx; // Curve amount on x-axis
uniform float CRT_CURVE_AMNTy; // Curve amount on y-axis
uniform vec2 u_resolution; // Resolution of the screen (width, height)
#define SCAN_LINE_MULT 1250.0  // Scanline multiplier

varying vec2 v_texCoords;  // Texture coordinates from vertex shader

void main() {
    // Get normalized texture coordinates
    vec2 tc = v_texCoords;

    // Distance from the center of the screen
    float dx = abs(0.5 - tc.x);
    float dy = abs(0.5 - tc.y);

    // Apply curvature effect on texture coordinates
    dx *= dx;
    dy *= dy;

    // Adjust texture coordinates based on curvature amount
    tc.x -= 0.5;
    tc.x *= 1.0 + (dy * CRT_CURVE_AMNTx);
    tc.x += 0.5;

    tc.y -= 0.5;
    tc.y *= 1.0 + (dx * CRT_CURVE_AMNTy);
    tc.y += 0.5;

    // Sample the texture at the adjusted coordinates
    vec3 color = texture2D(u_texture, tc).rgb;

    // Simulate scanlines
    float scanline = sin(tc.y * SCAN_LINE_MULT) * 0.05;  // Vertical scanline effect
    color -= scanline;

    // Apply vignette effect (slight darkening towards the edges)
    float dist = distance(tc, vec2(0.5));  // Distance from the center of the screen
    color *= 1.0 - dist * 0.4;  // Darken edges

    // Set the final fragment color with the applied effects
    gl_FragColor = vec4(color, 1.0);  // Output final color with alpha = 1
}