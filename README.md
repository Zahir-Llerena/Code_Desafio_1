Tetris Bitwise Engine (C++ Custom Core)
Este proyecto consiste en el desarrollo de un motor de Tetris para consola, diseñado bajo restricciones de alta eficiencia 
en el manejo de memoria y manipulación de datos a nivel de hardware. La lógica central del sistema se aleja de las representaciones 
matriciales convencionales, implementando en su lugar un núcleo basado en álgebra booleana y aritmética de punteros.

🎯 Objetivo del Proyecto
Desarrollar un sistema funcional de Tetris que gestione el tablero, las piezas, el movimiento y las colisiones mediante operadores 
bit a bit (Bitwise) y memoria dinámica, cumpliendo estrictamente con la normativa académica de prescindir de la Standard Template 
Library (STL) y tipos de datos de alto nivel como std::string.

🛠️ Características Técnicas
Gestión de Memoria: Implementación de un tablero dinámico utilizando un arreglo de punteros a tipos unsigned, donde cada fila se 
representa como una palabra binaria.

Motor Lógico Bitwise: Las colisiones se resuelven mediante máscaras de bits y el operador & (AND), mientras que el movimiento lateral y 
las rotaciones se ejecutan mediante desplazamientos (<<, >>) y operaciones | (OR).

Arquitectura Modular: Organización del código en múltiples archivos de cabecera (.h) e implementación (.cpp) para garantizar una 
separación clara entre la lógica del motor y la interfaz de usuario.

Cero Dependencias Externas: Desarrollo basado exclusivamente en librerías y funciones base de C++, optimizando el uso de 
recursos del sistema.

🚀 Desafío de Ingeniería
El principal reto de esta implementación es la traducción de mecánicas de juego visuales a operaciones lógicas directas sobre la memoria, 
permitiendo que un tablero de W bits de ancho sea procesado con la velocidad de una instrucción de CPU por fila.
