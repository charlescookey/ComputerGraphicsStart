#pragma once

class Vec2
{
public:
    float x = 0.f;
    float y = 0.f;

    //default constructor
    Vec2();

    //cretes a vec2 where x and y = a
    Vec2(float a);

    //creates vec2 
    Vec2(float _x, float _y);

    ~Vec2();

    //returns the lenght of the Vector
    float lenght();

    //Prints te vector
    void printXY();

    //retunrs the sum of two vectors
    Vec2 operator+ (const Vec2& addend);

    //returns the difference of two vectors
    Vec2 operator- (const Vec2& subend);
};