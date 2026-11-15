
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>

#include "../Header/People.h"
#include "../Header/Seats.h"
#include "../Header/Util.h"

// VAO i VBO za teksturiranog gledaoca
unsigned int VAOperson;
unsigned int VBOperson;

// PNG tekstura gledaoca
unsigned int personTexture;

// Maksimalno 50 gledalaca, jer ima 50 sedista
const int MAX_PEOPLE = 50;

// Koliko gledalaca ulazi u salu
int peopleCount = 0;

// Ciljni red i kolona za svakog gledaoca
int personTargetRow[MAX_PEOPLE];
int personTargetCol[MAX_PEOPLE];

// Trenutna pozicija svakog gledaoca
float personX[MAX_PEOPLE];
float personY[MAX_PEOPLE];

// Pozicija ulaza u salu
const float PEOPLE_START_X = -0.89f;
const float PEOPLE_START_Y = 0.74f;

// Brzina kretanja gledalaca
const float PEOPLE_SPEED = 0.60f;

// Vremenski razmak izmedju ulazaka gledalaca
const float ENTRY_DELAY = 0.40f;

// Vreme proteklo od pocetka ulaska
float peopleElapsed = 0.0f;

// Vreme proteklo od pocetka izlaska
float exitElapsed = 0.0f;

// Razmak izmedju pocetaka izlaska gledalaca
const float EXIT_DELAY = 0.40f;

// Pojedinacni gledaoci koji su napustili salu
bool personExited[MAX_PEOPLE] = {};

// Inicijalizacija teksture gledaoca
void initPeople()
{
    // Jedan pravougaonik sa UV koordinatama
    // x, y, r, g, b, a, s, t

    float verticesPerson[] = {
        -0.045f,  0.080f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.045f, -0.080f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.045f, -0.080f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.045f,  0.080f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    // Kreiranje VAO i VBO
    glGenVertexArrays(1, &VAOperson);
    glGenBuffers(1, &VBOperson);

    glBindVertexArray(VAOperson);
    glBindBuffer(GL_ARRAY_BUFFER, VBOperson);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(verticesPerson),
        verticesPerson,
        GL_STATIC_DRAW
    );

    // Atribut 0 - pozicija
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    // Atribut 1 - RGBA boja
    glVertexAttribPointer(
        1, 4, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Atribut 2 - UV koordinate
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    // Ucitavanje slike
    personTexture = loadImageToTexture("Resources/person.png");

    // Podesavanje teksture
    glBindTexture(GL_TEXTURE_2D, personTexture);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE
    );
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE
    );
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR
    );
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR
    );

    glBindTexture(GL_TEXTURE_2D, 0);
}


// Odredjujemo nasumicni broj gledalaca i dodeljujemo im sedista
void preparePeople()
{

    // Resetovanje vremena animacije
    peopleElapsed = 0.0f;
    // Resetovanje vremena izlaska
    exitElapsed = 0.0f;
    int candidateRows[MAX_PEOPLE];
    int candidateCols[MAX_PEOPLE];

    int candidateCount = 0;

    // Pronalazimo sva rezervisana i kupljena sedista
    for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            int status = getSeatStatus(row, col);

            if (status == 1 || status == 2)
            {
                candidateRows[candidateCount] = row;
                candidateCols[candidateCount] = col;

                candidateCount++;
            }
        }
    }

    // Pocetak pseudo-slucajnog niza
    // Koristimo GLFW vreme da se izbor menja
    std::srand(
        (unsigned int)(glfwGetTime() * 1000000.0)
    );

    // Nasumican broj od 1 do candidateCount 
    peopleCount = std::rand() % candidateCount + 1;

    std::cout << "--------------------------------\n";
    std::cout << "Rezervisanih i kupljenih sedista: "
        << candidateCount << "\n";
    std::cout << "Broj gledalaca koji ulaze: "
        << peopleCount << "\n";

    // Svakom gledaocu dodeljujemo razlicito sediste
    for (int i = 0; i < peopleCount; i++)
    {
        // Biramo jedno od preostalih sedista
        int randomIndex =
            i + std::rand() % (candidateCount - i);

        // Razmenjujemo izabrano sediste sa sedistem i
        int tempRow = candidateRows[i];
        int tempCol = candidateCols[i];

        candidateRows[i] = candidateRows[randomIndex];
        candidateCols[i] = candidateCols[randomIndex];

        candidateRows[randomIndex] = tempRow;
        candidateCols[randomIndex] = tempCol;

        // Pamtimo ciljno sediste gledaoca
        personTargetRow[i] = candidateRows[i];
        personTargetCol[i] = candidateCols[i];

        // Svaki gledalac krece od ulaza u salu
        personX[i] = PEOPLE_START_X;
        personY[i] = PEOPLE_START_Y;

        // Gledalac jos nije napustio salu
        personExited[i] = false;

		// Ispis u konzoli koji gledalac ide na koje sediste
        std::cout << "Gledalac " << i + 1
            << " -> red " << personTargetRow[i] + 1
            << ", sediste " << personTargetCol[i] + 1
            << "\n";
    }

    std::cout << "--------------------------------"
        << std::endl;
}

