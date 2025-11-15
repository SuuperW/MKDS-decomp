struct foo {
	struct {
		int a;
		int b;
	} a;

	struct {
		int c;
		int d;
	}; // no name: should be lifted (field c is directly on foo)

	union {
		int e;
		struct {
			short f;
			short g;
		}; // no name, but can't be lifted because it's inside a union
	};
};
