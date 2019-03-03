// Kevin Sittser
// 525003900
// CSCE420
// Due: February 14, 2019
// hw2pr4.cpp


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
using namespace std;

struct Node {
    vector<pair<int,int>> jugs;
    int val;
    vector<Node*> children;
    Node(vector<pair<int,int>> js,int v) : jugs(js),val(v),children(vector<Node*>()) {}
    Node(int v) : jugs(vector<pair<int,int>>()),children(vector<Node*>()) {}
};
/*
Node* processString(string s,bool findMax) {  // findMax is true if this level is finding max; false if min
    string ss;
    for (char c : s)
        if (c != ' ')
            ss += c;
    s = ss;
    Node* n = new Node(0);
    if (s.at(0) != '(') {
        n->val = stoi(s);
        return n;
    }
    int parens = 0;
    string substr;
    for (int i=1; i<s.size()-1; i++) {
        substr += s.at(i);
        if (s.at(i) == '(')
            parens++;
        else if (s.at(i) == ')')
            parens--;
        if (s.at(i) == ',' || i == s.size()-2)
            if (!parens) {
                if (substr.at(substr.size()-1) == ',')
                    substr = substr.substr(0,substr.size()-1);
                n->children.push_back(processString(substr,!findMax));
                int headVal = findMax ? INT_MIN : INT_MAX;  // min or max value
                for (Node* nn : n->children) {
                    if (findMax && nn->val > headVal)
                        headVal = nn->val;
                    else if (!findMax && nn->val < headVal)
                        headVal = nn->val;
                }
                n->val = headVal;
                substr = "";
            }
    }
    return n;
}
*/
void printTree(Node* n,int depth=0) {
    if (n->children.size() == 0) {
        cout << endl;
        for (int i=0; i<depth+1; i++)
            cout << "  ";
        cout << n->val << "";
    }
    else {
        cout << endl;
        for (int i=0; i<depth; i++)
            cout << "  ";
        cout << "([" << n->val << "]:";
    }
    for (pair<int,int> j : n->jugs)
        cout << "  " << j.second << "/" << j.first;
    for (int i=0; i<n->children.size(); i++) {
        printTree(n->children.at(i),depth+1);
        if (i < n->children.size() - 1)
            cout << ",";
    }
    if (n->children.size() > 0)
        cout << ")";
}

//Node* minNode(Node*,int,int);

Node* maxNode(Node* n,int alpha=INT_MIN,int beta=INT_MAX) {
    if (n->children.size() == 0)
        return n;
    int v = INT_MIN;
    Node* mn = new Node(INT_MIN);
    for (Node* nn : n->children) {
        int vv = maxNode(nn,alpha,beta)->val;
        if (vv > v) {
            mn = nn;
            v = vv;
        }
        /*if (v >= beta)
            return mn;
        alpha = max(alpha,v);*/
    }
    return mn;
}
/*
Node* minNode(Node* n,int alpha=INT_MIN,int beta=INT_MAX) {
    if (n->children.size() == 0)
        return n;
    int v = INT_MAX;
    Node* mn = new Node(INT_MAX);
    for (Node* nn : n->children) {
        int vv = maxNode(nn,alpha,beta)->val;
        if (vv < v) {
            mn = nn;
            v = vv;
        }
        if (v <= alpha)
            return mn;
        beta = min(beta,v);
    }
    return mn;
}
*/
class Game {
public:
    Game(vector<int> js,int g) : jugs(vector<pair<int,int>>()),goal(g) {
        for (int j : js)
            jugs.push_back(make_pair(j,0));
    }
    bool makeMove(int jug,int action) {
        if (action == -2) {  // empty it
            if (jugs.at(jug).second == 0)
                return false;
            jugs.at(jug).second = 0;
            cout << "Empty " << jugs.at(jug).first << endl;
        }
        else if (action == -1) {  // fill it
            if (jugs.at(jug).second == jugs.at(jug).first)
                return false;
            jugs.at(jug).second = jugs.at(jug).first;
            cout << "Fill " << jugs.at(jug).first << endl;
        }
        else {  // pour from jug to action
            if (jugs.at(jug).second == 0 || jugs.at(action).second == jugs.at(action).first)
                return false;
            int temp = jugs.at(jug).second;
            jugs.at(jug).second = max(jugs.at(jug).second-(jugs.at(action).first-jugs.at(action).second),0);
            jugs.at(action).second = min(jugs.at(action).first,temp + jugs.at(jug).second);
            cout << "Fill " << jugs.at(action).first << " from " << jugs.at(jug).first << endl;
        }
        return true;
    }
    vector<pair<int,int>> getJugs() { return jugs; }
    bool done() {
        for (pair<int,int> j : jugs)
            if (j.second == goal)
                return true;
        return false;
    }
    int score(vector<pair<int,int>> js) {
        int sc = 0;
        for (pair<int,int> j : js)
            sc += j.second;
        sc = -pow(50*(abs(goal - sc)/(double)goal),2);
        return sc;
    }
    
