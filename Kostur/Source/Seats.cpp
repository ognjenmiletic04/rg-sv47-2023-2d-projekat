
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Seats.h"

// Broj redova i sedista u jednom redu
const int SEAT_ROWS = 5;
const int SEAT_COLS = 10;

// Dimenzije jednog sedista
const float SEAT_WIDTH = 0.13f;
const float SEAT_HEIGHT = 0.14f;

// Pocetne pozicije i razmaci sedista
const float START_X = -0.765f;
const float START_Y = 0.42f;
const float SEAT_SPACING_X = 0.17f;
const float SEAT_SPACING_Y = 0.25f;

// Statusi:
// 0 - Slobodno
// 1 - Rezervisano
int seatStatus[SEAT_ROWS][SEAT_COLS] = {};

// Trenutna faza simulacije:
// 0 - Rezervacija i kupovina
// 1 - Ulazak gledalaca
// 2 - Projekcija filma
// 3 - Izlazak gledalaca
int cinemaPhase = 0;

// Funkcija pomocu koje se proverava fazа
int getCinemaPhase()
{
    return cinemaPhase;
}

// Pocetak projekcije filma
void startProjection()
{
    // Projekcija pocinje tek nakon ulaska svih gledalaca
    if (cinemaPhase == 1)
    {
        cinemaPhase = 2;
    }
}

// Zavrsetak projekcije filma
void finishProjection()
{
    // Iz faze projekcije prelazimo u fazu izlaska
    if (cinemaPhase == 2)
    {
        cinemaPhase = 3;
    }
}

// VAO i VBO za plava sedista
unsigned int VAOseatBlue;
unsigned int VBOseatBlue;

// VAO i VBO za zuta sedista
unsigned int VAOseatYellow;
unsigned int VBOseatYellow;

// VAO i VBO za crvena sedista
unsigned int VAOseatRed;
unsigned int VBOseatRed;



