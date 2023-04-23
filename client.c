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
#include <stdlib.h>
#include <signal.h>

int	ft_atoi(char *str)
{
	int	res;

	res = 0;
	while (*str)
	{
		res = res * 10 + ((int)*str) - 48;
		str++;
	}
	return (res);
}

int	main(int argc, char **argv)
{
	int	pid_p;
	int	i;
	int	j;

	if (argc < 3 || argc > 3)
		exit (1);
	pid_p = ft_atoi(argv[1]);
	i = 0;
	while (argv[2][i])
	{
		j = sizeof(argv[2][i]) * 8 - 1;
		while (j >= 0)
		{
			if (argv[2][i] & 1 << j)
				kill(pid_p, SIGUSR2);
			else
				kill(pid_p, SIGUSR1);
			j--;
			usleep(1000);
		}
		++i;
	}
	return (0);
}
