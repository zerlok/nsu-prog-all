#include "mod.h"


/* ---------- DEBUG FUNCTIONS ---------- */
#ifdef DEBUG

void print_flag(char flag[])
/*	
Function simply prints a flag.

Input:
	flag - (*char) the flag.
*/
{
	printf("flag : '%s'\n", flag);
}


void print_func_name(const char name[])
{
	printf("\n>-----RUN----->\t'%s' function:\n", name);
}

void print_end_func(const char name[])
{
	printf("\n<-----END-----<\t'%s' function:\n", name);
}

void print_decor()
/*
Prints decorating symbols in output.
*/
{
	printf("+++++++++++++++++++++++++\n");
}


void decorate(char str[])
/*
Prints char symbols in decorating view.
*/
{
	print_decor();
	printf("+\n");
	printf("+ %10s\n", str);
	printf("+\n");
	print_decor();
	// printf("\n");
}


void run_test(char *file_name)
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif

	print_bin_file(file_name);
	
	#ifdef DEBUG
		print_end_func(__func__);
	#endif
}


#endif


/* ---------- ACCESSORY FUNCTIONS ---------- */

void print_doc()
/*	Function shows documentation of archivator.	*/
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif
	
	printf("%s - %s\n", ARC_NAME, VERSION);
	printf("You're reading a documentation about %s\n", ARC_NAME);
	printf("Archive tag is %s\n", ARC_FILE_TAG);
	printf("Using: $ harc <flag> <file> <archive>\n");
	printf("\n");
	print_flag_help();
	printf("\n");
	printf("Examples:\n");
	printf("\t$ harc -a file.txt arch.trar (append arch.trar with file.txt file)\n");
	printf("\t$ harc -l arch.trar (show the list of arch.trar's files)\n");
	
	#ifdef DEBUG
		print_end_func(__func__);
	#endif
}


void print_flag_help()
/*	Function shows help about flags.	*/
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif
	
	printf("You can use this flags:\n");
	printf("   %s\t\t-\tto add file to archive. \t\t Usage: %s <file> <archive>\n", FLAG_ADD, FLAG_ADD);
	printf("   %s\t\t-\tto extract file from archive. \t\t Usage: %s <file> <archive>\n", FLAG_EXTR, FLAG_EXTR);
	printf("   %s\t\t-\tto get a list of files in archive. \t Usage: %s <archive>\n", FLAG_LIST, FLAG_LIST);
	printf("   %s\t\t-\tto get a control sum of archive. \t Usage: %s <archive>\n", FLAG_TEST, FLAG_TEST);
	printf("   %s\t-\tto show this help documentation.\n", FLAG_HELP);
	
	#ifdef DEBUG
		print_end_func(__func__);
	#endif
}


void print_err(int err_code)
/*
Function prints error's messages.

Input:
	err_code - (int) the number of error code.
*/
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif

	switch (err_code)
	{
		case ERR_NO_FLAG_ARG:
			printf("Please write arguments after flags!\n");
			print_flag_help();
			break;
		case ERR_NO_FLAGS:
			printf("Please write flags!\n");
			print_flag_help();
			break;
		case ERR_UNKNOWN_FLAG:
			printf("Please read which flags can be used in help.\n");
			print_flag_help();
			break;
		case ERR_NOT_ARCH_FILE:
			printf("Unknown type of archive file!\n");
			printf("It should be a %s type of archive.\n", ARC_FILE_TAG);
			break;
		default:
		{
			if (err_code > ERROR_CODE)
			{
				printf("Unknown error (code %d)\n", err_code);
			}
			else
			{
				printf("No errors.\n");
			}
			break;
		}
	}

	#ifdef DEBUG
		print_end_func(__func__);
	#endif
}


int is_arch_file(char arg[])
/*
If file is an archive file returns 0.
Otherwise returns 1.
Input:
	arg - (*char) the name of file.
*/
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif
	
	if (!strstr(arg, ARC_FILE_TAG)) // If not archive file
	{
		return 1;
	}

	#ifdef DEBUG
		print_end_func(__func__);
	#endif

	return 0;
}


