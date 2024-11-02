/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aevstign <aevstign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 19:10:28 by aevstign          #+#    #+#             */
/*   Updated: 2023/10/22 19:34:04 by aevstign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	int		i;
	char	*temp;

	if (!dst || !src)
		return (NULL);
	temp = (char *)malloc(n * sizeof(char));
	i = (int)(n - 1);
	while (i >= 0)
	{
		*(temp + i) = *(char *)(src + i);
		i--;
	}
	i = (int)(n - 1);
	while (i >= 0)
	{
		*(char *)(dst + i) = *(temp + i);
		i--;
	}
	return ((void *)dst);
}

// int main () {
//    char dest[] = "oldstring";
//    const char src[]  = "string";

//    printf("Before memmove dest = %s, src = %s\n", dest, src);
//    ft_memmove(dest, src, 2);
//    printf("After memmove dest = %s, src = %s\n", dest, src);

//    return(0);
// }