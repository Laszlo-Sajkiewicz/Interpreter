#ifndef ARBREABSTRAIT_H
#define ARBREABSTRAIT_H

// Contient toutes les déclarations de classes nécessaires
//  pour représenter l'arbre abstrait

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Symbole.h"
#include "Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
class Noeud {
// Classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
// Remarque : la classe ne contient aucun constructeur
  public:
    virtual int  executer() =0 ; // Méthode pure (non implémentée) qui rend la classe abstraite
    virtual void ajoute(Noeud* instruction) { throw OperationInterditeException(); }
    virtual ~Noeud() {} // Présence d'un destructeur virtuel conseillée dans les classes abstraites
};

////////////////////////////////////////////////////////////////////////////////
class NoeudSeqInst : public Noeud {
// Classe pour représenter un noeud "sequence d'instruction"
//  qui a autant de fils que d'instructions dans la séquence
  public:
     NoeudSeqInst();   // Construit une séquence d'instruction vide
    ~NoeudSeqInst() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();    // Exécute chaque instruction de la séquence
    void ajoute(Noeud* instruction);  // Ajoute une instruction à la séquence

  private:
    vector<Noeud *> m_instructions; // pour stocker les instructions de la séquence
};

////////////////////////////////////////////////////////////////////////////////
class NoeudAffectation : public Noeud {
// Classe pour représenter un noeud "affectation"
//  composé de 2 fils : la variable et l'expression qu'on lui affecte
  public:
     NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation
    ~NoeudAffectation() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();        // Exécute (évalue) l'expression et affecte sa valeur à la variable

  private:
    Noeud* m_variable;
    Noeud* m_expression;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudOperateurBinaire : public Noeud {
// Classe pour représenter un noeud "opération binaire" composé d'un opérateur
//  et de 2 fils : l'opérande gauche et l'opérande droit
  public:
    NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit);
    // Construit une opération binaire : operandeGauche operateur OperandeDroit
   ~NoeudOperateurBinaire() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();            // Exécute (évalue) l'opération binaire)

  private:
    Symbole m_operateur;
    Noeud*  m_operandeGauche;
    Noeud*  m_operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstSi : public Noeud {
// Classe pour représenter un noeud "instruction si"
//  et ses 2 fils : la condition du si et la séquence d'instruction associée
  public:
    NoeudInstSi(Noeud* condition, Noeud* sequence);
     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
   ~NoeudInstSi() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();  // Exécute l'instruction si : si condition vraie on exécute la séquence

  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

class NoeudInstTantQue : public Noeud {
    //Classe pour representer un noeud Instruction tant que
public :
    NoeudInstTantQue(Noeud* condition, Noeud* sequence);
    //construit une instruction tantque avec sa condition et sa sequence d'utilisatioh
    ~NoeudInstTantQue() {} //A cause du destructeur virtuel de la classe Noeud
    int executer();
    
private :
    Noeud* m_condition;
    Noeud* m_sequence;
};

class NoeudInstRepeter : public Noeud {
    //Classe pour representer un noeud Instruction repeter
     //<instRepeter> ::= repeter ( <seqInst> ) jusqua ( <expression> )
public :
    NoeudInstRepeter(Noeud* sequence, Noeud* condition);
    //construit une instruction repeter avec sa condition et sa sequence d'utilisatioh
    ~NoeudInstRepeter() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();
private:
    Noeud* m_sequence;
    Noeud* m_condition;
};

class NoeudInstPour : public Noeud {
    //Classe pour representer un noeud Instruction repeter
public: 
    // <instPour> ::= pour ( [ <affectation> ] ; <expression> ; [ <affectation> ]) <seqInst> finpour
    NoeudInstPour(Noeud* affectation,Noeud* expression,Noeud* affectation2,Noeud* sequence);
    ~NoeudInstPour();
    int executer();
private :
    Noeud* m_affectation;
    Noeud* m_expression;
    Noeud* m_sequence;
    Noeud* m_affectation2;
};
class NoeudInstEcrire : public Noeud {
    //Classe pour representer un noeud Instruction ecrire
public:
      //<instEcrire> ::= ecrire ( <expression> | <chaine> { , <expression> | <chaine> } 
    NoeudInstEcrire(); 
    ~NoeudInstEcrire() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();    // Exécute chaque instruction de la séquence
    void ajoute(Noeud* expression);  // Ajoute une expression à la séquence

  private:
    vector<Noeud *> m_expression; // pour stocker les expressions de la séquence
    
};
class NoeudInstLire : public Noeud {
     //<instLire> ::= lire ( <variable> { , <variable> } )
    public : 
        NoeudInstLire();
        ~NoeudInstLire() {}
        void ajoute(Noeud* param);
        int executer();
        
    private :
        vector<Noeud*>  m_params;
};
class NoeudFacteur : public Noeud {
    // <facteur> ::= <entier>  |  <variable>  |  - <facteur>  | non <facteur> | ( <expression> )
public:
    NoeudFacteur();
    ~NoeudFacteur();
    int executer();
};
class NoeudExpression : public Noeud {
       //<expression> ::= <facteur> { <opBinaire> <facteur> }

public :
    NoeudExpression();
    ~NoeudExpression();
    int executer();
    
};

#endif /* ARBREABSTRAIT_H */
