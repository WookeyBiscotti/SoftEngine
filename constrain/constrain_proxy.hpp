#pragma once

#include "../point/point_id.hpp"
#include "../utils/types.hpp"
#include "constrain.hpp"

namespace soften {

class ConstrainProxy final {
	friend class Group;
	friend class GroupProxy;

  public:
	FlagsStorage flags() const;
	ConstrainProxy& flags(FlagsStorage flags);

	void i(PointIdx i) { _constrain->i = i.idx; }
	PointIdx i() const { return PointIdx(_constrain->i); }

	void j(PointIdx j) { _constrain->j = j.idx; }
	PointIdx j() const { return PointIdx(_constrain->j); }

	float hookCoeff() const;
	ConstrainProxy& hookCoeff(float coeff);

	float distance() const;
	ConstrainProxy& distance(float distance);

  private:
	explicit ConstrainProxy(const Constrain* constrain): _constrain(const_cast<Constrain*>(constrain)) {}
	ConstrainProxy() = delete;
	ConstrainProxy(const ConstrainProxy&) = delete;

  private:
	Constrain* _constrain;
};

inline FlagsStorage ConstrainProxy::flags() const {
	return _constrain->flags;
}

inline ConstrainProxy& ConstrainProxy::flags(FlagsStorage flags) {
	_constrain->flags = flags;
	return *this;
}

inline float ConstrainProxy::hookCoeff() const {
	return _constrain->hookCoeff;
}

inline ConstrainProxy& ConstrainProxy::hookCoeff(float coeff) {
	_constrain->hookCoeff = coeff;
	return *this;
}

inline float ConstrainProxy::distance() const {
	return _constrain->distance;
}

inline ConstrainProxy& ConstrainProxy::distance(float distance) {
	_constrain->distance = distance;
	return *this;
}

} // namespace soften