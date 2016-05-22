#ifndef SCORE_H_INCLUDE
#define SCORE_H_INCLUDE
#include <fstream>
#include <string>

struct Score{
    Score();
    int score;
    std::string name;
    friend std::istream &operator>>(std::istream &ist, Score &s);
    friend std::ostream &operator<<(std::ostream &ost, const Score &s);
    
};

struct ScoreFile{
    Score beg_score;
    Score int_score;
    Score adv_score;
    
    friend std::istream &operator>>(std::istream &ist, ScoreFile &scf);
    friend std::ostream &operator<<(std::ostream &ost, const ScoreFile &scf);
};

#endif 

