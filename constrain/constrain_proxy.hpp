#pragma once

#include "../point/point_id.hpp"
#include "../types.hpp"
#include "constrain.hpp"

namespace soften {

class ConstrainProxy final {
	friend class Group;
	friend class GroupProxy;

  public:
	FlagsStorage flags() const;
	void flags(FlagsStorage flags);

	void i(PointIdx i) { const_cast<Constrain&>(_constrain).i = i.idx; }
	PointIdx i() const { return PointIdx(_constrain.i); }

	void j(PointIdx j) { const_cast<Constrain&>(_constrain).j = j.idx; }
	PointIdx j() const { return PointIdx(_constrain.j); }

  private:
	explicit ConstrainProxy(const Constrain& Constrain): _constrain(Constrain) {}
	ConstrainProxy() = delete;
	ConstrainProxy(const ConstrainProxy&) = delete;

  private:
	const Constrain& _constrain;
};

inline FlagsStorage ConstrainProxy::flags() const {
	return _constrain.flags;
}

inline void ConstrainProxy::flags(FlagsStorage flags) {
	const_cast<Constrain&>(_constrain).flags = flags;
}

} // namespace soften