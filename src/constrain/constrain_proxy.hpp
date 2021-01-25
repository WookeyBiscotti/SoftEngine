#pragma once

#include "../utils/types.hpp"
#include "constrain.hpp"

namespace soften {

class ConstrainProxy final {
	friend class Group;
	friend class GroupProxy;

  public:
	FlagsStorage flags() const;
	ConstrainProxy& flags(FlagsStorage flags);

	void i(Id i) { _constrain->i = i; }
	Id i() const { return Id(_constrain->i); }

	void j(Id j) { _constrain->j = j; }
	Id j() const { return Id(_constrain->j); }

	float hookCoeff() const;
	ConstrainProxy& hookCoeff(float coeff);

	float distance() const;
	ConstrainProxy& distance(float distance);

	float breakDistance() const;
	ConstrainProxy& breakDistance(float distance);

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

inline float ConstrainProxy::breakDistance() const {
	return _constrain->breakDistance;
}

inline ConstrainProxy& ConstrainProxy::breakDistance(float distance) {
	_constrain->breakDistance = distance;
    return *this;
}

} // namespace soften