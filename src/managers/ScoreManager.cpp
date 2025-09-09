#include "../../include/managers/ScoreManager.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;

namespace Manager {

    ScoreManager::ScoreManager() {
        ensureDirectoryExists();
    }

    void ScoreManager::ensureDirectoryExists() {
        std::filesystem::create_directories("assets/data");
    }

    void ScoreManager::save(const ScoreEntry& entry) {
        auto scores = this->load();
        scores.push_back(entry);
        std::sort(scores.begin(), scores.end());

        json j = json::array();
        for (const auto& s : scores) {
            json obj;
            obj["name"] = s.name;
            obj["score"] = s.score;
            j.push_back(obj);
        }

        std::ofstream out(this->filePath);
        if (out.is_open()) {
            out << j.dump(4);
        }
    }

    std::vector<ScoreEntry> ScoreManager::load() {
        std::vector<ScoreEntry> scores;
        std::ifstream in(this->filePath);
        if (in.is_open()) {
            json j;
            try {
                in >> j;
                for (auto& el : j) {
                    scores.push_back({
                        el.value("name", "???"),
                        el.value("score", 0)
                    });
                }
            } catch (...) {
            }
        }

        std::sort(scores.begin(), scores.end());
        return scores;
    }

}
