#version 120

uniform sampler2D u_texture; // SFML built in
uniform float flickerFactor;  // Flicker control
uniform float bloomThreshold; // Threshold for pixels to bloom
uniform vec2 resolution;     // Screen resolution (needed for blur)

// Gaussian blur function (simplified for vertical pass)
vec4 gaussianBlurVertical(sampler2D tex, vec2 uv, float blurRadius, float sigma) {
    vec4 sum = vec4(0.0);
    float weightSum = 0.0;
    float weight;
    float offset = 1.0 / resolution.y; // Vertical offset

    for (float i = -blurRadius; i <= blurRadius; i++) {
        weight = exp(-(i * i) / (2.0 * sigma * sigma));
        sum += texture2D(tex, uv + vec2(0.0, i * offset)) * weight;
        weightSum += weight;
    }
    return sum / weightSum;
}

// Gaussian blur function (simplified for horizontal pass)
vec4 gaussianBlurHorizontal(sampler2D tex, vec2 uv, float blurRadius, float sigma) {
    vec4 sum = vec4(0.0);
    float weightSum = 0.0;
    float weight;
    float offset = 1.0 / resolution.x; // Horizontal offset

    for (float i = -blurRadius; i <= blurRadius; i++) {
        weight = exp(-(i * i) / (2.0 * sigma * sigma));
        sum += texture2D(tex, uv + vec2(i * offset, 0.0)) * weight;
        weightSum += weight;
    }
    return sum / weightSum;
}

vec2 curveUV(vec2 uv) {
    vec2 centered = uv * 2.0 - 1.0;
    float r2 = dot(centered, centered);
    float edgeFactor = smoothstep(0.2, 1.0, r2);

    float distortion = 0.3;
    float distortionRate = 0.1;
    float distortionAmount = distortion * (1.0 - pow(r2, distortionRate));

    centered *= 1.0 - distortionAmount * edgeFactor;
    return centered * 0.5 + 0.5;
}

float scanLine(vec2 uv) {
    float contrast = 0.05;
    float transparency = 0.99;
    float amountOfLines = 1000.0;
    float darkLine = transparency + contrast * sin(uv.y * amountOfLines * 1.0);
    return darkLine;
}

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec2 distortedUV = curveUV(uv);
    vec4 originalColor = texture2D(u_texture, distortedUV);
    vec4 bloom = vec4(0.0);

    float bloomThreshold = 0.01;
    float bloomIntensity = 0.9;
    // Apply bloom
    vec4 brightParts = step(bloomThreshold, originalColor);
    brightParts *= originalColor;

    // Blur the bright parts (two-pass Gaussian blur for better quality)
    float blurRadius = 5.0;
    float sigma = 3.0;
    vec4 blurredVertical = gaussianBlurVertical(u_texture, distortedUV, blurRadius, sigma);
    vec4 blurredHorizontal = gaussianBlurHorizontal(u_texture, distortedUV, blurRadius, sigma);
    bloom = blurredHorizontal * bloomIntensity * brightParts;

    vec4 finalColor = originalColor + bloom;

    // Scanline effect
    finalColor.rgb *= scanLine(uv);

    // Flicker logic
    if (flickerFactor > 0.0) {
        finalColor = vec4(0.0, 0.0, 0.0, 0.5);
    }

    // Tinting (you can make this a uniform as well for more control!)
    finalColor *= vec4(0.8, 0.9, 0.8, 1.0);

    gl_FragColor = finalColor;
}