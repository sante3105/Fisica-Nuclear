# Instrumentacion-Nuclear
Importante: Se usan las librerias gsl para los codigos en c++
---

## **Instrucciones de Uso**

### **1. Abrir Archivos de Texto con Emacs**
Para editar cualquier archivo de texto (como scripts `.cpp`, `.gnu`, o cualquier otro), utiliza el siguiente comando:

```bash
emacs nombre_del_archivo
```

Por ejemplo, para abrir un archivo llamado `punto1.cpp`:
```bash
emacs punto1.cpp
```

Para salir de Emacs:
- Presiona `Ctrl + X` y luego `Ctrl + C`.

---

### **2. Compilar Código en C++**
Para compilar un archivo `.cpp` con las librerías necesarias (`GSL`, `GSL CBLAS` y matemáticas), utiliza el siguiente comando:

```bash
g++ nombre_del_archivo.cpp -o nombre_de_salida -lgsl -lgslcblas -lm
```

Por ejemplo, para compilar `punto1.cpp` y generar un ejecutable llamado `punto1`:
```bash
g++ punto1.cpp -o punto1 -lgsl -lgslcblas -lm
```

Ejecuta el archivo compilado con:
```bash
./punto1
```

---

### **3. Ejecutar Scripts de Gnuplot**
#### **Abrir la Interfaz de Gnuplot**
Para abrir la interfaz interactiva de Gnuplot:
```bash
gnuplot
```

#### **Seleccionar un Terminal Compatible**
Antes de graficar, selecciona un terminal adecuado. Puedes usar:
1. **Terminal `wxt`** (gráfico interactivo):
   ```gnuplot
   set terminal wxt
   ```

2. **Terminal `png`** (guardar como imagen):
   ```gnuplot
   set terminal png
   set output 'nombre_del_grafico.png'
   ```

#### **Cargar un Script de Gnuplot**
Para cargar y ejecutar un archivo `.gnu`, utiliza:
```gnuplot
load 'nombre_del_archivo.gnu'
```

Por ejemplo:
```gnuplot
load 'grafica_punto1.gnu'
```

Para salir de Gnuplot, utiliza:
```gnuplot
exit
```

---

### **4. Usar JupyterLab**
#### **Activar el Entorno Virtual**
Primero, activa el entorno virtual en la carpeta principal del proyecto (`Instrumentacion-Nuclear`):
```bash
source myenv/bin/activate
```

#### **Iniciar JupyterLab**
1. Navega al directorio donde se encuentra tu archivo de trabajo.
   Por ejemplo:
   ```bash
   cd python/Simulaciones/simu1
   ```

2. Inicia JupyterLab con permisos de root:
   ```bash
   jupyter lab --allow-root
   ```

Esto abrirá JupyterLab en tu navegador. Si no se abre automáticamente, copia el enlace que aparece en la terminal y pégalo en tu navegador.

---

### **Notas Adicionales**
- **Emacs**: Para salir, presiona `Ctrl + X` y luego `Ctrl + C`.
- **Gnuplot**: Recuerda usar `exit` para cerrar la interfaz.
- **Python**: Siempre activa el entorno virtual con `source myenv/bin/activate` antes de ejecutar JupyterLab.