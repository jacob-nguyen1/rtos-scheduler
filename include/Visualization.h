#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "Job.h"
#include "Scheduler.h"

class Renderer {
    sf::RenderWindow window;
    sf::Font font;
    int screenWidth;
    int screenHeight;
    float sectionHeight;

    std::map<std::string, sf::Color> colorMap;
    sf::Color getColorForTask(const std::string& name);

    void drawLiveJobs(const std::vector<Job>& jobs, int currTime);
    void drawClock(int currentTimeMs);
    
public:
    Renderer(Scheduler& scheduler, int width = 1200, int height = 800);

    void renderLive(const std::vector<Job>& jobs, int currTime);
    bool isOpen();
};