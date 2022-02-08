#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float alpha;

void main()
{
    vec2 SecondTexCoord = TexCoord;
    SecondTexCoord.y = 1.0 - SecondTexCoord.y;
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, SecondTexCoord), clamp(alpha, 0.0, 1.0));
}