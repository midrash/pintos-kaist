#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/loader.h"
#include "userprog/gdt.h"
#include "threads/flags.h"
#include "intrinsic.h"

void syscall_entry (void);
void syscall_handler (struct intr_frame *);

/* System call.
 *
 * Previously system call services was handled by the interrupt handler
 * (e.g. int 0x80 in linux). However, in x86-64, the manufacturer supplies
 * efficient path for requesting the system call, the `syscall` instruction.
 *
 * The syscall instruction works by reading the values from the the Model
 * Specific Register (MSR). For the details, see the manual. */

#define MSR_STAR 0xc0000081         /* Segment selector msr */
#define MSR_LSTAR 0xc0000082        /* Long mode SYSCALL target */
#define MSR_SYSCALL_MASK 0xc0000084 /* Mask for the eflags */

void
syscall_init (void) {
	write_msr(MSR_STAR, ((uint64_t)SEL_UCSEG - 0x10) << 48  |
			((uint64_t)SEL_KCSEG) << 32);
	write_msr(MSR_LSTAR, (uint64_t) syscall_entry);

	/* The interrupt service rountine should not serve any interrupts
	 * until the syscall_entry swaps the userland stack to the kernel
	 * mode stack. Therefore, we masked the FLAG_FL. */
	write_msr(MSR_SYSCALL_MASK,
			FLAG_IF | FLAG_TF | FLAG_DF | FLAG_IOPL | FLAG_AC | FLAG_NT);
}

/* The main system call interface */
void
syscall_handler (struct intr_frame *f UNUSED) {
	// TODO: Your implementation goes here.
	printf ("system call!\n");
	int sys_number = f->R.rax;
	printf("시스템콜 번호 : %d\n",sys_number );
	printf("agu1: %lld\n",f->R.rdi);
	// printf("agu1: %lld\n",f->R.rdi);
	// printf("agu2: %s\n",*(char**) f->R.rsi);
	// printf("agu3: %s\n",*(char**) f->R.rdx);
	switch(sys_number) {
		case SYS_HALT:
			halt();
			break;
		case SYS_EXIT:
			exit(f->R.rdi);
		// case SYS_FORK:
		// 	fork(f->R.rdi);		
		// case SYS_EXEC:
		// 	exec(f->R.rdi);
		// case SYS_WAIT:
		// 	wait(f->R.rdi);
		case SYS_CREATE:
			create(f->R.rdi, f->R.rsi);		
		case SYS_REMOVE:
			remove(f->R.rdi);		
		// case SYS_OPEN:
		// 	open(f->R.rdi);		
		// case SYS_FILESIZE:
		// 	filesize(f->R.rdi);
		// case SYS_READ:
		// 	read(f->R.rdi, f->R.rsi, f->R.rdx);
		// case SYS_WRITE:
		// 	write(f->R.rdi, f->R.rsi, f->R.rdx);
		// 	break;		
		// case SYS_SEEK:
		// 	seek(f->R.rdi, f->R.rsi);		
		// case SYS_TELL:
		// 	tell(f->R.rdi);		
		// case SYS_CLOSE:
		// 	close(f->R.rdi);
		default:
			printf("여기 올거같은데\n");
			halt();
			break;	
	}

	thread_exit ();
}

void check_address(void *addr){
	struct thread* t = thread_current();
	if(!is_user_vaddr(addr) || addr == NULL|| pml4_get_page(t->pml4, addr)== NULL){
		thread_exit();
	}
}

void
halt (void) {
	power_off();
}

void
exit (int status) {
	
}

bool
create (const char *file, unsigned initial_size) {
	return true;
}

bool
remove (const char *file) {
	return true;
}