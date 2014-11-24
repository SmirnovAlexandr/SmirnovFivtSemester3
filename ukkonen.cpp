#include <memory.h>
#include <string>
#include <algorithm>
#include <vector>
#include "./ukkonen.h"

int SuffixTree::ord(char x) const {
    if (x == termChar) return ALPH - 1;
    if ('Z' >= x && x >= 'A')
        return x - 'A';
    else
        return x - 'a' + 'Z' - 'A' + 1;
}

void SuffixTree::init(int k) {
    memset(stree[k].to, -1, sizeof(stree[k].to));
    suff[k] = -1;
    stree[k].height = 0;
    stree[k].parent = 0;
}

int SuffixTree::new_vertex(int p) {
    stree.push_back(vertex());
    suff.push_back(-1);
    init(ssz);
    stree[ssz].parent = p;
    return ssz++;
}

int SuffixTree::split(int v, int c, int l) {
    if (l == 0) return v;
    if (l == stree[v].r[c] - stree[v].l[c]) return stree[v].to[c];
    int r = new_vertex(v), tp = ord(t[stree[v].l[c] + l]);
    stree[r].to[tp] = stree[v].to[c];
    stree[r].r[tp] = stree[v].r[c];
    stree[r].l[tp] = stree[v].l[c] + l;
    stree[v].r[c] = stree[v].l[c] + l;
    stree[v].to[c] = r;
    stree[stree[r].to[tp]].parent = r;
    stree[r].height = stree[v].height + l;
    return r;
}

int SuffixTree::add_leaf(int v, int c, int l) {
    int r = new_vertex(v);
    stree[v].to[c] = r;
    stree[v].l[c] = l;
    stree[v].r[c] = t.size();
    stree[r].height = t.size() - l - 1 + stree[v].height;
    return r;
}


bool SuffixTree::try_c(int vpos, int epos, int cpos, int c) {
    if (epos == 0)
        return stree[vpos].to[c] != -1;
    else
        return ord(t[stree[vpos].l[cpos] + epos]) == c;
}

void SuffixTree::jump(int & vpos, int & epos, int & cpos, int l, int r) {
   epos = r - l - stree[vpos].height;
   while (stree[vpos].to[cpos] != -1 &&
           epos >= stree[vpos].r[cpos] - stree[vpos].l[cpos]) {
        int len = stree[vpos].r[cpos] - stree[vpos].l[cpos];
        vpos = stree[vpos].to[cpos];
        epos -= len;
        cpos = ord(t[stree[vpos].height + l]);
   }
}

SuffixTree::SuffixTree(std::string a): t(a + termChar) {
    length = a.length();
    ssz = 1;
    int l = 0, r = 0;
    suff.push_back(0);
    stree.push_back(vertex());
    init(0);
    suff[0] = 0;
    int vpos = 0;
    int cpos = 0;
    int epos = 0;
    int alone = -1;
    while (l < t.size() && r < t.size()) {
        int c = ord(t[r]);
        while (l <= r && !try_c(vpos, epos, cpos, c)) {
            int b = split(vpos, cpos, epos);
            if (alone != -1) {suff[alone] = b; alone = -1;}
            if (suff[b] == -1) alone = b;
            add_leaf(b, c, r);
            l++;
            vpos = suff[vpos];
            cpos = ord(t[l + stree[vpos].height]);
            if (l <= r)
                jump(vpos, epos, cpos, l, r);
        }
        if (alone != -1) {
            suff[alone] = vpos;
            alone = -1;
        }
        r++;
        if (l != r)
            jump(vpos, epos, cpos, l, r);
    }
}

SuffixTree::vertex::vertex() {
    memset(to, -1, sizeof(to));
}


Edge::Edge(const SuffixTree & ref, int vertex, int first_char):
            vpos(vertex), cpos(first_char), ref(ref) {
}


char Edge::operator[] (int i) {
    return ref.t[ref.stree[vpos].l[cpos] + i];
}

int Edge::getLen() {
    return ref.stree[vpos].r[cpos] - ref.stree[vpos].l[cpos];
}

Vertex::Vertex(const SuffixTree & ref, int vertex): ref(ref), vnum(vertex) {
}

std::vector<Edge> Vertex::getIncidenceList() {
    std::vector<Edge> res;
    for (int i = 0; i < ALPH; i++) {
        if (ref.stree[vnum].to[i] != -1)
            res.push_back(Edge(ref, vnum, i));
    }
    return res;
}

Vertex SuffixTree::getRoot() const {
    return Vertex(*this, 0);
}

void Edge::operator = (const Edge & oth) {
    this->cpos = oth.cpos;
    this->vpos = oth.vpos;
}

namespace {
    void intDfs(Vertex v, int depth, std::vector<int> & res) {
        std::vector<Edge> nh = v.getIncidenceList();
        if (nh.empty()) res.push_back(depth);
        for (int i = 0; i < nh.size(); i++) {
            intDfs(nh[i].toVertex(), depth + nh[i].getLen(), res);
        }
    }
}

int SuffixTree::getLength() const {
    return length;
}


void Vertex::operator = (const Vertex & oth) {
    vnum = oth.vnum;
}

bool Vertex::hasEdge(char x) {
    return ref.stree[vnum].to[ref.ord(x)] != -1;
}

Edge Vertex::byChar(char x) {
    return Edge(ref, vnum, ref.ord(x));
}

std::vector<int> findAlloccurences(const SuffixTree & stree, const std::string & s) {
    std::vector<int> res;
    Vertex cur = stree.getRoot();
    int clen = 0;
    while (clen < s.size()) {
        if (!cur.hasEdge(s[clen])) return res;
        Edge e = cur.byChar(s[clen]);
        for (int j = 0; j < std::min(e.getLen(), static_cast<int>(s.length()) - clen); j++) {
            if (e[j] != s[clen + j]) return res;
        }
        clen += e.getLen();
        cur = e.toVertex();
    }
    ::intDfs(cur, 0, res);
    for (int i = 0; i < res.size(); i++) {
        res[i] = stree.getLength() + 1 - clen - res[i];
    }
    return res;
}


Vertex Edge::toVertex() {
    return Vertex(ref, ref.stree[vpos].to[cpos]);
}
