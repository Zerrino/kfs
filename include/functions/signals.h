/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 13:10:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 13:10:00 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "../core/libs.h"
# include "../core/structs.h"

void	signal_init(void);
void	signal_register(uint16_t signal, SignalHandler handler);
int		signal_schedule(uint16_t signal, const t_registers *regs);
void	signal_dispatch(void);

#endif
