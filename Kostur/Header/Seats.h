
#pragma once

#include <GLFW/glfw3.h>

// Inicijalizacija geometrije sedista
void initSeats();

// Iscrtavanje svih sedista
void drawSeats(unsigned int shader);

// Obrada klika misa na sediste
void seatMouseCallback(GLFWwindow* window, int button, int action, int mods);

// Obrada pritiska tastera za kupovinu karata
void seatKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Vraca trenutnu fazu bioskopske simulacije
int getCinemaPhase();

// Vraca status sedista na zadatoj poziciji
int getSeatStatus(int row, int col);

// Vraca X koordinatu centra sedista
float getSeatX(int col);

// Vraca Y koordinatu centra sedista
float getSeatY(int row);

// Zavrsava ulazak gledalaca i zapocinje projekciju
void startProjection();

// Zavrsava projekciju i zapocinje fazu izlaska
void finishProjection();

// Vraca bioskop u pocetno stanje
void resetCinemaSession();