
#version 330 core

in vec4 chCol;

out vec4 outCol;

// Boja bioskopskog platna
uniform vec3 uScreenColor;

void main()
{
    outCol = vec4(
        chCol.rgb * uScreenColor,
        chCol.a
    );
}
