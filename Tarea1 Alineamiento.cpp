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
	Resultado(cadena a, cadena b, int p = 0, int r = 0) {
		alineado_A = a;
		alineado_B = b;
		puntaje = p;
		rupturas = r;
	}
	void printall() {
		cout << "Cadena 1:\n" << string(alineado_A.begin(), alineado_A.end()) << endl;
		cout << "Cadena 2:\n" << string(alineado_B.begin(), alineado_B.end()) << endl;
		cout << "Puntaje :" << puntaje<<endl;
		cout << "Rupturas :" << rupturas<<endl<<endl;
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

	void AlineadaIterativa(int i, int j, cadena alineado_A, cadena alineado_B) {
		if (i == 0 && j == 0) {
			reverse(alineado_A.begin(), alineado_A.end());
			reverse(alineado_B.begin(), alineado_B.end());
			// Puedes juntar las dos cadenas en un solo objeto o usar una estructura
			cout << string(alineado_A.begin(), alineado_A.end()) << endl;
			cout << string(alineado_B.begin(), alineado_B.end()) << endl;
			cout << "Puntaje: " << table[tam_A][tam_B].first << "\n\n";
			return;
		}

		for (auto it : table[i][j].second) {
			cadena new_A = alineado_A;
			cadena new_B = alineado_B;

			if (it == 'd') {
				new_A.push_back(A[i - 1]);
				new_B.push_back(B[j - 1]);
				AlineadaIterativa(i - 1, j - 1, new_A, new_B);
			}
			else if (it == 't') {
				new_A.push_back(A[i - 1]);       // el caracter real de A
				new_B.push_back('-');            // gap en B
				AlineadaIterativa(i - 1, j, new_A, new_B);
			}

			else if (it == 'i') {
				new_A.push_back('-');            // gap en A
				new_B.push_back(B[j - 1]);       // el caracter real de B
				AlineadaIterativa(i, j - 1, new_A, new_B);
			}

		}
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
				cout << string(alineado_A.begin(), alineado_A.end()) << endl;
				cout << string(alineado_B.begin(), alineado_B.end()) << endl;
				cout << "Puntaje: " << table[tam_A][tam_B].first << "\n\n";
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


	void Alineada_iterativa() {
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

				return; // ✅ Salir después de encontrar una alineación
			}

			// Elegir solo una dirección (por ejemplo, la primera del set)
			if (!table[i][j].second.empty()) {
				char it = *table[i][j].second.begin(); // solo la primera opción

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
	void make() {
		alignment();
		cout << "Tabla completa" << endl;
		Alineada_iterativa();
		cout<< "Posibles combinaciones: " <<resultados.size() << "\nLa mejor alineacion por rupturas:\n";
		printbetter();
	}

};


int main()
{
	//string inputA = "tcaagcgttagagaagtcattatgtgataaaaaaattcaacttggtatcaacttaactaagggtcttggtgctggtgctttgcctgatgttggtaaaggtgcagcagaagaatcaattgatgaaattatggagcatataaaagatagccatatgctctttatcacagcagggatgggtggtggtactggaacaggtgctgcaccggtaattgcaaaagcagccagagaagcaagagcggtagttaaagataaaggagcaaaagaaaaaaagatactgactgttggagttgtaactaagccgttcggttttgaaggtgtgcgacgtatgcgcattgcagagcttggacttgaagagttgcaaaaatacgtagatacacttattgtcattcccaatcaaaatttatttagaattgctaacgagaaaactacatttgctgacgcatttcaactcgccgataatgttctgcatattggcataagaggagtaactgatttgatgatcatgccaggactgattaatcttgattttgctgatatagaaacagtaatgagtgagatgggtaaagcaatgattggtactggagaggcagaaggagaagatagggcaattagcgctgcagaggctgcgatatctaatccattacttgataatgtatcaatgaaaggtgcacaaggaatattaattaatattactggtggtggagatatgactctatttgaagttgatgctgcagccaatagagtgcgtgaagaagtcgatgaaaatgcaaatataatatttggtgccacttttgatcaggcgatggaaggaagagttagagtttctattcttgcaactggcattgatagctgtaacgacaattcatctgttaatcaaaacaagatcccagcagaggaaaaaaattttaaatggccttataatcaagttcctatatcagaaacaaaagaatatgcttcaactgagcaaacaaacgaaagagttaagtggggcagcaatgtttatgatataccagcttatctaagaagaaaaaaataatgcaattttggctactcaagtcgg";
	//string inputB = "attaaaggtttataccttcccaggtaacaaaccaaccaactttcgatctcttgtagatctgttctctaaacgaactttaaaatctgtgtggctgtcactcggctgcatgcttagtgcactcacgcagtataattaataactaattactgtcgttgacaggacacgagtaactcgtctatcttctgcaggctgcttacggtttcgtccgtgttgcagccgatcatcagcacatctaggtttcgtccgggtgtgaccgaaaggtaagatggagagccttgtccctggtttcaacgagaaaacacacgtccaactcagtttgcctgttttacaggttcgcgacgtgctcgtacgtggctttggagactccgtggaggaggtcttatcagaggcacgtcaacatcttaaagatggcacttgtggcttagtagaagttgaaaaaggcgttttgcctcaacttgaacagccctatgtgttcatcaaacgttcggatgctcgaactgcacctcatggtcatgttatggttgagctggtagcagaactcgaaggcattcagtacggtcgtagtggtgagacacttggtgtccttgtccctcatgtgggcgaaataccagtggcttaccgcaaggttcttcttcgtaagaacggtaataaaggagctggtggccatagttacggcgccgatctaaagtcatttgacttaggcgacgagcttggcactgatccttatgaagattttcaagaaaactggaacactaaacatagcagtggtgttacccgtgaactcatgcgtgagcttaacggaggggcatacactcgctatgtcgataacaacttctgtggccctgatggctaccctcttgagtgcattaaagaccttctagcacgtgctggtaaagcttcatgcactttgtccgaacaactggactttattgacactaagaggggtgtatactgctgccgtgaacatgagcatgaaattgcttggtacacggaacgttctgaaaagagctatgaattgcagacaccttttgaaattaaattggcaaagaaatttgacaccttcaatggggaatgtccaaa";

	string bacteria = "tcaagcgttagagaagtcattatgtgataaaaaaattcaacttggtatcaacttaactaagggtcttggtgctggtgctttgcctgatgttggtaaaggtgcagcagaagaatcaattgatgaaattatggagcatataaaagatagccatatgctctttatcacagcagggatgggtggtggtactggaacaggtgctgcaccggtaattgcaaaagcagccagagaagcaagagcggtagttaaagataaaggagcaaaagaaaaaaagatactgactgttggagttgtaactaagccgttcggttttgaaggtgtgcgacgtatgcgcattgcagagcttggacttgaagagttgcaaaaatacgtagatacacttattgtcattcccaatcaaaatttatttagaattgctaacgagaaaactacatttgctgacgcatttcaactcgccgataatgttctgcatattggcataagaggagtaactgatttgatgatcatgccaggactgattaatcttgattttgctgatatagaaacagtaatgagtgagatgggtaaagcaatgattggtactggagaggcagaaggagaagatagggcaattagcgctgcagaggctgcgatatctaatccattacttgataatgtatcaatgaaaggtgcacaaggaatattaattaatattactggtggtggagatatgactctatttgaagttgatgctgcagccaatagagtgcgtgaagaagtcgatgaaaatgcaaatataatatttggtgccacttttgatcaggcgatggaaggaagagttagagtttctattcttgcaactggcattgatagctgtaacgacaattcatctgttaatcaaaacaagatcccagcagaggaaaaaaattttaaatggccttataatcaagttcctatatcagaaacaaaagaatatgcttcaactgagcaaacaaacgaaagagttaagtggggcagcaatgtttatgatataccagcttatctaagaagaaaaaaataatgcaattttggctactcaagtcgg";
	string covid = "attaaaggtttataccttcccaggtaacaaaccaaccaactttcgatctcttgtagatctgttctctaaacgaactttaaaatctgtgtggctgtcactcggctgcatgcttagtgcactcacgcagtataattaataactaattactgtcgttgacaggacacgagtaactcgtctatcttctgcaggctgcttacggtttcgtccgtgttgcagccgatcatcagcacatctaggtttcgtccgggtgtgaccgaaaggtaagatggagagccttgtccctggtttcaacgagaaaacacacgtccaactcagtttgcctgttttacaggttcgcgacgtgctcgtacgtggctttggagactccgtggaggaggtcttatcagaggcacgtcaacatcttaaagatggcacttgtggcttagtagaagttgaaaaaggcgttttgcctcaacttgaacagccctatgtgttcatcaaacgttcggatgctcgaactgcacctcatggtcatgttatggttgagctggtagcagaactcgaaggcattcagtacggtcgtagtggtgagacacttggtgtccttgtccctcatgtgggcgaaataccagtggcttaccgcaaggttcttcttcgtaagaacggtaataaaggagctggtggccatagttacggcgccgatctaaagtcatttgacttaggcgacgagcttggcactgatccttatgaagattttcaagaaaactggaacactaaacatagcagtggtgttacccgtgaactcatgcgtgagcttaacggaggggcatacactcgctatgtcgataacaacttctgtggccctgatggctaccctcttgagtgcattaaagaccttctagcacgtgctggtaaagcttcatgcactttgtccgaacaactggactttattgacactaagaggggtgtatactgctgccgtgaacatgagcatgaaattgcttggtacacggaacgttctgaaaagagctatgaattgcagacaccttttgaaattaaattggcaaagaaatttgacaccttcaatggggaatgtccaaa";
	string influenza = "atggaagcaatatcactgatgactatactactggtggtaacaacaagtaatgcagacaaaatctgcatcggtcaccaatcaacaaattccacggaaactgtagacacgctaacagaaacaaatgttcctgtaacacaagccaaagaattgctccacacagaacacaatgggatgctatgtgcaacaaatctgggacgtcctcttatcctagacacatgcaccattgaaggactgatctatggcaacccatcttgtgacatgctgttaggaggaagggaatggtcctacatcgtcgaaagaccgtccgcagtaaatggaacatgctaccctggaaatgtagaaaacctagaggaacttagaacactttttagctcctctagttcttaccaaagagtccaactctttccagactcaatctggaatgtgacttacactgggacaagcaaatcatgttcagattcattctataggaatatgagatggttaactcaaaagaatgggggttatccaattcaagatgcccagtacacaaacaataggggaaaggacattcttttcgtgtggggcatacatcatccaccaaccgatactgcacagacgaatttatatacaaggaccgacacaacaacaagtgtaacaacggagactttagataggaccttcaaaccattgatagggccaaggccccttgtcaatggtctaattggaagaattaattactattggtcggtactaaaaccaggccaaacgttgcgagtgagatcaaatggaaatctaattgctccatggtttggacatgttctctcaggtgagagccatgtgagaatcctgagaactgatttaagcagcggtaattgtgtggtacaatgccagactgaaaaaggtggcctaaacagtacaatgccatttcacaacatcagcaaatatgcttttgggacctgtcccaaatatattggagtcaagagtctcaaactggcaattggccttagaaacgtacatgctaggtcaagtagaggactattcggagcgatagctggattcatagaaggaggttggccaggactagtcgccggttggtat";

	/*string inputA = { "GCGGCCCCGGGGGGGGGCCCGCGGGCCGCGCCCCGGGCGG" };
	string inputB = { "CGCCGGGGGCCCGGGGCGCGCGCGCGCCGGCGCGGCGCGC" };*/

	/*cadena A = convertirAcadena(inputA);
	cadena B = convertirAcadena(inputB);*/


	cadena Bacteria = convertirAcadena(bacteria);
	cadena Covid = convertirAcadena(covid);
	cadena Influenza = convertirAcadena(influenza);

	Global_alignment B_C(Bacteria, Covid);
	Global_alignment B_I(Bacteria, Influenza);
	Global_alignment C_I(Covid, Influenza);


	cout << "Bacteria y Covid"<<endl;
	B_C.make();
	cout << "-----------------------------------------------------";
	cout << "Bacteria e influenza"<<endl;
	B_I.make();
	cout << "-----------------------------------------------------";
	cout << "Covid e influenza" << endl;
	C_I.make();

	//B_C.printbetter();
	

	//Global_alignment cad1(A, B);
	//cad1.alignment();
	////cad1.printTable_guide();
	//cad1.Alineada_iterativa();
	//cad1.printbetter();


}


