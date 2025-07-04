/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rperez-t <rperez-tstudent.s19.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 11:48:23 by alexafer          #+#    #+#             */
/*   Updated: 2025/07/04 15:34:46 by rperez-t         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
# define KERNEL_H

/* ──────────── Master Include System ──────────── */
/* Import all organized headers in dependency order */

#include "core/libs.h"           /* Standard C libraries and cross-compiler validation */
#include "core/defines.h"        /* All constants and defines */
#include "core/enums.h"          /* All enumerations */
#include "core/structs.h"        /* All structure definitions */
#include "core/globals.h"        /* All global variables */

/* Function declarations - all modules consolidated */
#include "core/functions.h"          /* All function declarations by module */

#endif /* KERNEL_H */
