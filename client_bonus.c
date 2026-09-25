/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fasharif <fasharif@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 03:04:40 by fasharif          #+#    #+#             */
/*   Updated: 2023/04/22 03:04:40 by fasharif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
#include <signal.h>
#include <limits.h>

static volatile sig_atomic_t	g_ack;

static int	fail(char *message)
{
	while (*message)
		write(2, message++, 1);
	return (1);
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

static void	handler(int sig)
{
	if (sig == SIGUSR1)
		g_ack = 1;
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
		usleep(100);
		bit--;
	}
}

int	main(int argc, char **argv)
{
	pid_t	pid;
	int		i;

	if (argc != 3 || parse_pid(argv[1], &pid) != 0)
		return (fail("Usage: ./client_bonus <server PID> <message>\n"));
	if (kill(pid, 0) == -1)
		return (fail("client: cannot signal that PID (not running?)\n"));
	signal(SIGUSR1, handler);
	i = 0;
	while (argv[2][i])
		send_char(pid, (unsigned char)argv[2][i++]);
	send_char(pid, '\0');
	i = 0;
	while (!g_ack && i++ < 100)
		usleep(10000);
	if (!g_ack)
		return (fail("client: no acknowledgement from the server\n"));
	write(1, "SIGNAL RECEIVED\n", 16);
	return (0);
}
