#version 330

// Input texture
uniform sampler2D texture0;
in vec2 fragTexCoord;

// Output color
out vec4 fragColor;

// Simulate Protanopia (red-blind)
vec3 ApplyProtanopia(vec3 color)
{
    float R = color.r;
    float G = color.g;
    float B = color.b;

    // Adjusted colors to simulate Protanopia
    float newR = 0.567 * R + 0.433 * G;
    float newG = 0.558 * R + 0.442 * G;
    float newB = B;

    return vec3(newR, newG, newB);
}

void main()
{
    // Fetch the pixel color from the texture
    vec4 texColor = texture(texture0, fragTexCoord);
    
    // Apply Protanopia simulation
    vec3 colorBlindColor = ApplyProtanopia(texColor.rgb);
    
    // Set the final fragment color
    fragColor = vec4(colorBlindColor, texColor.a);
}
