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
#ifndef _IDT
#define _IDT

//Estrutura de uma entrada na IDT
struct registro_idt
{
    unsigned short base_low;
    unsigned short seg;             //Segmento de mem�ria do kernel
    unsigned char  sempre_zero;     //Valor deve ser sempre 0
    unsigned char  flags;           //Valores do flag
    unsigned short base_high;
	
} __attribute__((packed));

//Ponteiro para IDT
struct idt_ptr
{
    unsigned short limite;
    unsigned int base;
	
} __attribute__((packed));

//Struct que representa o estado na pilha, quando a fun��o de tratamento de interrup��es e exce��es � chamada
struct regs
{
    unsigned int gs, fs, es, ds;                           // registradores de segmento, empilhados por �ltimo
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;   // registradores empilhados pela instru��o 'pusha'
    unsigned int int_no, err_code;                         // n�mero da interrup��o, e c�digo de erro
    unsigned int eip, cs, eflags, useresp, ss;             // valores empilhados autom�ticamente pelo processador
	
}__attribute__((packed));

//classe IDT  
class Idt
{
	public:

	//Pr�totipos	
	void adicionar_registro(unsigned char, unsigned long, unsigned short, unsigned char);
	void inicializar();
	void instalar_isrs();
};

//Definido externamente em  start.asm
extern "C" void  carregar_idt();

extern Idt idt;

#endif
