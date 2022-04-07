#include <stdio.h>

enum mnemonics {
  // IO operations
  READ = 10,
  WRITE = 11,

  // Data-transfer operations
  LOAD = 20,
  STORE = 21,

  // Arithmetic operations
  ADD = 30,
  SUB = 31,
  MUL = 32,
  DIV = 33,

  // Transfer-of-control operations
  BRANCH = 40,
  BRANCHNEG = 41,
  BRANCHZERO = 42,
  HALT = 43
};

#define MEM_ADDR_SIZE 100

void load(int memory[], size_t memory_arr_size);
void execute(unsigned int opcode, int op, int *mem, int *accumulatorPtr, int *instructionCounterPtr);
void memoryDump(const int const *mem, const int const *a, const int const *ic,
                const int const *ir, const int const *opcode, const int const *opr);

int main(void) {
  // initializing all memory in 0
  int memory[MEM_ADDR_SIZE] = {0};
  // registers
  int accumulator = 0, instructionCounter = 0, instructionRegister = 0,
      operationCode = 0, operand = 0;

  puts("Welcome to Simpletron!\n\n"
       "Please enter your program one instruction (or data word) at a time.\n"
       "Type the sentinel -99999 to stop entering your program.\n");

  load(memory, MEM_ADDR_SIZE);

  puts("\n*** Program loading completed ***");
  puts("*** Program execution begins ***\n");

  while (instructionCounter < MEM_ADDR_SIZE && operationCode != 43) {
    instructionRegister = memory[instructionCounter];
    operationCode = instructionRegister / 100; // fetch opcode
    operand = instructionRegister % 100;       // fetch operand

    execute(operationCode, operand, memory, &accumulator, &instructionCounter);
  }

  puts("\n\n*** Simpletron execution terminated ***");

  memoryDump(memory, &accumulator, &instructionCounter, &instructionRegister, 
              &operationCode, &operand);

  puts("");

  return 0;
}

void load(int memory[], size_t memory_arr_size) {
  for (size_t i = 0; i < memory_arr_size; i++) {
    do {
      printf("%02zu ? ", i);
      scanf("%d", &memory[i]);
    } while (memory[i] < -9999 & memory[i] != -99999 || memory[i] > 9999);

    if (memory[i] == -99999) {
      memory[i] = 0;
      break;
    }
  }
}

void execute(unsigned int opcode, int op, int *mem, int *accumulatorPtr, int *instructionCounterPtr) {
  enum mnemonics instruction = opcode;

  switch (instruction) {
  // IO operations
  case READ:
    do {
      printf("%s", "< ");
      scanf("%d", &mem[op]);
    } while (mem[op] < -9999 || mem[op] > 9999); // verify memory limit store value
    
    (*instructionCounterPtr)++;
    break;
  case WRITE:
    printf("> %d", mem[op]);
    (*instructionCounterPtr)++;
    break;
    
  // Data-transfer operations
  case LOAD:
    *accumulatorPtr = mem[op];
    (*instructionCounterPtr)++;
    break;
  case STORE:
    mem[op] = *accumulatorPtr;
    (*instructionCounterPtr)++;
    break;

  // Arithmetic operations
  case ADD:
    *accumulatorPtr += mem[op];
    (*instructionCounterPtr)++;
    break;
  case SUB:
    *accumulatorPtr -= mem[op];
    (*instructionCounterPtr)++;
    break;
  case MUL:
    *accumulatorPtr *= mem[op];
    (*instructionCounterPtr)++;
    break;
  case DIV:
    *accumulatorPtr /= mem[op];
    (*instructionCounterPtr)++;
    break;
  
  // Transfer-of-control operations
  case BRANCH:
    *instructionCounterPtr = op;
    break;
  case BRANCHNEG:
    if (*accumulatorPtr < 0)
      *instructionCounterPtr = op;
    else {
      (*instructionCounterPtr)++;
    }
    break;
  case BRANCHZERO:
    if (*accumulatorPtr == 0)
      *instructionCounterPtr = op;
    else
      (*instructionCounterPtr)++;
    break;
  }
}


void memoryDump(const int const *mem, const int const *a, const int const *ic,
                const int const *ir, const int const *opcode, const int const *opr) {
  
  puts("\nREGISTERS:");
  printf("%-25s%+05d\n", "Accumulator", *a);
  printf("%-25s%+05d\n", "instructionCounter", *ic);
  printf("%-25s%+05d\n", "instructionRegister", *ir);
  printf("%-25s%+05d\n", "operationCode", *opcode);
  printf("%-25s%+05d\n", "operand", *opr);

  puts("\nMEMORY:");
  printf("%2s", "");

  for (int i = 0; i < 10; i++) {
    printf("  %5d", i);
  }

  for (int i = 0; i < 10; i++) {
    printf("\n%2d", i * 10);

    for (size_t p = 0; p < 10; p++) {
      printf("  %+05d", mem[p + i * 10]);
    }
  }

}
