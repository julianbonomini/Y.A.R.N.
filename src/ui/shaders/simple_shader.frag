#version 120

uniform vec2 resolution;

void main() {
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    vec3 color = vec3(0.0);

    // Rounded corners (very basic)
    float dist_to_center = distance(uv, vec2(0.5, 0.5));
    float vignette = smoothstep(0.4, 0.5, dist_to_center);
    color = mix(vec3(0.0), vec3(1.0), vignette); // Fade towards black at edges

    // Basic scanlines
    float scanline = sin(uv.y * resolution.y * 2.0) * 0.1;
    color += vec3(scanline);

    // Output the color
    gl_FragColor = vec4(color, 1.0);
}