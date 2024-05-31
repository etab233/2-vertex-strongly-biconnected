#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;



/*  readGraph(String inputFile,int&N,int&M)
*   extracts a graph from a txt file
*   this function is important to get the graph which other functions will deal with
*
*
*   extracts edges from \a inputFile which its type is .txt and stores it in form of array of vectors,
*   stores the number of nodes in \a N and the number of Edges in \a M,
*   returns \c Array of vectors which is the Graph
*   each index i in Array represents vertix i
*   and sotres inside the vector Array[i] nodes adjacent to the node i
*
*
*   written by:   Hasan Deeb 6656
*/
vector<int>* readGraph(string inputFile, int& N, int& M);



/*  writeGraph(const vector<int>* const graph,const int n,const m,string outputFileName)
*   creates a txt file and stores a Graph inside it
*   this function will help us doing tests and give us the ability to know the results of other functions
*
*   parameter \a outputFileName contains the name of the file
*   parameters \a n and \a m contains the number of vertices and edges
*   paramtere \a graph contain the graph which will be stored inside \a outputFileName .txt
*
*
* Name: Youshaa Albari 6653
*/
void writeGraph(const vector<int>* const graph, const int n, const int m, string outputFileName);



/*  coder: Ahmad Mousa
    card's number:5739
the function writeSet(setName, filename)
stores the set \a setName inside a txt file \a filename .txt
this fuction will help to know what every set contains
*/
void writeSet(const set<pair<int, int>>* setName, string filename);

/*
    this fuction will take a Graph and return its underlying graph

    Ahmad Ali 6254
*/
vector<int>* getUnderlyingGraph(const vector<int>* const graph, const int N, int& M);

/* Toufek Alsskif 5876
next functions will simplify the codes of other functions
*/
bool notExist(map<int, int>& M, int E);
bool notExist(set<pair<int, int>>& S, pair<int, int> E);
bool notExist(set<pair<int, int>>* S, pair<int, int> E);
/*Toufek Alsskif 5876
next function will help us to know whether the dfs function catigorized the edges without lossing any edge or not
*/
void catigorizeingMatchingTest(int y, set<pair<int, int>>*, set<pair<int, int>>*);

/*
    Rose Kasm 6176
    Ayah Shehirly 6393

    these functions will be used inside Jeans Functions to catigoraize the edges and store them inside two sets
*/
void dfs(int x, const vector<int>* const graph, set<pair<int, int>>* FE, set<pair<int, int>>* BE, string* const status, set<pair<int, int>>* check);
void DFS(const vector<int>* const graph, set<pair<int, int>>* FE, set<pair<int, int>>* BE, const int N);


//Rahaf Saleh 6020
bool neighbors(vector<int>* graph, int& x, int& y);

//Raghad Saleh 6019
void modify(vector<int>* graph, int& dfs_counter, vector<int>* dfsNum, vector<int>* comp, vector<string>* state, stack<int>* Ostack, int& node);

//Etab Wanos 6221
void dfs_Gabow(vector<int>* graph, int& dfs_counter, int& representative_num, vector<int>* dfsNum, vector<int>* comp, vector<string>* state, stack<int>* Ostack, int& node);

//Ali Alrhaya 6425
//Jolie jahjah 6175
bool Cheriyan_Mehlorn_Gabow(vector<int>* graph, int& n);



/*
###################################################################
                        The definitions
###################################################################
*/



vector<int>* readGraph(string inputFile, int& N, int& M)
{

    string aLine;
    ifstream theFile(inputFile);
    /*
     we may face a problem when we have n nodes but one of them at least is bigger than or equals to n
     for instance: 3 nodes and they are 1 5 6
     so will rename the n nodes from 0 to n-1
     which means for the previous example the 3 nodes become : 1->0 5->1 6->2
     */
    map<int, int> node;
    int counter = 0;

    //for discarding the first two lines
    for (int i = 0; i < 2; i++)
        getline(theFile, aLine); /*"there is no edge to deal with"*/

    /*now we need to know the number of vertices n and edges m*/

    getline(theFile, aLine);
    N = stoi(aLine.substr(aLine.find(":") + 1));
    int fileEdges = stoi(aLine.substr(aLine.rfind(":") + 1));

    M = 0; // represents the number of the edges in the resulte

    vector<int>* graph = new vector<int>[N]; /*creating the graph and storing it in form of adjacency list*/

    /*discarding one more line because it's empty*/
    getline(theFile, aLine);

    /*now we will fill our adjacency list*/
    for (int i = 0; i < fileEdges; i++)
    {
        getline(theFile, aLine);
        int x, y;
        istringstream is(aLine);
        is >> x >> y;
        if (notExist(node, x))
        {
            node[x] = counter++;
        }
        if (notExist(node, y))
        {
            node[y] = counter++;
        }
        if (x != y)
        {
            graph[node[x]].push_back(node[y]);
            M++;
        }
    }

    return graph;
}

