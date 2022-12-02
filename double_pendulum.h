//
// Created by finn on 12.04.21.
//

#ifndef PENDULUMBUTTERFLY_DOUBLE_PENDULUM_H
#define PENDULUMBUTTERFLY_DOUBLE_PENDULUM_H
#include <SFML/Graphics.hpp>
#include <math.h>
#include <array>
class double_pendulum {
private:
    float L1;
    float L2;
    float g;
    float m1;
    float m2;
    float alpha;
    float beta;
    float alphap;
    float betap;
    int resx;
    int resy;
    sf::Color col;
    sf::VertexArray lines;
    std::array<int, 2> map(float x, float y);
public:
    double_pendulum(float L1, float L2, float m1, float m2, float alpha, float beta, float g, int resx, int resy, int r, int g_, int b);
    void update(float timestep, sf::RenderWindow* w);
};


#endif //PENDULUMBUTTERFLY_DOUBLE_PENDULUM_H
