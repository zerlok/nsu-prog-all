#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "class.h"


void *new_object(void *_class, ...)
{
	Class *cls = _class;
	void *ptr_obj = calloc(1, cls->size);
	(*(Class **)ptr_obj) = cls;

	if (cls->constructor == NULL)
		return ptr_obj;

	va_list args;
	va_start(args, _class);

	printf("### Constructor : %p ###\n", cls);
	ptr_obj = cls->constructor(ptr_obj, &args);

	va_end(args);
	return ptr_obj;
}


void delete_object(void *ptr_obj)
{
	if (ptr_obj == NULL)
		return;

	Class **cls = ptr_obj;

	printf("### Destructor : %p ###\n", *cls);
	if ((cls != NULL)
			&& ((*cls)->destructor != NULL))
		(*cls)->destructor(ptr_obj);

	free(ptr_obj);
}


void print(void *ptr_obj)
{
	if (ptr_obj == NULL)
		return;

	Class **cls = ptr_obj;

	if ((cls != NULL) && (*cls)->printor != NULL)
		(*cls)->printor(ptr_obj);
}
