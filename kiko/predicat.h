#ifndef PREDICAT_H
#define PREDICAT_H

#include <memory>
#include <string>
#include <vector>
#include <map>

template <typename T>
class Noeud {
public:
    bool acceptant = false;
    std::map<T, std::unique_ptr<Noeud<T>>> voisins;

    void setVoisin(const T &t, Noeud<T>& n);
    void setAccept();
    Noeud<T>* addVoisin(const T &t);
    bool accept(const std::vector<T> &mots, int pos = 0) const;
    std::string toString() const;
};

template <typename T>
class Automate {
    std::vector<std::unique_ptr<Noeud<T>>> noeuds;
    Noeud<T>* root = nullptr;

public:
    explicit Automate(const std::vector<T> &mots);
    bool accept(const std::vector<T> &mots) const;
    void afficher() const;
};

#endif