int read_flags(int argc, char *argv[])
/*	
Function reads flags from argv and returns zero.
If there are some errors, returns error code.

Input:
	argc - (int) a length of argv,
	argv - (**char) a list of arguments.
*/
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif

	int str_indx, arg_indx = 1;
	
	while (arg_indx < argc)
	{
		if (!strcmp(argv[arg_indx], FLAG_ADD))
		{
			arg_indx++;
			if(arg_indx + 1 < argc)
			{
				if (!is_arch_file(argv[arg_indx + 1]))
				{
					return APPEND_CODE + arg_indx + 1;
				}
				else
				{
					return ERR_NOT_ARCH_FILE + arg_indx + 1;
				}
			}
			else
			{
				return ERR_NO_FLAG_ARG + arg_indx - 1;
			}
		}
		else if (!strcmp(argv[arg_indx], FLAG_EXTR))
		{
			arg_indx++;
			if(arg_indx + 1 < argc)
			{
				if (!is_arch_file(argv[arg_indx + 1]))
				{
					return EXTRACT_CODE + arg_indx + 1;
				}
				else
				{
					return ERR_NOT_ARCH_FILE + arg_indx + 1;
				}
			}
			else
			{
				return ERR_NO_FLAG_ARG + arg_indx - 1;
			}
		}
		else if (!strcmp(argv[arg_indx], FLAG_LIST))
		{
			if(arg_indx + 1 < argc)
			{
				if (!is_arch_file(argv[arg_indx + 1]))
				{
					return LIST_CODE + arg_indx + 1;
				}
				else
				{
					return ERR_NOT_ARCH_FILE + arg_indx + 1;
				}
			}
			else
			{
				return ERR_NO_FLAG_ARG + arg_indx;
			}
		}
		else if (!strcmp(argv[arg_indx], FLAG_TEST))
		{
			if(arg_indx + 1 < argc)
			{
				if (!is_arch_file(argv[arg_indx + 1]))
				{
					return TESTSUM_CODE + arg_indx + 1;
				}
				else
				{
					return ERR_NOT_ARCH_FILE + arg_indx + 1;
				}
			}
			else
			{
				return ERR_NO_FLAG_ARG;
			}
		}
		else if (!strcmp(argv[arg_indx], FLAG_HELP))
		{
			return HELP_CODE;
		}

		#ifdef DEBUG
			/* FOR DEBUG ONLY! */
			else if (!strcmp(argv[arg_indx], TESTMODE_FLAG))
			{
				return TESTMODE_CODE + arg_indx + 1;
			}
		#endif

		else if (strstr(argv[arg_indx], "-"))
		{
			// print_flag(argv[arg_indx]);
			return ERR_UNKNOWN_FLAG;
		}
		else
		{
			printf("Last arg: '%s'\n", argv[arg_indx]);
		}

		arg_indx++;
	}
	return ERR_NO_FLAGS + arg_indx;
}


/* ---------- ARCHIVE FUNCTIONS ---------- */

int get_hash(unsigned char symbol)
{

	int hash;

	if (!symbol)
	{
		// printf("sym: '%x'\n", symbol);
		// printf("hsh: '%d'\n", 0);
		return 0;
	}

	hash = (
		abs(((int)symbol * 'A')
		 + abs((symbol - 'a') * (symbol - 'z'))
	 	 - 'Z'
	 	)) % 10000;

	if ((int)(hash / 1000) == 0)
	{
		hash += 1000 * ((int)symbol % 10);
	}

	// printf("sym: '%x'\n", symbol);
	// printf("hsh: '%d'\n", hash);
	return hash;
}


void print_bin_file(char *file_name)
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif

	FILE *file;
	unsigned char str;
	int i = 1;
	struct List *lst = init_list();
	

	if((file = fopen(file_name, "rb")) == NULL)
	{
		printf("Error while file opening!\n");
		exit(1);
	}

	int h;
	printf("The content of '%s' file:\n", file_name);
	while (fread(&str, sizeof(str), 1, file))
	{
		// printf("%02x", str);
		h = get_hash(str);
		
		printf("sym: '%x'\n", str);
		printf("hsh: '%d'\n", h);

		insert(&lst, get_hash(str), str);
		if (!(i % 2)) printf(" ");
		if (!(i % 16)) printf("\n");
		i++;
	}

	fclose(file);
	
	#ifdef DEBUG
		print_end_func(__func__);
	#endif
}


int *get_symbols_table(char *seq)
/*
...
*/
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif



	#ifdef DEBUG
		print_end_func(__func__);
	#endif

	return NULL;
}


int *get_bin_tree(int *symbols_table)
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif



	#ifdef DEBUG
		print_end_func(__func__);
	#endif

	return NULL;
}


char *encode_symbols(int *tree)
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif

	// get_symbols_table(file);
	// get_bin_tree(symbols_table);

	// ...

	#ifdef DEBUG
		print_end_func(__func__);
	#endif

	return NULL;
}


int *decode_symbols(char *symbols)
{
	#ifdef DEBUG
		print_func_name(__func__);
	#endif

	// read_bin_tree(archive); ???
	
	// ...

	#ifdef DEBUG
		print_end_func(__func__);
	#endif

	return NULL;
}


