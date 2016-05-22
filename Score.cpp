#include <cctype>
#include "Score.h"

Score::Score() : score(999), name("no name"){

}

std::istream &operator>>(std::istream &ist, Score &s){
    ist >> s.score;
    while (isspace(ist.peek())) ist.get();
    std::getline(ist, s.name);
    
    if (!ist) {
        s.score = 999;
        s.name = "no name";
    }
    return ist;
}

std::ostream &operator<<(std::ostream &ost, const Score &s){
    ost << s.score << " " << s.name << std::endl;
    return ost;
}

std::istream &operator>>(std::istream &ist, ScoreFile &scf){
    ist >> scf.beg_score >> scf.int_score >> scf.adv_score;
    return ist;
}

std::ostream &operator<<(std::ostream &ost, const ScoreFile &scf){
    ost << scf.beg_score << scf.int_score << scf.adv_score;
    return ost;
}
