#include "group_proxy.hpp"

using namespace soften;

void GroupProxy::addShall(const ShellDef& def) {
	_group->shell.edges.resize(def.edges.size());
	for (ulong i = 0; i != _group->shell.edges.size(); ++i) {
		_group->shell.edges[i].i = def.edges[i].i;
		_group->shell.edges[i].j = def.edges[i].j;

		_group->points[_group->shell.edges[i].j].flags |= PointFlags::INTERACTIVE;
	}
}