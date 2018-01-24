/*   This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _SYSCALL
#define _SYSCALL

#include "Idt.h"

#define INFO_PROCESSOS 	1
#define INFO_MENSAGENS 	2
#define INFO_MEMORIA 	3
#define INFO_VFS 		4

class Syscall
{
	
   public :
   
   unsigned int num;
   
   void inicializar(Idt idt);
   void chamar_systemcall(struct regs *r);
   void sbrk(struct regs *r);
   void obter_info_kernel(struct regs *r);
   void enviar_msg(struct regs *r);
   void enviar_msg_pid(struct regs *r);
   void receber_msg(struct regs *r);
   void escutar_porta(struct regs *r);
   void abrir_arquivo(struct regs *r);
   void ler_arquivo(struct regs *r);
   void escrever_arquivo(struct regs *r);
   void buscar_arquivo(struct regs *r);
   void excluir_arquivo(struct regs *r);
   void fechar_arquivo(struct regs *r);
   void executar(struct regs *r);
   void sair(struct regs *r);
   void finalizar_proceso(struct regs *r);
   void criar_thread(struct regs *r);
   void obter_pid(struct regs *r);
   void obter_info_arquivo(struct regs *r);
   void adicionar_no(struct regs *r);
   void montar_no(struct regs *r);
   void remover_no(struct regs *r);
};
 
extern Syscall syscall;

#endif