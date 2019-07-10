#include "common.h"
#include "patcher.h"

#include "RouteNode.h"

WRAPPER void CRouteNode::Initialise() { EAXJMP(0x4EE470); }