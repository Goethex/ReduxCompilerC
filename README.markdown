# ReduxCompilerC

## Español

### Descripción
**ReduxCompilerC** es un compilador experimental escrito en C que procesa un lenguaje de programación simplificado. Diseñado con fines educativos, realiza análisis léxico, sintáctico y semántico, soportando declaraciones de variables (`int`, `float`), sentencias `if-else`, y nombres de variables con caracteres UTF-8 (como `á`, `ñ`, `ü`) y especiales (`$`, `#`, `@`). El compilador genera una tabla de símbolos en formato tabular, mostrando el nombre, tipo, ámbito y línea de declaración de cada variable. Es ideal para aprender sobre diseño de compiladores y manejo de codificaciones UTF-8.

### Características
- **Análisis léxico**: Reconoce tokens como `int`, `float`, `if`, `else`, operadores (`+`, `-`, `>`, `<`, `>=`, `<=`, `==`) y números.
- **Análisis sintáctico**: Valida la estructura del código, incluyendo declaraciones, bloques `{}` y sentencias `if-else`.
- **Análisis semántico**: Gestiona ámbitos (global y local) y detecta errores como variables no declaradas o redeclaradas.
- **Soporte UTF-8**: Permite identificadores con caracteres españoles (`á`, `ñ`, `ü`) y especiales (`$`, `#`, `@`, etc.).
- **Tabla de símbolos**: Muestra nombre, tipo, ámbito y línea de declaración en un formato tabular.
- **Interfaz de consola**: Acepta entrada de código fuente y produce una salida detallada de tokens, análisis y errores.

### Requisitos
- **Compilador C**: GCC (recomendado) o cualquier compilador compatible con C99.
- **Sistema operativo**: Linux, macOS o Windows (con MinGW-w64 para Windows).
- **Terminal con soporte UTF-8**: Para mostrar correctamente caracteres como `á`, `ñ`, `ü`.
- Opcional: `make` para usar el Makefile (si está presente).

### Instrucciones de instalación
1. **Clonar el repositorio**:
   ```bash
   git clone https://github.com/Goethex/ReduxCompilerC.git
   cd ReduxCompilerC
   ```
2. **Compilar el código**:
   - Con GCC:
     ```bash
     gcc -o compiler -std=c99 -Wall -Wextra -finput-charset=UTF-8 main.c lexer.c parser.c
     ```
   - Con Makefile (si está presente):
     ```bash
     make
     ```
3. **Configurar UTF-8 en la terminal**:
   - Linux/macOS:
     ```bash
     export LANG=es_ES.UTF-8
     ```
   - Windows (Command Prompt):
     ```bash
     chcp 65001
     ```

### Uso
1. **Ejecutar el compilador**:
   ```bash
   ./compiler
   ```
2. **Ingresar el código fuente**:
   - Escribe el código línea por línea.
   - Termina con dos líneas vacías (presiona Enter dos veces).
3. **Ejemplo de entrada**:
   ```
   int áx = 10;
   int x$y = 5;
   if (áx > x$y) {
       int ñ = 2;
   } else {
       int ü#z = 3;
   }
   [Enter]
   [Enter]
   ```
4. **Salida esperada**:
   ```
   === Analisis Lexico ===
   Token: INT        Lexema: int        Linea: 1
   Token: ID         Lexema: áx        Linea: 1
   ...
   Token: ID         Lexema: ñ         Linea: 4
   ...
   Token: ID         Lexema: ü#z       Linea: 6
   ...

   === Analisis Sintactico ===
   Analisis sintactico completado con exito.

   === Analisis Semantico ===
   Tabla de simbolos:
   +-----------------+--------+----------------+-------+
   | Nombre          | Tipo   | Ambito         | Linea |
   +-----------------+--------+----------------+-------+
   | áx              | int    | global         | 1     |
   | x$y             | int    | global         | 2     |
   | ñ               | int    | local          | 4     |
   | ü#z             | int    | local          | 6     |
   +-----------------+--------+----------------+-------+
   Analisis semantico completado con exito.
   ```

### Estructura del proyecto
- `main.c`: Punto de entrada, coordina el análisis léxico, sintáctico y semántico.
- `lexer.c` / `lexer.h`: Analizador léxico, genera tokens del código fuente.
- `parser.c` / `parser.h`: Analizador sintáctico y semántico, construye el AST y la tabla de símbolos.
- `token.h`: Define los tipos de tokens y estructuras relacionadas.
- `Makefile` (si está presente): Facilita la compilación.

### Contribución
¡Las contribuciones son bienvenidas! Para contribuir:
1. Haz un fork del repositorio.
2. Crea una rama para tu funcionalidad:
   ```bash
   git checkout -b mi-funcionalidad
   ```
3. Realiza tus cambios y haz commit:
   ```bash
   git commit -m "Descripción de los cambios"
   ```
4. Envía un pull request al repositorio principal.

Sigue las convenciones de código (C99, comentarios claros) y prueba tus cambios.

