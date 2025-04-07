/*
 * Actividad Integradora - Algoritmos de Búsqueda de Patrones
 * Implementa:
 * 1. Búsqueda de patrones maliciosos con algoritmo KMP
 * 2. Detección del palíndromo más largo
 * 3. Búsqueda del substring común más largo entre archivos
 * Autores: Equipo 3
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// --------------------------------------------------
// FUNCIÓN: readFileContent
// DESCRIPCIÓN: Lee todo el contenido de un archivo
// COMPLEJIDAD: O(n) donde n = número de caracteres
// --------------------------------------------------
/**
 * @brief Lee el contenido completo de un archivo
 * @param fileName Nombre del archivo a leer
 * @return string Contenido del archivo o string vacío si hay error
 */
string readFileContent(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << fileName << endl;
        return "";
    }

    string content;
    string line;
    while (getline(file, line)) {
        content += line + "\n";
    }
    file.close();

    // Elimina el último salto de línea si existe
    if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }

    return content;
}

// --------------------------------------------------
// FUNCIÓN: computeLPS
// DESCRIPCIÓN: Preprocesa el patrón para KMP (tabla LPS)
// COMPLEJIDAD: O(m) donde m = longitud del patrón
// --------------------------------------------------
/**
 * @brief Construye la tabla LPS (Longest Prefix Suffix)
 * @param pattern Patrón a buscar
 * @return vector<int> Tabla LPS calculada
 */
vector<int> computeLPS(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0; // Longitud del prefijo sufijo más largo actual
    int i = 1;   // Índice para recorrer el patrón

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// --------------------------------------------------
// FUNCIÓN: kmpSearch
// DESCRIPCIÓN: Implementa el algoritmo KMP para búsqueda
// COMPLEJIDAD: O(n + m) donde n = texto, m = patrón
// --------------------------------------------------
/**
 * @brief Busca un patrón en texto usando KMP
 * @param text Texto donde buscar
 * @param pattern Patrón a buscar
 * @return int Posición (1-based) o -1 si no se encuentra
 */
int kmpSearch(const string& text, const string& pattern) {
    int n = text.length();
    int m = pattern.length();

    if (m == 0) return -1;

    vector<int> lps = computeLPS(pattern);
    int i = 0; // Índice para texto
    int j = 0; // Índice para patrón

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
            if (j == m) {
                return (i - j) + 1; // +1 para 1-based
            }
        } else {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return -1;
}

// --------------------------------------------------
// FUNCIÓN: longestPalindrome
// DESCRIPCIÓN: Encuentra el palíndromo más largo
// COMPLEJIDAD: O(n²) donde n = longitud del texto
// --------------------------------------------------
/**
 * @brief Encuentra el palíndromo más largo en una cadena
 * @param s Cadena de entrada
 * @return pair<int,int> Posiciones inicio/fin (1-based)
 */
pair<int, int> longestPalindrome(const string& s) {
    if (s.empty()) return {0, 0};
    
    int n = s.length();
    int maxLength = 1;
    int start = 0;
    
    vector<vector<bool>> dp(n, vector<bool>(n, false));
    
    // Todos los substrings de longitud 1 son palíndromos
    for (int i = 0; i < n; i++) {
        dp[i][i] = true;
    }
    
    // Verificar substrings de longitud 2
    for (int i = 0; i < n - 1; i++) {
        if (s[i] == s[i + 1]) {
            dp[i][i + 1] = true;
            start = i;
            maxLength = 2;
        }
    }
    
    // Verificar substrings de longitud > 2
    for (int k = 3; k <= n; k++) {
        for (int i = 0; i < n - k + 1; i++) {
            int j = i + k - 1;
            
            if (dp[i + 1][j - 1] && s[i] == s[j]) {
                dp[i][j] = true;
                
                if (k > maxLength) {
                    start = i;
                    maxLength = k;
                }
            }
        }
    }
    
    return {start + 1, start + maxLength};
}

// --------------------------------------------------
// FUNCIÓN: findLongestPalindromeInFile
// DESCRIPCIÓN: Busca palíndromo en archivo
// COMPLEJIDAD: O(n + m²) (lectura + búsqueda)
// --------------------------------------------------
/**
 * @brief Encuentra el palíndromo más largo en un archivo
 * @param fileName Nombre del archivo
 * @return pair<int,int> Posiciones inicio/fin (1-based)
 */
pair<int, int> findLongestPalindromeInFile(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << fileName << endl;
        return {0, 0};
    }

    string fullText;
    string line;
    while (getline(file, line)) {
        fullText += line + "\n";
    }
    file.close();

    if (!fullText.empty() && fullText.back() == '\n') {
        fullText.pop_back();
    }

    return longestPalindrome(fullText);
}

// --------------------------------------------------
// FUNCIÓN: findLongestCommonSubstring
// DESCRIPCIÓN: Encuentra el substring común más largo
// COMPLEJIDAD: O(n*m) donde n,m = longitudes de textos
// --------------------------------------------------
/**
 * @brief Encuentra el substring común más largo
 * @param text1 Primer texto
 * @param text2 Segundo texto
 * @return pair<int,int> Posiciones en text1 (1-based)
 */
pair<int, int> findLongestCommonSubstring(const string& text1, const string& text2) {
    int m = text1.length();
    int n = text2.length();
    int maxLength = 0;
    int endIndex = 0;

    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLength) {
                    maxLength = dp[i][j];
                    endIndex = i - 1;
                }
            }
        }
    }

    if (maxLength == 0) return {0, 0};

    int startIndex = endIndex - maxLength + 1;
    return {startIndex + 1, endIndex + 1};
}

// --------------------------------------------------
// FUNCIÓN: main
// DESCRIPCIÓN: Coordina las tres partes del programa
// COMPLEJIDAD: O(k*(n+m) + m² + n*m) (combinación)
// --------------------------------------------------
int main() {
    // Archivos a analizar
    string transmissions[2] = {"transmission1.txt", "transmission2.txt"};
    string maliciousCodes[3] = {"mcode1.txt", "mcode2.txt", "mcode3.txt"};

    // Parte 1: Búsqueda de patrones maliciosos
    for (int i = 0; i < 2; i++) {
        string transmission = readFileContent(transmissions[i]);
        if (transmission.empty()) continue;

        for (int j = 0; j < 3; j++) {
            string pattern = readFileContent(maliciousCodes[j]);
            if (pattern.empty()) continue;

            int pos = kmpSearch(transmission, pattern);
            if (pos != -1) {
                cout << "true " << pos << endl;
            } else {
                cout << "false " << endl;
            }
        }
    }

    // Parte 2: Palíndromos más largos
    for (const auto& file : transmissions) {
        auto result = findLongestPalindromeInFile(file);
        cout << result.first << " " << result.second << endl;
    }

    // Parte 3: Substring común más largo
    string t1 = readFileContent(transmissions[0]);
    string t2 = readFileContent(transmissions[1]);

    if (!t1.empty() && !t2.empty()) {
        auto lcs = findLongestCommonSubstring(t1, t2);
        cout << lcs.first << " " << lcs.second << endl;
    }

    return 0;
}