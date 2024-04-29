#include "predicat.h"
#include <iostream>
#include <vector>

int main() {
    // Création d'un automate pour reconnaître la séquence "127.0.0.1"
    Automate<char> IP({'1','2','7','.','0','.','0','.','1'});
    // Création d'un automate pour reconnaître la séquence "GET|POST|DELETE|PUT"
    Automate<char> method({'G','E','T','|','P','O','S','T','|','D','E','L','E','T','E','|','P','U','T'});

    std::cout << "Test avec '127.0.0.1': " << (IP.accept({'1','2','7','.','0','.','0','.','1'}) ? "Accepté" : "Rejeté") << std::endl;

    std::cout << "Test avec '133.5.1.1': " << (IP.accept({'1','3','3','.','5','.','1','.','1'}) ? "Accepté" : "Rejeté") << std::endl;

     std::cout << "Test avec '451': " << (IP.accept({'4','5','1'}) ? "Accepté" : "Rejeté") << std::endl;
    
    std::cout << "Test avec 'GET': " << (method.accept({'G','E','T'}) ? "Accepté" : "Rejeté") << std::endl;

    
    std::cout << "Test avec 'POST': " << (method.accept({'P','O','S','T'}) ? "Accepté" : "Rejeté") << std::endl;

    
    std::cout << "Test avec 'KEO': " << (method.accept({'K','E','O'}) ? "Accepté" : "Rejeté") << std::endl;

    IP.afficher();
    method.afficher();

    return 0;
}