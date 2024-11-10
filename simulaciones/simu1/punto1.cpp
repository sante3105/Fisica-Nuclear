#include <iostream>
#include <vector>

// Definimos f_max como una constante
const double f_max = 1.0 / 6.0; // Altura máxima para asegurar que el área total sea 1

// Función que define f(t) según el tiempo en horas
double f(double t) {
    if (t >= 6 && t <= 14) {
        return f_max * (t - 6) / 8.0; // Crecimiento lineal de 6 am a 2 pm
    } else if (t > 14 && t <= 18) {
        return f_max * (18 - t) / 4.0; // Decrecimiento lineal de 2 pm a 6 pm
    }
    return 0.0;
}

// Método (a): Calcular el valor medio en el punto medio de cada intervalo
std::vector<double> metodoA() {
    std::vector<double> probabilidades;
    for (int hora = 6; hora < 18; ++hora) {
        double t_media = hora + 0.5; // Punto medio de la hora
        probabilidades.push_back(f(t_media));
    }
    return probabilidades;
}

// Función para calcular la integral según el Caso 1
double integralCaso1(double t1, double t2) {
    return (f_max / 8.0) * ((t2 * t2 / 2.0) - 6 * t2 - (t1 * t1 / 2.0) + 6 * t1);
}

// Función para calcular la integral según el Caso 2
double integralCaso2(double t1, double t2) {
    return (-f_max / 4.0) * ((t2 * t2 / 2.0) - 18 * t2 - (t1 * t1 / 2.0) + 18 * t1);
}

// Función para calcular la integral según el Caso 3
double integralCaso3(double t1, double t2) {
    double part1 = (f_max / 8.0) * (14 - (t1 * t1 / 2.0) + 6 * t1);
    double part2 = (-f_max / 4.0) * ((t2 * t2 / 2.0) - 18 * t2 + 154);
    return part1 + part2;
}

// Método (b): Calcular probabilidades usando las fórmulas integrales proporcionadas
std::vector<double> metodoB() {
    std::vector<double> probabilidades;
    for (int hora = 6; hora < 18; ++hora) {
        double t1 = hora;
        double t2 = hora + 1;
        
        // Determinamos cuál caso aplicar según el intervalo de horas
        if (t2 <= 14) {
            // Caso 1: Intervalos entre 6 am y 2 pm
            probabilidades.push_back(integralCaso1(t1, t2));
        } else if (t1 >= 14) {
            // Caso 2: Intervalos entre 2 pm y 6 pm
            probabilidades.push_back(integralCaso2(t1, t2));
        } else {
            // Caso 3: Intervalo mixto que cruza 2 pm
            probabilidades.push_back(integralCaso3(t1, t2));
        }
    }
    return probabilidades;
}

int main() {
    // Calcular probabilidades con ambos métodos
    std::vector<double> probabilidadesA = metodoA();
    std::vector<double> probabilidadesB = metodoB();
    
    // Mostrar resultados para el método (a)
    std::cout << "Probabilidades usando el método (a) - valor medio:" << std::endl;
    for (int i = 0; i < probabilidadesA.size(); ++i) {
        std::cout << "Hora " << (6 + i) << " a " << (7 + i) << ": " << probabilidadesA[i] << std::endl;
    }

    // Mostrar resultados para el método (b)
    std::cout << "\nProbabilidades usando el método (b) - integral:" << std::endl;
    for (int i = 0; i < probabilidadesB.size(); ++i) {
        std::cout << "Hora " << (6 + i) << " a " << (7 + i) << ": " << probabilidadesB[i] << std::endl;
    }

    return 0;
}
