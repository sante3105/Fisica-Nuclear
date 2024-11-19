#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <numeric>
#include <gsl/gsl_integration.h>


// Función que define f(t) según el tiempo en horas
double f(double t, void* params) {
    const double f_max = 1.0 / 6.0; // Constante de normalización
    if (t >= 6 && t <= 14) {
        return f_max * (t - 6) / 8.0; // Crecimiento lineal de 6 am a 2 pm
    } else if (t > 14 && t <= 18) {
        return f_max * (18 - t) / 4.0; // Decrecimiento lineal de 2 pm a 6 pm
    }
    return 0.0;
}
// Calcular con el metodo de punto medio
std::vector<double> metodoA() {
    std::vector<double> probabilidades;
    for (int hora = 6; hora < 18; ++hora) {
        double t_media = hora + 0.5; // Punto medio de la hora
        probabilidades.push_back(f(t_media, nullptr));
    }
    return probabilidades;
}
//Calcular con valor de la integral
std::vector<double> metodoB() {
    std::vector<double> probabilidades;

    // Crear un "workspace" para la integración una vez
    gsl_integration_workspace* w = gsl_integration_workspace_alloc(1000);

    // Configurar la función a integrar
    gsl_function F;
    F.function = &f;        // Apunta a tu función f definida previamente
    F.params = nullptr;     // No se pasan parámetros adicionales

    // Iterar sobre los intervalos horarios [hora, hora + 1]
    for (int hora = 6; hora < 18; ++hora) {
        double result, error;

        // Realizar la integración en el intervalo [hora, hora + 1]
        gsl_integration_qags(&F, hora, hora + 1, 0, 1e-7, 1000, w, &result, &error);

        // Almacenar el resultado en el vector de probabilidades
        probabilidades.push_back(result);
    }

    // Liberar el espacio de trabajo
    gsl_integration_workspace_free(w);

    return probabilidades;
}



int main() {
    // Calcular probabilidades con ambos métodos
    std::vector<double> probabilidadesA = metodoA();
    std::vector<double> probabilidadesB = metodoB();

    // Crear archivos para guardar los resultados
    std::ofstream archivoContinua("probabilidad_continua.txt");
    std::ofstream archivoIntegral("probabilidad_discretizada_integral.txt");
    std::ofstream archivoValorMedio("probabilidad_discretizada_valor_medio.txt");

    // Validar que los archivos se abran correctamente
    if (!archivoContinua.is_open() || !archivoIntegral.is_open() || !archivoValorMedio.is_open()) {
        std::cerr << "Error: no se pudieron abrir los archivos para escritura.\n";
        return 1;
    }

    // Suponiendo que tienes una función continua f(x) que evalúas en un rango [6, 12] con pasos pequeños
    archivoContinua << "x\tf(x)\n"; // Cabecera
    double paso = 0.1; // Resolución para la función continua
    for (double x = 6; x <= 18; x += paso) {
      double fx = f(x, nullptr); // Supongamos que f(x) calcula el valor continuo
        archivoContinua << x << "\t" << fx << "\n";
    }

    // Guardar resultados del método A en su archivo
    archivoValorMedio << "Hora\tProbabilidad\n"; // Cabecera
    for (int i = 0; i < probabilidadesA.size(); ++i) {
        archivoValorMedio << (6 + i) << "\t" << probabilidadesA[i] << "\n";
    }

    // Guardar resultados del método B en su archivo
    archivoIntegral << "Hora\tProbabilidad\n"; // Cabecera
    for (int i = 0; i < probabilidadesB.size(); ++i) {
        archivoIntegral << (6 + i) << "\t" << probabilidadesB[i] << "\n";
    }

    // Cerrar los archivos
    archivoContinua.close();
    archivoIntegral.close();
    archivoValorMedio.close();

    // Notificar al usuario
    std::cout << "Archivos generados para gnuplot:\n"
              << "- probabilidad_continua.txt\n"
              << "- probabilidad_discretizada_integral.txt\n"
              << "- probabilidad_discretizada_valor_medio.txt\n";

    return 0;
}
