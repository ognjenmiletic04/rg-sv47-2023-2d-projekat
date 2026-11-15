
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Header/Util.h"
#include "../Header/Seats.h"
#include "../Header/Cinema.h"
#include "../Header/People.h"


int main()
{
    // Inicijalizacija GLFW-a
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Kreiranje prozora
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Kreiranje prozora preko celog ekrana
    GLFWwindow* window = glfwCreateWindow(
        mode->width,
        mode->height,
        "Bioskop",
        monitor,
        NULL
    );

    if (window == NULL)
        return endProgram("Prozor nije uspeo da se kreira.");

    glfwMakeContextCurrent(window);

    // Podesavanje OpenGL viewport-a
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    // Inicijalizacija GLEW-a
    if (glewInit() != GLEW_OK)
        return endProgram("GLEW nije uspeo da se inicijalizuje.");

    // Omogucavanje providnosti
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Kreiranje shader programa
    unsigned int basicShader = createShader(
        "Shaders/basic.vert",
        "Shaders/basic.frag"
    );

    // Sejder za prikaz gledalaca pomocu teksture
    unsigned int peopleShader = createShader(
        "Shaders/people.vert",
        "Shaders/people.frag"
    );

    // Temena bioskopskog platna
    // x, y, r, g, b, a
    float verticesScreen[] = {
        -0.60f,  0.06f,  1.0f, 1.0f, 1.0f, 1.0f,
        -0.60f, -0.06f,  1.0f, 1.0f, 1.0f, 1.0f,
         0.60f, -0.06f,  1.0f, 1.0f, 1.0f, 1.0f,
         0.60f,  0.06f,  1.0f, 1.0f, 1.0f, 1.0f
    };

    // Kreiranje VAO i VBO
    unsigned int VAOscreen;
    unsigned int VBOscreen;

    glGenVertexArrays(1, &VAOscreen);
    glGenBuffers(1, &VBOscreen);

    glBindVertexArray(VAOscreen);
    glBindBuffer(GL_ARRAY_BUFFER, VBOscreen);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(verticesScreen),
        verticesScreen,
        GL_STATIC_DRAW
    );

    // Atribut 0 - pozicija
    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);

    // Atribut 1 - boja RGBA
    glVertexAttribPointer(
        1, 4, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

	// Inicijalizacija sedista
	initSeats();

	// Inicijalizacija bioskopske sale
	initCinema();

	// Inicijalizacija gledalaca
	initPeople();

    // Povezivanje klika misa sa rezervacijom sedista
    glfwSetMouseButtonCallback(window, seatMouseCallback);

    // Povezivanje tastature sa kupovinom karata
    glfwSetKeyCallback(window, seatKeyCallback);

    // Tamna pozadina
    glClearColor(0.10f, 0.10f, 0.15f, 1.0f);

    // Da li je izvrsena dodela sedista gledaocima?
    bool peoplePrepared = false;

    // Vreme prethodnog frejma za animaciju
    double previousFrameTime = glfwGetTime();

    // Glavna petlja
    while (!glfwWindowShouldClose(window))
    {
        double initFrameTime = glfwGetTime();

        // Vreme proteklo od prethodnog frejma
        float deltaTime =
            (float)(initFrameTime - previousFrameTime);

        previousFrameTime = initFrameTime;

        // Izlaz na ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            break;
        }

        // Kada Enter zapocne simulaciju, pripremamo gledaoce
        if (getCinemaPhase() == 1 && !peoplePrepared)
        {
            preparePeople();
            peoplePrepared = true;
        }

        // Azuriranje kretanja gledalaca
        if (getCinemaPhase() == 1)
        {
            updatePeople(deltaTime);
        }

        // Brisanje prethodnog frejma
        glClear(GL_COLOR_BUFFER_BIT);

        // 1. Zidovi i zatvorena vrata
        // Vrata su otvorena kada simulacija pocne
        drawCinema(basicShader, getCinemaPhase() != 0);


        // 2. Bioskopsko platno
        glUseProgram(basicShader);

        glUniform2f(
            glGetUniformLocation(basicShader, "uPos"),
            0.0f, 0.78f
        );

        glBindVertexArray(VAOscreen);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        // 3. Pedeset plavih sedista
        drawSeats(basicShader);

        // 4. Gledalac se pojavljuje nakon pritiska na Enter
        // Prikaz gledaoca sa PNG teksturom
        if (getCinemaPhase() == 1)
        {
            drawPeople(peopleShader);
        }

        // 5. Tamnosivi sloj sa providnoscu 0.5
        if (getCinemaPhase() == 0)
        {
            drawCinemaOverlay(basicShader);
        }

        // Prikaz frejma
        glfwSwapBuffers(window);

        glfwPollEvents();

        // Frame limiter - 75 FPS
        while (glfwGetTime() - initFrameTime < 1 / 75.0) {}
    }

    // Oslobadjanje resursa
    glDeleteProgram(basicShader);
    glDeleteProgram(peopleShader);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
