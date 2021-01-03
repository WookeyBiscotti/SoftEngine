#pragma once

#include "../types.hpp"
#include "constrain.hpp"

namespace soften {

class ConstrainProxy final {
	friend class Group;

  public:
	FlagsStorage flags() const;
	void flags(FlagsStorage flags);

  private:
	explicit ConstrainProxy(const Constrain& Constrain): _constrain(Constrain) {}
	ConstrainProxy() = delete;
	ConstrainProxy(const ConstrainProxy&) = delete;

  private:
	const Constrain& _constrain;
};

FlagsStorage ConstrainProxy::flags() const {
	return _constrain.flags;
}

void ConstrainProxy::flags(FlagsStorage flags) {
	const_cast<Constrain&>(_constrain).flags = flags;
}

} // namespace soften