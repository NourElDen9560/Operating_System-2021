#include <inc/lib.h>
// malloc()
//	This function use FIRST FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
struct US_INFO {
	uint32 start_address;
	int size_of_allocated;
};
#define UserHeapRange  ((USER_HEAP_MAX - USER_HEAP_START)/ PAGE_SIZE)
struct US_INFO allocated_pages[((USER_HEAP_MAX - USER_HEAP_START)/ PAGE_SIZE)];
bool HeapSpace[((USER_HEAP_MAX - USER_HEAP_START)/ PAGE_SIZE)] = { 0 };
int counter_of_allocated = 0;


void* Best_fit_strategy(uint32 size) {
	uint32 B_Fit = 99999999;
	int index = 0 ,
	 IndexToStartAllocate = 0 ,
	 Free_Spaces = 0,
	 // number of all pages in heap
	 number_of_pages =((USER_HEAP_MAX - USER_HEAP_START)/ PAGE_SIZE) ;
	bool FoundedSpace = 0;
	for (int i = 0; i < number_of_pages; i++) {
		// Start Search
		if (HeapSpace[i] == 0 && HeapSpace[i - 1] == 1)
			index = i;
		if (HeapSpace[i] == 0)
			Free_Spaces++;
		// IF CRASHED WITH ALLOCATED SPACE
		 if (HeapSpace[i] == 1) {
			if (Free_Spaces < B_Fit  ) {
				if(Free_Spaces >= size){
				IndexToStartAllocate = index;
				B_Fit = Free_Spaces;
				FoundedSpace = 1;
				}
			}
			Free_Spaces = 0;
		}
		if (Free_Spaces < B_Fit && Free_Spaces == size && FoundedSpace == 0) {
			bool Empty = 0;
			for (int j = i; j < number_of_pages; j++) {
				if (HeapSpace[j] == 1) {
					Empty = 1;
					break;
				}
			}
			if (Empty == 0) {
				IndexToStartAllocate = index;
				B_Fit = Free_Spaces;
				FoundedSpace = 1;
				Free_Spaces = 0;
				break;
			}
		}
	}
	if (FoundedSpace == 1) {
		// loop to mark 1 on used pages
	for (int i = 0; i < size; i++)
		HeapSpace[ IndexToStartAllocate+ i] = 1;

	    allocated_pages[counter_of_allocated].size_of_allocated = size;
		uint32 StartAddressTOAllocate = (IndexToStartAllocate* PAGE_SIZE) + USER_HEAP_START ;
		allocated_pages[counter_of_allocated].start_address =StartAddressTOAllocate ;
		return (void*) StartAddressTOAllocate;
	}
	return NULL;
}
void* malloc(uint32 size) {
	uint32* returned_address;
	// roundup to be ready to allocate
	returned_address = Best_fit_strategy(ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE);
	if (returned_address != NULL){
		 sys_allocateMem((uint32) returned_address, size);
			  counter_of_allocated++;
	}
	else
	 return NULL ;
	return (void*) returned_address;
}
// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.
void free(void* virtual_address) {
	//TODO: [PROJECT 2021 - [2] User Heap] free() [User Side]
	// Write your code here, remove the panic and write your code
	//panic("free() is not implemented yet...!!");
	//you should get the size of the given allocation using its address
	//refer to the project presentation and documentation for details
	uint32 size;
	int index;
	bool founded = 0 ;
	for (int i = 0; i < counter_of_allocated; i++) {
		if (allocated_pages[i].start_address == (uint32) virtual_address) {
			founded = 1 ;
			index  = i ;
			break;
		}
	}
	if(founded){
		 size = allocated_pages[index].size_of_allocated * PAGE_SIZE;
		sys_freeMem((uint32) virtual_address, size);
	    allocated_pages[index].start_address =allocated_pages[counter_of_allocated - 1].start_address;
	    allocated_pages[index].size_of_allocated =allocated_pages[counter_of_allocated - 1].size_of_allocated;
		counter_of_allocated--;
	}
	uint32 StartIndex = ((uint32) virtual_address - USER_HEAP_START) / PAGE_SIZE;
	size = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
	for (int i = 0; i < size; i++)
		HeapSpace[ StartIndex + i] = 0;

}
//==================================================================================//
//================================ OTHER FUNCTIONS =================================//
//==================================================================================//

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable) {
	panic("this function is not required...!!");
	return 0;
}

void* sget(int32 ownerEnvID, char *sharedVarName) {
	panic("this function is not required...!!");
	return 0;
}

void sfree(void* virtual_address) {
	panic("this function is not required...!!");
}

void *realloc(void *virtual_address, uint32 new_size) {
	panic("this function is not required...!!");
	return 0;
}

void expand(uint32 newSize) {
	panic("this function is not required...!!");
}
void shrink(uint32 newSize) {
	panic("this function is not required...!!");
}

void freeHeap(void* virtual_address) {
	// panic("this function is not required...!!");

}
