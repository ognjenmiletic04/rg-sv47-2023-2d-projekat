
#include <GL/glew.h>

#include "../Header/Cinema.h"

// VAO i VBO za bioskopsku salu
unsigned int VAOcinema;
unsigned int VBOcinema;

// Funkcija za definisanje jednog pravougaonika
void addRectangle(
    float* vertices,
    int index,
    float left, float right,
    float bottom, float top,
    float r, float g, float b, float a)
{
    // Svaki pravougaonik ima 4 temena.
    // Svako teme ima 6 vrednosti: x, y, r, g, b, a.
    int start = index * 4 * 6;

    float rectangle[] = {
        left,  top,    r, g, b, a,
        left,  bottom, r, g, b, a,
        right, bottom, r, g, b, a,
        right, top,    r, g, b, a
    };

    for (int i = 0; i < 24; i++)
    {
        vertices[start + i] = rectangle[i];
    }
}

// INICIJALIZACIJA BIOSKOPSKE SALE
void initCinema()
{
    // Ukupno 7 pravougaonika:
    // 5 delova zidova, vrata i providni sloj
    float verticesCinema[7 * 4 * 6];

    // Boja zidova - siva
    float wallR = 0.45f;
    float wallG = 0.50f;
    float wallB = 0.60f;

    // 0 - Gornji zid
    addRectangle(verticesCinema, 0,
        -1.0f, 1.0f, 0.94f, 1.0f,
        wallR, wallG, wallB, 1.0f);

    // 1 - Donji zid
    addRectangle(verticesCinema, 1,
        -1.0f, 1.0f, -1.0f, -0.94f,
        wallR, wallG, wallB, 1.0f);

    // 2 - Desni zid
    addRectangle(verticesCinema, 2,
        0.95f, 1.0f, -0.94f, 0.94f,
        wallR, wallG, wallB, 1.0f);

    // 3 - Donji deo levog zida
    addRectangle(verticesCinema, 3,
        -1.0f, -0.95f, -0.94f, 0.62f,
        wallR, wallG, wallB, 1.0f);

    // 4 - Gornji deo levog zida
    addRectangle(verticesCinema, 4,
        -1.0f, -0.95f, 0.86f, 0.94f,
        wallR, wallG, wallB, 1.0f);

    // 5 - Vrata u gornjem levom delu sale
    addRectangle(verticesCinema, 5,
        -1.0f, -0.95f, 0.62f, 0.86f,
        0.75f, 0.35f, 0.15f, 1.0f);

    // 6 - Tamnosivi sloj preko celog ekrana
    // Alpha = 0.5f prema specifikaciji
    addRectangle(verticesCinema, 6,
        -1.0f, 1.0f, -1.0f, 1.0f,
        0.20f, 0.20f, 0.20f, 0.5f);

    // Kreiranje VAO i VBO
    glGenVertexArrays(1, &VAOcinema);
    glGenBuffers(1, &VBOcinema);

    glBindVertexArray(VAOcinema);
    glBindBuffer(GL_ARRAY_BUFFER, VBOcinema);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(verticesCinema),
        verticesCinema,
        GL_STATIC_DRAW
    );

    // Atribut 0 - pozicija
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    // Atribut 1 - RGBA boja
    glVertexAttribPointer(
        1, 4, GL_FLOAT, GL_FALSE,
        6 * sizeof(float),
        (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);
}


// ISCRTAVANJE ZIDOVA I VRATA
void drawCinema(unsigned int shader, bool doorOpen)
{
    glUseProgram(shader);
    glBindVertexArray(VAOcinema);

    // Resetovanje pozicije
    glUniform2f(
        glGetUniformLocation(shader, "uPos"),
        0.0f, 0.0f
    );

    // Crtanje pet delova zidova
    for (int i = 0; i < 5; i++)
    {
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

    // Crtanje vrata samo ako su zatvorena
    if (!doorOpen)
    {
        glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
    }
}


// TAMNOSIVI PROVIDNI SLOJ
void drawCinemaOverlay(unsigned int shader)
{
    glUseProgram(shader);
    glBindVertexArray(VAOcinema);

    // Resetovanje pozicije
    glUniform2f(
        glGetUniformLocation(shader, "uPos"),
        0.0f, 0.0f
    );

    // Sedmi pravougaonik - preko celog ekrana
    glDrawArrays(GL_TRIANGLE_FAN, 24, 4);
}
