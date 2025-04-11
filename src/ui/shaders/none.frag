#version 120

uniform sampler2D u_texture; // SFML built in
uniform float flickerFactor;  // Flicker control

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 cta = texture2D(u_texture, uv);

    // Flicker logic
    if (flickerFactor > 0.0) {
        cta = vec4(0.0, 0.0, 0.0, 0.75); // flicker is full black with 75% opacity
    }

    // Tinting
    gl_FragColor = cta * vec4(1.0, 1.0, 1.0, 1.0);
}