static void __dtors(void);
static void __ctors (void);

typedef void (*func_ptr) (void);

void __init (void);
void __fini (void);

extern func_ptr __CTOR_LIST__[];
extern func_ptr __CTOR_END__[];
extern func_ptr __DTOR_LIST__[];
extern func_ptr __DTOR_END__[];

static void	__dtors (void);
static void	__ctors (void);

static void
__dtors()
{
	void (**p)(void);

	for(p=__DTOR_END__; p!=__DTOR_LIST__;)
		(*--p)();

}

static void
__ctors()
{
	void (**p)(void);

	for(p=__CTOR_LIST__; p!=__CTOR_END__;p++)
		(*p)();
}

void
__init()
{
	static int initialized = 0;

	/*
	 * Call global constructors.
	 * Arrange to call global destructors at exit.
	 */
	if (!initialized) {
		initialized = 1;
		__ctors();
	}
}

void
__fini()
{
	/*
	 * Call global destructors.
	 */
	__dtors();
}

