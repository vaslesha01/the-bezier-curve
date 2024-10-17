#include <SFML/Graphics.hpp>

using namespace sf;

Vector2f bezier(float t, const Vector2f p0, const Vector2f p1, const Vector2f p2, const Vector2f p3) {
                                            // t is the parameter of the curve function, which changes in [0;1]
    Vector2f point = (1-t)*(1-t)*(1-t) *p0; // (1-t)^3 * P0
    point += 3 * (1-t)*(1-t) * t * p1;      // 3 * (1-t)^2 * t * P1
    point += 3 * (1-t) * t*t * p2;          // 3 * (1-t) * t^2 * P2
    point += t*t*t * p3;                    // t^3 * P3

    return point;
}

int main() {

    RenderWindow window(VideoMode(1080,720), "The Bezier Curve");

    Vector2f p0(100, 500);
    Vector2f p1(200, 100);
    Vector2f p2(600, 100);
    Vector2f p3(700, 500);

    Vector2f dragOffset;

    CircleShape p00(3.f);
    p00.setFillColor(Color::Cyan);
    p00.setPosition(97, 500);

    CircleShape p11(3.f);
    p11.setFillColor(Color::Blue);
    p11.setPosition(197, 100);

    CircleShape p22(3.f);
    p22.setFillColor(Color::Blue);
    p22.setPosition(597, 100);

    CircleShape p33(3.f);
    p33.setFillColor(Color::Cyan);
    p33.setPosition(697, 500);

    Vertex line1[] = {
        Vertex(p0, Color(255,255,255,117)),
        Vertex(p1, Color(255,255,255,117))
    };

    Vertex line2[] = {
        Vertex(p2, Color(255,255,255,117)),
        Vertex(p3, Color(255,255,255,117))
    };

    VertexArray curve(PrimitiveType::LineStrip);

    bool draggingStart0 = false;
    bool draggingEnd0 = false;

    bool draggingStart1 = false;
    bool draggingEnd1 = false;

    int f = 5; // f is the number of pixels to track the transfer

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    float mBx = event.mouseButton.x;
                    float mBy = event.mouseButton.y;
                    if (hypot(mBx - p0.x, mBy - p0.y) < f) { 
                        draggingStart0 = true;
                    }
                    else if (hypot(mBx - p1.x, mBy - p1.y) < f) {
                        draggingEnd0 = true;
                    }
                }
            }

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    float mBx = event.mouseButton.x;
                    float mBy = event.mouseButton.y;
                    if (hypot(mBx - p2.x, mBy - p2.y) < f) {
                        draggingStart1 = true;
                    }
                    else if (hypot(mBx - p3.x, mBy - p3.y) < f) {
                        draggingEnd1 = true;
                    }
                }
            }

            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    draggingStart0 = false;
                    draggingEnd0 = false;
                }
            }
            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    draggingStart1 = false;
                    draggingEnd1 = false;
                }
            }
            if (event.type == sf::Event::MouseMoved) {
                if (draggingStart0) {
                    float dx = event.mouseMove.x - p0.x;
                    float dy = event.mouseMove.y - p0.y;
                    p0.x += dx;
                    p1.x += dx;
                    p0.y += dy;
                    p1.y += dy;
                    p11.setPosition(p1.x-3, p1.y);
                }
                if (draggingEnd1) {
                    float dx = event.mouseMove.x - p3.x;
                    float dy = event.mouseMove.y - p3.y;
                    p2.x += dx;
                    p3.x += dx;
                    p2.y += dy;
                    p3.y += dy;
                    p22.setPosition(p2.x-3, p2.y);
                }
            }
        }

        if (draggingStart0) {
            p0 = window.mapPixelToCoords(Mouse::getPosition(window));
            p00.setPosition(p0.x-3, p0.y);
        }
        else if (draggingEnd0) {
            p1 = window.mapPixelToCoords(Mouse::getPosition(window));
            p11.setPosition(p1.x-3, p1.y);
        }
        if (draggingStart1) {
            p2 = window.mapPixelToCoords(Mouse::getPosition(window));
            p22.setPosition(p2.x-3, p2.y);
        }
        else if (draggingEnd1) {
            p3 = window.mapPixelToCoords(Mouse::getPosition(window));
            p33.setPosition(p3.x-3, p3.y);
        }
        
        if (draggingStart0 || draggingEnd0 || draggingStart1 || draggingEnd1) {
            curve = {};
        }

        for (float t = 0; t <= 1; t += 0.001) {
            curve.append(Vertex(bezier(t, p0, p1, p2, p3), Color::Cyan));
        }

        line1[0].position = p0;
        line1[1].position = p1;

        line2[0].position = p2;
        line2[1].position = p3;

        window.clear();
        window.draw(curve);
        window.draw(p00);
        window.draw(p11);
        window.draw(p22);
        window.draw(p33);
        window.draw(line1, 2, Lines);
        window.draw(line2, 2, Lines);
        
        window.display();

    }

    return 0;
}