// Kevin Sittser
// 525003900
// CSCE420
// Due: February 14, 2019
// hw2pr3.cpp


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
using namespace std;

struct Node {
    int brady;
    vector<int> rams;
    int val;
    vector<Node*> children;
    Node(int b,vector<int> rs,int v) : brady(b),rams(rs),val(v),children(vector<Node*>()) {}
    Node(int v) : brady(-1),rams(vector<int>()),children(vector<Node*>()) {}
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

Node* minNode(Node*,int,int);

Node* maxNode(Node* n,int alpha=INT_MIN,int beta=INT_MAX) {
    if (n->children.size() == 0)
        return n;
    int v = INT_MIN;
    Node* mn = new Node(INT_MIN);
    for (Node* nn : n->children) {
        int vv = minNode(nn,alpha,beta)->val;
        if (vv > v) {
            mn = nn;
            v = vv;
        }
        if (v >= beta)
            return mn;
        alpha = max(alpha,v);
    }
    return mn;
}

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

class Game {
public:
    Game() : brady(0),rams(vector<int>()) {
        for (int i=29; i<=32; i++)
            rams.push_back(i);
    }
    void setBradyInitial(int loc) {
        brady = loc;
    }
    bool moveBrady(int dest) {
        if (!canMoveTo(dest))
            return false;
        // ensure move direction is legal
        if ((brady-1) % 8 <= 3 && !(dest == brady+4 || dest == brady+5 || dest == brady-3 || dest == brady-4))
            return false;
        if ((brady-1) % 8 > 3 && !(dest == brady+3 || dest == brady+4 || dest == brady-4 || dest == brady-5))
            return false;
        // don't go off left/right edge
        if (brady % 8 == 5 && (dest == brady+3 || dest == brady-5))
            return false;
        if (brady % 8 == 4 && (dest == brady-3 || dest == brady+5))
            return false;
        brady = dest;
        return true;
    }
    bool moveRam(int orig,int dest) {
        if (!canMoveTo(dest)) {
            return false;
        }
        for (int i=0; i<rams.size(); i++)
            if (orig == rams.at(i)) {
                // ensure move direction is legal
                if ((rams.at(i)-1) % 8 <= 3 && !(dest == rams.at(i)-3 || dest == rams.at(i)-4))
                    return false;
                if ((rams.at(i)-1) % 8 > 3 && !(dest == rams.at(i)-4 || dest == rams.at(i)-5))
                    return false;
                // don't go off left/right edge
                if (brady % 8 == 5 && dest == brady-5)
                    return false;
                if (brady % 8 == 4 && dest == brady-3)
                    return false;
                rams.at(i) = dest;
                return true;
            }
        return false;
    }
    int getBrady() { return brady; }
    vector<int> getRams() { return rams; }
    bool canMoveTo(int dest) {
        if (dest < 1 || dest > 32)
            return false;
        if (dest == brady)
            return false;
        for (int r : rams)
            if (dest == r)
                return false;
        return true;
    }
    int winner(int b,vector<int> rs) {
        if ((b-1) % 8 <= 3 && !canMoveTo(b+4) && !canMoveTo(b+5) && !canMoveTo(b-3) && !canMoveTo(b-4))
            return 2;  // rams win
        if ((b-1) % 8 > 3 && !canMoveTo(b+3) && !canMoveTo(b+4) && !canMoveTo(b-4) && !canMoveTo(b-5))
            return 2;  // rams win
        if (b % 8 == 5 && !canMoveTo(b-4) && !canMoveTo(b+4))
            return 2;
        if (b % 8 == 4 && !canMoveTo(b-4) && !canMoveTo(b+4))
            return 2;
        if (b >= 29)
            return 1;
        vector<int> ramsTemp = rs;
        sort(ramsTemp.begin(),ramsTemp.end());
        if (ramsTemp.at(3) == 4)
            return 1;  // brady wins
        return 0;  // no winner yet
    }
    int score(int bPos,vector<int> rsPos,bool disp=false) {
        int bradyScore,ramsScore;
        if (winner(bPos,rsPos) == 1)  // brady win
            bradyScore = INT_MAX;
        else if (winner(bPos,rsPos) == 2)  // rams win
            ramsScore = INT_MAX;
        else {
            int closenessToEnd = (bPos / 4);  // value 0-8; higher means better for brady
            double distFromClosestRam = 0;  // value 1-7; higher means better for brady
            vector<int> distsToRams;
            for (int r : rsPos) {
                int vDist = abs(floor((double)(r-1) / 4) - floor((double)(brady-1) / 4));
                int b = (bPos-1) % 8 + 1;
                int r1 = (r-1) % 8 + 1;
                if (b <= 4)
                    b = 2*b;
                else
                    b = 2*b - 9;
                if (r1 <= 3)
                    r1 = 2*r1;
                else
                    r1 = 2*r1 - 9;
                int hDist = abs(r1 - b);
                distsToRams.push_back(pow(pow(hDist,2)+pow(vDist,2),0.5));
            }
            sort(distsToRams.begin(),distsToRams.end());
            distFromClosestRam = distsToRams.at(0);
            
            // brady is doing well if he is close to end and, to a lesser extent, far from any rams
            bradyScore = pow(9*closenessToEnd,2) * 2 + pow(9*distFromClosestRam,2);  // 1-113,000
            
            // rams are doing well if they are close to brady
            int avgRamDist = 0;  // value 1-7; lower means better for rams
            for (int d : distsToRams)
                avgRamDist += d;
            avgRamDist /= rsPos.size();
            //if (disp)
                //cout << "aRD: " << avgRamDist << endl;
            ramsScore = pow(140 - (20 * avgRamDist),2);  // 0 to 490,000
        }
        //if (disp)
            //cout << "bS: " << bradyScore << " / rS: " << ramsScore << endl;
        return bradyScore - ramsScore;
    }
    
