#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

namespace Manager {
class ScoreManager {
    private:
        int score = 0;
    
    public:
        void reset() { score = 0; }
        void add(int amount) { score += amount; }
        void subtract(int amount) { score -= amount; }
        void multiply(int factor) { score *= factor; }
    
        int getScore() const { return score; }
    };
}


#endif
