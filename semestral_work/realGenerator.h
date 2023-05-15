#pragma once
#include <random>

class RealGenerator
{
public:
	RealGenerator(float minValue, float maxValue);
	float getNext();
private:

	float minValue;
	float maxValue;

	std::mt19937 gen;
	std::uniform_real_distribution<float> dis;
};

