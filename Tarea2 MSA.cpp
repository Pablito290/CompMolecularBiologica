#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <stack>
using namespace std;



typedef vector<char> cadena;

cadena convertirAcadena(const string& str) {
	return cadena(str.begin(), str.end());
}

struct Estado {
	int i, j;
	cadena alineado_A, alineado_B;
};

struct Resultado {
	cadena alineado_A, alineado_B;
	int puntaje;
	int rupturas;

	// Constructor por defecto
	Resultado() : alineado_A(), alineado_B(), puntaje(0), rupturas(0) {}

	// Constructor principal
	Resultado(cadena a, cadena b, int p = 0, int r = 0)
		: alineado_A(std::move(a)), alineado_B(std::move(b)), puntaje(p), rupturas(r) {}

	void printall() {
		cout << "Cadena 1:\n" << string(alineado_A.begin(), alineado_A.end()) << endl;
		cout << "Cadena 2:\n" << string(alineado_B.begin(), alineado_B.end()) << endl;
		cout << "Puntaje :" << puntaje << endl;
		cout << "Rupturas :" << rupturas << endl << endl;
	}
};



struct Global_alignment {
	vector<Resultado> resultados;

	vector<vector<pair<int, set<char>>>> table;

	cadena A;
	int tam_A;

	cadena B;
	int tam_B;

	int match = 1;
	int missmatch = -1;
	int gap = -2;

	Global_alignment(cadena _A, cadena _B) {
		A = _A;
		B = _B;
		tam_A = A.size();
		tam_B = B.size();
		table.resize(tam_A + 1, vector<pair<int, set<char>>>(tam_B + 1, { 0,{} }));
	}

	void alignment() {
		int rows = A.size() + 1;
		int cols = B.size() + 1;

		for (int i = 0; i < table.size(); i++) { //llenado de valores conocidos
			table[i][0].first = -2 * i;
			table[i][0].second.insert('t');
		}
		for (int i = 0; i < table[0].size(); i++) {
			table[0][i].first = -2 * i;
			table[0][i].second.insert('i');

		}

		for (int i = 1; i < rows; i++) {
			for (int j = 1; j < cols; j++) {
				int score_diag;
				if (A[i - 1] == B[j - 1]) {
					score_diag = table[i - 1][j - 1].first + match;
				}
				else {
					score_diag = table[i - 1][j - 1].first + missmatch;
				}

				int score_left = table[i][j - 1].first + gap;
				int score_top = table[i - 1][j].first + gap;

				// Elegir el máximo
				int max_score = max({ score_diag, score_left, score_top });
				table[i][j].first = max_score;

				// Insertar la dirección según el valor
				if (score_diag == max_score) table[i][j].second.insert('d');
				if (score_left == max_score) table[i][j].second.insert('i');
				if (score_top == max_score) table[i][j].second.insert('t');
			}
		}

	}
	
	void printTable() {
		for (int i = 0; i < table.size(); i++) {
			for (int j = 0; j < table[i].size(); j++) {
				cout << table[i][j].first << "\t";
			}
			cout << endl;
		}
	}

	void printTable_guide() {
		for (int i = 0; i < table.size(); i++) {
			for (int j = 0; j < table[i].size(); j++) {
				cout << table[i][j].first << " ";
				for (auto it : table[i][j].second) {
					cout << it << " ";
				}
				cout << "\t";
			}
			cout << endl;
		}
	}

	int contarRupturasPorGaps(const cadena& a, const cadena& b) {
		if (a.size() != b.size()) return -1; // seguridad

		int rupturas = 0;
		char estado_anterior;

		// Determinar el estado inicial
		estado_anterior = (a[0] == '-' || b[0] == '-') ? 'G' : 'A';

		for (size_t i = 1; i < a.size(); ++i) {
			char estado_actual = (a[i] == '-' || b[i] == '-') ? 'G' : 'A';

			if (estado_actual != estado_anterior) {
				rupturas++;
			}
			estado_anterior = estado_actual;
		}
		return rupturas;
	}

	void Alineada_iterativaCompleta() {
		stack<Estado> pila;
		pila.push({ tam_A, tam_B, {}, {} });

		while (!pila.empty()) {
			Estado actual = pila.top();
			pila.pop();

			int i = actual.i;
			int j = actual.j;
			cadena alineado_A = actual.alineado_A;
			cadena alineado_B = actual.alineado_B;

			if (i == 0 && j == 0) {
				reverse(alineado_A.begin(), alineado_A.end());
				reverse(alineado_B.begin(), alineado_B.end());

				int r = contarRupturasPorGaps(alineado_A, alineado_B);
				resultados.push_back(Resultado(alineado_A, alineado_B, table[tam_A][tam_B].first, r));

				continue;
			}

			for (auto it : table[i][j].second) {
				cadena new_A = alineado_A;
				cadena new_B = alineado_B;

				if (it == 'd') {
					new_A.push_back(A[i - 1]);
					new_B.push_back(B[j - 1]);
					pila.push({ i - 1, j - 1, new_A, new_B });
				}
				else if (it == 't') {
					new_A.push_back(A[i - 1]);
					new_B.push_back('-');
					pila.push({ i - 1, j, new_A, new_B });
				}
				else if (it == 'i') {
					new_A.push_back('-');
					new_B.push_back(B[j - 1]);
					pila.push({ i, j - 1, new_A, new_B });
				}
			}
		}
	}

