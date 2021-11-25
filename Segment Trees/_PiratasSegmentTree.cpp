#include <iostream>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
#include <sstream>

using namespace std;
int caso = 0;
int v[1024000];

class SegmentTree {
	vector<int> st;
	vector<int> lazy;
	int tam; // Numero de hojas que manejamos
public:
	// Tamano maximo que podremos guardar
	// (numero de hojas).
	// Antes de las consultas, se necesita rellenar
	// con los datos iniciales usando build().
	SegmentTree(int maxN) {
		st.assign(4 * maxN + 10, 0);
		lazy.assign(4 * maxN + 10, 3); // 3 es que no hay operacion
	}

	int query(int a, int b) {
		return query(1, 0, tam - 1, a, b);
	}
	int query(int vertex, int L, int R, int i, int j) {
		pushLazyUpdate(vertex, L, R);
		if ((j < L) || (R < i))
			return 0;
		if (i <= L && R <= j)
			return st[vertex];
		int mitad = (L + R) / 2;
		return query(2 * vertex, L, mitad, i, j) +
			query(2 * vertex + 1, mitad + 1, R, i, j);
	}

	void updateRange(int a, int b, int op) {
		updateRange(1, 0, tam - 1, a, b, op);
	}

	void updateRange(int vertex, int L, int R, int a, int b, int op) {
		// Resolvemos posibles operaciones pendientes
		pushLazyUpdate(vertex, L, R);
		if ((b < L) || (R < a)) return;
		// ¿Intervalo afectado por completo?
		if ((a <= L) && (R <= b)) {
			// Nos aplicamos la operacion y propagamos la
			// pereza a los hijos. Para evitar copiar/pegar,
			// lo hacemos aplicandonos la pereza, y luego
			// resolviendola
			setLazyUpdate(vertex, op);
			pushLazyUpdate(vertex, L, R);
			return;
		}
		// Intervalo no afectado por completo. No podemos
		// ser perezosos. Aplicamos la operacion en
		// los hijos
		int m = (L + R) / 2;
		updateRange(2 * vertex, L, m, a, b, op);
		updateRange(2 * vertex + 1, m + 1, R, a, b, op);
		// Combinamos
		st[vertex] = st[2 * vertex] + st[2 * vertex + 1];
	}

	void setLazyUpdate(int vertex, int op) {
		// Mezclamos operaciones
		if (lazy[vertex] == 3) // no teniamos operacion
			lazy[vertex] = op;
		// teniamos que poner a 0 y nos llega poner a 1 o invertir -> poner a 1
		if (lazy[vertex] == 0 && op != 0)
			lazy[vertex] = 1;
		// teniamos que poner a 1 y nos llega poner a 0 o invertir -> poner a 0
		if (lazy[vertex] == 1 && op != 1)
			lazy[vertex] = 1;
		// teniamos que inverir y nos llega cambiar a 0 o 1
		if (lazy[vertex] == 2 && op != 2)
			lazy[vertex] = op;
		// invertir dos veces
		if(lazy[vertex] == 2 && op == 2)
			lazy[vertex] = 3;
	}

	void pushLazyUpdate(int vertex, int L, int R) {
		if (lazy[vertex] == 0)
			st[vertex] = 0;
		else if (lazy[vertex] == 1)
			st[vertex] = R - L + 1;
		else if (lazy[vertex] == 2)
			st[vertex] = (R - L + 1) - st[vertex];
		if (L != R) {
			// Tenemos hijos
			int m = (L + R) / 2;
			setLazyUpdate(2 * vertex, lazy[vertex]);
			setLazyUpdate(2 * vertex + 1, lazy[vertex]);
		}
		lazy[vertex] = 3;
	}

	void build(int* values, int n) {
		tam = n;
		build(values, 1, 0, n - 1);
	}

	void build(int* values, int p, int l, int r) {
		if (l == r) {
			st[p] = values[l];
			return;
		}
		int m = (l + r) / 2;
		build(values, 2 * p, l, m);
		build(values, 2 * p + 1, m + 1, r);
		st[p] = st[2 * p] + st[2 * p + 1];
	}
};

void resuelveCaso() {
	caso++;
	cout << "Case " << caso << ":\n";
	int m; 
	cin >> m;
	int tam = 0;
	int t;
	string conf;
	for (int i = 0; i < m; ++i) {
		cin >> t >> conf;
		for (int j = 0; j < t; ++j) {
			for (int k = 0; k < conf.size(); ++k) {
				if(conf[k] == '1')
					v[tam] = 1;
				else v[tam] = 0;
				++tam;
			}
		}
	}
	SegmentTree s(tam);
	s.build(v, tam);
	int q;
	cin >> q;
	char op;
	int a, b;
	for (int i = 0; i < q; ++i) {
		cin >> op >> a >> b;
		if (op == 'F') { //piratas a 1
			s.updateRange(a - 1, b - 1, 1);
		}
		else if (op == 'E') { //piratas a 0
			s.updateRange(a - 1, b - 1, 0);
		}
		else if (op == 'I') { //piratas a inverso
			s.updateRange(a - 1, b - 1, 2);
		}
		else { //query
			int piratas = s.query(a-1, b-1);
			cout << "Q" << i << ": " << piratas << "\n";
		}
	}
}

int main() {

#ifndef DOMJUDGE
	std::ifstream in("datos.txt");
	std::ofstream out("salida.txt");
	auto cinbuf = std::cin.rdbuf(in.rdbuf());
	auto coutbuf = std::cout.rdbuf(out.rdbuf());
#endif

	int T;
	cin >> T;
	while (T > 0) { 
		resuelveCaso();
		--T;
	};


#ifndef DOMJUDGE 
	std::cin.rdbuf(cinbuf);
	std::cout.rdbuf(coutbuf);
	system("PAUSE");
#endif

	return 0;
}
