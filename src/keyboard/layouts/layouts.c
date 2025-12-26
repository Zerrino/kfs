/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layouts.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-t@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 15:50:00 by rperez-t          #+#    #+#             */
/*   Updated: 2025/12/24 15:44:07 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/kernel.h"

extern const t_key_layout g_layout_qwerty_us;
extern const t_key_layout g_layout_azerty_be;

void	keyboard_layouts_init(void)
{
	kernel.keyboard_layout_count = 0;
	kernel.keyboard_layouts[kernel.keyboard_layout_count++] = &g_layout_qwerty_us;
	kernel.keyboard_layouts[kernel.keyboard_layout_count++] = &g_layout_azerty_be;
	kernel.keyboard_layout = &g_layout_qwerty_us;
}

const t_key_layout	*keyboard_find_layout(const char *name)
{
	if (name == NULL)
		return (NULL);
	for (size_t i = 0; i < kernel.keyboard_layout_count; i++)
	{
		if (ft_strcmp(name, kernel.keyboard_layouts[i]->name) == 0)
			return (kernel.keyboard_layouts[i]);
	}
	return (NULL);
}

bool	keyboard_set_layout(const char *name)
{
	const t_key_layout *layout;

	layout = keyboard_find_layout(name);
	if (layout == NULL)
		return (false);
	kernel.keyboard_layout = layout;
	terminal_writestring("Keyboard layout set to ");
	terminal_writestring(layout->name);
	terminal_writestring("\n");
	return (true);
}

void	keyboard_list_layouts(void)
{
	terminal_writestring("Available layouts: ");
	for (size_t i = 0; i < kernel.keyboard_layout_count; i++)
	{
		terminal_writestring(kernel.keyboard_layouts[i]->name);
		if (i + 1 < kernel.keyboard_layout_count)
			terminal_writestring(", ");
	}
	terminal_writestring("\nCurrent layout: ");
	if (kernel.keyboard_layout)
		terminal_writestring(kernel.keyboard_layout->name);
	else
		terminal_writestring("none");
	terminal_writestring("\n");
}
