#version 330 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, vec2(TexCoord.x, 1.0 - TexCoord.y)), 0.2);
}
