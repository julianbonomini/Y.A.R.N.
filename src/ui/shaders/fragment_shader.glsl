#version 330 core

in vec2 v_texCoords; // Texture coordinates from the vertex shader
in vec4 v_color;     // Color from the vertex shader

out vec4 fragColor;  // Final color output

uniform sampler2D u_texture; // The texture to sample
uniform float u_time;        // Time uniform to be used for effects

void main() {
    // Apply a time-based effect (e.g., oscillating color or CRT effect)
    vec4 textureColor = texture(u_texture, v_texCoords);
    fragColor = textureColor * (sin(u_time) * 0.5 + 0.5); // Example effect based on time
}
