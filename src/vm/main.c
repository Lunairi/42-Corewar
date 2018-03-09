/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlu <mlu@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 20:59:44 by mlu               #+#    #+#             */
/*   Updated: 2018/03/08 18:20:52 by ihodge           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"

void	error(void)
{
	ft_printf("YOU DUN GOOFED\n");
	exit(0);
}

void	write_info(t_io *io, int fd, int *i, unsigned char *test)
{
	int	j;

	if (!read_input(fd, io))
		error();
	j = -1;
	while (++j < io->head.prog_size)
	{
		test[*i] = io->body[j];
		*i = *i + 1;
	}
}

void	print_core(unsigned char *test, int i)
{
	while (++i < 4096)
	{
		if (i % 64 == 0)
			ft_printf("\n");
		if (test[i] < 16 && test[i] >= 0)
			ft_printf("0");
		ft_printf("%hhx ", test[i]);
	}
}

int		blank_pos(char **av)
{
	int	i;

	i = 0;
	while (av[i] && i < 4)
		++i;
	return (i);
}

int		get_n_players(int ac, char **av, t_vm *vm, int n_start)
{
	int	n;
	int	i;

	i = -1;
	while (++i < 4)
		vm->players[i] = 0;
	i = 0;
	while (++n_start < ac)
	{
		if (!ft_strcmp(av[n_start], "-n"))
		{
			if (ft_general_validate("%d", av[n_start + 1])
					&& ft_atoi(av[n_start + 1]) < 4)
				vm->players[ft_atoi(av[n_start + 1])] = av[n_start + 2];
			else
				error();
			n_start += 2;
		}
		else
			vm->players[blank_pos(vm->players)] = av[n_start];
		++n;
		++i;
	}
	return (n);
}

void 	init_players(int ac, char **av, t_vm *vm)
{
	int	i;
	int	n_start;

	n_start = 0;
	vm->dump_cycle = -1;
	if (!ft_strcmp(av[1], "-dump"))
	{
		if (ft_general_validate("%d", av[2]) && av[2][0] != '-')
			vm->dump_cycle = ft_atoz(av[2]);
		else
			error();
		n_start = 2;
	}
	vm->num_players = get_n_players(ac, av, vm, n_start);
}

void	assign_player_num(t_vm *vm, int i, unsigned char **reg)
{
	*reg = vm->info[i].regs[1];
	(*reg)[0] = 255;
	(*reg)[1] = 255;
	(*reg)[2] = 255;
	(*reg)[3] = 255 - i;
}

void 	init_vm(t_vm *vm)
{
	int	i;
	int	x;
	int	fd;
	unsigned char *reg;

	i = -1;
	x = 0;
	vm->process_count = 0;
	vm->cycle_to_die = CYCLE_TO_DIE;
	ft_bzero(vm->core, 4096);
	ft_printf("vm->num_players [%i]\n", vm->num_players);
	while (++i < vm->num_players)
	{
		vm->process_count++;
		fd = open(vm->players[i], O_RDONLY);
		write_info(&vm->info[i], fd, &x, vm->core);
		vm->info[i].location = i * (4096 / vm->num_players);
		vm->info[i].start = vm->info[i].location;
		assign_player_num(vm, i, &reg);
		x += ((4096 / vm->num_players) - vm->info[i].head.prog_size);
	}
}

void jumptable(int a, t_vm *vm, int i)
{
	void (*jt[16])(t_vm *vm, int i);

	jt[0] = vm_live;
	jt[1] = vm_ld;
	jt[2] = vm_st;
	jt[3] = vm_add;
	jt[4] = vm_sub;
	jt[5] = vm_and;
	jt[6] = vm_or;
	jt[7] = vm_xor;
	jt[8] = vm_zjmp;
	jt[9] = vm_ldi;
	jt[10] = vm_sti;
	jt[11] = vm_fork;
	jt[12] = vm_lld;
	jt[13] = vm_lldi;
	jt[14] = vm_lfork;
	jt[15] = vm_aff;
	jt[a - 1](vm, i);
}

int		valid_acb(unsigned char acb, int b1, int b2, int b3)
{
	return (ACB1(acb) == b1 && ACB2(acb) == b2 && ACB3(acb) == b3);
}

int		main(int ac, char **av)
{
	t_vm			vm;

	if (ac < 2)
		error();
	init_players(ac, av, &vm);
	init_vm(&vm);
	ft_printf("prog size: [%i]\n", vm.info[0].head.prog_size);
	//init_curses(&vm);
	print_core(vm.core, -1);
	read_bytes(&vm, -1);
	ft_printf("\n");
	return (0);
}