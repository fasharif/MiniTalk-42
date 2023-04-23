/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fasharif fasharif@student.42abudhabi.ae    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 03:06:33 by fasharif          #+#    #+#             */
/*   Updated: 2023/04/22 03:06:33 by fasharif         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

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
	if (bin[0] == '1')
		return ;
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

void	sig_h(int sig)
{
	static char	binary[8] = {0};
	static int	i = 0;

	if (sig == SIGUSR2)
		binary[(i % 8)] = '1';
	else if (sig == SIGUSR1)
		binary[(i % 8)] = '0';
	if (i && (i + 1) % 8 == 0)
		to_text(binary);
	i++;
}

int	main(void)
{
	ft_putnbr(getpid());
	write(1, "\n", 1);
	signal(SIGUSR1, sig_h);
	signal(SIGUSR2, sig_h);
	while (1)
	{
		pause();
	}
	return (0);
}
