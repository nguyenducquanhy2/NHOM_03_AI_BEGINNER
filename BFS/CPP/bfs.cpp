#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int n = 0;
vector<vector<int>> adj;
vector<int> color;
vector<int> parent;
vector<int> d;
queue<int> q;

vector<string> name;
map<string, int> id;

int start_state = -1, goal_state = -1;
bool found = false;

struct Row {
  string phatTrien;
  string ke;
  string L;
};
vector<Row> rows;

int getId(const string &s) {
  if (id.count(s))
    return id[s];
  id[s] = n;
  name.push_back(s);
  adj.push_back(vector<int>());
  color.push_back(0);
  parent.push_back(-1);
  d.push_back(0);
  return n++;
}

string trim(string s) {
  size_t b = s.find_first_not_of(" \t\r\n");
  if (b == string::npos)
    return "";
  size_t e = s.find_last_not_of(" \t\r\n;");
  return s.substr(b, e - b + 1);
}

string joinQueue() {
  queue<int> tmp = q;
  string s;
  while (!tmp.empty()) {
    if (!s.empty())
      s += ", ";
    s += name[tmp.front()];
    tmp.pop();
  }
  return s;
}

string joinAdj(int u) {
  string s;
  for (size_t i = 0; i < adj[u].size(); i++) {
    if (i)
      s += ", ";
    s += name[adj[u][i]];
  }
  return s;
}

/* ================== BFS GOC - GIU NGUYEN ================== */
void BFS() {
  color[start_state] = 1;
  d[start_state] = 0;
  parent[start_state] = start_state;
  q.push(start_state);

  Row first;
  first.phatTrien = "";
  first.ke = "";
  first.L = joinQueue();
  rows.push_back(first);

  while (q.size() > 0) {
    int u = q.front();
    q.pop();

    if (u == goal_state) {
      Row r;
      r.phatTrien = name[u];
      r.ke = "TTKT";
      r.L = "";
      rows.push_back(r);
      found = true;
      return;
    }

    for (size_t i = 0; i < adj[u].size(); i++) {
      int v = adj[u][i];
      if (color[v] == 0) {
        color[v] = 1;
        parent[v] = u;
        d[v] = d[u] + 1;
        q.push(v);
      }
    }
    color[u] = 2;

    Row r;
    r.phatTrien = name[u];
    r.ke = joinAdj(u);
    r.L = joinQueue();
    rows.push_back(r);
  }
}

/* ================== DOC INPUT - GIU NGUYEN ================== */
bool readInput(const string &file) {
  ifstream fin(file.c_str());
  if (!fin) {
    cerr << "Khong mo duoc file " << file << endl;
    return false;
  }

  string startName, goalName;
  vector<pair<string, vector<string>>> edges;
  string line;

  while (getline(fin, line)) {
    string s = trim(line);
    if (s.empty())
      continue;

    size_t p = s.find(':');
    if (p == string::npos)
      continue;

    string key = trim(s.substr(0, p));
    string val = trim(s.substr(p + 1));

    if (key == "TTD") {
      startName = val;
      continue;
    }
    if (key == "TTKT") {
      goalName = val;
      continue;
    }

    vector<string> vs;
    stringstream ss(val);
    string tok;
    while (getline(ss, tok, ',')) {
      tok = trim(tok);
      if (!tok.empty())
        vs.push_back(tok);
    }
    edges.push_back(make_pair(key, vs));
  }
  fin.close();

  for (size_t i = 0; i < edges.size(); i++) {
    int u = getId(edges[i].first);
    for (size_t j = 0; j < edges[i].second.size(); j++) {
      int v = getId(edges[i].second[j]);
      adj[u].push_back(v);
    }
  }

  if (startName.empty() || goalName.empty()) {
    cerr << "Thieu TTD hoac TTKT trong file input." << endl;
    return false;
  }

  start_state = getId(startName);
  goal_state = getId(goalName);
  return true;
}

/* ================== GHI OUTPUT - GIU NGUYEN ================== */
void writeOutput(const string &file) {
  ostringstream out;

  out << "BREADTH-FIRST SEARCH - NHOM 3\n";
  out << "============================================\n";
  out << "TTD: " << name[start_state] << "\n";
  out << "TTKT: " << name[goal_state] << "\n\n";

  out << "L = [" << rows[0].L << "]\n";
  for (size_t i = 1; i < rows.size(); i++) {
    out << i << ". " << rows[i].phatTrien;
    if (rows[i].ke == "TTKT")
      out << " -> TTKT, dung\n";
    else
      out << " | ke: " << (rows[i].ke.empty() ? "-" : rows[i].ke) << " | L = ["
          << rows[i].L << "]\n";
  }
  out << "\n";

  if (found) {
    vector<string> path;
    for (int v = goal_state;; v = parent[v]) {
      path.push_back(name[v]);
      if (v == start_state)
        break;
    }

    out << "Duong di: ";
    for (int i = (int)path.size() - 1; i >= 0; i--) {
      out << path[i];
      if (i)
        out << " -> ";
    }
    out << "\n";
    out << "Do dai: " << d[goal_state] << "\n";
  } else {
    out << "Khong tim thay duong di tu " << name[start_state] << " den "
        << name[goal_state] << "\n";
  }

  ofstream fout(file.c_str());
  fout << out.str();
  fout.close();
}

