// Kevin Sittser
// 525003900
// CSCE420
// Due: February 14, 2019
// hw2pr2.cpp


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

struct Node {
    int val;
    vector<Node*> children;
    Node(int v,vector<Node*> cs) : val(v),children(cs) {}
    Node(int v) : val(v),children(vector<Node*>()) {}
};

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

void printTree(Node* n,int depth=0) {
    if (n->children.size() == 0)
        cout << n->val << "";
    else {
        cout << endl;
        for (int i=0; i<depth; i++)
            cout << "  ";
        cout << "([" << n->val << "]:";
    }
    for (int i=0; i<n->children.size(); i++) {
        printTree(n->children.at(i),depth+1);
        if (i < n->children.size() - 1)
            cout << ",";
    }
    if (n->children.size() > 0)
        cout << ")";
}

int minValue(Node*,int,int);

int maxValue(Node* n,int alpha=INT_MIN,int beta=INT_MAX) {
    if (n->children.size() == 0)
        return n->val;
    int v = INT_MIN;
    for (Node* nn : n->children) {
        v = max(v,minValue(nn,alpha,beta));
        if (v >= beta) {
            cout << "beta pruning" << endl;
            return v;
        }
        alpha = max(alpha,v);
    }
    return v;
}

int minValue(Node* n,int alpha=INT_MIN,int beta=INT_MAX) {
    if (n->children.size() == 0)
        return n->val;
    int v = INT_MAX;
    for (Node* nn : n->children) {
        v = min(v,maxValue(nn,alpha,beta));
        if (v <= alpha) {
            cout << "alpha pruning" << endl;
            return v;
        }
        beta = min(beta,v);
    }
    return v;
}

int main() {
    string s = "";
    int parens = 0;
    string sub;
    do {
        cin >> sub;
        s += sub;
        for (char c : sub)
            if (c == '(')
                parens++;
            else if (c == ')')
                parens--;
    } while (parens != 0);
    Node* tree = processString(s,true);
    //printTree(tree); cout << endl;
    int max = maxValue(tree);
    cout << "Root value in tree is: " << max << endl;
}