// Kreiranje geometrije jednog sedista
void createSeatVAO(float r, float g, float b,
    unsigned int& VAO, unsigned int& VBO)
{
    float verticesSeat[] = {
        // x, y, r, g, b, a

        -0.065f,  0.07f, r, g, b, 1.0f,
        -0.065f, -0.07f, r, g, b, 1.0f,
         0.065f, -0.07f, r, g, b, 1.0f,
         0.065f,  0.07f, r, g, b, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(verticesSeat),
        verticesSeat,
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


// Inicijalizacija sedista
void initSeats()
{
    // Sva sedista su na pocetku slobodna
    for (int row = 0; row < SEAT_ROWS; row++)
    {
        for (int col = 0; col < SEAT_COLS; col++)
        {
            seatStatus[row][col] = 0;
        }
    }

    // Plava sedista - Slobodno
    createSeatVAO(
        0.10f, 0.30f, 0.95f,
        VAOseatBlue, VBOseatBlue
    );

    // Zuta sedista - Rezervisano
    createSeatVAO(
        1.0f, 0.90f, 0.10f,
        VAOseatYellow, VBOseatYellow
    );

    // Crvena sedista - Kupljeno
    createSeatVAO(
        0.95f, 0.10f, 0.10f,
        VAOseatRed, VBOseatRed
    );
}


// Iscrtavanje svih sedista u bioskopu
void drawSeats(unsigned int shader)
{
    glUseProgram(shader);

    for (int row = 0; row < SEAT_ROWS; row++)
    {
        for (int col = 0; col < SEAT_COLS; col++)
        {
            // Pozicija sedista
            float seatX = START_X + col * SEAT_SPACING_X;
            float seatY = START_Y - row * SEAT_SPACING_Y;

            // Izbor boje prema statusu
            if (seatStatus[row][col] == 0)
            {
                glBindVertexArray(VAOseatBlue);
            }
			else if (seatStatus[row][col] == 1)
            {
                glBindVertexArray(VAOseatYellow);
            }
            else {
				glBindVertexArray(VAOseatRed);

            }

            // Pomeranje sedista uniformom
            glUniform2f(
                glGetUniformLocation(shader, "uPos"),
                seatX, seatY
            );

            // Crtanje jednog sedista
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
    }
}


// Rezervacija sedista klikom misa
void seatMouseCallback(GLFWwindow* window,
    int button, int action, int mods)
{

    // Rezervacija je dozvoljena samo u pocetnoj fazi
    if (cinemaPhase != 0)
    {
        return;
    }

    // Reagujemo samo na levi klik misa
    if (button == GLFW_MOUSE_BUTTON_LEFT &&
        action == GLFW_PRESS)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        if (width == 0 || height == 0)
            return;

        // Trenutna pozicija kursora u pikselima
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Normalizacija u OpenGL koordinate (-1, 1)
        float xposNorm = (xpos / width) * 2 - 1;
        float yposNorm = -((ypos / height) * 2 - 1);

        // Prolaz kroz sva sedista
        for (int row = 0; row < SEAT_ROWS; row++)
        {
            for (int col = 0; col < SEAT_COLS; col++)
            {
                // Centar trenutnog sedista
                float seatX = START_X + col * SEAT_SPACING_X;
                float seatY = START_Y - row * SEAT_SPACING_Y;

                // Granice sedista
                float left = seatX - SEAT_WIDTH / 2;
                float right = seatX + SEAT_WIDTH / 2;
                float bottom = seatY - SEAT_HEIGHT / 2;
                float top = seatY + SEAT_HEIGHT / 2;

                // Da li je korisnik kliknuo na sediste?
                if (xposNorm >= left &&
                    xposNorm <= right &&
                    yposNorm >= bottom &&
                    yposNorm <= top)
                {
                    // Slobodno -> Rezervisano
                    if (seatStatus[row][col] == 0)
                    {
                        seatStatus[row][col] = 1;
                    }
                    // Rezervisano -> Slobodno
                    else if (seatStatus[row][col] == 1)
                    {
                        seatStatus[row][col] = 0;
                    }

                    // Sediste je pronadjeno
                    return;
                }
            }
        }
    }
}


// Kupovina N sedista klikom tastera 1-9
void buySeats(int count)
{

    // Kupovina je dozvoljena samo pre pocetka simulacije
    if (cinemaPhase != 0)
    {
        return;
    }

    // Pretraga pocinje od poslednjeg reda
    for (int row = SEAT_ROWS - 1; row >= 0; row--)
    {
        // U svakom redu krecemo od najdesnijeg sedista
        for (int col = SEAT_COLS - 1; col >= count - 1; col--)
        {
            bool allFree = true;

            // Proveravamo count susednih sedista
            for (int i = 0; i < count; i++)
            {
                if (seatStatus[row][col - i] != 0)
                {
                    allFree = false;
                    break;
                }
            }

            // Ako smo pronasli count susednih slobodnih sedista
            if (allFree)
            {
                // Menjamo njihov status u Kupljeno
                for (int i = 0; i < count; i++)
                {
                    seatStatus[row][col - i] = 2;
                }

               
                return;
            }
        }
    }
}

// Obrada pritiska tastera/tastature
void seatKeyCallback(GLFWwindow* window,
    int key, int scancode,
    int action, int mods)
{
    // Reagujemo samo na pritisak tastera
    if (action != GLFW_PRESS)
    {
        return;
    }

    // ENTER - POCETAK SIMULACIJE
    if (key == GLFW_KEY_ENTER && cinemaPhase == 0)
    {
        // Prelazimo u fazu ulaska gledalaca
        cinemaPhase = 1;

        return;
    }

    // KUPOVINA KARATA 1-9
    // Ako je simulacija pocela, nema vise kupovine
    if (cinemaPhase != 0)
    {
        return;
    }

    // Dozvoljeni tasteri su od 1 do 9
    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9)
    {
        // Broj karata koji kupujemo
        int count = key - GLFW_KEY_0;

        buySeats(count);
    }
}

// Pronalazak statusa sedista
int getSeatStatus(int row, int col)
{
    if (row < 0 || row >= SEAT_ROWS ||
        col < 0 || col >= SEAT_COLS)
    {
        return -1;
    }

    return seatStatus[row][col];
}

float getSeatX(int col)
{
    return START_X + col * SEAT_SPACING_X;
}

float getSeatY(int row)
{
    return START_Y - row * SEAT_SPACING_Y;
}

// Resetovanje stanja bioskopa
void resetCinemaSession()
{
    // Reset dozvoljavamo samo nakon projekcije
    if (cinemaPhase != 3)
    {
        return;
    }

    // Sva sedista ponovo postaju slobodna
    for (int row = 0; row < SEAT_ROWS; row++)
    {
        for (int col = 0; col < SEAT_COLS; col++)
        {
            seatStatus[row][col] = 0;
        }
    }
    cinemaPhase = 0;
}