// Actividad Integradora - Algoritmos de Búsqueda de Patrones
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Función para leer el contenido completo de un archivo
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

    // Eliminar el último salto de línea si existe
    if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }

    return content;
}

// Función para construir la tabla LPS (Longest Prefix Suffix)
vector<int> computeLPS(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0; 
    int i = 1;

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

// Función para buscar el patrón en el texto usando KMP
// Devuelve el índice de inicio del patrón en el texto, o -1 si no se encuentra
int kmpSearch(const string& text, const string& pattern) {
    int n = text.length();
    int m = pattern.length();

    if (m == 0) return -1;  // Patrón vacío

    vector<int> lps = computeLPS(pattern);
    int i = 0;  // Índice para el texto
    int j = 0;  // Índice para el patrón

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
            if (j == m) {
                return (i - j) + 1;  // Patrón encontrado, devolver índice
            }
        } else {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return -1;  // Patrón no encontrado
}

// Función para leer el texto del archivo y buscar el patrón
int searchPatternInFile(const string& fileName, const string& pattern) {
    string fullText = readFileContent(fileName);
    return kmpSearch(fullText, pattern);
}

pair<int, int> longestPalindrome(const string& s) {
    if (s.empty()) return {0, 0};
    
    int n = s.length();
    int maxLength = 1;
    int start = 0;
    
    // Matriz para almacenar si s[i..j] es palíndromo
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
    
    // Verificar substrings de longitud mayor a 2
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
    
    return {start + 1, start + maxLength}; // +1 porque las posiciones comienzan en 1
}

// Función para leer el archivo y buscar el palíndromo más largo
pair<int, int> findLongestPalindromeInFile(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo: " << fileName << endl;
        return {0, 0};
    }

    string line;
    string fullText;

    // Leer todo el contenido del archivo en una sola cadena
    while (getline(file, line)) {
        fullText += line + "\n"; // Conservar saltos de línea
    }
    file.close();

    // Eliminar el último salto de línea si es necesario
    if (!fullText.empty() && fullText.back() == '\n') {
        fullText.pop_back();
    }

    return longestPalindrome(fullText);
}

pair<int, int> findLongestCommonSubstring(const string& text1, const string& text2) {
    int m = text1.length();
    int n = text2.length();
    int maxLength = 0;
    int endIndex = 0;

    // Matriz DP para almacenar longitudes de substrings comunes
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLength) {
                    maxLength = dp[i][j];
                    endIndex = i - 1; // Índice en text1
                }
            }
        }
    }

    if (maxLength == 0) {
        return {0, 0}; // No hay substrings comunes
    }

    int startIndex = endIndex - maxLength + 1;
    return {startIndex + 1, endIndex + 1}; // +1 para posiciones basadas en 1
}

int main() {
    string files[2] = {"transmission1.txt", "transmission2.txt"};
    string patterns[3] = {"mcode1.txt", "mcode2.txt", "mcode3.txt"};

    // Parte 1: Búsqueda de patrones maliciosos
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            string pattern = readFileContent(patterns[j]);
            if (pattern.empty()) continue;

            int index = searchPatternInFile(files[i], pattern);
            if (index != -1) {
                cout << "true " << index << endl;
            } else {
                cout << "false" << endl;
            }
        }
    }

    // Parte 2: Búsqueda de código espejeado (palíndromos)
    for (const auto& file : files) {
        auto palindrome = findLongestPalindromeInFile(file);
        cout <<  palindrome.first << " " << palindrome.second << endl;
    }

    // Parte 3: Comparación de similitud entre archivos
    string content1 = readFileContent(files[0]);
    string content2 = readFileContent(files[1]);

    if (!content1.empty() && !content2.empty()) {
        auto lcs = findLongestCommonSubstring(content1, content2);
        cout << lcs.first << " " << lcs.second << endl;
    }

    return 0;
}