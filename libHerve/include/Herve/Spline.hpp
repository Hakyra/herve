// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_SPLINE_HPP_
#define HERVE_SPLINE_HPP_

#include <vector>
#include <cassert>

namespace Herve {

/// \brief abstract template for spline interpolation (multiple keys)
///
template <typename T_value, typename T_time>
class Spline {
protected:
	std::vector<T_time> _times;
	std::vector<T_value> _values;

public:
	/// \brief add a key to interpolate
	///
	void addKey(T_time t, const T_value & P) {
		assert(_times.empty() or t >= _times.back());
		_times.push_back(t);
		_values.push_back(P);
	}

	/// \brief get interpolated value at time t
	///
	virtual T_value getValue(T_time t) const = 0;

	/// \brief get interpolated value at time t and derivative
	///
	virtual void getValueAndDerivative(T_time t, T_value & value, T_value & derivative) const = 0;

	/// \brief get first valid time
	///
	virtual T_time getStartTime() const = 0;

	/// \brief get last valid time
	///
	virtual T_time getEndTime() const = 0;
};

/// \brief linear interpolation (multiple keys)
///
template <typename T_value, typename T_time>
class Spline1 : public Spline<T_value, T_time> {
public:
	/// \brief get interpolated value at time t
	///
	T_value getValue(T_time t) const {
		assert(t >= getStartTime());
		assert(t < getEndTime());
		int iLater = 0;
		while (iLater < this->_times.size() and this->_times[iLater] <= t) iLater++;
		if (iLater == 0) return this->_values.front();
		if (iLater == this->_times.size()) return this->_values.back();
		T_time dt = (this->_times[iLater] - this->_times[iLater-1]);
		T_time tu = (t - this->_times[iLater-1]) / dt;
		return (1-tu)*this->_values[iLater-1] + tu*this->_values[iLater];
	}

	/// \brief get interpolated value at time t and derivative
	///
	void getValueAndDerivative(T_time t, T_value & value, T_value & derivative) const {
		assert(t >= getStartTime());
		assert(t < getEndTime());
		int size = this->_times.size();
		int iLater = 0;
		while (iLater < size and this->_times[iLater] <= t) iLater++;
		if (iLater == 0) {
			value = this->_values.front();
			T_time dt = (this->_times[1] - this->_times[0]);
			T_value dP = this->_values[1] - this->_values[0];
			derivative = dP / dt;
		}
		else if (iLater == size) {
			value = this->_values.back();
			T_time dt = this->_times[size-1] - this->_times[size-2];
			T_value dP = this->_values[size-1] - this->_values[size-2];
			derivative = dP / dt;
		} else {
			T_time dt = (this->_times[iLater] - this->_times[iLater-1]);
			T_time tu = (t - this->_times[iLater-1]) / dt;
			value = (1-tu)*this->_values[iLater-1] + tu*this->_values[iLater];
			T_value dP = this->_values[iLater] - this->_values[iLater-1];
			derivative = dP / dt;
		}
	}

	/// \brief get first valid time
	///
	T_time getStartTime() const {
		assert(not this->_times.empty());
		return this->_times.front();
	}

	/// \brief get last valid time
	///
	T_time getEndTime() const {
		assert(not this->_times.empty());
		return this->_times.back();
	}
};

/// \brief cubic spline interpolation (multiple keys)
///
template <typename T_value, typename T_time>
class Spline3 : public Spline<T_value, T_time> {
public:
	/// \brief get interpolated value at time t
	///
	T_value getValue(T_time t) const {
		assert(t >= getStartTime());
		assert(t < getEndTime());

		// find i
		int i=2;
		while (this->_times[i] <= t) i++;

		// compute points and tangents
		const T_time G = (1+sqrt(5))/2;
		T_value P1 = this->_values[i-1];
		T_value P4 = this->_values[i];
		T_value R1 = G / (this->_times[i]-this->_times[i-2])*(this->_values[i]-this->_values[i-2]);
		T_value R4 = G / (this->_times[i+1]-this->_times[i-1])*(this->_values[i+1]-this->_values[i-1]);

		// compute hermite interpolation
		T_time tu = (t-this->_times[i-1]) / (this->_times[i]-this->_times[i-1]);
		T_time tu2 = tu*tu;
		T_time tu3 = tu2*tu;
		return (2*tu3 - 3*tu2 + 1) * P1
				+ (-2*tu3 + 3*tu2) * P4
				+ (tu3 - 2*tu2 + tu) * R1
				+ (tu3 - tu2) * R4;
	}

	/// \brief get interpolated value at time t and derivative
	///
	void getValueAndDerivative(T_time t, T_value & value, T_value & derivative) const {
		assert(t >= getStartTime());
		assert(t < getEndTime());

		// find i
		int i=2;
		while (this->_times[i] <= t) i++;

		// compute points and tangents
		const T_time G = (1+sqrt(5))/2;
		T_value P1 = this->_values[i-1];
		T_value P4 = this->_values[i];
		T_value R1 = G / (this->_times[i]-this->_times[i-2])*(this->_values[i]-this->_values[i-2]);
		T_value R4 = G / (this->_times[i+1]-this->_times[i-1])*(this->_values[i+1]-this->_values[i-1]);

		// compute hermite interpolation
		T_time tu = (t-this->_times[i-1]) / (this->_times[i]-this->_times[i-1]);
		T_time tu2 = tu*tu;
		T_time tu3 = tu2*tu;
		value = (2*tu3 - 3*tu2 + 1) * P1
				+ (-2*tu3 + 3*tu2) * P4
				+ (tu3 - 2*tu2 + tu) * R1
				+ (tu3 - tu2) * R4;

		derivative = (6*tu2 - 6*tu) * P1
				+ (-6*tu2 + 6*tu) * P4
				+ (3*tu2 - 4*tu) * R1
				+ (3*tu2 - 2*tu) * R4;
	}

	/// \brief get first valid time
	///
	T_time getStartTime() const {
		assert(this->_times.size() > 3);
		return this->_times[1];
	}

	/// \brief get last valid time
	///
	T_time getEndTime() const {
		assert(this->_times.size() > 3);
		return this->_times[this->_times.size()-2];
	}
};

} // namespace Herve

#endif
