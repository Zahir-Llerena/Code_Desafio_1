
// ======================================================================================
// Universidad de Antioquia
// Facultad de Ingenieria - Departamento de Telecomunicaciones -
// @authors:
//        Clayder Areiza          CC: 1042150543
//        Arlington Zahir Llerena CC: 72 277 711
// ======================================================================================

// Librerias principales a invocar:

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cstdint> // Necesario para uint64_t (Soporta hasta 64 columnas)

// The beginning ------------------------------------------------------------------------

/**
 * BLOQUE 1: DEFINICIONES DE DATOS ---
 * Aquí definimos "qué" información vamos a manejar.
 */

// Usamos uint64_t para que cada fila sea un número de 64 bits (64 espacios).
typedef uint64_t board_t;

// La estructura que define cómo es una pieza de Tetris.
struct Pieza {
    board_t filas[4];
    int ancho;
    int alto;
};

/**
 * BLOQUE 2: LAS HERRAMIENTAS (FUNCIONES) ---
 * Definimos las tareas específicas antes de que el programa principal las use.
 */

// Tarea: Girar la pieza 90 grados.
void rotarPieza(Pieza &p) {
    Pieza nueva = {{0,0,0,0}, p.alto, p.ancho};
    for (int j = 0; j < p.ancho; j++) {
        board_t nuevaFila = 0;
        for (int i = 0; i < p.alto; i++) {
            if ((p.filas[i] >> j) & 1) {
                nuevaFila |= ((board_t)1 << (p.alto - 1 - i));
            }
        }
        nueva.filas[j] = nuevaFila;
    }
    p = nueva;
}

// Tarea: Detectar si la pieza toca un muro, el suelo o bloques ya fijos.
bool hayColision(const board_t* t, int ALTO, int ANCHO, const Pieza* p, int px, int py) {
    for (int i = 0; i < p->alto; i++) {
        int yCoord = py + i;
        if (yCoord >= ALTO) return true;
        if (px < 0 || (px + p->ancho) > ANCHO) return true;

        // Alineación precisa de la máscara según el ancho dinámico
        board_t piezaEnPos = p->filas[i] << (ANCHO - p->ancho - px);
        if ((t[yCoord] & piezaEnPos) != 0) return true;
    }
    return false;
}

// Tarea: Soldar la pieza al tablero cuando ya no puede bajar más.
void fijarPieza(board_t* t, int ANCHO, const Pieza* p, int px, int py) {
    for (int i = 0; i < p->alto; i++) {
        t[py + i] |= (p->filas[i] << (ANCHO - p->ancho - px));
    }
}

// Tarea: Revisar si hay filas completas para borrarlas y bajar las superiores.
void limpiarFilas(board_t* t, int ALTO, int ANCHO) {
    board_t filaLlena = 0;
    for(int i=0; i<ANCHO; i++) filaLlena |= ((board_t)1 << i);

    for (int i = 0; i < ALTO; i++) {
        if (t[i] == filaLlena) {
            for (int k = i; k > 0; k--) t[k] = t[k - 1];
            t[0] = 0;
        }
    }
}

// Tarea: Dibujar el estado actual del juego en la consola.
void renderizar(const board_t* t, int ALTO, int ANCHO, const Pieza* p, int px, int py) {
    system("clear || cls");
    for (int i = 0; i < ALTO; i++) {
        board_t filaVisual = t[i];
        if (p && i >= py && i < py + p->alto) {
            filaVisual |= (p->filas[i - py] << (ANCHO - p->ancho - px));
        }

        std::cout << "|";
        for (int j = ANCHO - 1; j >= 0; j--) {
            if ((filaVisual >> j) & 1) std::cout << "#";
            else std::cout << ".";
        }
        std::cout << "|\n";
    }
}

// Tarea: Entregar una de las 7 piezas clásicas de forma aleatoria.
Pieza obtenerPiezaAleatoria() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 6);

    Pieza piezas[7] = {
        {{0b1111,0,0,0}, 4, 1}, {{0b11,0b11,0,0}, 2, 2}, {{0b010,0b111,0,0}, 3, 2},
        {{0b011,0b110,0,0}, 3, 2}, {{0b110,0b011,0,0}, 3, 2}, {{0b100,0b111,0,0}, 3, 2},
        {{0b001,0b111,0,0}, 3, 2}
    };
    return piezas[dis(gen)];
}

/**
 * BLOQUE 3: Invocacion de las funciones ---
 * Aquí es donde el programa cobra vida, llamando a las funciones anteriores.
 */

int main() {
    int anchoUser, altoUser;

    std::cout << "--- CONFIGURACION DINAMICA ---" << std::endl;
    std::cout << "Ancho (multiplo de 8, max 64): ";
    std::cin >> anchoUser;
    std::cout << "Alto (minimo 8): ";
    std::cin >> altoUser;

    if (anchoUser < 8 || anchoUser % 8 != 0 || anchoUser > 64 || altoUser < 8) {
        std::cout << "Error: El ancho debe ser multiplo de 8 (max 64) y alto min 8." << std::endl;
        return 1;
    }

    // Memoria dinámica: Reservamos espacio para el tablero en el estante de la RAM.
    board_t* tablero = new board_t[altoUser];

    // Inicialización manual: Ponemos todo en "falso" (0)
    for (int i = 0; i < altoUser; i++) tablero[i] = 0;

    bool jugando = true;
    while (jugando) {
        Pieza piezaActual = obtenerPiezaAleatoria();
        int px = (anchoUser / 2) - (piezaActual.ancho / 2);
        int py = 0;

        // Comprobación de finalizacion al inicio de cada turno.
        if (hayColision(tablero, altoUser, anchoUser, &piezaActual, px, py)) {
            renderizar(tablero, altoUser, anchoUser, nullptr, 0, 0);
            std::cout << "--- GAME OVER ---" << std::endl;
            break;
        }

        bool turnoPieza = true;
        while (turnoPieza) {
            renderizar(tablero, altoUser, anchoUser, &piezaActual, px, py);

            char accion;
            std::cout << "Accion: [A]Izq [D]Der [S]Bajar [W]Rotar [Q]Salir: ";
            std::cin >> accion;

            if (accion == 'q' || accion == 'Q') { jugando = false; break; }

            int n_px = px, n_py = py;
            Pieza p_aux = piezaActual;

            if (accion == 'a' || accion == 'A') n_px--;
            else if (accion == 'd' || accion == 'D') n_px++;
            else if (accion == 's' || accion == 'S') n_py++;
            else if (accion == 'w' || accion == 'W') rotarPieza(p_aux);

            // Verificamos si el movimiento es posible antes de hacer la vuekta.
            if (!hayColision(tablero, altoUser, anchoUser, &p_aux, n_px, n_py)) {
                px = n_px; py = n_py; piezaActual = p_aux;

            } else if (accion == 's' || accion == 'S') {
                fijarPieza(tablero, anchoUser, &piezaActual, px, py);
                limpiarFilas(tablero, altoUser, anchoUser);
                turnoPieza = false;
            }
        }
    }

    // Devolvemos la memoria prestada antes de cerrar el programa.
    delete[] tablero;
    return 0;
}

// The End ------------------------------------------------------------------------------







