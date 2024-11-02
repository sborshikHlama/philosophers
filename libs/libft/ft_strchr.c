/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arsenii <arsenii@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:55:34 by aevstign          #+#    #+#             */
/*   Updated: 2023/10/26 12:57:06 by arsenii          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	while (*str != '\0')
	{
		if (*str == c)
			return ((char *)str);
		str++;
	}
	if ((char)c == '\0')
		return ((char *)str);
	return (NULL);
}

// int main () {
// 	char str[] = "https://www.tutorialspoint.com";
//    char ch = '.';
//    char *ret;
//    ret = ft_strchr(str, ch);
//    printf("%s", ret);
//    return(0);
// }
