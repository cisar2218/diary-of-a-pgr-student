#include "utils.h"
#include "movingObject.h"

void printErrIfNotSatisfied(const bool condition, const std::string& errMessage) {
	if (!condition) {
		cerr << errMessage << endl;
	}
}