### Licencia
Este proyecto está licenciado bajo la [Licencia MIT](LICENSE). Consulta el archivo `LICENSE` para más detalles.

### Contacto
- **Autor**: Goethex
- **GitHub**: [github.com/Goethex](https://github.com/Goethex)

Abre un issue en el repositorio para preguntas, sugerencias o errores. ¡Gracias por usar ReduxCompilerC!

---

## English

### Description
**ReduxCompilerC** is an experimental compiler written in C that processes a simplified programming language. Designed for educational purposes, it performs lexical, syntactic, and semantic analysis, supporting variable declarations (`int`, `float`), `if-else` statements, and identifiers with UTF-8 characters (e.g., `á`, `ñ`, `ü`) and special characters (e.g., `$`, `#`, `@`). The compiler outputs a tabular symbol table displaying each variable’s name, type, scope, and declaration line. It is ideal for learning about compiler design and UTF-8 encoding.

### Features
- **Lexical analysis**: Recognizes tokens like `int`, `float`, `if`, `else`, operators (`+`, `-`, `>`, `<`, `>=`, `<=`, `==`), and numbers.
- **Syntactic analysis**: Validates code structure, including declarations, blocks `{}`, and `if-else` statements.
- **Semantic analysis**: Manages scopes (global and local) and detects errors like undeclared or redeclared variables.
- **UTF-8 support**: Allows identifiers with Spanish characters (`á`, `ñ`, `ü`) and special characters (`$`, `#`, `@`, etc.).
- **Symbol table**: Displays name, type, scope, and declaration line in a tabular format.
- **Console interface**: Accepts source code input and produces detailed output of tokens, analysis, and errors.

### Requirements
- **C Compiler**: GCC (recommended) or any C99-compatible compiler.
- **Operating System**: Linux, macOS, or Windows (with MinGW-w64 for Windows).
- **UTF-8 terminal**: To properly display characters like `á`, `ñ`, `ü`.
- Optional: `make` for using the included Makefile (if present).

### Installation Instructions
1. **Clone the repository**:
   ```bash
   git clone https://github.com/Goethex/ReduxCompilerC.git
   cd ReduxCompilerC
   ```
2. **Compile the code**:
   - Using GCC:
     ```bash
     gcc -o compiler -std=c99 -Wall -Wextra -finput-charset=UTF-8 main.c lexer.c parser.c
     ```
   - Using Makefile (if present):
     ```bash
     make
     ```
3. **Set up UTF-8 in the terminal**:
   - Linux/macOS:
     ```bash
     export LANG=en_US.UTF-8
     ```
   - Windows (Command Prompt):
     ```bash
     chcp 65001
     ```

### Usage
1. **Run the compiler**:
   ```bash
   ./compiler
   ```
2. **Input the source code**:
   - Enter the code line by line.
   - End with two blank lines (press Enter twice).
3. **Example input**:
   ```
   int áx = 10;
   int x$y = 5;
   if (áx > x$y) {
       int ñ = 2;
   } else {
       int ü#z = 3;
   }
   [Enter]
   [Enter]
   ```
4. **Expected output**:
   ```
   === Lexical Analysis ===
   Token: INT        Lexema: int        Linea: 1
   Token: ID         Lexema: áx        Linea: 1
   ...
   Token: ID         Lexema: ñ         Linea: 4
   ...
   Token: ID         Lexema: ü#z       Linea: 6
   ...

   === Syntactic Analysis ===
   Syntactic analysis completed successfully.

   === Semantic Analysis ===
   Symbol table:
   +-----------------+--------+----------------+-------+
   | Name            | Type   | Scope          | Line  |
   +-----------------+--------+----------------+-------+
   | áx              | int    | global         | 1     |
   | x$y             | int    | global         | 2     |
   | ñ               | int    | local          | 4     |
   | ü#z             | int    | local          | 6     |
   +-----------------+--------+----------------+-------+
   Semantic analysis completed successfully.
   ```

### Project Structure
- `main.c`: Entry point, coordinates lexical, syntactic, and semantic analysis.
- `lexer.c` / `lexer.h`: Lexical analyzer, generates tokens from source code.
- `parser.c` / `parser.h`: Syntactic and semantic analyzer, builds the AST and symbol table.
- `token.h`: Defines token types and related structures.
- `Makefile` (if present): Simplifies project compilation.

### Contribution
Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a branch for your feature:
   ```bash
   git checkout -b my-feature
   ```
3. Make changes and commit:
   ```bash
   git commit -m "Description of changes"
   ```
4. Submit a pull request to the main repository.

Follow coding conventions (C99, clear comments) and test your changes.

### License
This project is licensed under the [MIT License](LICENSE). See the `LICENSE` file for details.

### Contact
- **Author**: Goethex
- **GitHub**: [github.com/Goethex](https://github.com/Goethex)

Open an issue in the repository for questions, suggestions, or bugs. Thank you for using ReduxCompilerC!