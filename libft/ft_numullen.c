/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numullen.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 12:46:35 by mlu               #+#    #+#             */
/*   Updated: 2017/06/06 12:46:38 by mlu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned long long	ft_numullen(unsigned long long n)
{
	unsigned long long	i;

	i = 1;
	while (n /= 10)
		i++;
	return (i);
}