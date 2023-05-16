#include "realGenerator.h"

/**
 * \brief Constructor for RealGenerator
 *
 * This constructor initializes the random number generator with a uniform distribution
 * between the specified minimum and maximum values.
 *
 * \param minValue The minimum value in the range of generated numbers
 * \param maxValue The maximum value in the range of generated numbers
 */
RealGenerator::RealGenerator(float minValue, float maxValue)
	: minValue(minValue), maxValue(maxValue), gen(std::random_device()()), dis(minValue, maxValue)
{}

/**
 * \brief Generate the next random real number
 *
 * This function generates and returns the next random real number in the range specified
 * during the construction of the object.
 *
 * \return The next random real number in the specified range
 */
float RealGenerator::getNext()
{
	return dis(gen);
}