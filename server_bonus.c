/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fasharif fasharif@student.42abudhabi.ae    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 03:06:20 by fasharif          #+#    #+#             */
/*   Updated: 2023/04/22 03:06:20 by fasharif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(int number)
{
	if (number >= 0 && number <= 9)
		ft_putchar(number + '0');
	else
	{
		ft_putnbr(number / 10);
		ft_putnbr(number % 10);
	}
}

void	to_text(char *bin)
{
	char	c;
	int		i;
	int		base;
	int		res;

	i = 7;
	base = 1;
	res = 0;
	while (i >= 0)
	{
		if (bin[i] == '1')
			res += base;
		base *= 2;
		i--;
	}
	c = (char)res;
	write(1, &c, 1);
}

void	sig_h(int sig, siginfo_t *info, void *c)
{
	static char	binary[8] = {0};
	static int	i = 0;
	int			j;

	c++;
	if (sig == SIGUSR2)
		binary[(i % 8)] = '1';
	else if (sig == SIGUSR1)
		binary[(i % 8)] = '0';
	if (i && (i + 1) % 8 == 0)
	{
		j = -1;
		while (++j < 8)
			if (binary[j] == '1')
				break ;
		if (j == 8)
			kill(info->si_pid, SIGUSR1);
		else
			to_text(binary);
	}
	i++;
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = sig_h;
	ft_putnbr(getpid());
	write(1, "\n", 1);
	while (1)
	{
		sigaction(SIGUSR1, &sa, 0);
		sigaction(SIGUSR2, &sa, 0);
		pause();
	}
	return (0);
}
