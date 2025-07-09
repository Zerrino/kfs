/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 17:05:27 by zerrino           #+#    #+#             */
/*   Updated: 2025/07/09 19:57:14 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/kernel.h"

void	*ft_memset(void *s, int c, size_t n)
{
		unsigned char	*ptr;

		ptr = s;
		while (n-- > 0)
				*ptr++ = (unsigned char)c;
				
		return (s);
}

void	ft_bzero(void *s, size_t n)
{
		unsigned char	*ptr;

		ptr = s;
		while (n-- > 0)
				*ptr++ = 0;
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
		const unsigned char		*ptr_src;
		unsigned char			*ptr_dst;

		if (dst == src)
				return (dst);
				
		ptr_src = src;
		ptr_dst = dst;
		while (n-- > 0)
				*ptr_dst++ = *ptr_src++;
				
		return (dst);
}

void	*ft_memmove(void *dst, const void *src, size_t n)
{
		const unsigned char		*ptr_src;
		unsigned char			*ptr_dst;

		if (dst == src)
				return (dst);
				
		ptr_src = src;
		ptr_dst = dst;
		if (ptr_dst < ptr_src)
				while (n-- > 0)
						*ptr_dst++ = *ptr_src++;
		else
		{
				ptr_src += n - 1;
				ptr_dst += n - 1;
				while (n-- > 0)
						*ptr_dst-- = *ptr_src--;
		}
		
		return (dst);
}

void	*ft_memchr(const void *s, int c, size_t n)
{
		const unsigned char		*ptr;

		ptr = s;
		while (n-- > 0)
		{
				if (*ptr == (unsigned char)c)
						return ((void *)ptr);
						
				ptr++;
		}
		
		return (0);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
		const unsigned char		*ptr_s1;
		const unsigned char		*ptr_s2;

		ptr_s1 = s1;
		ptr_s2 = s2;
		while (n-- > 0)
		{
				if (*ptr_s1 - *ptr_s2)
						return (*ptr_s1 - *ptr_s2);
						
				ptr_s1++;
				ptr_s2++;
		}
		
		return (0);
}

int ft_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
