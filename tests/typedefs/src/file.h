// typedef that matches spelling of a NDS Watcher primitive should not show up in typedefs.txt
typedef int fx32;

typedef struct foo {
	fx32 a;
} foo_t;

// Identical typedef name should not show up in typedefs.txt
typedef struct bar {
	foo_t a;
} bar;
