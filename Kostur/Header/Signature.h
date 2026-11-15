#pragma once

// Inicijalizacija teksture sa potpisom
void initSignature();

// Iscrtavanje potpisa u donjem desnom uglu
void drawSignature(unsigned int shader);

// Oslobadjanje resursa potpisa
void deleteSignature();