#pragma once

typedef struct bar bar;
struct bar2;

struct foo1 {
	bar* bar;
	struct bar2* bar2;
};
