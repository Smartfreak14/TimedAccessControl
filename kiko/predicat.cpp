#include "predicat.h"
#include <iostream>
#include "xdot.h"

template<class T>
void Noeud<T>::setVoisin(const T &t, Noeud<T>& n) {
    voisins[t].reset(&n);
}

template<class T>
void Noeud<T>::setAccept() {
    acceptant = true;
}

template<class T>
Noeud<T>* Noeud<T>::addVoisin(const T &t) {
    if (voisins.count(t) == 0) {
        voisins[t] = std::make_unique<Noeud<T>>();
    }
    return voisins[t].get();
}

template<class T>
bool Noeud<T>::accept(const std::vector<T> &mots, int pos) const {
    if (pos >= mots.size() || mots[pos] == '|') {
        //printf("vu slash: %d", pos);
        return acceptant;
    }
    if (voisins.count(mots[pos]) == 0) return false;
    return voisins.at(mots[pos])->accept(mots, pos + 1);
}

template<class T>
std::string Noeud<T>::toString() const {
    std::string result;
    int k = 0;
    
    if (acceptant) {
        result += "Q" + std::to_string((long)this) + " [shape = doublecircle];";
    } 

    for (const auto &pair : voisins) {
        result += "Q" + std::to_string((long)this) + " -> Q" + std::to_string((long)pair.second.get()) +
                  " [label = \"" + pair.first + "\"];";
    }
    return result;
}

template<class T>
Automate<T>::Automate(const std::vector<T> &mots) {
    root = new Noeud<T>();
    noeuds.push_back(std::unique_ptr<Noeud<T>>(root));
    Noeud<T>* pos = root;
    for (auto &a : mots) {
        if (a == '|'){
            pos->setAccept();
            pos = root;
        } else {
            pos = pos->addVoisin(a);
            noeuds.push_back(std::unique_ptr<Noeud<T>>(pos));
        }
    }
    pos->setAccept();
}

template<class T>
bool Automate<T>::accept(const std::vector<T> &mots) const {
    return root->accept(mots);
}

template<class T>
void Automate<T>::afficher() const {
    std::string dot = "digraph mon_graphe {";
   
    for (const auto& node : noeuds) {
        std::cout << node->toString();
        dot += node->toString();
    }

    
    dot += "}";
    //std::cout << dot << std::endl;
    //xdot::show(dot);
    xdot::showImg(dot);
}

template class Noeud<char>;
template class Automate<char>;