    Node* makeInitialTree(int maxDepth) {
        vector<int> rsPos = {29,30,31,32};
        Node* n = new Node(-1,rsPos,-1);
        vector<int> moves;
        for (int i=1; i<=4; i++)
            moves.push_back(i);
        int nodeVal = INT_MIN;
        for (int m : moves) {
            int sc = score(m,rsPos);
            if (sc > nodeVal)
                nodeVal = sc;
            Node* nn = makeBradyTree(maxDepth,m,rsPos);
            if (nn != nullptr)
                n->children.push_back(nn);
        }
        n->val = nodeVal;
        return n;
    }
    Node* makeBradyTree(int maxDepth,int bPos,vector<int> rsPos) {
        // where next move is Brady's
        // max tree level
        if (maxDepth > 0) {
            Node* n = new Node(bPos,rsPos,-1);
            vector<int> moves;
            if ((bPos-1) % 8 <= 3) {
                if (canMoveTo(bPos+4))
                    moves.push_back(bPos+4);
                if (canMoveTo(bPos+5) && brady%8 != 4)
                    moves.push_back(bPos+5);
                if (canMoveTo(bPos-3) && brady%8 != 4)
                    moves.push_back(bPos-3);
                if (canMoveTo(bPos-4))
                    moves.push_back(bPos-4);
            }
            else {
                if (canMoveTo(bPos+3) && brady%8 != 5)
                    moves.push_back(bPos+3);
                if (canMoveTo(bPos+4))
                    moves.push_back(bPos+4);
                if (canMoveTo(bPos-4))
                    moves.push_back(bPos-4);
                if (canMoveTo(bPos-5) && brady%8 != 5)
                    moves.push_back(bPos-5);
            }
            int nodeVal = INT_MIN;
            for (int m : moves) {
                int sc = score(m,rsPos);
                if (sc > nodeVal)
                    nodeVal = sc;
                Node* nn = makeRamsTree(maxDepth-1,m,rsPos);
                if (nn != nullptr)
                    n->children.push_back(nn);
            }
            n->val = nodeVal;
            return n;
        }
        return nullptr;
    }
    Node* makeRamsTree(int maxDepth,int bPos,vector<int> rsPos) {
        // where next move is Rams'
        // min tree level
        if (maxDepth > 0) {
            Node* n = new Node(bPos,rsPos,-1);
            vector<pair<int,int>> moves;
            for (int r : rsPos)
                if ((bPos-1) % 8 <= 3) {
                    if (canMoveTo(r-3) && r%8 != 4)
                        moves.push_back(make_pair(r,r-3));
                    if (canMoveTo(r-4))
                        moves.push_back(make_pair(r,r-4));
                }
                else {
                    if (canMoveTo(r-4))
                        moves.push_back(make_pair(r,r-4));
                    if (canMoveTo(r-5) && r%8 != 5)
                        moves.push_back(make_pair(r,r-5));
                }
            int nodeVal = INT_MAX;
            for (pair<int,int> m : moves) {
                vector<int> rsNew;
                for (int r : rsPos) {
                    if (r == m.first)
                        rsNew.push_back(m.second);
                    else
                        rsNew.push_back(r);
                }
                int sc = score(bPos,rsNew);
                if (sc < nodeVal)
                    nodeVal = sc;
                Node* nn = makeBradyTree(maxDepth-1,bPos,rsNew);
                if (nn != nullptr)
                    n->children.push_back(nn);
            }
            n->val = nodeVal;
            return n;
        }
        return nullptr;
    }
    int aiBradyMove(int maxDepth) {
        int dest;
        Node* n = maxNode(makeBradyTree(maxDepth,brady,rams));
        dest = n->brady;
        score(n->brady,n->rams,true);
        moveBrady(dest);
        return dest;
    }
    void drawBoard() {
        for (int i=3; i>=0; i--) {
            for (int j=8; j>=5; j--) {
                cout << "+";
                if (i*8+j == brady)
                    cout << "#";
                else if (find(rams.begin(),rams.end(),i*8+j) < rams.end())
                    cout << "&";
                else
                    cout << ".";
            }
            cout << "  " << 8*i+5 << endl;
            for (int j=4; j>=1; j--) {
                if (i*8+j == brady)
                    cout << "#";
                else if (find(rams.begin(),rams.end(),i*8+j) < rams.end())
                    cout << "&";
                else
                    cout << ".";
                cout << "+";
            }
            cout << "  " << 8*i+1 << endl;
        }   
    }
private:
    int brady;
    vector<int> rams;
};

int main() {
    int MAX_DEPTH = 9;
    Game g;
    int pos;
    Node* n = maxNode(g.makeInitialTree(MAX_DEPTH));
    pos = n->brady;
    // choose brady intial square
    cout << "Tom Brady picks square " << pos << endl;
    g.setBradyInitial(pos);
    // main game loop
    while (!g.winner(g.getBrady(),g.getRams())) {
        cout << "Rams are on squares: ";
        for (int r : g.getRams())
            cout << r << " ";
        cout << endl;
        // brady's move
        cout << "Tom Brady moves to square " << g.aiBradyMove(MAX_DEPTH) << endl;
        if (g.winner(g.getBrady(),g.getRams()))
            break;
        //g.drawBoard();
        int dest;
        int orig;
        // rams' move
        bool tryAgain = true;
        while (tryAgain) {
            tryAgain = false;
            while (true) {
                cout << "Which Ram do you want to move?: ";
                cin >> orig;
                bool ramFound = false;
                for (int r : g.getRams())
                    if (r == orig)
                        ramFound = true;
                if (cin.fail() || !ramFound) {
                    cin.clear();
                    cin.ignore(1000000,'\n');
                    cout << "Not a valid Ram" << endl;
                }
                else
                    break;
            }
            while (true) {
                cout << "To which square? (0 to choose different Ram): ";
                cin >> dest;
                if (dest == 0) {
                    tryAgain = true;
                    break;
                }
                if (cin.fail() || !g.moveRam(orig,dest)) {
                    cin.clear();
                    cin.ignore(1000000,'\n');
                    cout << "Not a valid move" << endl;
                }
                else
                    break;
            }
        }
    }
    if (g.winner(g.getBrady(),g.getRams()) == 1)
        cout << "Tom Brady wins" << endl;
    else if (g.winner(g.getBrady(),g.getRams()) == 2)
        cout << "Rams win" << endl;
}