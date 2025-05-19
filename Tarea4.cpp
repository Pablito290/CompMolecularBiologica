#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

typedef vector<vector<int>> matriz;


int alpha1(char a, char b) {
    if ((a == 'C' && b == 'G') || (a == 'G' && b == 'C'))
        return -5;
    if ((a == 'A' && b == 'U') || (a == 'U' && b == 'A'))
        return -4;
    if ((a == 'G' && b == 'U') || (a == 'U' && b == 'G'))
        return -1;
    return 0;
}

int alpha2(char a, char b) {
    if ((a == 'C' && b == 'G') || (a == 'G' && b == 'C') ||
        (a == 'A' && b == 'U') || (a == 'U' && b == 'A')) {
        return -1;
    }
    return 0;
}

struct algoritmo {
    string cadena;
    matriz m;
    int tam;
    vector<pair<int, int>> pares;

    algoritmo(string c) {
        cadena = c;
        tam = c.size();
        m = vector<vector<int>>(tam, vector<int>(tam, 0));
    }

    
    void print(matriz m) {
        cout << "Matriz de energia:\n";

        for (int i = 0; i < m.size(); i++) {
            for (int j = 0; j < m[0].size(); j++) {
                cout << m[i][j] << "\t";
            }
            cout << endl;
        }
        cout << "\nPares formados:\n";
        for (auto p : pares) {
            cout << p.first << " - " << p.second << " (" << cadena[p.first] << "-" << cadena[p.second] << ")\n";
        }
    }

    void calcular(int (*alpha)(char, char)) {
        for (int d = 1; d < tam; d++) {
            for (int i = 0; i + d < tam; i++) {
                int j = i + d;
                int maximo = m[i + 1][j];
                maximo = min(maximo, m[i][j - 1]);
                maximo = min(maximo, m[i + 1][j - 1] + alpha(cadena[i], cadena[j]));

                for (int k = i + 1; k < j; ++k) {
                    maximo = min(maximo, m[i][k] + m[k + 1][j]);
                }

                m[i][j] = maximo;
            }
        }
    }

    void backtrack(int i, int j, vector<pair<int, int>>& pares, int (*alpha)(char, char)) {
        if (i >= j) return;

        if (m[i][j] == m[i + 1][j]) {
            backtrack(i + 1, j, pares, alpha);
        }
        else if (m[i][j] == m[i][j - 1]) {
            backtrack(i, j - 1, pares, alpha);
        }
        else if (m[i][j] == m[i + 1][j - 1] + alpha(cadena[i], cadena[j])) {
            if (alpha(cadena[i], cadena[j]) < 0) {
                pares.push_back({ i, j });
                backtrack(i + 1, j - 1, pares, alpha);
            }
        }
        else {
            for (int k = i + 1; k < j; ++k) {
                if (m[i][j] == m[i][k] + m[k + 1][j]) {
                    backtrack(i, k, pares, alpha);
                    backtrack(k + 1, j, pares, alpha);
                    break;
                }
            }
        }
    }

     void resolver(int (*alpha)(char, char)) {
        calcular(alpha);
        vector<pair<int, int>> p;
        backtrack(0, tam - 1, p, alpha);
        pares = p;
        print(m);
    }

     void clear() {
         m = vector<vector<int>>(tam, vector<int>(tam, 0));
         pares.clear();
     }
};

int main() {
    string rna = "GGAAAUCC";

    algoritmo a(rna);
    cout << "Cadena: " << rna << endl<<endl<<endl;

    cout << "funcion 1" << endl;
    a.resolver(alpha1);
    a.clear();
    cout << endl << "-----------------------------------------------------" << endl;

    cout << "funcion 2"<<endl;
    a.resolver(alpha2);

    cout << endl << "--------------------------------------------------------------------------------------------" << endl;

    rna = "ACUCGAUUCCGAG";
    cout << "Cadena: " << rna << endl << endl << endl;


    algoritmo b(rna);
    cout << "funcion 1" << endl;
    b.resolver(alpha1);
    b.clear();
    cout << endl << "-----------------------------------------------------" << endl;

    cout << "funcion 2" << endl;
    b.resolver(alpha2);


    return 0;
}
