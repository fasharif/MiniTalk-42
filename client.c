/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fasharif <fasharif@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 03:06:46 by fasharif          #+#    #+#             */
/*   Updated: 2023/04/22 03:06:46 by fasharif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
#include <signal.h>
#include <limits.h>

static void	ft_putstr_fd(char *s, int fd)
{
	while (*s)
		write(fd, s++, 1);
}

/*
** Only a positive decimal PID is accepted: kill() treats 0 as "my whole
** process group" and -1 as "every process I may signal".
*/
static int	parse_pid(char *str, pid_t *pid)
{
	long	res;

	res = 0;
	if (*str == '\0')
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		if (res > INT_MAX)
			return (-1);
		str++;
	}
	if (*str != '\0' || res == 0)
		return (-1);
	*pid = (pid_t)res;
	return (0);
}

static void	send_char(pid_t pid, unsigned char c)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		if (c & (1 << bit))
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		usleep(1000);
		bit--;
	}
}

int	main(int argc, char **argv)
{
	pid_t	pid;
	int		i;

	if (argc != 3 || parse_pid(argv[1], &pid) != 0)
	{
		ft_putstr_fd("Usage: ./client <server PID> <message>\n", 2);
		return (1);
	}
	if (kill(pid, 0) == -1)
	{
		ft_putstr_fd("client: cannot signal that PID (not running?)\n", 2);
		return (1);
	}
	i = 0;
	while (argv[2][i])
		send_char(pid, (unsigned char)argv[2][i++]);
	return (0);
}
