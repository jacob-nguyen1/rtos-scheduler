#include "Visualization.h"
#include <iostream>
#include "Executable.h"

//#define ASSET_PATH

Renderer::Renderer(Scheduler& scheduler, int width, int height)
    : screenWidth(width), screenHeight(height), sectionHeight(height / 3.0f)
{
    window.create(sf::VideoMode(width, height), scheduler.getName());
    window.setFramerateLimit(60);

    if (!font.loadFromFile(std::string(ASSET_PATH) + "/Arial.ttf")) {
        std::cerr << "Warning: could not load font (assets/Arial.ttf)\n";
    }

    colorMap = {
        {"ABS_Control", sf::Color(255, 100, 100)},
        {"BrakePressure", sf::Color(255, 180, 80)},
        {"FuelInjection", sf::Color(100, 255, 100)},
        {"ThrottleControl", sf::Color(80, 180, 255)},
        {"TransmissionCtrl", sf::Color(200, 120, 255)},
        {"DashboardUpdate", sf::Color(255, 255, 120)},
        {"Diagnostics", sf::Color(255, 205, 155)},
        {"DataLogging", sf::Color(180, 255, 255)}
    };
}

sf::Color Renderer::getColorForTask(const std::string& name) {
    if (colorMap.count(name))
        return colorMap[name];
    return sf::Color(200, 200, 200);
}

