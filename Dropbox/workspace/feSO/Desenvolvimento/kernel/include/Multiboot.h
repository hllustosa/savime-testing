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
#ifndef _MULTIBOOT
#define _MULTIBOOT

//Estruturas utilizadas pelo GRUB
typedef struct elf_section_header_table
{
   unsigned long num;
   unsigned long size;
   unsigned long addr;
   unsigned long shndx;
};

typedef struct aout_symbol_table
{
   unsigned long tabsize;
   unsigned long strsize;
   unsigned long addr;
   unsigned long reserved;
} ;

//Informações sobre a memória obtidas pelo GRUB
struct multiboot_info
{
   unsigned long flags;
   unsigned long mem_lower;
   unsigned long mem_upper;
   unsigned long boot_device;
   unsigned long cmdline;
   unsigned long mods_count;
   unsigned long mods_addr;
   union
   {
	 aout_symbol_table aout_sym;
	 elf_section_header_table elf_sec;
   } u;
   unsigned long mmap_length;
   unsigned long mmap_addr;
   
};
 
typedef struct multiboot_memory_map 
{
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
	unsigned int length_low,length_high;
	unsigned int type;
	
}multiboot_memory_map_t;

//Informações sobre os módulos carregados pelo GRUB
struct multiboot_mod_list
{
   //A memória usada pelo módulo vai de mod_start até mod_end
   unsigned int mod_start;
   unsigned int mod_end;
 
   //Linha de comando do módulo
   unsigned int cmdline;
 
   //Enchimento para alinhar começo dos módulos
   unsigned int pad;
};
typedef struct multiboot_mod_list multiboot_module_t;
	
#endif	