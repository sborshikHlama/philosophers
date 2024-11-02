/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsenii <arsenii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:28:55 by aevstign          #+#    #+#             */
/*   Updated: 2023/10/26 00:30:46 by arsenii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str1, const char *str2, size_t n)
{
	size_t	len;
	size_t	i;
	char	c;

	c = *str2;
	if (c == '\0')
		return ((char *)str1);
	len = ft_strlen(str2);
	while (len <= n--)
	{
		if (*str1 == c)
		{
			i = 1;
			while (i)
			{
				if (i == len)
					return ((char *)str1);
				if (str1[i] != str2[i])
					break ;
				i++;
			}
		}
		str1++;
	}
	return (NULL);
}

// int	main(void)
// {
// 	const char *largestring = "Foo Bar Baz";
//     const char *smallstring = "Bar";
//     char *ptr;

//     ptr = ft_strnstr(largestring, smallstring, 4);
// 	printf("%s", ptr);
// }
