#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <string>
#include <vector>

namespace Manager {

    struct ScoreEntry {
        std::string name;
        int score;

        bool operator<(const ScoreEntry& other) const {
            return score > other.score;
        }
    };

    class ScoreManager {
    public:
        ScoreManager();

        void reset() { score = 0; }
        void add(int amount) { score += amount; }
        void subtract(int amount) { score -= amount; }
        void multiply(int factor) { score *= factor; }

        int getScore() const { return score; }

        void save(const ScoreEntry& entry);
        std::vector<ScoreEntry> load();

    private:
        int score = 0;
        std::string filePath = "assets/data/scores.json";

        void ensureDirectoryExists();
    };

}

#endif
