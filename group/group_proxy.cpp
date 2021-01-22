#include "group_proxy.hpp"

using namespace soften;

void GroupProxy::addShall(const ShellDef& def) {
    auto& group = _groups[_idx];

    group.shell.edges.resize(def.edges.size());
	for (ulong i = 0; i != group.shell.edges.size(); ++i) {
        group.shell.edges[i].i = def.edges[i].i;
        group.shell.edges[i].j = def.edges[i].j;
        group.shell.edges[i].friction = def.edges[i].friction;

        group.points[group.shell.edges[i].j].flags |= PointFlags::SHELL;
	}
}