bool notExist(map<int, int>& M, int E)
{ // to know whether a map S conatins an element E or doesn't;
    auto it1 = M.find(E);
    return (it1 == M.end());
}
bool notExist(set<pair<int, int>>& S, pair<int, int> E)
{ // to know whether a set S conatins the pair E or doesn't;
    auto it1 = S.find(E);
    return (it1 == S.end());
}
bool notExist(set<pair<int, int>>* S, pair<int, int> E)
{ // to know whether a set S conatins the pair E or doesn't;
    auto it1 = S->find(E);
    return (it1 == S->end());
}

vector<int>* getUnderlyingGraph(const vector<int>* const graph, const int N, int& M)
{
    M = 0;
    vector<int>* underL = new vector<int>[N]; // here we will store the underlying Graph
    set<pair<int, int>> toCheck;              // to asure there is no duplicate
    for (int i = 0; i < N; i++)
    {
        int outDegree = graph[i].size();
        for (int j = 0; j < outDegree; j++)
        {
            int x = i, y = graph[i][j]; // the edge(x,y) from x to y

            pair<int, int> edge(x, y);
            if (notExist(toCheck, edge))
            { // if there is no edge (x,y) then add it
                underL[x].push_back(y);
                M++;
                toCheck.insert(edge);
            }
            pair<int, int> reversed(y, x);
            if (notExist(toCheck, reversed))
            { // if there is no edge (y,x) then add it
                underL[y].push_back(x);
                M++;
                toCheck.insert(reversed);
            }
        }
    }
    return underL;
}
void writeGraph(const vector<int>* const graph, const int n, const int m, string outputFileName)
{
    /*this function store the graph representation inside outputFileName.txt*/

    fstream theOutput;
    theOutput.open(outputFileName, ios::out);

    /*storing the value in the file*/
    theOutput << "we have " << n << " nodes\n";
    theOutput << "we have " << m << " edges\n";
    for (int i = 0; i < n; i++)
    {
        theOutput << "\n " << i << " : ";
        for (int j = 0; j < graph[i].size(); j++)
            theOutput << graph[i][j] << "  ";
        theOutput << "\n";
    }
    /*we closed the file to free up the memory*/
    theOutput.close();
    /*you can delete it*/
    cout << "done\n";
}

void dfs(int x, const vector<int>* const graph, set<pair<int, int>>* TE, set<pair<int, int>>* BE, string* const status, set<pair<int, int>>* check)
{   //we will catigorize the edges into two catigores and store each one inside a set
    status[x] = "active";
    for (auto y : graph[x])
    {
        pair<int, int> edge(x, y), reversed(y, x);
        if (notExist(check, edge))
        {
            check->insert(edge);
            check->insert(reversed);
            if (status[y] == "new")
            {
                TE->insert(edge);
                dfs(y, graph, TE, BE, status, check);
            }
            else
                BE->insert(edge);
        }
    }
    status[x] = "finished";
}

void DFS(const vector<int>* const graph, set<pair<int, int>>* TE, set<pair<int, int>>* BE, const int N)
{   //no need to document
    string* const status = new string[N];
    for (int i = 0; i < N; i++)
        status[i] = "new";

    set<pair<int, int>>* check = new set<pair<int, int>>();

    for (int i = 0; i < N; i++)
    {
        if (status[i] == "new")
            dfs(i, graph, TE, BE, status, check);
    }

    delete[] status;
}

