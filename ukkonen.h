#ifndef UKKONEN_H_
#define UKKONEN_H_

#include <string>
#include <vector>

class SuffixTree;
class Vertex;

// оболочка для рёбер суффиксного дерева

class Edge {
    private:
        friend Vertex;
        const SuffixTree & ref;
        int vpos, cpos;
        friend SuffixTree;
        Edge(const SuffixTree & ref, int vertex, int first_char);
    public:
        char getFirstChar();//вернуть первый символ на ребре
        char operator[] (int i); // вернуть i-ый символ на ребре
        Vertex toVertex(); // вернуть вершину в которую ведет ребро
        int getLen(); // вернуть длину ребра
        void operator =(const Edge &);
};
// оболочка для вершин суффиксного дерева
class Vertex {
    private:
        friend Edge;
        const SuffixTree & ref;
        int vnum;
        friend SuffixTree;
        Vertex(const SuffixTree & ref, int vertex);
    public:
        std::vector<Edge> getIncidenceList(); // вернуть список ребер, исходящих из вершины
        Edge byChar(char x); // вернуть ребро с первым символом x
        bool hasEdge(char x); // проверяет существование ребра с первым символом x
        void operator = (const Vertex & oth);
};

const int ALPH = (26 * 2 + 1); //размер алфавита над которым мы строим суффиксное дерево
const char termChar = '$';

class SuffixTree {
    private:
        friend Vertex;
        friend Edge;
        struct vertex{
            int to[ALPH], l[ALPH], r[ALPH];
            int parent;
            int height;
            vertex();
        };

        std::vector<vertex> stree;

        int ssz;
        int length;

        std::vector<int> suff;
        const std::string & t;

        int ord(char c) const;
        void init(int vnum);
        int new_vertex(int parent);
        int split(int, int, int);
        int add_leaf(int, int, int);
        bool try_c(int, int, int, int);
        void jump(int &, int &, int &, int, int);

    public:
        explicit SuffixTree(std::string t);
        Vertex getRoot() const; // вернуть вершину, соответствующую корню дерева
        int getLength() const; // вернуть длину строки, от которой строилось суффиксное дерево
};

std::vector<int> findAlloccurences(const SuffixTree &, const std::string & s);

#endif  // UKKONEN_H_
