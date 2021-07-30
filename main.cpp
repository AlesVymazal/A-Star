#include <iostream>
#include <list>
#include <vector>
#include <cmath>
using namespace std;

struct Coor{
    int x,y;
    Coor(){};
    Coor(int x_, int y_) : x(x_), y(y_){};
    bool operator == (Coor A)
    {
        return (A.x == x && A.y == y) ? true : false;
    }
};
class Node
{
public:
    Node(Coor c, bool, bool): x(c.x), y(c.y), Visited(false), Wall(false){}
    void visited(bool){Visited = true;}
    void wall(bool){Wall = true;}
    bool visited() { return Visited;}
    bool wall() { return Wall;}
    void parent(Node *previous) {Parent = previous;}
    void setPrev(Coor **c) {(*c)->x = x, (*c)->y = y;}
    void setNeighbour(vector<Node> Neigh){Neighbours = Neigh;}
    Coor getCoor(){Coor c(x,y); return c;}
    vector<Node> getNeighbours(){return Neighbours;}
    Node* parent() { return Parent;}
    private:
    int x,y;
    bool Visited;
    bool Wall;
    vector<Node> Neighbours;
    Node *Parent;
};

class AStar{
    public:
    AStar(int Size, Coor Start, Coor End, vector<vector<int>> Walls)
    {
        size = Size;
        end = End;
        vector<Node> Subvector;
        for(int i = 0; i < size; ++i)
        {
            for(int j = 0; j < size; ++j)
            {
                Subvector.push_back(Node(Coor(i,j),false,false));
            }
            Matrix.push_back(Subvector);
            Subvector.clear();
        }
        for(auto w : Walls)
        {
            Matrix[w[0]][w[1]].wall(true);
        }
        Matrix[Start.x][Start.y].visited(true);
    }
    void isInMatrix(Coor c) //zjistí, jestli souradnice je v rozsahu 0,0 az size-1, size-1
    {
        vector<int> next = {1,1,-1,-1};
        vector<pair<char,int>>limits {{'i',size - 1},{'j',size - 1},{'i',0},{'j',0}};
        for(unsigned int k = 0; k < limits.size(); ++k)
        {
            if(limits[k].first == 'i' && limits[k].second != c.x)
            {
                Moves.push_back(make_pair('i',next[k]+ c.x));
            }
            if(limits[k].first == 'j' && limits[k].second != c.y)
            {
                Moves.push_back(make_pair('j',next[k]+ c.y));
            }
        }
    }
    bool findNeighbours(Coor c) //Iteruje i a nebo j, pokud dalsi uzel neni obsazeny, nebo neni zed.
    {                                               //Prve zkousi jit dolu, pak doprava, nahoru a nakonec doleva.
        bool isNextNode = false;
        Coor Previous = c;
        vector<Node> subvector;
        while(!Moves.empty())
        {
            Moves.front().first == 'i' ? c.x = Moves.front().second : c.y = Moves.front().second;
            Moves.pop_front();
            if(!Matrix[c.x][c.y].visited() && !Matrix[c.x][c.y].wall())
            {
                isNextNode = true;
                subvector.push_back(Matrix[c.x][c.y]);
            }
            c = Previous;
        }
        if(isNextNode)
        {
            Matrix[Previous.x][Previous.y].setNeighbour(subvector);
        }
        return isNextNode;
    }
    void heuristic(Coor *c, list<Node>*Solution)
    {
        Solution->push_back(Matrix[c->x][c->y]);
        double shortestDistance = 99999.0;
        vector<Node>::iterator it, shortest;
        Coor Previous = *c;
        auto Neighbours = Matrix[c->x][c->y].getNeighbours();
        for(it = Neighbours.begin(); it != Neighbours.end(); ++it)
        {
            if(shortestDistance >= sqrt(double(pow((it->getCoor().x - end.x),2) + pow((end.y - it->getCoor().y),2))))
            {
                shortestDistance = sqrt(double(pow((it->getCoor().x - end.x),2) + pow((end.y - it->getCoor().y),2)));
                shortest = it;
            }
        }
        *c = shortest->getCoor();
        Neighbours.erase(shortest);
        Matrix[c->x][c->y].visited(true);
        Matrix[c->x][c->y].parent(&Matrix[Previous.x][Previous.y]);
        Matrix[Previous.x][Previous.y].setNeighbour(Neighbours);
    }
    void getBack(Coor *c, list<Node>*Solution)
    {
        Matrix[c->x][c->y].parent()->setPrev(&c);
        Solution->pop_back();
    }
    Node getNode(int i, int j)
    {
        return Matrix[i][j];
    }
    private:
    int size;
    Coor start;
    Coor end;
    list<pair<char, int>> Moves;
    vector<vector<Node>> Matrix;
};

int main()
{
    int Size = 20;
    list<Node>Solution;
    vector<vector<int>> Walls = {{0,3},{1,3},{1,2}, {1,1}, {1,4}, {1,5}, {2,1}, {3,1}, {4,1}};
    Coor Start(0,1);
    Coor End(19,15);
    AStar AS(20, Start, End, Walls);
    while(!(Start == End))
    {
        AS.isInMatrix(Start);
        (AS.findNeighbours(Start)) ? AS.heuristic(&Start, &Solution) : AS.getBack(&Start, &Solution);
    }
    ///**********************************************///
    for(int i = 0; i < Size; ++i)
    {
        for(int j = 0; j < Size; ++j)
        {
            (AS.getNode(i,j).visited()) ? cout << "* " : cout;
            (AS.getNode(i,j).wall()) ? cout << "X ": cout;
            (!AS.getNode(i,j).wall() && !AS.getNode(i,j).visited()) ? cout << "0 ": cout;
        }
        cout << endl;
    }
    return 0;
}