	void ordenarPorRupturas() {
		sort(resultados.begin(), resultados.end(), [](const Resultado& a, const Resultado& b) {
			return a.rupturas < b.rupturas;
			});
	}

	void prints() {
		for (auto it : resultados) {
			it.printall();
		}

	}

	void printbetter() {
		resultados[0].printall();
	}
	
	Resultado getbetter() {
		alignment();
		//cout << "Tabla completa" << endl;
		Alineada_iterativaCompleta();
		ordenarPorRupturas();
		return resultados[0];
	}

};

struct MSA {
	vector<cadena> cadenas;
	vector<vector<Resultado>> tabla;
	int maxlenght = 0;
	MSA(vector<cadena> c) {
		cadenas = c;
		tabla.resize(cadenas.size(), vector<Resultado>(cadenas.size()));
		maxlenght = mayor();
	}
	int mayor() {
		int mayor = 0;
		for (auto it : cadenas) {
			mayor = std::max((int)it.size(), mayor);
		}
		return mayor;
	}
	int BestValue() {
		int tam = tabla.size();
		for (int i = 0; i < tam; i++) {
			for (int j = i; j < tam; j++) {
				Global_alignment auxi(cadenas[i], cadenas[j]);
				tabla[i][j] = auxi.getbetter();
				tabla[j][i] = tabla[i][j];
			}
		}
		int max = -100000;
		int id = -1;
		for (int i = 0; i < tam; i++) {
			int localsum = 0;
			for (int j = 0; j < tam; j++) {
				if (i != j) {
					localsum += tabla[j][i].puntaje;
					
				}
			}
			if (localsum > max) {
				max = localsum;
				id = i;
			}
				
		}
		cout << "Mejor cadena: " << id << endl << endl;
		return id;
	}


	void aStar(int order=0){
		int tam = tabla.size();
		int best = BestValue();
		for (int i = 0; i < tam; i++) {
			if (order == 0)
			{
				auto aux = tabla[i][best].alineado_A;
				cout << string(aux.begin(), aux.end());// << endl;
				for (int i = string(aux.begin(), aux.end()).length(); i < maxlenght; i++) {
					cout << "-";
				}
				cout << endl;

			}
			else {
				auto aux = tabla[i][best].alineado_B;
				cout << string(aux.begin(), aux.end());// << endl;
				for (int i = string(aux.begin(), aux.end()).length(); i < maxlenght; i++) {
					cout << "-";
				}
				cout << endl;
			}

			
		}
		cout << endl<< "--------------------------------------------" << endl;
	}
};


int main()
{
	cout << "Cadenas de ejemplo:" << endl<<endl;

	cadena S1 = convertirAcadena("ATTGCCATT");
	cadena S2 = convertirAcadena("ATGGCCATT");
	cadena S3 = convertirAcadena("ATCCAATTTT");
	cadena S4 = convertirAcadena("ATCTTCTT");
	cadena S5 = convertirAcadena("ACTGACC");

	MSA star({ S1,S2,S3,S4,S5 });
	
	star.aStar(1);

	cout <<endl<< "Cadenas de F:" << endl<<endl;


	cadena F1 = convertirAcadena("TGCCGGCAGGGATGTGCTTG");
	cadena F2 = convertirAcadena("GTTTAGGTTTTTGCTTATGCAGCATCCA");
	cadena F3 = convertirAcadena("GGAAAAGCACAGAACTGGCCAACA");
	cadena F4 = convertirAcadena("GCCAGTTGGTTGATTTCCACCTCCA");
	cadena F5 = convertirAcadena("ACCCCCGACATGCAGAAGCTG");
	cadena F6 = convertirAcadena("TGACGTGTCTGCTCCACTTCCA");

	MSA starF({ F1,F2,F3,F4,F5,F6 });
	
	starF.aStar();

	cout << endl << "Cadenas de R:" << endl << endl;

	cadena R1 = convertirAcadena("TGCTTGCAGTTTGCTTTCACTGATGGA");
	cadena R2 = convertirAcadena("TCAGGTACCCTGACCTTCTCTGAAC");
	cadena R3 = convertirAcadena("GTGGGTTGTAAAGGTCCCAAATGGT");
	cadena R4 = convertirAcadena("TGCCTTGGGTCCCTCTGACTGG");
	cadena R5 = convertirAcadena("GTGGTGCATTGATGGAAGGAAGCA");
	cadena R6 = convertirAcadena("AGTGAGAGGAGCTCCCAGGGC");

	MSA starR({ R1,R2,R3,R4,R5,R6 });

	starR.aStar(1);

}


