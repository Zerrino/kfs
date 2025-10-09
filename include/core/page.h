/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexafer <alexafer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 11:00:36 by alexafer          #+#    #+#             */
/*   Updated: 2025/10/09 11:51:26 by alexafer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PAGE_H
# define PAGE_H

# include <stdint.h>

/* ────────────       PAGING       ──────────── */

# define PAGE_DIRECTORY_MAX 1024
# define PAGE_TABLES_MAX 1024*1024

typedef enum {
    PD_FLAG_PRESENT  = (1 << 0), // if set mean in physical mem
    PD_FLAG_R_AND_W  = (1 << 1), // if set R&W if not RO
    PD_FLAG_USER     = (1 << 2), // if set user if not superuser
    PD_FLAG_WRITE_T  = (1 << 3), // We don't really care
    PD_FLAG_CACHE    = (1 << 4), // We don't really care
    PD_FLAG_ACCESSED = (1 << 5), // if page got accessed is set to  1 (automatic)
} PD_FLAGS;

typedef enum {
    PT_FLAG_PRESENT  = (1 << 0), // if set mean in physical mem
    PT_FLAG_R_AND_W  = (1 << 1), // if set R&W if not RO
    PT_FLAG_USER     = (1 << 2), // if set user if not superuser
    PT_FLAG_WRITE_T  = (1 << 3), // We don't really care
    PT_FLAG_CACHE    = (1 << 4), // We don't really care
    PT_FLAG_ACCESSED = (1 << 5), // if page got accessed is set to  1 (automatic)
    PT_FLAG_DIRTY    = (1 << 6), // if page got writen   is set to  1 (automatic) 
} PT_FLAGS;

typedef struct s_page_dir
{
    uint32_t    infos;
} t_page_dir;


#endif