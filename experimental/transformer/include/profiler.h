#include <iostream>
#include <chrono>
#include <string>
#include <map>

class Profiler {
public:
    static Profiler& getInstance() {
        static Profiler instance;
        return instance;
    }

    void start(const std::string& section) {
        start_times[section] = std::chrono::high_resolution_clock::now();
    }

    void reset() {
        start_times.clear();
        durations.clear();
        counts.clear();
    }

    void stop(const std::string& section) {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_times[section]).count();
        durations[section] += duration;
        counts[section]++;
    }

    void report() const {
        #ifdef PROFILER
        std::cout << "Section, Total time(us), Average time(us), Count" << std::endl;
        for (const auto& entry : durations) {
            std::string row;
            row += entry.first + ", ";
            row += std::to_string(entry.second) + ", ";
            row += std::to_string(entry.second / counts.at(entry.first)) + ", ";
            row += std::to_string(counts.at(entry.first));
            std::cout << row << std::endl;
        }
        #endif
    }

private:
    Profiler() {}
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    std::map<std::string, std::chrono::high_resolution_clock::time_point> start_times;
    std::map<std::string, long long> durations;
    std::map<std::string, int> counts;
};