    Node* makeTree(int maxDepth,vector<pair<int,int>> js) {
        cout << "invec: depth " << maxDepth << endl;
        for (pair<int,int> p : js)
            cout << p.second << "/" << p.first << endl;
        // max tree level
        if (maxDepth >= 0) {
            Node* n = new Node(js,-1);
            vector<pair<int,int>> moves;  // each pair contains (jug,action)
            for (int i=0; i<jugs.size(); i++) {
                if (jugs.at(i).second != 0)  // emptying
                    moves.push_back(make_pair(i,-2));
                if (jugs.at(i).second != jugs.at(i).first)  // filling
                    moves.push_back(make_pair(i,-1));
                for (int j=0; j<jugs.size(); j++)
                    if (jugs.at(i).second != 0 && jugs.at(j).second != jugs.at(j).first)  // pouring into another
                        moves.push_back(make_pair(i,j));
            }
            //cout << "moves: depth " << maxDepth << endl;
            //for (pair<int,int> m : moves)
                //cout << m.first << " " << m.second << endl;
            //cout << "--" << endl;
            int nodeVal = INT_MIN;
            for (pair<int,int> m : moves) {
                vector<pair<int,int>> jsNew = js;
                if (m.second == -2)  // emptying
                    jsNew.at(m.first).second = 0;
                else if (m.second == -1)  // filling
                    jsNew.at(m.first).second = jsNew.at(m.first).first;
                else {  // pouring into another
                    int temp = jsNew.at(m.first).second;
                    jsNew.at(m.first).second = max(jsNew.at(m.first).second-(jsNew.at(m.second).first-jsNew.at(m.second).second),0);
                    jsNew.at(m.second).second = min(jsNew.at(m.second).first,temp + jsNew.at(m.first).second);
                }
                int sc = score(jsNew);
                if (sc > nodeVal)
                    nodeVal = sc;
                Node* nn = makeTree(maxDepth-1,jsNew);
                if (nn != nullptr)
                    n->children.push_back(nn);
            }
            n->val = nodeVal;
            cout << "tree: depth " << maxDepth << endl;
            printTree(n);
            cout << endl << "///" << endl;
            return n;
        }
        return nullptr;
    }
    void aiMove(int maxDepth) {
        Node* n = maxNode(makeTree(maxDepth,jugs));
        //printTree(n);
        //cout << "score: " << n->val << endl;
        vector<pair<int,int>> result = n->jugs;
        pair<int,int> move = make_pair(-3,-3);  // this will contain (emptiedjug,filledjug)
        for (int i=0; i<jugs.size(); i++)
            if (result.at(i).second < jugs.at(i).second)
                move.first = i;
            else if (result.at(i).second > jugs.at(i).second)
                move.second = i;
        if (move.second == -3)  // 
            makeMove(move.first,-2);
        else if (move.first == -3)
            makeMove(move.second,-1);
        else
            makeMove(move.first,move.second);
    }
private:
    vector<pair<int,int>> jugs;  // pair is capacity, then contents
    int goal;
};

int main() {
    int MAX_DEPTH = 2;
    int numJugs;
    while (true) {
        cout << "Number of jugs: ";
        cin >> numJugs;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000000,'\n');
            cout << "Not a valid number" << endl;
        }
        else
            break;
    }
    vector<int> jugs;
    for (int i=0; i<numJugs; i++)
        while (true) {
            int jug;
            cout << "Volume of jug " << i+1 << ": ";
            cin >> jug;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000000,'\n');
                cout << "Not a valid number" << endl;
            }
            else {
                jugs.push_back(jug);
                break;
            }
        }
    int vol;
    while (true) {
        cout << "Desired volume: ";
        cin >> vol;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000000,'\n');
            cout << "Not a valid number" << endl;
        }
        else
            break;
    }
    Game g(jugs,vol);
    cout << endl << "Solution:" << endl;
    // main loop
    for (int i=1; !g.done(); i++) {
        cout << i << ". ";
        g.aiMove(MAX_DEPTH);
        cin >> vol;
    }
}