void writeSet(const set<pair<int, int>>* setName, string fileName)
{
    fstream theOutput;
    theOutput.open(fileName, ios::out);
    theOutput << "there is " << setName->size() << "\n";
    for (auto P : *setName)
        theOutput << "( " << P.first << " , " << P.second << " )\n";

    cout << "finished creating the file :" << fileName << "\n";
}

void catigorizeingMatchingTest(int y, set<pair<int, int>>* TE, set<pair<int, int>>* BE)
{
    cout << "we have TE=" << TE->size() << " and BE=" << BE->size() << " and undircted graph's edges " << y / 2 << "\n";
    if ((TE->size() + BE->size()) * 2 == y)
        cout << "numbers match => True\n";
    else
        cout << "numbers don't match => False\n";
}


/* to check if a graph is strongly connected
   we will implement Cheriyan-Mehlorn-Gabow algorithm.
   if the Graph is made up of one strongly connected component,then
   it is strongly connected. Otherwise it is not.
*/

//first we need a function to check if there is an edge between two nodes
bool neighbors(vector<int>* graph, int& x, int& y) {
    if (count(graph[x].begin(), graph[x].end(), y) != 0)return true;
    return false;
}

//function to update variables that we use in Gabow algorithm
void modify(vector<int>* graph, int& dfs_counter, vector<int>* dfsNum, vector<int>* comp, vector<string>* state, stack<int>* Ostack, int& node) {
    Ostack->push(node);
    state->at(node) = "active";
    dfsNum->at(node) = dfs_counter;
    dfs_counter++;
}
void dfs_Gabow(vector<int>* graph, int& dfs_counter, int& representative_num, vector<int>* dfsNum, vector<int>* comp, vector<string>* state, stack<int>* Ostack, int& node) {
    stack<int>* dfs_stack = new stack<int>();
    //push the source node
    dfs_stack->push(node);
    while (!dfs_stack->empty()) {
        int node = dfs_stack->top();
        dfs_stack->pop();
        /*
          the stack may have active nodes
          but we need new ones only
        */
        if (state->at(node) == "new") {
            modify(graph, dfs_counter, dfsNum, comp, state, Ostack, node);
            /*
              Get all adjacent vertices of the popped vertex "node"
              If it has not been visited, then push it to the stack.
            */
            for (int j = 0; j < graph[node].size(); j++) {
                if (state->at(graph[node][j]) == "new") dfs_stack->push(graph[node][j]);
            }
        }
    }
    /* Ostack stores the nodes that belong to the same
       connected component in an organized way
       supppose C1={1,6} and C2={3,5,99}
       => then Ostack= [1,6,3,5,99)
    */
    while (!Ostack->empty()) {
        comp->at(Ostack->top()) = representative_num;
        int x = Ostack->top();
        Ostack->pop();
        while (!Ostack->empty() && neighbors(graph, x, Ostack->top())) {
            /*
               the node at top of Ostack belong to the same connected
               component of x, so they have the same representative_num
            */
            comp->at(Ostack->top()) = representative_num;
            Ostack->pop();
        }
        // here we finish this connected component
        representative_num++;
    }
    delete dfs_stack;//free the memory allocation
}
bool Cheriyan_Mehlorn_Gabow(vector<int>* graph, int& n) {
    // Init from line 355 to 363
    int dfs_counter = 1;
    int representative_num = 1;
    stack <int>* Ostack = new stack<int>();
    //initialize dfsNum and comp of all n vertex with value -1
    vector<int>* dfsNum = new vector<int>(n, -1);
    vector<int>* comp = new vector<int>(n, -1);
    //firstly all n vertexes are "new"
    vector<string>* state = new vector<string>(n, "new");
    //end of Init

    //and then we will iterate over all nodes and apply Depth First Search
    for (int i = 0; i < n; i++) {
        if (state->at(i) == "new") dfs_Gabow(graph, dfs_counter, representative_num, dfsNum, comp, state, Ostack, i);
    }
    //store the value of this function in a boolean variable so we can delete the
    //allocated memory that used here
    bool result = count(comp->begin(), comp->end(), comp->at(0)) == n;

    // Clean up dynamically allocated memory
    delete Ostack;
    delete dfsNum;
    delete comp;
    delete state;

    //********************Finally The End Of GABOW algorithm****************//
    return result;
}
