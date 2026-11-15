#pragma once

// Inicijalizacija teksture i geometrije gledalaca
void initPeople();

// Odredjivanje broja gledalaca i njihovih sedista
void preparePeople();

// Prikaz gledaoca teksturom
void drawPeople(unsigned int shader);

// Trenutni broj gledalaca
int getPeopleCount();

// Azuriranje pozicija gledalaca tokom animacije
void updatePeople(float deltaTime);