/* ================== CMD UI ================== */

void clearScreen() {
  cout << "[2J[H";
  cout.flush();
}

string queueDisplay(const string &s) { return "[" + s + "]"; }

vector<string> makePath() {
  vector<string> path;
  if (!found)
    return path;

  for (int v = goal_state;; v = parent[v]) {
    path.push_back(name[v]);
    if (v == start_state)
      break;
  }
  reverse(path.begin(), path.end());
  return path;
}

string pathDisplay(const vector<string> &path) {
  string s;
  for (size_t i = 0; i < path.size(); i++) {
    if (i)
      s += " -> ";
    s += path[i];
  }
  return s;
}

void drawStep(int stepIndex) {
  clearScreen();

  cout << "+--------------------------------------------------------------+\n";
  cout << "|              BREADTH-FIRST SEARCH - NHOM 3                  |\n";
  cout << "+--------------------------------------------------------------+\n";
  cout << "|                                                              |\n";

  if (stepIndex < 0)
    stepIndex = 0;
  if (stepIndex >= (int)rows.size())
    stepIndex = (int)rows.size() - 1;

  Row &r = rows[stepIndex];

  cout << "  DO THI                         |  QUA TRINH BFS\n";
  cout << "                                 |\n";
  cout << "              A                  |  Buoc " << stepIndex << "\n";
  cout << "           /  |  \\               |  ";

  if (r.phatTrien.empty())
    cout << "Dang khoi tao L\n";
  else
    cout << "Dang phat trien: " << r.phatTrien << "\n";

  cout << "          B   C<--D              |  Ke: "
       << (r.ke.empty() ? "-" : r.ke) << "\n";
  cout << "         / \\  | \\  \\             |  Queue: " << queueDisplay(r.L)
       << "\n";
  cout << "        I   \\ E  F--'            |\n";
  cout << "         \\  |/|\\ |               |  TTD: " << name[start_state]
       << "\n";
  cout << "            G    K               |  TTKT: " << name[goal_state]
       << "\n";
  cout << "                                 |\n";
  cout << "---------------------------------+------------------------------\n";

  vector<string> path = makePath();

  cout << "\n";
  if (found && stepIndex == (int)rows.size() - 1) {
    cout << "  Trang thai : DA TIM THAY\n";
    cout << "  Duong di   : " << pathDisplay(path) << "\n";
    cout << "  Do dai     : " << d[goal_state] << "\n";
  } else {
    cout << "  Trang thai : DANG TIM KIEM\n";
    cout << "  Duong di   : Dang truy vet sau khi tim thay TTKT\n";
  }

  cout << "\n";
  cout << "  [ENTER] Buoc tiep theo    [A] Tu dong    [R] Reset    [Q] Thoat\n";
}

void runCMDUI() {
  if (rows.empty())
    return;

  int step = 0;

  while (true) {
    drawStep(step);

    cout << "\nNhap phim (ENTER = tiep, A = tu dong, R = reset, Q = thoat): ";

    string key;
    getline(cin, key);

    if (key.empty()) {
      // ENTER: sang buoc tiep theo
      if (step < (int)rows.size() - 1)
        ++step;
    } else if (key == "a" || key == "A") {
      // Chay tu dong den buoc cuoi
      while (step < (int)rows.size() - 1) {
        ++step;
        drawStep(step);
      }
    } else if (key == "r" || key == "R") {
      // Reset
      step = 0;
    } else if (key == "q" || key == "Q") {
      // Thoat
      break;
    }
  }
}

int main(int argc, char **argv) {

  string fin = (argc > 1) ? argv[1] : "input.txt";
  string fout = (argc > 2) ? argv[2] : "output.txt";

  if (!readInput(fin))
    return 1;

  BFS();

  writeOutput(fout);

  runCMDUI();

  clearScreen();

  cout << "+--------------------------------------------------------------+\n";
  cout << "|              BREADTH-FIRST SEARCH - NHOM 3                  |\n";
  cout << "+--------------------------------------------------------------+\n";
  cout << "\n";
  cout << "Ket qua da duoc luu vao: " << fout << "\n";
  cout << "\n";

  cout << "Nhan ENTER de ket thuc...";
  string dummy;
  getline(cin, dummy);

  return 0;
}
