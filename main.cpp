#include <iostream>
#include "double_pendulum.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);
void take_screenshot(sf::RenderWindow* window, std::string filename);
rgb get_color(int i, int max);
int main() {
    bool record_video = false;
    std::string path_to_f = "recording";
    if(record_video){
        system(("rm -r " + path_to_f).c_str());
        system(("mkdir -p " + path_to_f).c_str());
    }
    float dt_ = 0.001;
    int fps = 60;
    int length = 30;
    bool paused = true;
    bool first = true;
    const int n = 10000; // Number of pendulums
    std::array<double_pendulum*, n> pends;
    float eps = 0.0000001; // rad/s offset per pendulum for first degree of freedom (alpha)
    float eps_add = eps;
    int resx = 1920;
    int resy = 1080;
    for(int i = 0; i<n; i++){ // generate n pendulums
        rgb col = get_color(i, n);
        std::cout << "R: " << col.r  << std::endl;
        std::cout << "G: " << col.g  << std::endl;
        std::cout << "B: " << col.b  << std::endl;
        pends.at(i) = new double_pendulum(.5, 0.5, 10, 10, M_PI+eps, M_PI*51/52, 1, resx, resy, (col.r*255), (col.g*255), (col.b*255));
        eps += eps_add;
    }
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(resx, resy), "croff", sf::Style::Fullscreen); // New Renderwindow
    window->clear(sf::Color::Black); // make it black
    sf::Text* text = new sf::Text; // dt text field
    sf::Text* text_t = new sf::Text; // n_pendulums text field
    // init texts
    sf::Font font;
    if (!font.loadFromFile("Roboto-Thin.ttf"))
    {
        std::cout << "No font file found!\n";
    }
    text->setFont(font);
    text->setCharacterSize(18);
    text->setFillColor(sf::Color::White);
    text->setPosition(resx-170,0+25);
    text_t->setFont(font);
    text_t->setCharacterSize(18);
    text_t->setFillColor(sf::Color::White);
    text_t->setPosition(resx-170,0);
    text_t->setString("n: " + std::to_string(n) + " pendulums");
    sf::Clock clock;
    if(!record_video) {
    	std::cout<< "Press space to start and Q to quit!" << std::endl;
        // Open SFML Window
        while (window->isOpen()) {
            sf::Event event;
            // catch events, do nothing TODO
            while (window->pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window->close();
                        break;
                    case sf::Event::KeyPressed:
                        switch (event.key.code) {
                            case sf::Keyboard::Space:
                                if (paused) {
                                    clock.restart();
                                }
                                paused = !paused;
                                break;
                            case sf::Keyboard::Q:
                                window->close();
                                break;
                        }
                }
            }
            if (first) {
                for (double_pendulum *c_pend : pends) {
                    c_pend->update(0, window);
                }
                first != first;
            }
            if (!paused) {
                // get current elapsed time for ODE solver
                sf::Time deltatime = clock.getElapsedTime();
                double dt = deltatime.asSeconds();
                clock.restart();
                window->clear(sf::Color::Black); // clear drawings
                window->draw(*text);
                window->draw(*text_t);
                // iterate over all pendulums, update and draw
                for (double_pendulum *c_pend : pends) {
                    c_pend->update(dt, window);
                }
                // display current step size
                text->setString("dt: " + std::to_string(dt * 1000) + " ms");
            } else {
                window->draw(*text_t);
            }
            window->display();
        }
    }
    else{
        // Open SFML Window
        float c_time = 0;
        float next_time = 0;
        int i = 0;
        while (window->isOpen()) {
            sf::Event event;
            // catch events, do nothing TODO
            while (window->pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window->close();
                        break;
                    case sf::Event::KeyPressed:
                        switch (event.key.code) {
                            case sf::Keyboard::Q:
                                window->close();
                                break;
                        }
                }
            }
            window->clear(sf::Color::Black); // clear drawings
            window->draw(*text_t);
            window->display();
            // iterate over all pendulums, update and draw
            for (double_pendulum *c_pend : pends) {
                c_pend->update(dt_, window);
            }
            c_time += dt_;
            if(c_time > next_time){
                take_screenshot(window, path_to_f + "/" + std::to_string(i) + ".jpg");
                next_time += 1/double(fps);
                i+=1;
            }
            std::cout << "Time: " << c_time << ", FPS: " << fps << ", length: " << length << ", next: " << next_time << std::endl;
            if(c_time > length){
                window->close();
            }
        }
    }
}

rgb get_color(int i, int max){ // 6*255 steps in rainbow way
    std::cout << i << std::endl;
    std::cout << 360*double(i)/max << std::endl;
    return(hsv2rgb({360*double(i)/max, .8, 1}));
}
rgb get_color_2(int i, int max){
    // 255 Steps per 7 Stages

}
hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
        case 0:
            out.r = in.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = in.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = in.v;
            out.b = t;
            break;

        case 3:
            out.r = p;
            out.g = q;
            out.b = in.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = in.v;
            break;
        case 5:
        default:
            out.r = in.v;
            out.g = p;
            out.b = q;
            break;
    }
    return out;
}
void take_screenshot(sf::RenderWindow* window, std::string filename)
{
    sf::Texture texture;
    texture.create(window->getSize().x, window->getSize().y);
    texture.update(*window);
    if (texture.copyToImage().saveToFile(filename))
    {
        std::cout << "screenshot saved to " << filename << std::endl;
    }
    else
    {
        // Error handling
    }
}
