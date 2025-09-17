#version 120

uniform sampler2D texture;
uniform float offsetX;
uniform float offsetY;
uniform vec4 colour;

void main()
{
    vec2 texCoord = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(texture, texCoord);

    if (pixel.a == 1.0)
    {
        gl_FragColor = pixel;
    }
    else
    {
        float au = texture2D(texture, vec2(texCoord.x, texCoord.y - offsetY)).a;
        float ad = texture2D(texture, vec2(texCoord.x, texCoord.y + offsetY)).a;
        float al = texture2D(texture, vec2(texCoord.x - offsetX, texCoord.y)).a;
        float ar = texture2D(texture, vec2(texCoord.x + offsetX, texCoord.y)).a;

        if (au == 1.0 || ad == 1.0 || al == 1.0 || ar == 1.0)
        {
            gl_FragColor = colour;
        }
        else
        {
            gl_FragColor = pixel;
        }
    }
}