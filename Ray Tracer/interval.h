#ifndef INTERVAL_H
#define INTERVAL_H
//interval class to manage real-valued intervals with min and max

class interval {
public:
	double min, max;

	interval() : min(+infinity), max(-infinity) {} // treat default interval as empty

	interval(double min, double max) : min(min), max(max) {}
	//returns the length of the interval
	double size() const {
		return max - min;
	}
	//Does x lie in the interval (inclusive interval).
	bool contains(double x) const {
		return min <= x && x <= max;
	}
	//Does x lie in the interval (open, non-inclusive interval).
	bool surrounds(double x) const {
		return min < x && x < max;
	}
	static const interval empty, universe;
};
//an interval which contains nothing
const interval interval::empty = interval(+infinity, -infinity);
//interval which contains everything
const interval interval::universe = interval(-infinity, +infinity);

#endif