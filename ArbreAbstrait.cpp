#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"
#include <typeinfo>
////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

int NoeudSeqInst::executer() {
    for (unsigned int i = 0; i < m_instructions.size(); i++)
        m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
    return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud * instruction) {
    if (instruction != nullptr) {
        m_instructions.push_back(instruction);
    }
}
////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud * expression)
: m_variable(variable), m_expression(expression) {
}

int NoeudAffectation::executer() {
    int valeur = m_expression->executer(); // On exécute (évalue) l'expression
    ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
    return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud * operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

int NoeudOperateurBinaire::executer() {
    int og, od, valeur;
    if (m_operandeGauche != nullptr) og = m_operandeGauche->executer(); // On évalue l'opérande gauche
    if (m_operandeDroit != nullptr) od = m_operandeDroit->executer(); // On évalue l'opérande droit
    // Et on combine les deux opérandes en fonctions de l'opérateur
    if (this->m_operateur == "+") valeur = (og + od);
    else if (this->m_operateur == "-") valeur = (og - od);
    else if (this->m_operateur == "*") valeur = (og * od);
    else if (this->m_operateur == "==") valeur = (og == od);
    else if (this->m_operateur == "!=") valeur = (og != od);
    else if (this->m_operateur == "<") valeur = (og < od);
    else if (this->m_operateur == ">") valeur = (og > od);
    else if (this->m_operateur == "<=") valeur = (og <= od);
    else if (this->m_operateur == ">=") valeur = (og >= od);
    else if (this->m_operateur == "et") valeur = (og && od);
    else if (this->m_operateur == "ou") valeur = (og || od);
    else if (this->m_operateur == "non") valeur = (!og);
    else if (this->m_operateur == "/") {
        if (od == 0) throw DivParZeroException();
        valeur = og / od;
    }
    return valeur; // On retourne la valeur calculée
}
////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence), m_conditionsSsi(), m_sequencesSsi(), m_sequenceSinon() {
}

/**
 * Fonction qui ajoute une condition au tableau des conditions
 * @param cond est une référence à une condition
 */
void NoeudInstSi::ajouterConditionSsi(Noeud* cond) {
    m_conditionsSsi.push_back(cond);
}

/**
 * Fonction qui ajoute une sequence au tableau de séquences
 * @param seq est une référence à une sequence
 */
void NoeudInstSi::ajouterSequenceSsi(Noeud* seq) {
    m_sequencesSsi.push_back(seq);
}

void NoeudInstSi::ajouterSequenceSinon(Noeud* seq) {
    m_sequenceSinon = seq;
}
int NoeudInstSi::executer() {
    int i=0;
    bool pass = false;
    if (m_condition->executer()) {
        m_sequence->executer();
        pass=true;
    }
 
    if (m_conditionsSsi.size() > 0 && m_sequencesSsi.size() <= m_conditionsSsi.size()){
        for(Noeud* p : m_conditionsSsi){
            if (p->executer()) {
                m_sequencesSsi[i]->executer(); 
                pass=true;
            }
        }
    }
    if (m_sequenceSinon != nullptr){
        if(!pass) {
            m_sequenceSinon->executer();
        }
    }
    return 0; 
}


NoeudInstTantQue::NoeudInstTantQue(Noeud* condition, Noeud * sequence)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudInstTantQue::executer() {
    while (m_condition->executer()) {
        m_sequence->executer();
    }
    return 0;
}

NoeudInstPour::NoeudInstPour(Noeud* initialisation, Noeud* condition, Noeud* incrementation, Noeud * sequence)
: m_expression(condition), m_affectation2(incrementation), m_affectation(initialisation), m_sequence(sequence) {
}

int NoeudInstPour::executer() {
    for (m_affectation->executer(); m_expression->executer(); m_affectation2->executer()) {
        m_sequence->executer();
    }
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstRepeter
////////////////////////////////////////////////////////////////////////////////

NoeudInstRepeter::NoeudInstRepeter(Noeud* sequence, Noeud * condition)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudInstRepeter::executer() {
    do
        m_sequence->executer(); while (m_condition->executer());
    return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstEcrire
////////////////////////////////////////////////////////////////////////////////

NoeudInstEcrire::NoeudInstEcrire()
: m_expression() {
}

void NoeudInstEcrire::ajoute(Noeud * param) {
    m_expression.push_back(param);
}

int NoeudInstEcrire::executer() {
    for (Noeud* p : m_expression) {
        // on regarde si l’objet pointé par p est de type SymbolValue et si c’est une chaîne
        if (typeid (*p) == typeid (SymboleValue) && *((SymboleValue*) p) == "<CHAINE>") {
            std::cout << ((SymboleValue*) p)->getChaine();
        } else {
            std::cout << p->executer();
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstLire
////////////////////////////////////////////////////////////////////////////////

NoeudInstLire::NoeudInstLire()
: m_params() {
}

void NoeudInstLire::ajoute(Noeud * param) {
    m_params.push_back(param);
}

int NoeudInstLire::executer() {
    for (Noeud* p : m_params) {
        int val;
        std::cin >> val;
        ((SymboleValue*) p)->setValeur(val);
    }

    return 0;
}
