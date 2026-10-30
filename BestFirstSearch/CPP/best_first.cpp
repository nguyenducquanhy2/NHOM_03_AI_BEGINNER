#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Node {
    string name;
    int h;
};

struct Step {
    int no;
    string current;
    string neighbors;
    string open;
    string status;
};

vector<Node> nodes;
vector<vector<int> > adj;
vector<int> parent;
map<string, int> ids;
vector<Step> steps;
int S = -1, T = -1;
bool found = false;

string trim(string s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n;");
    return s.substr(a, b - a + 1);
}

int getId(string name, int h = 0, bool hasH = false) {
    if (ids.count(name)) {
        int x = ids[name];
        if (hasH) nodes[x].h = h;
        return x;
    }
    int x = (int)nodes.size();
    ids[name] = x;
    nodes.push_back({name, h});
    adj.push_back(vector<int>());
    return x;
}

bool readInput(const string &fn) {
    ifstream f(fn.c_str());
    if (!f) return false;

    string line;
    vector<string> graph;
    string startName, goalName;

    while (getline(f, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        if (line.find("TTD:") == 0) {
            startName = trim(line.substr(4));
            continue;
        }
        if (line.find("TTKT:") == 0) {
            goalName = trim(line.substr(5));
            continue;
        }
        graph.push_back(line);
    }

    for (size_t i = 0; i < graph.size(); ++i) {
        string s = graph[i];
        size_t c = s.find(':');
        if (c == string::npos) continue;

        string left = trim(s.substr(0, c));
        string right = trim(s.substr(c + 1));

        size_t p = left.find('(');
        size_t q = left.find(')');
        string nm = left;
        int h = 0;
        bool hasH = false;
        if (p != string::npos && q != string::npos && q > p) {
            nm = trim(left.substr(0, p));
            h = atoi(left.substr(p + 1, q - p - 1).c_str());
            hasH = true;
        }

        int u = getId(nm, h, hasH);
        stringstream ss(right);
        string tok;
        while (getline(ss, tok, ',')) {
            tok = trim(tok);
            if (!tok.empty()) {
                size_t vp = tok.find('(');
                size_t vq = tok.find(')');
                string vnm = tok;
                int vh = 0;
                bool vHasH = false;
                if (vp != string::npos && vq != string::npos && vq > vp) {
                    vnm = trim(tok.substr(0, vp));
                    vh = atoi(tok.substr(vp + 1, vq - vp - 1).c_str());
                    vHasH = true;
                }
                int v = getId(vnm, vh, vHasH);
                adj[u].push_back(v);
            }
        }
    }

    if (!startName.empty()) S = getId(startName);
    if (!goalName.empty()) T = getId(goalName);

    return S >= 0 && T >= 0;
}

bool cmpOpen(int a, int b) {
    if (nodes[a].h != nodes[b].h) return nodes[a].h < nodes[b].h;
    return nodes[a].name < nodes[b].name;
}

string listOpen(vector<int> o) {
    sort(o.begin(), o.end(), cmpOpen);
    string s = "[";
    for (int i = 0; i < (int)o.size(); ++i) {
        if (i) s += ", ";
        s += nodes[o[i]].name + "(" + to_string(nodes[o[i]].h) + ")";
    }
    return s + "]";
}

string listAdj(int u) {
    if (adj[u].empty()) return "-";
    string s;
    for (size_t i = 0; i < adj[u].size(); ++i) {
        int v = adj[u][i];
        if (!s.empty()) s += ", ";
        s += nodes[v].name + "(" + to_string(nodes[v].h) + ")";
    }
    return s;
}

void BestFirst() {
    steps.clear();
    found = false;
    parent.assign(nodes.size(), -1);

    vector<int> open(1, S);
    vector<bool> seen(nodes.size(), false);
    seen[S] = true;

    int step = 0;
    steps.push_back({step++, "-", "-", listOpen(open), "Khoi tao OPEN"});

    while (!open.empty()) {
        sort(open.begin(), open.end(), cmpOpen);
        int u = open.front();
        open.erase(open.begin());

        if (u == T) {
            steps.push_back({step++, nodes[u].name, "TTKT - dừng", listOpen(open), "Đã tìm thấy đích"});
            found = true;
            break;
        }

        for (size_t i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            if (!seen[v]) {
                seen[v] = true;
                parent[v] = u;
                open.push_back(v);
            }
        }
        steps.push_back({step++, nodes[u].name, listAdj(u), listOpen(open), "Đang tìm kiếm"});
    }
}

vector<int> getPath() {
    vector<int> p;
    if (!found) return p;
    for (int x = T; x != -1; x = parent[x]) {
        p.push_back(x);
        if (x == S) break;
    }
    reverse(p.begin(), p.end());
    return p;
}

void output(const string &fn) {
    ofstream o(fn.c_str());
    o << "BEST FIRST SEARCH - NHOM 3\n";
    o << "TTD: " << nodes[S].name << " | TTKT: " << nodes[T].name << "\n\n";
    o << "Buoc | Mo rong | Ke | OPEN\n";
    for (size_t i = 0; i < steps.size(); ++i) {
        const Step &r = steps[i];
        o << r.no << " | " << r.current << " | " << r.neighbors << " | " << r.open << "\n";
    }
    vector<int> p = getPath();
    o << "\nTrang thai: " << (found ? "DA TIM THAY" : "KHONG TIM THAY") << "\nDuong di: ";
    if (p.empty()) {
        o << "Khong co";
    } else {
        for (int i = 0; i < (int)p.size(); ++i) {
            if (i) o << " -> ";
            o << nodes[p[i]].name;
        }
    }
    o << "\n";
}

int main(int argc, char **argv) {
    string in = (argc > 1 ? argv[1] : "input.txt");
    string out = (argc > 2 ? argv[2] : "output.txt");

    if (!readInput(in)) {
        cerr << "Khong doc duoc " << in << " hoac thieu TTD/TTKT\n";
        return 1;
    }

    BestFirst();
    output(out);

    cout << "+--------------------------------------------------------------+\n";
    cout << "|                    BEST FIRST SEARCH - NHOM 3                |\n";
    cout << "+--------------------------------------------------------------+\n";
    cout << "TTD: " << nodes[S].name << " | TTKT: " << nodes[T].name << "\n\n";

    for (size_t i = 0; i < steps.size(); ++i) {
        const Step &r = steps[i];
        cout << "Buoc " << r.no << " | Mo rong: " << r.current
             << " | Ke: " << r.neighbors << " | OPEN: " << r.open << "\n";
    }

    vector<int> p = getPath();
    cout << "\nTrang thai: " << (found ? "DA TIM THAY" : "KHONG TIM THAY") << "\nDuong di: ";
    if (p.empty()) {
        cout << "Khong co";
    } else {
        for (int i = 0; i < (int)p.size(); ++i) {
            if (i) cout << " -> ";
            cout << nodes[p[i]].name;
        }
    }
    cout << "\nDa ghi ket qua vao " << out << "\n";
    return 0;
}
