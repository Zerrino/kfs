/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:44:42 by zerrino           #+#    #+#             */
/*   Updated: 2026/05/14 01:48:18 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/kernel.h"

void	timer(t_registers* regs)
{
	(void)regs;
	kernel.tick++;
	signal_dispatch_next();
}
