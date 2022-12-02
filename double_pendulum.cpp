//
// Created by finn on 12.04.21.
//

#include "double_pendulum.h"
double_pendulum::double_pendulum(float L1, float L2, float m1, float m2, float alpha, float beta, float g, int resx, int resy, int r, int g_, int b){
    // Init
    this->g = g;
    this->L1 = L1;
    this->L2 = L2;
    this->m1 = m1;
    this->m2 = m2;
    this->alpha = alpha;
    this->beta = beta;
    this->resx = resx;
    this->resy = resy;
    // Init Vertices for drawing
    col = sf::Color(r, g_, b, 50);
    lines.setPrimitiveType(sf::LinesStrip);
    lines.append(sf::Vertex(sf::Vector2f(int(round(resx/2)), int(round(resy/2))), col));
    lines.append(sf::Vertex(sf::Vector2f(sin(alpha) * L1, -cos(alpha) * L1), col));
    lines.append(sf::Vertex(sf::Vector2f(sin(alpha) * L1 + sin(beta)*L2, -cos(alpha) * L1 -cos(beta)*L2), col));
}

void double_pendulum::update(float dt, sf::RenderWindow* w){
    // Integrate ODE
    double num1 = -g * (2 * m1 + m2) * std::sin(alpha);
    double num2 = -m2 * g * std::sin(alpha - 2 * beta);
    double num3 = -2 * sin(alpha - beta) * m2;
    double num4 = betap * betap * L2 + alphap * alphap * L1 * std::cos(alpha -beta);
    double den = L1 * (2 * m1 + m2 - m2 * std::cos(2 * alpha - 2 * beta));
    double alphapp = (num1 + num2 + num3 * num4) / den;

    num1 = 2 * std::sin(alpha - beta);
    num2 = (alphap * alphap * L1 * (m1 + m2));
    num3 = g * (m1 + m2) * std::cos(alpha);
    num4 = betap * betap * L2 * m2 * std::cos(alpha - beta);
    den = L2 * (2 * m1 + m2 - m2 * std::cos(2 * alpha - 2 * beta));
    double betapp = (num1 * (num2 + num3 + num4)) / den;

    alphap += alphapp * dt;
    betap += betapp * dt;
    alpha += alphap * dt;
    beta += betap * dt;
    std::array<int, 2> mapped1 = map(sin(alpha) * L1, -cos(alpha) * L1);
    std::array<int, 2> mapped2 = map(sin(alpha) * L1 + sin(beta)*L2, -cos(alpha) * L1 -cos(beta)*L2);
    lines[1] = sf::Vertex(sf::Vector2f(mapped1[0], mapped1[1]), col);
    lines[2] = sf::Vertex(sf::Vector2f(mapped2[0], mapped2[1]), col);
    // Draw updated lines
    w->draw(lines);
}

std::array<int, 2> double_pendulum::map(float x, float y) {
    // Map from real to pixel coords
    std::array<int, 2> ret = {int(round(resx/2) + round(resy*x/2)), resy-int(round(resy/2) + round(resy*y/2))};
    return ret;
}
