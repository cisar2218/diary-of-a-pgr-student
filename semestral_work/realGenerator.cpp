#include "realGenerator.h"

RealGenerator::RealGenerator(float minValue, float maxValue) 
	: minValue(minValue), maxValue(maxValue), gen(std::random_device()()), dis(minValue, maxValue)
{}

float RealGenerator::getNext()
{
	return dis(gen);
}