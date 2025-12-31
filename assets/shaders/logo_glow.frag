uniform sampler2D texture;
uniform float time;
uniform vec2 textureSize;

void main()
{
    vec2 pixelCoord = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, pixelCoord);
    
    // Calculate distance from center for radial effects
    vec2 center = vec2(0.5, 0.5);
    vec2 offset = pixelCoord - center;
    float dist = length(offset);
    float angle = atan(offset.y, offset.x);
    
    // Multiple pulsing frequencies for complex rhythm
    float pulse1 = 0.5 + 0.18 * sin(time * 1.8);
    float pulse2 = 0.5 + 0.12 * sin(time * 3.2 + dist * 5.0);
    float pulse3 = 0.5 + 0.1 * sin(time * 0.9 + angle * 3.0);
    float combinedPulse = (pulse1 + pulse2 + pulse3) / 3.0;
    
    // Create bloom/glow by sampling neighboring pixels
    // This creates the overflow effect beyond the image boundaries
    float glowRadius = 0.018 + 0.01 * pulse1; // Pulsing glow radius
    vec4 glowColor = vec4(0.0);
    float glowWeight = 0.0;
    
    // Sample multiple points in a circular pattern for bloom effect
    // Using fewer samples for better performance
    int samples = 8;
    for (int i = 0; i < samples; i++) {
        float angle = (float(i) / float(samples)) * 6.28318; // 2*PI
        vec2 sampleOffset = vec2(cos(angle), sin(angle)) * glowRadius;
        vec2 sampleCoord = pixelCoord + sampleOffset;
        
        // Sample the texture (can go outside bounds for overflow)
        vec4 samplePixel = texture2D(texture, sampleCoord);
        
        // Only accumulate glow from non-transparent areas
        if (samplePixel.a > 0.01) {
            float weight = 1.0 / (1.0 + length(sampleOffset) * 30.0);
            glowColor += samplePixel * weight * samplePixel.a;
            glowWeight += weight;
        }
    }
    
    // Add additional samples at larger radius for extended glow
    for (int i = 0; i < 4; i++) {
        float angle = (float(i) / 4.0) * 6.28318;
        vec2 sampleOffset = vec2(cos(angle), sin(angle)) * glowRadius * 2.0;
        vec2 sampleCoord = pixelCoord + sampleOffset;
        vec4 samplePixel = texture2D(texture, sampleCoord);
        
        if (samplePixel.a > 0.01) {
            float weight = 0.3 / (1.0 + length(sampleOffset) * 20.0);
            glowColor += samplePixel * weight * samplePixel.a;
            glowWeight += weight;
        }
    }
    
    // Normalize the glow
    if (glowWeight > 0.0) {
        glowColor /= glowWeight;
    }
    
    // Create radial glow intensity that extends beyond edges
    float glowIntensity = 1.0 - smoothstep(0.3, 1.2, dist); // Extended range
    glowIntensity *= (0.35 + 0.25 * combinedPulse); // Moderate intensity
    
    // Combine original pixel with bloom glow
    vec4 finalColor = pixel;
    
    // Add the overflow glow (stronger outside the image)
    if (pixel.a < 0.01) {
        // Outside the image - show only glow
        finalColor = glowColor * glowIntensity * 0.5;
        finalColor.a = glowIntensity * 0.4; // Fade out at edges
    } else {
        // Inside the image - combine original with glow
        finalColor.rgb = mix(pixel.rgb, glowColor.rgb, 0.2 * glowIntensity);
        finalColor.rgb *= (1.0 + glowIntensity * combinedPulse * 0.2);
    }
    
    // Dynamic glow that pulses and waves
    float waveEffect = sin(dist * 8.0 - time * 3.0) * 0.05;
    finalColor.rgb *= (1.0 + waveEffect * glowIntensity * 0.6);
    
    // Color shifting - animated hue rotation
    float hueShift = sin(time * 1.5 + dist * 2.0) * 0.08;
    vec3 colorShift = vec3(
        0.04 + hueShift * 0.15,
        0.03 + hueShift * 0.12,
        0.06 - hueShift * 0.12
    );
    
    // Add dynamic color enhancement
    finalColor.rgb += colorShift * combinedPulse * finalColor.a * 0.6;
    
    // Add radial color gradient effect
    float radialGradient = 1.0 - smoothstep(0.2, 0.7, dist);
    finalColor.r += 0.04 * radialGradient * pulse1 * finalColor.a;
    finalColor.g += 0.03 * radialGradient * pulse2 * finalColor.a;
    finalColor.b += 0.06 * radialGradient * pulse3 * finalColor.a;
    
    // Add subtle wave distortion effect
    float waveDistortion = sin(angle * 4.0 + time * 2.5) * 0.015;
    finalColor.rgb += vec3(waveDistortion) * finalColor.a;
    
    // Ensure we don't exceed valid color range
    finalColor.rgb = min(finalColor.rgb, vec3(1.0));
    
    // Add edge highlight for more definition
    float edgeHighlight = 1.0 - smoothstep(0.85, 1.2, dist);
    finalColor.rgb += vec3(0.05, 0.05, 0.08) * edgeHighlight * pulse1 * finalColor.a;
    finalColor.rgb = min(finalColor.rgb, vec3(1.0));
    
    // Multiply by vertex color (important for SFML)
    gl_FragColor = gl_Color * finalColor;
}

