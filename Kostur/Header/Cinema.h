#pragma once

// Inicijalizacija bioskopske sale
void initCinema();

// Iscrtavanje zidova i vrata sale
void drawCinema(unsigned int shader, bool doorOpen);

// Iscrtavanje tamnosivog providnog sloja
void drawCinemaOverlay(unsigned int shader);