void Renderer::drawLiveJobs(const std::vector<Job>& jobs, int currTime) {

    const float boxWidth = this->screenWidth / 8;
    const float boxHeight = this->screenHeight / 20;
    const float margin = screenWidth / 80;

    const int maxRows = 4;
    const int maxCols = 6;
    const int maxVisibleJobs = maxCols * maxRows;

    sf::Text label;
    label.setFont(font);
    label.setCharacterSize(18);
    label.setFillColor(sf::Color::White);

    float readyY = 10;
    float runningY = sectionHeight + 10;
    float completedY = 2 * sectionHeight + 10;

    label.setString("READY");
    label.setPosition(20, readyY);
    window.draw(label);

    label.setString("RUNNING");
    label.setPosition(20, runningY);
    window.draw(label);

    label.setString("COMPLETED");
    label.setPosition(20, completedY);
    window.draw(label);

    sf::RectangleShape line(sf::Vector2f(screenWidth, 2.0f));
    line.setFillColor(sf::Color(80, 80, 80));
    line.setPosition(0, sectionHeight);
    window.draw(line);
    line.setPosition(0, 2 * sectionHeight);
    window.draw(line);

    std::vector<const Job*> readyJobs;
    std::vector<const Job*> runningJobs;
    std::vector<const Job*> completedJobs;

    for (const auto& job : jobs) {
        switch (job.state) {
            case JobState::READY: readyJobs.push_back(&job); break;
            case JobState::RUNNING: runningJobs.push_back(&job); break;
            case JobState::COMPLETED: completedJobs.push_back(&job); break;
            default: break;
        }
    }

    //sort ready jobs for preemption visuals
    std::sort(readyJobs.begin(), readyJobs.end(),
            [](const Job* a, const Job* b) {
              return a->readyAge < b->readyAge;
            });

    // sort completed newest to oldest
    std::sort(completedJobs.begin(), completedJobs.end(),
        [](const Job* a, const Job* b) {
            return a->completionTime > b->completionTime;
        });
    if (completedJobs.size() > maxVisibleJobs)
        completedJobs.resize(maxVisibleJobs);

    // helper to draw one section as a grid
    auto drawGrid = [&](const std::vector<const Job*>& list,
                        float baseY,
                        bool isCompleted = false,
                        bool centerVertically = false) {
        const float startX = 100.0f;
        float startY = baseY + 25.0f;

        int visibleCount = std::min<int>(list.size(), maxVisibleJobs);

        // center vertically (for RUNNING)
        if (centerVertically && visibleCount > 0) {
            float gridHeight = (boxHeight + margin) * visibleCount - margin;
            startY = baseY + (sectionHeight - gridHeight) / 2.0f;
        }

        // track grid position
        int row = 0;
        int col = 0;

        for (int i = 0; i < visibleCount; ++i) {
            const Job* job = list[i];

            float x = startX + col * (boxWidth + margin);
            float y = startY + row * (boxHeight + margin);

            sf::RectangleShape rect(sf::Vector2f(boxWidth, boxHeight));
            rect.setPosition(x, y);

            if (isCompleted) {
                // fade effect for new completions (GPT-5)
                auto clamp = [](float v, float lo, float hi) {
                    return std::max(lo, std::min(hi, v));
                };
                float alpha = clamp(1.0f - job->fadeTimer, 0.0f, 1.0f);
                sf::Color base(100, 100, 100);
                sf::Color flash(200, 255, 200);
                sf::Uint8 r = static_cast<sf::Uint8>(flash.r * alpha + base.r * (1 - alpha));
                sf::Uint8 g = static_cast<sf::Uint8>(flash.g * alpha + base.g * (1 - alpha));
                sf::Uint8 b = static_cast<sf::Uint8>(flash.b * alpha + base.b * (1 - alpha));
                rect.setFillColor(sf::Color(r, g, b));
            } else {
                rect.setFillColor(getColorForTask(job->task));
            }

            if (job->state == JobState::RUNNING || job->state == JobState::READY) {
                rect.setOutlineThickness(3.0f);
                rect.setOutlineColor(sf::Color::White);
            }

            window.draw(rect);

            bool missed = false;

            // missed deadline
            if (job->state == JobState::COMPLETED && job->missedDeadline) {
                missed = true;
            }
            else if ((job->state == JobState::READY || job->state == JobState::RUNNING) &&
                    currTime > job->deadline) {
                missed = true;
            }

            // outline missed deadline
            if (missed) {
                sf::RectangleShape border(sf::Vector2f(boxWidth, boxHeight));
                border.setPosition(x, y);
                border.setFillColor(sf::Color::Transparent);
                border.setOutlineThickness(3.0f);
                border.setOutlineColor(sf::Color(255, 50, 50));
                window.draw(border);
            }

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(14);
            text.setString(job->task);
            text.setFillColor(sf::Color::Black);

            sf::FloatRect textBounds = text.getLocalBounds();

            text.setOrigin(textBounds.left + textBounds.width / 2.0f,
                           textBounds.top + textBounds.height / 2.0f);

            text.setPosition(x + boxWidth / 2.0f, y + boxHeight / 2.0f - 2.0f);
            window.draw(text);

            // draw progress bar
            if (job->state == JobState::RUNNING || job->state == JobState::READY) {
                rect.setOutlineThickness(3.0f);
                rect.setOutlineColor(sf::Color::White);

                float pct = job->progress();
                pct = std::max(0.f, std::min(1.f, pct));

                sf::Color baseColor = getColorForTask(job->task);
                sf::Color progressColor;

                float brightness = 0.299f * baseColor.r + 0.587f * baseColor.g + 0.114f * baseColor.b;
                if (brightness > 150.f)
                    progressColor = sf::Color(60, 180, 60); // darker green for bright backgrounds
                else {
                    progressColor = sf::Color(100, 255, 100); // lighter green for dark backgrounds
                }

                sf::RectangleShape progressBar;
                progressBar.setSize(sf::Vector2f((boxWidth - 6) * pct, 8));
                progressBar.setPosition(x + 3, y + boxHeight - 12);
                progressBar.setFillColor(progressColor);
                window.draw(progressBar);
            }

            // draw % text
            if (job->state == JobState::RUNNING) {
                float pct = job->progress();
                pct = std::max(0.f, std::min(1.f, pct));

                sf::Text pctText;
                pctText.setFont(font);
                pctText.setCharacterSize(12);
                pctText.setFillColor(sf::Color::White);
                pctText.setString(std::to_string(static_cast<int>(pct * 100)) + "%");
                pctText.setPosition(x + boxWidth / 2.f - 15.f, y + boxHeight + 4.f);
                window.draw(pctText);
            }


            // advance grid position
            row++;
            if (row >= maxRows) {
                row = 0;
                col++;
                if (col >= maxCols) break; // stop drawing when grid full
            }
        }
    };

    drawGrid(readyJobs, readyY);
    drawGrid(runningJobs, runningY, false, true);
    drawGrid(completedJobs, completedY, true);

    if (runningJobs.empty()) {
        sf::Text idleText;
        idleText.setFont(font);
        idleText.setCharacterSize(22);
        idleText.setFillColor(sf::Color(180, 180, 180));
        idleText.setString("IDLE");
        
        sf::FloatRect bounds = idleText.getLocalBounds();
        float x = (screenWidth - bounds.width) / 2.0f;
        float y = sectionHeight + (sectionHeight - bounds.height) / 2.0f - 10.f; 
        idleText.setPosition(x, y);

        window.draw(idleText);
    }

    if (readyJobs.empty()) {
        sf::Text idleText;
        idleText.setFont(font);
        idleText.setCharacterSize(22);
        idleText.setFillColor(sf::Color(180, 180, 180));
        idleText.setString("WAITING");
        
        sf::FloatRect bounds = idleText.getLocalBounds();
        float x = (screenWidth - bounds.width) / 2.0f;
        float y = sectionHeight - (sectionHeight - bounds.height) / 2.0f - 10.0f; 
        idleText.setPosition(x, y);

        window.draw(idleText);
    }

}



void Renderer::drawClock(int currentTimeMs) {
    sf::Text clock;
    clock.setFont(font);
    clock.setCharacterSize(24);
    clock.setFillColor(sf::Color::White);
    clock.setString("Time: " + std::to_string(currentTimeMs) + " ms");
    clock.setPosition(screenWidth / 2 - 80, screenHeight - 40);
    window.draw(clock);
}

bool Renderer::isOpen() {
    return window.isOpen();
}

void Renderer::close() {
    window.close();
}

bool Renderer::renderLive(const std::vector<Job>& jobs, int currTime) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            {
                close();
                return false;
            }
    }

    window.clear(sf::Color(25, 25, 25));

    drawLiveJobs(jobs, currTime);
    drawClock(currTime);

    window.display();

    return true;
}
