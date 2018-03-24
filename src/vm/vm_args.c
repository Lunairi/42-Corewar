/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_args.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anazar <anazar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/17 20:59:44 by anazar            #+#    #+#             */
/*   Updated: 2018/03/23 22:10:40 by ihodge           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <corewar.h>

void	vm_st(t_vm *vm, int i)
{
	t_instr		instr;

	instr = init_instr(vm, i);
	vm->info[i].index++;
	instr.core_index += 2;
	if(!valid_acb2(instr, 2))
		return ;
	if (!get_offset(&instr, ACB1(instr.acb), &instr.l1) ||
	!get_offset(&instr, ACB2(instr.acb) | 0b100, &instr.l2))
		return ; 
	if (ACB2(instr.acb) == 1)
		instr.l2 = &vm->core[vm->info[i].start +
			instr.opcode_pos + VAL(instr.l2) % 4096];
	reg_copy(instr.l2, instr.l1);
	vis_copy(&vm->vis[vm->info[i].start + instr.opcode_pos +
		VAL(instr.l2)], instr.l1, vm, i);
	vis_update(vm, vm->info[i].start + instr.opcode_pos + VAL(instr.l2));
}

void	vm_sti(t_vm *vm, int i)
{
	t_instr		instr;

	instr = init_instr(vm, i);
	vm->info[i].index++;
	instr.core_index += 2;
	if(!valid_acb3(instr, 10))
		return ;
	if (!get_offset_index(&instr, ACB1(instr.acb), &instr.l1) ||
		!get_offset_index(&instr, ACB2(instr.acb), &instr.l2) ||
		!get_offset_index(&instr, ACB3(instr.acb), &instr.l3))
		return ;
	vm->info[i].index = instr.core_index;
	instr.index = ((VAL2(instr.l2) + VAL2(instr.l3)) & 0xFFFF) % IDX_MOD;
	reg_copy(&vm->core[vm->info[i].start + instr.opcode_pos + instr.index],
		instr.l1);
	vis_copy(&vm->vis[vm->info[i].start + instr.opcode_pos + instr.index],
		instr.l1, vm, i);
	vis_update(vm, vm->info[i].start + instr.opcode_pos + instr.index);
}

void	vm_zjmp(t_vm *vm, int i)
{
	t_instr instr;

	ft_printf("zjmp called\n");
	instr = init_instr(vm, i);
	instr.acb = 0;
	vm->info[i].index++;
	if (!vm->info[i].carry)
		return ;
	vm->info[i].index += get_index_one(&vm->core[vm->info[i].start +
		vm->info[i].index]) - 1;
	vm->info[i].carry = 0;
	ft_printf("zjmp finished\n");
}

void	vm_live(t_vm *vm, int i)
{
	if (vm->core[PARAM1 + 1] == 0xff && vm->core[PARAM1 + 2] == 0xff
			&& vm->core[PARAM1 + 3] == 0xff
			&& (vm->core[PARAM1 + 4] >= 0xff - (vm->num_players - 1)
				&& vm->core[PARAM1 + 4] <= 0xff))
		vm->win_player = vm->core[PARAM1 + 4] - 0xff + 1;
	vm->info[i].live++;
	vm->live++;
	vm->info[i].alive = 1;
	vm->info[i].index += 5;
}

void	vm_aff(t_vm *vm, int i)
{
	t_instr		instr;

	instr = init_instr(vm, i);
	vm->info[i].index++;
	instr.core_index += 2;
	if (!valid_acb1(instr, 15))
		return ;
	if (!get_offset(&instr, ACB1(instr.acb), &instr.l1))
		return ;
	vm->info[i].index = instr.core_index;
}
