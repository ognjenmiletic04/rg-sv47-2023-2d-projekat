
#include <GL/glew.h>

#include "../Header/Signature.h"
#include "../Header/Util.h"

// VAO, VBO i tekstura potpisa
unsigned int VAOsignature;
unsigned int VBOsignature;
unsigned int signatureTexture;


// Inicijalizacija potpisa
void initSignature()
{
    // Pravougaonik za potpis
    // x, y, r, g, b, a, s, t

    float verticesSignature[] = {
        -0.1133f,  0.0233f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.1133f, -0.0233f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.1133f, -0.0233f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.1133f,  0.0233f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    // Kreiranje VAO i VBO
    glGenVertexArrays(1, &VAOsignature);
    glGenBuffers(1, &VBOsignature);

    glBindVertexArray(VAOsignature);
    glBindBuffer(GL_ARRAY_BUFFER, VBOsignature);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(verticesSignature),
        verticesSignature,
        GL_STATIC_DRAW
    );

    // Atribut 0 - pozicija (x, y)
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // Atribut 1 - RGBA boja
    glVertexAttribPointer(
        1, 4, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Atribut 2 - UV koordinate teksture
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    // Ucitavanje PNG slike potpisa
    signatureTexture =
        loadImageToTexture("Resources/signature.png");

    if (signatureTexture == 0)
    {
        return;
    }

    // Podesavanje teksture
    glBindTexture(GL_TEXTURE_2D, signatureTexture);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    glBindTexture(GL_TEXTURE_2D, 0);
}


// Iscrtavanje potpisa
void drawSignature(unsigned int shader)
{
    if (signatureTexture == 0)
    {
        return;
    }

    // Aktiviranje teksturnog sejdera
    glUseProgram(shader);

    // Aktiviranje teksture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, signatureTexture);

    // Uniforma teksture
    glUniform1i(
        glGetUniformLocation(shader, "uTex0"),
        0
    );

    // Pozicija potpisa u donjem desnom uglu
    glUniform2f(
        glGetUniformLocation(shader, "uPos"),
        0.8267f, -0.8967f
    );

    // Iscrtavanje pravougaonika sa potpisom
    glBindVertexArray(VAOsignature);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


// Oslobadjanje resursa
void deleteSignature()
{
    glDeleteVertexArrays(1, &VAOsignature);
    glDeleteBuffers(1, &VBOsignature);
    glDeleteTextures(1, &signatureTexture);
}
