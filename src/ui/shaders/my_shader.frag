#version 120
uniform sampler2D texture;
uniform float time;

void main()
{
    // Get the texture coordinates
    vec2 uv = gl_TexCoord[0].xy;

    // Apply a slight distortion to simulate CRT curvature
    uv = uv * 2.0 - 1.0; // Move from [0, 1] to [-1, 1]
    uv.x *= 1.5; // Slight horizontal stretch to simulate curvature
    uv = uv / (1.0 + 0.1 * uv.x * uv.x); // Simple distortion effect
    uv = (uv + 1.0) / 2.0; // Return back to [0, 1]

    // Sample the texture with the distorted coordinates
    vec4 color = texture2D(texture, uv);

    // Add scanlines effect (just darken every few rows)
    float scanlineEffect = 1.0 - mod(gl_TexCoord[0].y * 100.0, 2.0); // Darken every alternate row
    color.rgb *= scanlineEffect;

    // Optional: Flicker effect (for CRT flickering)
    float flicker = sin(time * 10.0) * 0.05 + 1.0;  // Flicker based on time
    color.rgb *= flicker;

    // Set the final color
    gl_FragColor = color;
}