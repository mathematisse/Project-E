#version 330

uniform sampler2D texture0; // Input texture
in vec2 fragTexCoord;       // Texture coordinates

out vec4 fragColor;         // Output color

// Function to enhance visibility
vec3 EnhanceVisibility(vec3 color) {
    // Convert to grayscale to find luminance
    float luminance = dot(color, vec3(0.299, 0.587, 0.114));
    
    // Create a more distinct color by boosting saturation
    float saturation = 1.5; // Increase saturation
    color = mix(vec3(luminance), color, saturation);
    
    // Brighten the color
    color += vec3(0.2); // Brighten the color (adjust value as needed)

    // Clamp the values to stay within valid range
    return clamp(color, 0.0, 1.0);
}

void main() {
    vec4 texColor = texture(texture0, fragTexCoord); // Fetch the pixel color from the texture
    vec3 enhancedColor = EnhanceVisibility(texColor.rgb); // Enhance the color
    fragColor = vec4(enhancedColor, texColor.a); // Set the final fragment color
}
