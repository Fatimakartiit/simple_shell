#include "shell.h"

/**
 * custom_main - Entry point ofcustom shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 if success, 1 if error
 */
int custom_main(int argc, char **argv)
{
	info_t shell_info[] = { INFO_INIT };
	int fd_descriptor = 2;

	asm ("mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (fd_descriptor)
			: "r" (fd_descriptor));

	if (argc == 2)
	{
		fd_descriptor = open(argv[1], O_RDONLY);
		if (fd_descriptor == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(argv[0]);
				_eputs(": 0: Can't open ");
				_eputs(argv[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		shell_info->readfd = fd_descriptor;
	}
	populate_environment_list(shell_info);
	read_shell_history(shell_info);
	run_custom_shell(shell_info, argv);
	return (EXIT_SUCCESS);
}