// Broj gledalaca
int getPeopleCount()
{
    return peopleCount;
}

// Animacija kretanja gledalaca ka njihovim sedistima
void updatePeople(float deltaTime)
{
    // Protok vremena od pocetka ulaska
    peopleElapsed += deltaTime;

    // Azuriramo svakog gledaoca
    for (int i = 0; i < peopleCount; i++)
    {
        // Gledaoci ne ulaze svi istovremeno
        if (peopleElapsed < i * ENTRY_DELAY)
        {
            continue;
        }

        // Ciljna pozicija dodeljenog sedista
        float targetX = getSeatX(personTargetCol[i]);
        float targetY = getSeatY(personTargetRow[i]);

        // Put koji gledalac moze preci tokom frejma
        float step = PEOPLE_SPEED * deltaTime;

        // 1. Prvo vertikalno kretanje nanize
        if (personY[i] > targetY)
        {
            float distanceY = personY[i] - targetY;

            if (step >= distanceY)
            {
                // Gledalac je stigao do svog reda
                personY[i] = targetY;

                // Preostali deo kretanja u ovom frejmu
                step -= distanceY;
            }
            else
            {
                // Jos nije stigao do reda
                personY[i] -= step;
                step = 0.0f;
            }
        }

        // 2. Zatim horizontalno kretanje
        if (personY[i] <= targetY &&
            personX[i] < targetX &&
            step > 0.0f)
        {
            personX[i] += step;

            // Ne dozvoljavamo prelazak preko sedista
            if (personX[i] > targetX)
            {
                personX[i] = targetX;
            }
        }
    }
}

// Provera da li su svi gledaoci stigli do sedista
bool allPeopleSeated()
{
    // Proveravamo svakog gledaoca
    for (int i = 0; i < peopleCount; i++)
    {
        // Koordinate dodeljenog sedista
        float targetX = getSeatX(personTargetCol[i]);
        float targetY = getSeatY(personTargetRow[i]);

        if (personY[i] > targetY ||
            personX[i] < targetX)
        {
            return false;
        }
    }

    // Svi gledaoci su stigli
    return true;
}

// Iscrtavanje gledalaca kao PNG teksture
void drawPeople(unsigned int shader)
{
    if (peopleCount == 0 || personTexture == 0)
    {
        return;
    }

    glUseProgram(shader);

    // Vezivanje PNG teksture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, personTexture);

    glUniform1i(
        glGetUniformLocation(shader, "uTex0"),
        0
    );

    glBindVertexArray(VAOperson);

    int posLocation =
        glGetUniformLocation(shader, "uPos");

    for (int i = 0; i < peopleCount; i++)
    {
        // Gledalac jos nije poceo da ulazi
        if (peopleElapsed < i * ENTRY_DELAY)
        {
            continue;
        }

        // Gledalac je napustio bioskop
        if (getCinemaPhase() == 3 && personExited[i])
        {
            continue;
        }

        // Trenutna pozicija gledaoca
        glUniform2f(
            posLocation,
            personX[i],
            personY[i]
        );

        // Crtanje teksturiranog gledaoca
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}

// Animacija kretanja gledalaca ka izlazu iz sale
void updatePeopleExit(float deltaTime)
{
    // Vreme od pocetka izlaska
    exitElapsed += deltaTime;

    // Azuriramo svakog gledaoca
    for (int i = 0; i < peopleCount; i++)
    {
        // Gledalac je vec izasao
        if (personExited[i])
        {
            continue;
        }

        // Gledaoci ne krecu svi istovremeno
        if (exitElapsed < i * EXIT_DELAY)
        {
            continue;
        }

        // Put koji gledalac predje u jednom frejmu
        float step = PEOPLE_SPEED * deltaTime;

        // 1. Prvo horizontalno kretanje ulevo
        if (personX[i] > PEOPLE_START_X)
        {
            float distanceX =
                personX[i] - PEOPLE_START_X;

            if (step >= distanceX)
            {
                // Stigao je do prolaza
                personX[i] = PEOPLE_START_X;

                // Preostalo kretanje u ovom frejmu
                step -= distanceX;
            }
            else
            {
                // Nastavlja ulevo
                personX[i] -= step;
                step = 0.0f;
            }
        }

		// 2. Zatim vertikalno kretanje nagore
        if (personX[i] <= PEOPLE_START_X &&
            personY[i] < PEOPLE_START_Y &&
            step > 0.0f)
        {
            float distanceY =
                PEOPLE_START_Y - personY[i];

            if (step >= distanceY)
            {
                // Stigao je do ulaza
                personY[i] = PEOPLE_START_Y;
            }
            else
            {
                // Nastavlja nagore
                personY[i] += step;
            }
        }

		// 3. Provera da li je gledalac izasao iz sale
        if (personX[i] <= PEOPLE_START_X &&
            personY[i] >= PEOPLE_START_Y)
        {
            personExited[i] = true;
        }
    }
}

// Provera da li su svi gledaoci napustili salu
bool allPeopleExited()
{
    for (int i = 0; i < peopleCount; i++)
    {
        // Ako makar jedan jos nije izasao
        if (!personExited[i])
        {
            return false;
        }
    }

    // Svi gledaoci su izasli
    return true;
}