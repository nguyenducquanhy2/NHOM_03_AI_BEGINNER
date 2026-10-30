#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <cmath>
using namespace std;

struct Edge { int to; double cost; };
struct Node { string name; double h; };
struct Step { int no, u; double g, h, f; string generated, open; };

vector<Node> nodes;
vector<vector<Edge> > adj;
map<string, int> id;
vector<double> g;
vector<int> parent;
vector<char> closed;
vector<Step> steps;
int S = -1, T = -1;
bool found = false;

string trim(string s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n;");
    return s.substr(a, b - a + 1);
}

int getId(const string &s, double h = 0, bool hasH = false) {
    map<string, int>::iterator it = id.find(s);
    if (it != id.end()) {
        if (hasH) nodes[it->second].h = h;
        return it->second;
    }
    int x = (int)nodes.size();
    id[s] = x;
    nodes.push_back({s, h});
    adj.push_back(vector<Edge>());
    return x;
}

string num(double x) {
    ostringstream o;
    if (fabs(x - round(x)) < 1e-9) o << (long long)round(x);
    else o << fixed << setprecision(2) << x;
    return o.str();
}

bool readInput(const string &fn) {
    ifstream f(fn.c_str());
    if (!f) return false;
    string line;
    vector<string> gl;
    string startName, goalName;

    while (getline(f, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        if (line.rfind("TTD:", 0) == 0) {
            startName = trim(line.substr(4));
            continue;
        }
        if (line.rfind("TTKT:", 0) == 0) {
            goalName = trim(line.substr(5));
            continue;
        }
        gl.push_back(line);
    }

    for (size_t i = 0; i < gl.size(); ++i) {
        string s = gl[i];
        size_t c = s.find(':');
        if (c == string::npos) continue;
        string left = trim(s.substr(0, c)), right = trim(s.substr(c + 1));
        size_t p = left.find('('), q = left.find(')');
        string un = left;
        double h = 0;
        bool hasH = false;
        if (p != string::npos && q != string::npos && q > p) {
            un = trim(left.substr(0, p));
            h = atof(left.substr(p + 1, q - p - 1).c_str());
            hasH = true;
        }
        int u = getId(un, h, hasH);
        if (right.empty()) continue;
        string tok;
        stringstream ss(right);
        while (getline(ss, tok, ',')) {
            tok = trim(tok);
            size_t a = tok.find('('), b = tok.find(')');
            if (a == string::npos || b == string::npos || b <= a) continue;
            string vn = trim(tok.substr(0, a));
            double w = atof(tok.substr(a + 1, b - a - 1).c_str());
            int v = getId(vn);
            adj[u].push_back({v, w});
        }
    }

    if (!startName.empty()) S = getId(startName);
    if (!goalName.empty()) T = getId(goalName);

    return S >= 0 && T >= 0;
}

string openText(vector<int> o) {
    sort(o.begin(), o.end(), [](int a, int b) {
        double fa = g[a] + nodes[a].h, fb = g[b] + nodes[b].h;
        if (fabs(fa - fb) > 1e-9) return fa < fb;
        return nodes[a].name < nodes[b].name;
    });
    string s;
    for (size_t i = 0; i < o.size(); ++i) {
        int x = o[i];
        if (!s.empty()) s += ", ";
        s += nodes[x].name + "(f=" + num(g[x] + nodes[x].h) + ")";
    }
    return s;
}

void AStar() {
    int n = (int)nodes.size();
    double INF = numeric_limits<double>::infinity();
    g.assign(n, INF);
    parent.assign(n, -1);
    closed.assign(n, 0);
    steps.clear();
    found = false;

    vector<int> open;
    g[S] = 0;
    open.push_back(S);
    int no = 1;

    while (!open.empty()) {
        int bi = 0;
        for (int i = 1; i < (int)open.size(); i++) {
            double a = g[open[i]] + nodes[open[i]].h;
            double b = g[open[bi]] + nodes[open[bi]].h;
            if (a < b - 1e-9 || (fabs(a - b) <= 1e-9 && nodes[open[i]].name < nodes[open[bi]].name))
                bi = i;
        }
        int u = open[bi];
        open.erase(open.begin() + bi);
        if (closed[u]) continue;
        closed[u] = 1;

        double gu = g[u], hu = nodes[u].h, fu = gu + hu;
        if (u == T) {
            steps.push_back({no++, u, gu, hu, fu, "TTKT -> dung", openText(open)});
            found = true;
            break;
        }

        string gen;
        for (size_t i = 0; i < adj[u].size(); ++i) {
            Edge e = adj[u][i];
            int v = e.to;
            double ng = gu + e.cost;
            if (closed[v] && ng >= g[v] - 1e-9) continue;
            if (ng < g[v] - 1e-9) {
                g[v] = ng;
                parent[v] = u;
                closed[v] = 0;
                if (find(open.begin(), open.end(), v) == open.end())
                    open.push_back(v);
                if (!gen.empty()) gen += ", ";
                gen += nodes[v].name + "(g=" + num(g[v]) + ", h=" + num(nodes[v].h) + ", f=" + num(g[v] + nodes[v].h) + ")";
            }
        }
        steps.push_back({no++, u, gu, hu, fu, gen, openText(open)});
    }
}

vector<int> path() {
    vector<int> p;
    if (!found) return p;
    for (int v = T; v != -1; v = parent[v]) {
        p.push_back(v);
        if (v == S) break;
    }
    reverse(p.begin(), p.end());
    return p;
}

string pathText() {
    vector<int> p = path();
    if (p.empty()) return "Khong tim thay";
    string s;
    for (size_t i = 0; i < p.size(); ++i) {
        if (!s.empty()) s += " -> ";
        s += nodes[p[i]].name;
    }
    return s;
}

void output(const string &fn) {
    ofstream o(fn.c_str());
    o << "A* - NHOM 03\nTTD: " << nodes[S].name << "\nTTKT: " << nodes[T].name << "\n\n";
    o << "BUOC | MO RONG | g | h | f | DINH SINH | OPEN\n";
    for (size_t i = 0; i < steps.size(); ++i) {
        const Step &s = steps[i];
        o << s.no << " | " << nodes[s.u].name << " | " << num(s.g) << " | " << num(s.h) << " | " << num(s.f)
          << " | " << (s.generated.empty() ? "-" : s.generated) << " | [" << s.open << "]\n";
    }
    o << "\nDuong di: " << pathText() << "\nChi phi: " << (found ? num(g[T]) : "Khong co") << "\n";
}

int main(int argc, char **argv) {
    string in = argc > 1 ? argv[1] : "input.txt";
    string out = argc > 2 ? argv[2] : "output.txt";
    if (!readInput(in)) {
        cerr << "Loi input.txt\n";
        return 1;
    }
    AStar();
    output(out);
    cout << "A* - NHOM 03\nTTD: " << nodes[S].name << "  TTKT: " << nodes[T].name << "\n";
    for (size_t i = 0; i < steps.size(); ++i) {
        const Step &s = steps[i];
        cout << "Buoc " << s.no << ": " << nodes[s.u].name << " | g=" << num(s.g)
             << " h=" << num(s.h) << " f=" << num(s.f) << " | OPEN=[" << s.open << "]\n";
    }
    cout << "Trang thai: " << (found ? "DA TIM THAY" : "KHÔNG TÌM THẤY")
         << "\nDuong di: " << pathText()
         << "\nChi phi: " << (found ? num(g[T]) : "Khong co") << "\n";
    return 0;
}
