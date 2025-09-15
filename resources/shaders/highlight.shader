#version 120

uniform sampler2D texture;
uniform vec4 colour;

void main()
{
    vec2 texCoord = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, texCoord);

    if (pixel.a == 1.0)
    {
        gl_FragColor = colour;
    }
}